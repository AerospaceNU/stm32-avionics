/*
 * spi_driver.c
 *
 *  Created on: Jun 2, 2020
 *      Author: John
 */

#include "spi_driver.h"

#ifdef HAL_SPI_MODULE_ENABLED

#define SPI_MAX_DELAY 10  // ms

uint8_t SPI_ReadRegister(SpiCtrl_t *sensor, uint8_t reg) {
  uint8_t rxBuff[2] = {0};
  uint8_t txBuff[2] = {0};
  txBuff[0] = reg;

  // bring CS pin low
  HAL_GPIO_WritePin(sensor->port, sensor->pin, 0);

  HAL_SPI_TransmitReceive(sensor->hspi, txBuff, rxBuff, 2, SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->port, sensor->pin, 1);

  // return the result
  return rxBuff[1];
}

void SPI_WriteRegister(SpiCtrl_t *sensor, uint8_t reg, uint8_t val) {
  uint8_t txBuff[2];
  txBuff[0] = reg;
  txBuff[1] = val;

  // bring CS pin low
  HAL_GPIO_WritePin(sensor->port, sensor->pin, 0);

  // send the device the register you want to read:
  // send a value to write
  HAL_SPI_Transmit(sensor->hspi, txBuff, 2, SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->port, sensor->pin, 1);
}

#endif  // HAL_SPI_MODULE_ENABLED
