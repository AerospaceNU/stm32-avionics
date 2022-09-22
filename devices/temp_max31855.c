/*
 * temp_max31855.c
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#include "temp_max31855.h"
#define SPI_TIMEOUT_MS 50

// From
// https://github.com/Tuckie/max31855/blob/71b2724f6280a0970ff094290c63e01c889126c3/max31855.py#L111

// Calculate internal raw counts, given the register value
static void convert_thermocouple(TempMax31855Ctrl_s *dev, Max31855Raw_s *raw) {
  int16_t without_resolution;
  unsigned int tc_data = raw->rawThermocoupleTemp;
  if (tc_data & 0x2000) {
    // Sign extension
    without_resolution = ~tc_data & 0x1FFF;
    without_resolution += 1;
    without_resolution *= -1;
  } else {
    without_resolution = tc_data & 0x1FFF;
  }
  dev->data.rawInternal = without_resolution;
  dev->data.internalTemp = without_resolution * 0.25;
}

// Calculate internal raw counts, given the register value
static void convert_reference_junction(TempMax31855Ctrl_s *dev,
                                       Max31855Raw_s *raw) {
  int16_t without_resolution;
  unsigned int rj_data = raw->rawInternalTemp;
  if (rj_data & 0x800) {
    // Sign extension
    without_resolution = ~rj_data & 0x7FF;
    without_resolution += 1;
    without_resolution *= -1;
  } else {
    without_resolution = rj_data & 0x7FF;
  }
  dev->data.rawInternal = without_resolution;
  dev->data.internalTemp = without_resolution * 0.0625;
}

void tempMax31855_read(TempMax31855Ctrl_s *dev) {
  // Read 32 bits from SPI
  uint8_t rxBuff[4] = {0};
  HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
  HAL_SPI_Recieve(dev->hspi, rxBuff, 4, SPI_TIMEOUT_MS);
  HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);

  // Fun pointer wizardry to convert to our struct
  Max31855Raw_s *raw = (Max31855Raw_s *)rxBuff;

  // Convert to real numbers
  convert_reference_junction(dev, raw);
  convert_thermocouple(dev, raw);
  dev->data.timestamp = HAL_GetTick();

  // Falut bitfield, with contents [fault, open, gnd, vcc] from LSB to MSB
  dev->data.faultFlags = (raw->faulted) | (raw->openCircuit << 1) |
                         (raw->gndFault << 2) | (raw->vccFault << 3);
}

void tempMax31855_init(TempMax31855Ctrl_s *dev, SPI_HandleTypeDef *hspi,
                       GPIO_TypeDef *csPort, uint16_t csPin) {
  dev->hspi = hspi;
  dev->csPort = csPort;
  dev->csPin = csPin;
}
