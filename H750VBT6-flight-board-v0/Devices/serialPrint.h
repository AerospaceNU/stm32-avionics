#ifndef _SERIAL_PRINT_
#define _SERIAL_PRINT_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "usbd_cdc_if.h"

//uses serial to implement a printf
//Buffer size is 256 characters
#ifdef __cplusplus
extern "C" {
#endif

int debugprintf(const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif
