/*
 * spi_driver.c
 *
 *  Created on: Jun 2, 2020
 *      Author: John
 */

#include "spi_driver.h"

#ifdef HAL_SPI_MODULE_ENABLED

#define SPI_MAX_DELAY 10  // ms

uint8_t spi_readRegister(SpiCtrl_t *sensor, uint8_t reg) {
  uint8_t rxBuff[2] = {0};
  uint8_t txBuff[2] = {0};
  txBuff[0] = reg;

  // bring CS pin low
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_RESET);

  HAL_SPI_TransmitReceive(sensor->hspi, txBuff, rxBuff, 2, SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_SET);

  // return the result
  return rxBuff[1];
}

void spi_writeRegisters(SpiCtrl_t *sensor, uint8_t reg, uint8_t *pTxData,
                        size_t len) {
  // bring CS pin low
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_RESET);

  HAL_SPI_Transmit(sensor->hspi, &reg, sizeof(reg), SPI_MAX_DELAY);
  HAL_SPI_Transmit(sensor->hspi, pTxData, len, SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_SET);
}

void spi_readRegisters(SpiCtrl_t *sensor, uint8_t reg, uint8_t *pRxData,
                       size_t len) {
  // bring CS pin low
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_RESET);

  HAL_SPI_Transmit(sensor->hspi, &reg, sizeof(reg), SPI_MAX_DELAY);
  HAL_SPI_Receive(sensor->hspi, pRxData, len, SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_SET);
}

void spi_writeRegister(SpiCtrl_t *sensor, uint8_t reg, uint8_t val) {
  uint8_t txBuff[2];
  txBuff[0] = reg;
  txBuff[1] = val;

  // bring CS pin low
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_RESET);

  // send the device the register you want to read:
  // send a value to write
  HAL_SPI_Transmit(sensor->hspi, txBuff, 2, SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_SET);
}

void spi_writeArray(SpiCtrl_t *sensor, uint8_t *pTxData, size_t len) {
  // bring CS pin low
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_RESET);

  // Send whole user buffer
  HAL_SPI_Transmit(sensor->hspi, pTxData, static_cast<uint16_t>(len),
                   SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->port, sensor->pin, GPIO_PIN_SET);
}

#endif  // HAL_SPI_MODULE_ENABLED
