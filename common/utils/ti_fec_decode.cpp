#include <stdint.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <queue>
#include <random>

/**************************************************************************************************************
 * FUNCTION PROTOTYPES
 */
unsigned short fecDecode(unsigned char *pOutputArray, unsigned char *pInData,
                         unsigned short RemBytes);

static unsigned char hammWeight(unsigned char a);
static unsigned char min(unsigned char a, unsigned char b);
static unsigned short calcCRC(unsigned char crcData, unsigned short crcReg);
/**************************************************************************************************************
 * GLOBAL VARIABLES
 */
// The payload + CRC are 31 bytes. This way the complete packet to be received
// will fit in the RXFIFO
unsigned char rxBuffer[4];   // Buffer used to hold data read from the RXFIFO (4
                             // bytes are read at a time)
unsigned char rxPacket[31];  // Data + CRC after being interleaved and decoded
// Look-up source state index when:
// Destination state --\ /-- Each of two possible source states
const unsigned char aTrellisSourceStateLut[8][2] = {
    {0, 4},  // State {0,4} -> State 0
    {0, 4},  // State {0,4} -> State 1
    {1, 5},  // State {1,5} -> State 2
    {1, 5},  // State {1,5} -> State 3
    {2, 6},  // State {2,6} -> State 4
    {2, 6},  // State {2,6} -> State 5
    {3, 7},  // State {3,7} -> State 6
    {3, 7},  // State {3,7} -> State 7
};
// Look-up expected output when:
// Destination state --\ /-- Each of two possible source states
const unsigned char aTrellisTransitionOutput[8][2] = {
    {0, 3},  // State {0,4} -> State 0 produces {"00", "11"}
    {3, 0},  // State {0,4} -> State 1 produces {"11", "00"}
    {1, 2},  // State {1,5} -> State 2 produces {"01", "10"}
    {2, 1},  // State {1,5} -> State 3 produces {"10", "01"}
    {3, 0},  // State {2,6} -> State 4 produces {"11", "00"}
    {0, 3},  // State {2,6} -> State 5 produces {"00", "11"}
    {2, 1},  // State {3,7} -> State 6 produces {"10", "01"}
    {1, 2},  // State {3,7} -> State 7 produces {"01", "10"}
};
// Look-up input bit at encoder when:
// Destination state --
const unsigned char aTrellisTransitionInput[8] = {
    0, 1, 0, 1, 0, 1, 0, 1,
};
/**************************************************************************************************************
 * DEFINES
 */
// NUMBER_OF_BYTES_AFTER_DECODING should be given the length of the payload +
// CRC (CRC is optional)
#define NUMBER_OF_BYTES_AFTER_DECODING 31
#define NUMBER_OF_BYTES_BEFORE_DECODING \
  (4 * ((NUMBER_OF_BYTES_AFTER_DECODING / 2) + 1))

/**************************************************************************************************************
 * @fn hammWeight
 *
 * @brief Calculates Hamming weight of byte (# bits set)
 *
 * @param a - Byte to find the Hamming weight for
 *
 * @return Hamming weight (# of bits set in a)
 */
static unsigned char hammWeight(unsigned char a) {
  a = ((a & 0xAA) >> 1) + (a & 0x55);
  a = ((a & 0xCC) >> 2) + (a & 0x33);
  a = ((a & 0xF0) >> 4) + (a & 0x0F);
  return a;
}
/**************************************************************************************************************
 * @fn min
 *
 * @brief Returns the minimum of two values
 *
 * @param a - Value 1
 * b - Value 2
 *
 * @return Minimum of two values
 * Value 1 (Value 1 < Value 2)
 * Value 2 (Value 2 < Value 1)
 */
static unsigned char min(unsigned char a, unsigned char b) {
  return (a <= b ? a : b);
}
/**************************************************************************************************************
 * @fn calcCRC
 *
 * @brief Calculates a checksum over n data bytes
 * Example of usage
 *
 * checksum = 0xFFFF;
 * for (i = 0; i < n; i++)
 * checksum = calcCRC(dataBytes[i], checksum);
 *
 * @param crcData - checksum (initially set to 0xFFFF)
 * crcReg - data byte
 *
 *
 * @return Checksum
 */
static unsigned short calcCRC(unsigned char crcData, unsigned short crcReg) {
  unsigned char i;
  for (i = 0; i < 8; i++) {
    if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80))
      crcReg = (crcReg << 1) ^ 0x8005;
    else
      crcReg = (crcReg << 1);
    crcData <<= 1;
  }
  return crcReg;
}

typedef struct {
  
} FecDecodeState_t;

/**************************************************************************************************************
 * @fn fecDecode
 *
 * @brief De-interleaves and decodes a given input buffer
 *
 * @param pOutputArray - Pointer to where to put decoded data (NULL when
 * initializing at start of packet)
 * @param pInData - Pointer to received data
 * @param nRemBytes remaining (decoded) bytes to decode
 *
 *
 * @return Number of bytes of decoded data stored at pOutputArray
 */
