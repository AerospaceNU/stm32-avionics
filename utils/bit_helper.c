#include "bit_helper.h"

uint8_t set_bit(uint8_t value, uint8_t offset) {
  return value | (uint8_t)(1u << offset);
}

uint8_t clear_bit(uint8_t value, uint8_t offset) {
  return value & (uint8_t) ~(1u << offset);
}

uint8_t get_bit(uint8_t value, uint8_t place) {
  uint8_t mask = 1 << place;
  // Mask is 00001000, value is xxxx0xxx for example
  // Result of AND is either >0 or =0
  return (value & mask) > 0 ? 1 : 0;
}

uint16_t combine_to_u16(uint8_t high, uint8_t low) {
  return (uint16_t)(((uint16_t)(high << 8)) | (uint16_t)low);
}
