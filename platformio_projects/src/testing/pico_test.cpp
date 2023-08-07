/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "ti_fec.h"
#include "Arduino.h"

typedef struct __attribute__((packed)) {
  uint8_t data[48] = {0};
  uint8_t crcHi;
  uint8_t crcLo;
} TestPacket_s;
uint8_t fec_output[104];

void setup() {
  Serial.begin(115200);
}

// Overwrite _write so printf prints to USB
// int _write(int file, char *ptr, int len) { Serial.write(ptr, (size_t)len); }
// int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

char buff[1024];

#pragma GCC optimize("-Ofast")
void loop() {
  uint8_t fec_output[104];

  {
    uint32_t t1 = micros();

    // =============

    // Fill packet with data
    TestPacket_s packet;
    uint8_t packet_str[] = {3, 1, 2, 3};
    memcpy(packet.data, packet_str, sizeof(packet_str));
    size_t inputNum = sizeof(TestPacket_s);
    uint8_t *input = (uint8_t *)&packet;

    // Generate CRC
    {
      uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
      for (int i = 0; i < sizeof(packet.data); i++) {
        checksum = calculateCRC(packet.data[i], checksum);
      }

      packet.crcHi = checksum >> 8;
      packet.crcLo = checksum & 0xff;
    }

    FecEncoder<sizeof(packet)> encoder;
    // borrowed pointer to internal array
    memcpy(fec_output, encoder.Encode(input, sizeof(packet)),
           sizeof(fec_output));

    // =============

    uint32_t t2 = micros();
    snprintf(buff, sizeof(buff), "Tx dt: %lu us\n", t2 - t1);
    Serial.print(buff);
  }

  buff[3] = 0xde;
  buff[4] = 0xad;

  {
    uint32_t t1 = micros();

    // =============

    FecDecoder decoder;
    // uint8_t decoded_data[sizeof(packet)];
    TestPacket_s rxPacket;
    decoder.FecDecode(fec_output, reinterpret_cast<uint8_t *>(&rxPacket),
                      sizeof(rxPacket));

    uint32_t t2 = micros();

    snprintf(buff, sizeof(buff), "Decoded packet: [%5d bytes]\n",
             sizeof(rxPacket));
    Serial.print(buff);
    for (int i = 0; i < sizeof(rxPacket); i++) {
      snprintf(buff, sizeof(buff), "%02X%s", ((uint8_t *)&rxPacket)[i],
               (i % 8 == 7)   ? "\n"
               : (i % 2 == 1) ? " "
                              : " ");
      Serial.print(buff);
    }
    snprintf(buff, sizeof(buff), "\n\n");
    Serial.print(buff);

    // Perform CRC check (Optional)
    {
      uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
      for (int i = 0; i < sizeof(rxPacket.data); i++) {
        checksum = calculateCRC(rxPacket.data[i], checksum);
      }

      uint16_t txChecksum = (((uint16_t)rxPacket.crcHi) << 8) | rxPacket.crcLo;
      snprintf(buff, sizeof(buff), "hi %u low %u total %lu\n", rxPacket.crcHi,
               rxPacket.crcLo, txChecksum);
      Serial.print(buff);

      snprintf(buff, sizeof(buff), "Local checksum %lu, remote checksum %lu\n",
               checksum, txChecksum);
      Serial.print(buff);
      if (checksum == txChecksum) {
        // Do something to indicate that the CRC is OK
        snprintf(buff, sizeof(buff), "Crc OK!\n");
        Serial.print(buff);
      } else {
        snprintf(buff, sizeof(buff), "Crc bad!\n");
        Serial.print(buff);
      }
    }

    // =============

    snprintf(buff, sizeof(buff), "Rx dt: %lu ms\n", t2 - t1);
    Serial.print(buff);
  }

  delay(1000);
}
