#ifndef STM32_PROJECTS_DEVICE_DRIVERS_GPS_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_GPS_H_

#include <stdbool.h>

#include "data_structures.h"
#include "bluetooth_uart.h"

void bluetooth_uart_init();

void cli_bleCallback(void*, uint8_t* pdata, size_t len);

/**
 * @brief Retrieve circular RX buffer
 * @return Pointer to circular buffer
 */
CircularBuffer_s* bleUart_getRxBuffer();

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_GPS_H_
