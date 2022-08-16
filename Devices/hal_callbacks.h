/*
 * hal_callbacks.h
 */

#ifndef DEVICES_HAL_CALLBACKS_H_
#define DEVICES_HAL_CALLBACKS_H_

#include "board_config_common.h"

#ifdef HAL_SPI_MODULE_ENABLED
void register_HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi,
                                     void (*callback)(void *), void *userData);
void register_HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi,
                                       void (*callback)(void *),
                                       void *userData);
#endif  // HAL_SPI_MODULE_ENABLED

#ifdef HAL_ADC_MODULE_ENABLED
void register_HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc,
                                       void (*callback)(void *),
                                       void *userData);
#endif  // HAL_ADC_MODULE_ENABLED

#ifdef HAL_UART_MODULE_ENABLED
void register_HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart,
                                          void (*callback)(void *),
                                          void *userData);
void register_HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart,
                                      void (*callback)(void *), void *userData);
void register_HAL_UART_RxIdleCallback(UART_HandleTypeDef *huart,
                                      void (*callback)(void *, size_t),
                                      void *userData);
#endif  // HAL_UART_MODULE_ENABLED

#endif  // DEVICES_HAL_CALLBACKS_H_
