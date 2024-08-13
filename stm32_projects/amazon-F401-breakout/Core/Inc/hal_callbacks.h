/*
 * hal_callbacks.h
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_HAL_CALLBACKS_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_HAL_CALLBACKS_H_

#include <functional>
#include "usart.h"

void halCallbacks_registerUartRxIdleCallback(UART_HandleTypeDef *huart,
											std::function<void(uint16_t)> callback);


#endif  // STM32_PROJECTS_DEVICE_DRIVERS_HAL_CALLBACKS_H_
