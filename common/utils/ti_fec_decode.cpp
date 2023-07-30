#include <stdint.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <queue>
#include <random>
#include "ti_fec.h"

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


// Look-up source state index when:
// Destination state --\ /-- Each of two possible source states
static constexpr const unsigned char aTrellisSourceStateLut[8][2] = {
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
static constexpr const unsigned char aTrellisTransitionOutput[8][2] = {
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
static constexpr const unsigned char aTrellisTransitionInput[8] = {
    0, 1, 0, 1, 0, 1, 0, 1,
};



void FecDecoder::Reset() {
  memset(nCost, 0, sizeof(nCost));
  for (size_t n = 1; n < 8; n++) {
    nCost[0][n] = 100;
  }
  iLastBuf = 0;
  iCurrBuf = 1;
  nPathBits = 0;
}

void FecDecoder::FecDecode(uint8_t* pInputMessage, uint8_t* pOutputBuffer,
                           size_t decodedMessageLen) {
  Reset();

  while (decodedMessageLen) {
    printf("%u to go. Input quartet: %X %X %X %X\n", 
      decodedMessageLen,
      pInputMessage[0],
      pInputMessage[1],
      pInputMessage[2],
      pInputMessage[3]
    );

    auto numBytesDecoded =
        FecDecode4(pOutputBuffer, pInputMessage, decodedMessageLen);

    printf("\n");

    // Keep track of remaining message length
    decodedMessageLen -= numBytesDecoded;

    // Advance output buffer
    pOutputBuffer += numBytesDecoded;

    // Advance input by 4 bytes (always 4)
    pInputMessage += 4;
  }
}

unsigned short FecDecoder::FecDecode4(unsigned char* pOutputArray,
                                      unsigned char* pInData,
                                      unsigned short nRemBytes) {
  // Variables used to hold # Viterbi iterations to run, # bytes output,
  // minimum cost for any destination state, bit index of input symbol
  unsigned char nIterations;
  unsigned short nOutputBytes = 0;
  unsigned char nMinCost;
  signed char iBit = 8 - 2;
  // Initialize variables at start of packet (and return without doing any more)
  if (pOutputArray == NULL) {
    Reset();
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

    printf("Un-interleaved: %02X %02X %02X %02X\n", 
      pInData[0],
      pInData[1],
      pInData[2],
      pInData[3]
    );

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
        nMinCost = std::min(nMinCost, nCost0);
        aPath[iCurrBuf][iDestState] =
            (aPath[iLastBuf][iSrcState0] << 1) | nInputBit;
      } else {
        nCost[iCurrBuf][iDestState] = nCost1;
        nMinCost = std::min(nMinCost, nCost1);
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
