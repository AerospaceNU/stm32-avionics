/*
 * spi_driver.h
 *
 *  Created on: Jun 2, 2020
 *      Author: John
 */

#ifndef DEVICES_SPI_DRIVER_H_
#define DEVICES_SPI_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config_common.h"

typedef struct SPICtrl_s {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *port;
  uint16_t pin;
} SpiCtrl_t;

uint8_t SPI_ReadRegister(SpiCtrl_t *sensor, uint8_t reg);
void SPI_WriteRegister(SpiCtrl_t *sensor, uint8_t reg, uint8_t val);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_SPI_DRIVER_H_
