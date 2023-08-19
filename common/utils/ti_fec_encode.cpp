#include <stdint.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <queue>
#include <random>

#include "ti_fec.h"

#pragma GCC optimize("-Os")

static constexpr const uint16_t fecEncodeTable[] = {0, 3, 1, 2, 3, 0, 2, 1,
                                                    3, 0, 2, 1, 0, 3, 1, 2};

void FecEncoder::Encode(uint8_t* inputPtr, size_t inLen) {
  memcpy(input, inputPtr, inLen);
  uint16_t inputNum = inLen;

  // Append Trellis Terminator
  input[inputNum] = 0x0B;
  input[inputNum + 1] = 0x0B;
  size_t fecNum = 2 * ((inputNum / 2) + 1);

  DEBUG_PRINTF("Added Trellis: [%5d bytes]\n", sizeof(input));
  for (unsigned int i = 0; i < sizeof(input); i++) {
    DEBUG_PRINTF("%02X%s", input[i],
                 (i % 8 == 7)   ? "\n"
                 : (i % 2 == 1) ? " "
                                : " ");
  }
  DEBUG_PRINTF("\n\n");

  // FEC encode
  uint16_t fecReg = 0;
  for (size_t i = 0; i < fecNum; i++) {
    fecReg = (fecReg & 0x700) | (input[i] & 0xFF);
    uint16_t fecOutput = 0;
    for (int j = 0; j < 8; j++) {
      fecOutput = (fecOutput << 2) | fecEncodeTable[fecReg >> 7];
      fecReg = (fecReg << 1) & 0x7FF;
    }
    fec[i * 2] = fecOutput >> 8;
    fec[i * 2 + 1] = fecOutput & 0xFF;
  }

  DEBUG_PRINTF("FEC: [%5d bytes]\n", sizeof(fec));
  for (unsigned int i = 0; i < sizeof(fec); i++) {
    DEBUG_PRINTF("%02X%s", fec[i],
                 (i % 8 == 7)   ? "\n"
                 : (i % 2 == 1) ? " "
                                : " ");
  }
  DEBUG_PRINTF("\n\n");

  // Perform interleaving
  for (size_t i = 0; i < fecNum * 2; i += 4) {
    uint32_t intOutput = 0;
    for (size_t j = 0; j < 4 * 4; j++)
      intOutput = (intOutput << 2) |
                  ((fec[i + (~j & 0x03)] >> (2 * ((j & 0x0C) >> 2))) & 0x03);
    interleaved[i] = (intOutput >> 24) & 0xFF;
    interleaved[i + 1] = (intOutput >> 16) & 0xFF;
    interleaved[i + 2] = (intOutput >> 8) & 0xFF;
    interleaved[i + 3] = (intOutput >> 0) & 0xFF;
  }

  DEBUG_PRINTF("Interleaved: [%5d bytes]\n", fecNum * 2);
  for (unsigned int i = 0; i < fecNum * 2; i++) {
    DEBUG_PRINTF("%02X%s", interleaved[i],
                 (i % 8 == 7)   ? "\n"
                 : (i % 2 == 1) ? " "
                                : " ");
  }
  DEBUG_PRINTF("\n\n");

  // return interleaved;
}
