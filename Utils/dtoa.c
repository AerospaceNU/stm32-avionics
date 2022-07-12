/*
 * dtoa.c
 *
 *  Created on: Jul 6, 2022
 *      Author: sam
 */

#include "dtoa.h"

#include <math.h>
#include <stdio.h>

void dtoa(char* out, size_t capacity, double in, int digits) {
  snprintf(out, capacity, "%d.%0*d", (int)in, digits,
           (int)(fabs(fmod(in, 1.0) * pow(10, digits))));
}
