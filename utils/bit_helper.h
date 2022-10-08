/*
 * bit_helper.h
 *
 *  Created on: Jul 16, 2022
 *      Author: matth
 */

#ifndef UTILS_BIT_HELPER_H_
#define UTILS_BIT_HELPER_H_

#include "stdint.h"
uint8_t set_bit(uint8_t value, uint8_t offset);

uint8_t clear_bit(uint8_t value, uint8_t offset);

uint8_t get_bit(uint8_t value, uint8_t place);

#endif  // UTILS_BIT_HELPER_H_
