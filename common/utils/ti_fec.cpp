#include "ti_fec.h"

#include <stdint.h>

#pragma GCC optimize("-Ofast")

namespace ti_fec {
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

uint16_t calculateCRC_array(uint8_t* data, size_t len) {
  uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
  for (size_t i = 0; i < len; i++) {
    checksum = calculateCRC(data[i], checksum);
  }
  return checksum;
}

}  // namespace ti_fec
