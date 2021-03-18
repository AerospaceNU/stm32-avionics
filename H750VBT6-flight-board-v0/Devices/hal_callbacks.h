/*
 * hal_callbacks.h
 */

#ifndef HAL_CALLBACKS_H_
#define HAL_CALLBACKS_H_

#include "stm32h7xx_hal.h"

void register_HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi, void (*callback)(void *), void *userData);
void register_HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi, void (*callback)(void *), void *userData);
void register_HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc, void(*callback)(void *), void *userData);

#endif /* HAL_CALLBACKS_H_ */
