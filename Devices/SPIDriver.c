/*
 * SPIDriver.c
 *
 *  Created on: Jun 2, 2020
 *      Author: John
 */

#include "SPIDriver.h"
#include <string.h>

uint8_t SPI_ReadRegister(SPICtrld_t* sensor, uint8_t reg) {
	uint8_t rxBuff[2] = {0};
	uint8_t txBuff[2] = {0};
	txBuff[0] = reg;

	// bring CS pin low
	HAL_GPIO_WritePin(sensor->port, sensor->pin, 0);

	HAL_SPI_TransmitReceive(sensor->hspi, txBuff, rxBuff, 2, HAL_MAX_DELAY);

	// bring CS pin high
	HAL_GPIO_WritePin(sensor->port, sensor->pin, 1);

	// return the result
	return rxBuff[1];
}

//! Read an array over SPI, given an array pData of length len. Assumes a maximum size of 255 bytes.
bool SPI_ReadArray(SPICtrld_t* sensor, uint8_t reg, uint8_t* pData, uint8_t len) {
	uint8_t rxBuff[len + 1];
	uint8_t txBuff[len + 1];
	memset(txBuff, 0, len + 1);
	// We don't technically need to clear rxBuff, as spi should fill it for us?
	txBuff[0] = reg;

	// bring CS pin low
	HAL_GPIO_WritePin(sensor->port, sensor->pin, 0);

	HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive(sensor->hspi, txBuff, rxBuff, 2, HAL_MAX_DELAY);

	// bring CS pin high
	HAL_GPIO_WritePin(sensor->port, sensor->pin, 1);

	if(ret != HAL_OK) return false;

	// Copy all but the first byte (the instruction) out of rxBuffer
	// rxBuff is len-plus-one long, so we copy out len
	// pData is assumed to have length len
	memcpy(pData, rxBuff + 1, len);
	return true;
}

void SPI_WriteRegister(SPICtrld_t* sensor, uint8_t reg, uint8_t val) {
	uint8_t txBuff[2];
	txBuff[0] = reg;
	txBuff[1] = val;

	// bring CS pin low
	HAL_GPIO_WritePin(sensor->port, sensor->pin, 0);

	// send the device the register you want to read:
	// send a value to write
	HAL_SPI_Transmit(sensor->hspi, txBuff, 2, HAL_MAX_DELAY);

	// bring CS pin high
	HAL_GPIO_WritePin(sensor->port, sensor->pin, 1);
}
