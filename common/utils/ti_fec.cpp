#include "ti_fec.h"

#include <stdint.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <queue>
#include <random>


static uint16_t calculateCRC(uint8_t crcData, uint16_t crcReg) {
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


typedef struct __attribute__((packed)) {
  uint8_t data[4];
  uint8_t crcHi;
  uint8_t crcLo;
} TestPacket_s;

int main() {
  TestPacket_s packet;

  uint8_t packet_str[] = {3, 1, 2, 3};
  memcpy(packet.data, packet_str, sizeof(packet_str));
  size_t inputNum = sizeof(TestPacket_s);
  uint8_t* input = (uint8_t*)&packet;

  printf("Input: [%5d bytes]\n", sizeof(packet.data));
  for (int i = 0; i < sizeof(packet.data); i++)
    printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  printf("\n\n");

  // Generate CRC
  if (true) {
    uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
    for (int i = 0; i <= sizeof(packet.data); i++) {
      checksum = calculateCRC(packet.data[i], checksum);
    }

    packet.crcHi = checksum >> 8;
    packet.crcLo = checksum & 0xff;
  }

  printf("Appended CRC: [%5d bytes]\n", inputNum);
  for (int i = 0; i < sizeof(packet); i++) {
    printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  }
  printf("\n\n");

  FecEncoder<sizeof(packet)> encoder;
  // borrowed pointer to internal array
  uint8_t* fec_output = encoder.Encode(input);

  printf("Encoded message: [%5d bytes]\n", encoder.outputSize);
  for (int i = 0; i < encoder.outputSize; i++) {
    printf("%02X%s", fec_output[i],
           (i % 8 == 7)   ? "\n"
           : (i % 2 == 1) ? " "
                          : " ");
  }
  printf("\n\n");

  FecDecoder decoder;
  uint8_t decoded_data[sizeof(packet)];
  decoder.FecDecode(fec_output, decoded_data, sizeof(packet));

  printf("Decoded packet: [%5d bytes]\n", sizeof(decoded_data));
  for (int i = 0; i < sizeof(decoded_data); i++)
    printf("%02u%s", decoded_data[i],
           (i % 8 == 7)   ? "\n"
           : (i % 2 == 1) ? " "
                          : " ");
  printf("\n\n");

  // Perform CRC check (Optional)
  {
    unsigned short i;
    auto nBytes = inputNum;
    uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
    for (i = 0; i < sizeof(packet.data); i++)
      checksum = calculateCRC(decoded_data[i], checksum);

    auto txChecksum = (input[inputNum - 2] << 8) || (input[inputNum - 1]);

    printf("Local checksum %lu, remote checksum %lu\n", checksum, txChecksum);
    if (checksum == txChecksum) {
      // Do something to indicate that the CRC is OK
      printf("Crc OK!\n");
    } else {
      printf("Local checksum %lu != remote checksum %lu\n", checksum,
             txChecksum);
    }
  }
}