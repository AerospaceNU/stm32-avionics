/*
 * dtoa.h
 *
 *  Created on: Jul 6, 2022
 *      Author: sam
 */

#ifndef UTILS_DTOA_H_
#define UTILS_DTOA_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void dtoa(char* out, size_t capacity, double in, int digits);

#ifdef __cplusplus
}
#endif

#endif  // UTILS_DTOA_H_
