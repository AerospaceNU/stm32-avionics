/*
 * SPIDriver.h
 *
 *  Created on: Jun 2, 2020
 *      Author: John
 */

#ifndef DEVICES_SPIDRIVER_H_
#define DEVICES_SPIDRIVER_H_

#include "board_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SPICtrl_s {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *port;
  uint16_t pin;
} SPICtrld_t;

uint8_t SPI_ReadRegister(SPICtrld_t *sensor, uint8_t reg);
void SPI_WriteRegister(SPICtrld_t *sensor, uint8_t reg, uint8_t val);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_SPIDRIVER_H_
