#include "bit_helper.h"

uint8_t set_bit(uint8_t value, uint8_t offset) { return value | (1 << offset); }

uint8_t clear_bit(uint8_t value, uint8_t offset) {
  return value & ~(1 << offset);
}

uint8_t get_bit(uint8_t value, uint8_t place) {
  uint8_t mask = 1 << place;
  // Mask is 00001000, value is xxxx0xxx for example
  // Result of AND is either >0 or =0
  return (value & mask) > 0 ? 1 : 0;
}
