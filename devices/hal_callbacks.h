/*
 * hal_callbacks.h
 */

#ifndef DEVICES_HAL_CALLBACKS_H_
#define DEVICES_HAL_CALLBACKS_H_

#include "board_config_common.h"
#include "board_config.h"

#ifdef HAL_SPI_MODULE_ENABLED
void halCallbacks_registerSpiTxCpltCallback(SPI_HandleTypeDef *hspi,
                                            void (*callback)(void *),
                                            void *userData);
void halCallbacks_registerSpiTxRxCpltCallback(SPI_HandleTypeDef *hspi,
                                              void (*callback)(void *),
                                              void *userData);
#endif  // HAL_SPI_MODULE_ENABLED

#ifdef HAL_ADC_MODULE_ENABLED
void halCallbacks_registerAdcConvCpltCallback(ADC_HandleTypeDef *hadc,
                                              void (*callback)(void *),
                                              void *userData);
#endif  // HAL_ADC_MODULE_ENABLED

#ifdef HAL_UART_MODULE_ENABLED
void halCallbacks_registerUartRxHalfCpltCallback(UART_HandleTypeDef *huart,
                                                 void (*callback)(void *),
                                                 void *userData);
void halCallbacks_registerUartRxCpltCallback(UART_HandleTypeDef *huart,
                                             void (*callback)(void *),
                                             void *userData);
void halCallbacks_registerUartRxIdleCallback(UART_HandleTypeDef *huart,
                                             void (*callback)(void *, size_t),
                                             void *userData);
#endif  // HAL_UART_MODULE_ENABLED

#ifdef HAL_EXTI_MODULE_ENABLED
void halCallbacks_registerExtInterruptCallback(uint16_t gpioPin,
                                               void (*callback)(void *),
                                               void *userData);
#endif  // HAL_EXTI_MODULE_ENABLED
#ifdef HAL_TIM_MODULE_ENABLED

void halCallbacks_registerTimPeriodElapsedCallback(TIM_HandleTypeDef *htim,
                                                   void (*callback)(void *),
                                                   void *userData);
#endif  // HAL_TIM_MODULE_ENABLED

#ifdef USB_IS_COMPOSITE
void halCallbacks_registerUsbCdcReceiveCallback(
    uint8_t cdc_ch, void (*callback)(void *, uint8_t, uint8_t *, uint32_t *),
    void *userData);
void halCallbacks_usbCdcReceive(uint8_t cdc_ch, uint8_t *buf, uint32_t *len);
#endif  // USB_IS_COMPOSITE

#endif  // DEVICES_HAL_CALLBACKS_H_
