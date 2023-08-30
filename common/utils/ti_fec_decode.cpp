#include <stdint.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <queue>
#include <random>

// #include <chrono>
// #include <iostream>

#include "ti_fec.h"

#pragma GCC optimize("-Ofast")

/**************************************************************************************************************
 * @fn hammWeight
 *
 * @brief Calculates Hamming weight of byte (# bits set)
 *
 * @param a - Byte to find the Hamming weight for
 *
 * @return Hamming weight (# of bits set in a)
 */
static inline uint8_t hammWeight(uint8_t a) {
  a = ((a & 0xAA) >> 1) + (a & 0x55);
  a = ((a & 0xCC) >> 2) + (a & 0x33);
  a = ((a & 0xF0) >> 4) + (a & 0x0F);
  return a;
}

// Look-up source state index when:
// Destination state --\ /-- Each of two possible source states
static constexpr const uint8_t aTrellisSourceStateLut[8][2] = {
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
static constexpr const uint8_t aTrellisTransitionOutput[8][2] = {
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
static constexpr const uint8_t aTrellisTransitionInput[8] = {
    0, 1, 0, 1, 0, 1, 0, 1,
};

#define MIN(a, b) ((b < a) ? b : a)
// template <class T>
// inline T MIN(T a, T b) {
//   return (a>b) ? a : b;
// }

void FecDecoder::Reset() {
  memset(nCost, 0, sizeof(nCost));
  for (size_t n = 1; n < 8; n++) {
    nCost[0][n] = 100;
  }
  iLastBuf = 0;
  iCurrBuf = 1;
  nPathBits = 0;
}

// The lookup table is indexed into as [iDestState[2:0], symbol[1:0], path]
// In at least one test, using addition instead of bitwise OR nets us ~3%
// speedup
#define destStateLUTIndex(iDestState, symbol, path) \
  ((iDestState << 3) + (symbol << 1) + (path))

void FecDecoder::FillLUT() {
  for (uint8_t iDestState = 0; iDestState < 8; iDestState++) {
    for (uint8_t symbol = 0; symbol < 8; symbol++) {
      // Figure out how much the cost will increase for a given pair of
      // (destination, current symbol)
      uint8_t costStep0 =
          hammWeight(symbol ^ aTrellisTransitionOutput[iDestState][0]);
      uint8_t costStep1 =
          hammWeight(symbol ^ aTrellisTransitionOutput[iDestState][1]);

      // And write it down
      costStepLUT[destStateLUTIndex(iDestState, symbol, 0)] = costStep0;
      costStepLUT[destStateLUTIndex(iDestState, symbol, 1)] = costStep1;
    }
  }
}

void FecDecoder::FecDecode(uint8_t* pInputMessage, uint8_t* pOutputBuffer,
                           size_t decodedMessageLen) {
  Reset();

  while (decodedMessageLen) {
    DEBUG_PRINTF("%u to go. Input quartet: %X %X %X %X\n", decodedMessageLen,
                 pInputMessage[0], pInputMessage[1], pInputMessage[2],
                 pInputMessage[3]);

    size_t numBytesDecoded =
        FecDecode4(pOutputBuffer, pInputMessage, decodedMessageLen);


    // Keep track of remaining message length
    decodedMessageLen -= numBytesDecoded;

    // Advance output buffer
    pOutputBuffer += numBytesDecoded;

    // Advance input by 4 bytes (always 4)
    pInputMessage += 4;

    
  }
}

uint16_t FecDecoder::FecDecode4(uint8_t* pOutputArray, uint8_t* pInData,
                                uint16_t nRemBytes) {
  // Variables used to hold # Viterbi iterations to run, # bytes output,
  // minimum cost for any destination state, bit index of input symbol
  uint8_t nIterations;
  uint16_t nOutputBytes = 0;
  uint8_t nMinCost;
  int8_t iBit = 8 - 2;

  // Declare variables up front
  uint8_t nCost0;
  uint8_t nCost1;
  uint8_t iSrcState0;
  uint8_t iSrcState1;
  uint8_t nInputBit;

  // Initialize variables at start of packet (and return without doing any more)
  if (pOutputArray == NULL) {
    Reset();
    return 0;
  }

  // auto start = std::chrono::high_resolution_clock::now();

  uint8_t aDeintData[4];
  {
    int8_t iOut;
    int8_t iIn;
    // De-interleave received data (and change pInData to point to
    // de-interleaved data)
    for (iOut = 0; iOut < 4; iOut++) {
      uint8_t dataByte = 0;
      for (iIn = 3; iIn >= 0; iIn--)
        dataByte = (dataByte << 2) | ((pInData[iIn] >> (2 * iOut)) & 0x03);
      aDeintData[iOut] = dataByte;
    }
    pInData = aDeintData;
  }

  // auto finish = std::chrono::high_resolution_clock::now();
  // std::cout << "De-interleave\t" <<
  // std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()
  // << "ns\n"; start = std::chrono::high_resolution_clock::now();

  DEBUG_PRINTF("Un-interleaved: %02X %02X %02X %02X\n", pInData[0], pInData[1],
               pInData[2], pInData[3]);

  // Process up to 4 bytes of de-interleaved input data, processing one encoder
  // symbol (2b) at a time
  uint8_t iDestState;
  uint8_t index;
  for (nIterations = 16; nIterations > 0; nIterations--) {
    uint8_t symbol = ((*pInData) >> iBit) & 0x03;
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

#if 1
// Unrolling this loop seems to net us 5-7% speed improvement
#define DO_STATE(iDestState)                                                  \
  {                                                                           \
    nInputBit = (iDestState % 2);                                             \
    /* Calculate cost of transition from each of the two source states */     \
    /* (cost is Hamming difference between received 2b symbol and expected */ \
    /* symbol for transition) */                                              \
    iSrcState0 = aTrellisSourceStateLut[iDestState][0];                       \
    iSrcState1 = aTrellisSourceStateLut[iDestState][1];                       \
    nCost0 = nCost[iLastBuf][iSrcState0];                                     \
    nCost1 = nCost[iLastBuf][iSrcState1];                                     \
    /* Being smart and writing down the index once nets us another 7.5% */    \
    /* But being cleaver and removing this increment doesn't change it */     \
    index = destStateLUTIndex(iDestState, symbol, 0);                         \
    nCost0 += costStepLUT[index];                                             \
    nCost1 += costStepLUT[index + 1];                                         \
    /* Select transition that gives lowest cost in destination state, copy */ \
    /* that source state's path and add new decoded bit*/                     \
    if (nCost0 <= nCost1) {                                                   \
      nCost[iCurrBuf][iDestState] = nCost0;                                   \
      nMinCost = MIN(nMinCost, nCost0);                                       \
      aPath[iCurrBuf][iDestState] =                                           \
          (aPath[iLastBuf][iSrcState0] << 1) | nInputBit;                     \
    } else {                                                                  \
      nCost[iCurrBuf][iDestState] = nCost1;                                   \
      nMinCost = MIN(nMinCost, nCost1);                                       \
      aPath[iCurrBuf][iDestState] =                                           \
          (aPath[iLastBuf][iSrcState1] << 1) | nInputBit;                     \
    }                                                                         \
  }

    DO_STATE(0);
    DO_STATE(1);
    DO_STATE(2);
    DO_STATE(3);
    DO_STATE(4);
    DO_STATE(5);
    DO_STATE(6);
    DO_STATE(7);

#else

    // might be a minimal speedup but u could cut down on all the calls to
    // hammweight / multiple lookups / comparisons if you used an lut with
    // iDestState (3bits), symbol (2bits), ilastbuf (1bit); i believe you can
    // condense most of this into a 64-input lookup table..? might be missing
    // sth iDestState (3) + symbol (2) + iLastBuf (1)

    for (iDestState = 0; iDestState < 8; iDestState++) {
      nInputBit = aTrellisTransitionInput[iDestState];
      // Calculate cost of transition from each of the two source states (cost
      // is Hamming difference between received 2b symbol and expected symbol
      // for transition)
      iSrcState0 = aTrellisSourceStateLut[iDestState][0];
      iSrcState1 = aTrellisSourceStateLut[iDestState][1];
      nCost0 = nCost[iLastBuf][iSrcState0];
      nCost1 = nCost[iLastBuf][iSrcState1];
      // Being smart and writing down the index once nets us another 7.5%
      index = destStateLUTIndex(iDestState, symbol, 0);
      nCost0 += costStepLUT[index];
      nCost1 += costStepLUT[index + 1];
      // Select transition that gives lowest cost in destination state, copy
      // that source state's path and add new decoded bit
      if (nCost0 <= nCost1) {
        nCost[iCurrBuf][iDestState] = nCost0;
        nMinCost = MIN(nMinCost, nCost0);
        aPath[iCurrBuf][iDestState] =
            (aPath[iLastBuf][iSrcState0] << 1) | nInputBit;
      } else {
        nCost[iCurrBuf][iDestState] = nCost1;
        nMinCost = MIN(nMinCost, nCost1);
        aPath[iCurrBuf][iDestState] =
            (aPath[iLastBuf][iSrcState1] << 1) | nInputBit;
      }
    }

#endif

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

      // finish = std::chrono::high_resolution_clock::now();
      // std::cout << "Early, trellis\t" <<
      // std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()
      // << "ns\n";

      return nOutputBytes;
    }
    // Swap current and last buffers for next iteration
    iLastBuf = (iLastBuf + 1) % 2;
    iCurrBuf = (iCurrBuf + 1) % 2;
  }

  // finish = std::chrono::high_resolution_clock::now();
  // std::cout << "Decode:  \t" <<
  // std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()
  // << "ns\n"; start = std::chrono::high_resolution_clock::now();

  // Normalize costs so that minimum cost becomes 0
  {
    uint8_t iState;
    for (iState = 0; iState < 8; iState++) nCost[iLastBuf][iState] -= nMinCost;
  }

  // finish = std::chrono::high_resolution_clock::now();
  // std::cout << "Normalize\t" <<
  // std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()
  // << "ns\n";

  return nOutputBytes;
}
