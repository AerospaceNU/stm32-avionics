/*
 * SPIDriver.h
 *
 *  Created on: Jun 2, 2020
 *      Author: John
 */

#ifndef SRC_SPIDRIVER_H_
#define SRC_SPIDRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include <stdbool.h>

typedef struct SPICtrl_s{
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* port;
	uint16_t pin;
}SPICtrld_t;

uint8_t SPI_ReadRegister(SPICtrld_t* sensor, uint8_t reg);
bool SPI_WriteRegister(SPICtrld_t* sensor, uint8_t reg, uint8_t val);
bool SPI_ReadArray(SPICtrld_t* sensor, uint8_t reg, uint8_t* pData, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* SRC_SPIDRIVER_H_ */
