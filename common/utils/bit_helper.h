/*
 * bit_helper.h
 *
 *  Created on: Jul 16, 2022
 *      Author: matth
 */

#ifndef COMMON_UTILS_BIT_HELPER_H_
#define COMMON_UTILS_BIT_HELPER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t set_bit(uint8_t value, uint8_t offset);

uint8_t clear_bit(uint8_t value, uint8_t offset);

uint8_t get_bit(uint8_t value, uint8_t place);

uint16_t combine_to_u16(uint8_t high, uint8_t low);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_UTILS_BIT_HELPER_H_