unsigned short fecDecode(unsigned char *pOutputArray, unsigned char *pInData,
                         unsigned short nRemBytes) {
  // Two sets of buffers (last, current) for each destination state for holding:
  static unsigned char nCost[2][8];  // Accumulated path cost
  static unsigned long aPath[2][8];  // Encoder input data (32b window)
  // Indices of (last, current) buffer for each iteration
  static unsigned char iLastBuf;
  static unsigned char iCurrBuf;
  // Number of bits in each path buffer
  static unsigned char nPathBits;
  // Variables used to hold # Viterbi iterations to run, # bytes output,
  // minimum cost for any destination state, bit index of input symbol
  unsigned char nIterations;
  unsigned short nOutputBytes = 0;
  unsigned char nMinCost;
  signed char iBit = 8 - 2;
  // Initialize variables at start of packet (and return without doing any more)
  if (pOutputArray == NULL) {
    unsigned char n;
    memset(nCost, 0, sizeof(nCost));
    for (n = 1; n < 8; n++) nCost[0][n] = 100;
    iLastBuf = 0;
    iCurrBuf = 1;
    nPathBits = 0;
    return 0;
  }

  unsigned char aDeintData[4];
  {
    signed char iOut;
    signed char iIn;
    // De-interleave received data (and change pInData to point to
    // de-interleaved data)
    for (iOut = 0; iOut < 4; iOut++) {
      unsigned char dataByte = 0;
      for (iIn = 3; iIn >= 0; iIn--)
        dataByte = (dataByte << 2) | ((pInData[iIn] >> (2 * iOut)) & 0x03);
      aDeintData[iOut] = dataByte;
    }
    pInData = aDeintData;
  }

  // Process up to 4 bytes of de-interleaved input data, processing one encoder
  // symbol (2b) at a time
  for (nIterations = 16; nIterations > 0; nIterations--) {
    unsigned char iDestState;
    unsigned char symbol = ((*pInData) >> iBit) & 0x03;
    // Find minimum cost so that we can normalize costs (only last iteration
    // used)
    nMinCost = 0xFF;
    // Get 2b input symbol (MSB first) and do one iteration of Viterbi decoding
    if ((iBit -= 2) < 0) {
      iBit = 6;
      pInData++;  // Update pointer to the next byte of received data
    }
    // For each destination state in the trellis, calculate hamming costs for
    // both possible paths into state and select the one with lowest cost.
    for (iDestState = 0; iDestState < 8; iDestState++) {
      unsigned char nCost0;
      unsigned char nCost1;
      unsigned char iSrcState0;
      unsigned char iSrcState1;
      unsigned char nInputBit;
      nInputBit = aTrellisTransitionInput[iDestState];
      // Calculate cost of transition from each of the two source states (cost
      // is Hamming difference between received 2b symbol and expected symbol
      // for transition)
      iSrcState0 = aTrellisSourceStateLut[iDestState][0];
      nCost0 = nCost[iLastBuf][iSrcState0];
      nCost0 += hammWeight(symbol ^ aTrellisTransitionOutput[iDestState][0]);
      iSrcState1 = aTrellisSourceStateLut[iDestState][1];
      nCost1 = nCost[iLastBuf][iSrcState1];
      nCost1 += hammWeight(symbol ^ aTrellisTransitionOutput[iDestState][1]);
      // Select transition that gives lowest cost in destination state, copy
      // that source state's path and add new decoded bit
      if (nCost0 <= nCost1) {
        nCost[iCurrBuf][iDestState] = nCost0;
        nMinCost = min(nMinCost, nCost0);
        aPath[iCurrBuf][iDestState] =
            (aPath[iLastBuf][iSrcState0] << 1) | nInputBit;
      } else {
        nCost[iCurrBuf][iDestState] = nCost1;
        nMinCost = min(nMinCost, nCost1);
        aPath[iCurrBuf][iDestState] =
            (aPath[iLastBuf][iSrcState1] << 1) | nInputBit;
      }
    }
    nPathBits++;
    // If trellis history is sufficiently long, output a byte of decoded data
    if (nPathBits == 32) {
      *pOutputArray++ = (aPath[iCurrBuf][0] >> 24) & 0xFF;
      nOutputBytes++;
      nPathBits -= 8;
      nRemBytes--;
    }
    // After having processed 3-symbol trellis terminator, flush out remaining
    // data
    if ((nRemBytes <= 3) && (nPathBits == ((8 * nRemBytes) + 3))) {
      while (nPathBits >= 8) {
        *pOutputArray++ = (aPath[iCurrBuf][0] >> (nPathBits - 8)) & 0xFF;
        nOutputBytes++;
        nPathBits -= 8;
      }
      return nOutputBytes;
    }
    // Swap current and last buffers for next iteration
    iLastBuf = (iLastBuf + 1) % 2;
    iCurrBuf = (iCurrBuf + 1) % 2;
  }

  // Normalize costs so that minimum cost becomes 0
  {
    unsigned char iState;
    for (iState = 0; iState < 8; iState++) nCost[iLastBuf][iState] -= nMinCost;
  }

  return nOutputBytes;
}
