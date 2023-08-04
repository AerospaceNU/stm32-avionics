#include "ti_fec.h"

#include <stdint.h>

uint16_t calculateCRC(uint8_t crcData, uint16_t crcReg) {
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

//#include <algorithm>
//#include <array>
//#include <cstdio>
//#include <cstring>
//#include <queue>
//#include <random>
// typedef struct __attribute__((packed)) {
//  uint8_t data[4];
//  uint8_t crcHi;
//  uint8_t crcLo;
//} TestPacket_s;
//
// int main_2() {
//  TestPacket_s packet;
//
//  uint8_t packet_str[] = {3, 1, 2, 3};
//  memcpy(packet.data, packet_str, sizeof(packet_str));
//  size_t inputNum = sizeof(TestPacket_s);
//  uint8_t* input = (uint8_t*)&packet;
//
//  printf("Input: [%5d bytes]\n", sizeof(packet.data));
//  for (unsigned int i = 0; i < sizeof(packet.data); i++)
//    printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : "
//    ");
//  printf("\n\n");
//
//  // Generate CRC
//  if (true) {
//    uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
//    for (unsigned int i = 0; i < sizeof(packet.data); i++) {
//      checksum = calculateCRC(packet.data[i], checksum);
//    }
//
//    packet.crcHi = checksum >> 8;
//    packet.crcLo = checksum & 0xff;
//    printf("hi %u low %u total %lu\n", packet.crcHi, packet.crcLo, checksum);
//  }
//
//  printf("Appended CRC: [%5d bytes]\n", inputNum);
//  for (int i = 0; i < sizeof(packet); i++) {
//    printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : "
//    ");
//  }
//  printf("\n\n");
//
//  FecEncoder<sizeof(packet)> encoder;
//  // borrowed pointer to internal array
//  uint8_t* fec_output = encoder.Encode(input, sizeof(packet));
//
//  auto outsize = 4 * (sizeof(packet) / 2 + 1);
//  printf("Encoded message: [%5d bytes]\n", outsize);
//  for (int i = 0; i < outsize; i++) {
//    printf("%02X%s", fec_output[i],
//           (i % 8 == 7)   ? "\n"
//           : (i % 2 == 1) ? " "
//                          : " ");
//  }
//  printf("\n\n");
//
//  FecDecoder decoder;
//  // uint8_t decoded_data[sizeof(packet)];
//  TestPacket_s rxPacket;
//  decoder.FecDecode(fec_output, reinterpret_cast<uint8_t*>(&rxPacket),
//                    sizeof(packet));
//
//  printf("Decoded packet: [%5d bytes]\n", sizeof(rxPacket));
//  for (unsigned int i = 0; i < sizeof(rxPacket); i++)
//    printf("%02X%s", ((uint8_t*)&rxPacket)[i],
//           (i % 8 == 7)   ? "\n"
//           : (i % 2 == 1) ? " "
//                          : " ");
//  printf("\n\n");
//
//  // Perform CRC check (Optional)
//  {
//    uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
//    for (unsigned int i = 0; i < sizeof(rxPacket.data); i++) {
//      checksum = calculateCRC(rxPacket.data[i], checksum);
//    }
//
//    uint16_t txChecksum = (((uint16_t)rxPacket.crcHi) << 8) | rxPacket.crcLo;
//    printf("hi %u low %u total %lu\n", rxPacket.crcHi, rxPacket.crcLo,
//           txChecksum);
//
//    printf("Local checksum %lu, remote checksum %lu\n", checksum, txChecksum);
//    if (checksum == txChecksum) {
//      // Do something to indicate that the CRC is OK
//      printf("Crc OK!\n");
//    } else {
//      printf("Crc bad!\n");
//    }
//  }
//}
