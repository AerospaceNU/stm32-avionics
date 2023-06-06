/*
 * small_strtod.h
 *
 *  Created on: May 11, 2022
 *      Author: matth
 */

#ifndef COMMON_UTILS_SMALL_STRTOD_H_
#define COMMON_UTILS_SMALL_STRTOD_H_

#ifdef __cplusplus
extern "C" {
#endif

double smallStrtod(char* str, char** endptr);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_UTILS_SMALL_STRTOD_H_
