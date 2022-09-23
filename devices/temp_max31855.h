/*
 * temp_max31855.h
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#ifndef TEMP_MAX31855_H_
#define TEMP_MAX31855_H_

#include "board_config.h"
#include HAL_HEADER

#include "stdbool.h"

typedef struct {
	bool openCircuit : 1;
	bool gndFault: 1;
	bool vccFault: 1;
	uint8_t res: 1; // Reserved
	unsigned int rawInternalTemp: 12;
	bool faulted : 1;
	uint8_t res2: 1; // Reserved
	unsigned int rawThermocoupleTemp: 14;
} Max31855Raw_s;

typedef struct __attribute__((packed)) {
	// In MS since boot
	uint32_t timestamp;
	// Bitfield, with contents [fault, open, gnd, vcc]
	uint8_t faultFlags;

	// In 0.25c increments
	int16_t rawInternal;
	int16_t rawThermocouple;

	// In degrees c
	float internalTemp;
	float thermocoupleTemp;
} Max31855Data_s;

typedef struct {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *csPort;
  uint16_t csPin;
  Max31855Data_s data;
} TempMax31855Ctrl_s;

void tempMax31855_init(TempMax31855Ctrl_s *dev,
                          SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort,
                          uint16_t csPin);
void tempMax31855_read(TempMax31855Ctrl_s *dev);

#endif /* TEMP_MAX31855_H_ */
