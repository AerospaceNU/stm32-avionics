#ifndef _SERIAL_PRINT_
#define _SERIAL_PRINT_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "usart.h"

//uses serial to implement a printf
//Buffer size is 256 characters
#ifdef __cplusplus
extern "C" {
#endif

int uartprintf(const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif
