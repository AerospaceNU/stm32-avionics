/*
 * dtoa.h
 *
 *  Created on: Jul 6, 2022
 *      Author: sam
 */

#ifndef COMMON_UTILS_DTOA_H_
#define COMMON_UTILS_DTOA_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void dtoa(char* out, size_t capacity, double in, int digits);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_UTILS_DTOA_H_
