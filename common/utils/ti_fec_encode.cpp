#include <stdint.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <queue>
#include <random>

//--------------------------------------------------------------------------------
uint16_t culCalcCRC(uint8_t crcData, uint16_t crcReg) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80))
      crcReg = (crcReg << 1) ^ 0x8005;
    else
      crcReg = (crcReg << 1);
    crcData <<= 1;
  }
  return crcReg;
}
//--------------------------------------------------------------------------------
// Variables
uint16_t fecEncodeTable[] = {0, 3, 1, 2, 3, 0, 2, 1, 3, 0, 2, 1, 0, 3, 1, 2};
uint8_t input[260];
uint16_t i, j, val, fecReg, fecOutput;
uint32_t intOutput;
uint16_t fec[520];
uint8_t interleaved[520];
uint16_t inputNum = 0, fecNum;
uint16_t checksum;
uint16_t length;


int main() {
  //--------------------------------------------------------------------------------
  // Example code
  uint8_t packet_str[] = {1, 2,  3,  4,  5,  6,  7,  8,
                          9, 10, 11, 12, 13, 14, 15, 16};
  // input[0] = length;
  // input[1] = 1;
  // input[2] = 2;
  // input[3] = 3;
  // inputNum = length + 1;
  inputNum = sizeof(packet_str);
  memcpy(input, packet_str, inputNum);

  printf("Input: [%5d bytes]\n", inputNum);
  for (i = 0; i < inputNum; i++)
    printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  printf("\n\n");

  // Generate CRC
  if (false) {
    checksum = 0xFFFF;  // Init value for CRC calculation
    for (i = 0; i <= inputNum; i++) {
      checksum = culCalcCRC(input[i], checksum);
    }
    input[inputNum++] = checksum >> 8;      // CRC1
    input[inputNum++] = checksum & 0x00FF;  // CRC0
  }

  printf("Appended CRC: [%5d bytes]\n", inputNum);
  for (i = 0; i < inputNum; i++) {
    printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  }
  printf("\n\n");

  // Append Trellis Terminator
  input[inputNum] = 0x0B;
  input[inputNum + 1] = 0x0B;
  fecNum = 2 * ((inputNum / 2) + 1);
  printf("Appended Trellis terminator: [%5d bytes]\n", fecNum);
  for (i = 0; i < fecNum; i++) {
    printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  }
  printf("\n\n");

  // FEC encode
  fecReg = 0;
  for (i = 0; i < fecNum; i++) {
    fecReg = (fecReg & 0x700) | (input[i] & 0xFF);
    fecOutput = 0;
    for (j = 0; j < 8; j++) {
      fecOutput = (fecOutput << 2) | fecEncodeTable[fecReg >> 7];
      fecReg = (fecReg << 1) & 0x7FF;
    }
    fec[i * 2] = fecOutput >> 8;
    fec[i * 2 + 1] = fecOutput & 0xFF;
  }

  printf("FEC encoder output: [%5d bytes]\n", fecNum * 2);
  for (i = 0; i < fecNum * 2; i++)
    printf("%02X%s", fec[i], (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " ");
  printf("\n\n");

  // Perform interleaving
  for (i = 0; i < fecNum * 2; i += 4) {
    intOutput = 0;
    for (j = 0; j < 4 * 4; j++)
      intOutput = (intOutput << 2) |
                  ((fec[i + (~j & 0x03)] >> (2 * ((j & 0x0C) >> 2))) & 0x03);
    interleaved[i] = (intOutput >> 24) & 0xFF;
    interleaved[i + 1] = (intOutput >> 16) & 0xFF;
    interleaved[i + 2] = (intOutput >> 8) & 0xFF;
    interleaved[i + 3] = (intOutput >> 0) & 0xFF;
  }
  printf("Interleaver output: [%5d bytes]\n", fecNum * 2);
  for (i = 0; i < fecNum * 2; i++)
    printf("%02X, %s", interleaved[i],
           (i % 16 == 15) ? "\n"
           : (i % 4 == 3) ? ""
                          : "");
  printf("\n\n");
}
