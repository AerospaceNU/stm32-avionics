/*
 * temp_max31855.c
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#include "temp_max31855.h"

#include <math.h>
#include <stdio.h>

#define SPI_TIMEOUT_MS 50
#define K_THERMOCOUPLE_SENSITIVITY 41.276

// Calculates voltage to temperature
// using factors from
// https://www.conaxtechnologies.com/wp-content/uploads/2016/04/EMF_Type_K_Thermocouple_Conversion_Table_Celsius.pdf
static float voltage_to_temperature(float voltage) {
  float temp = 0.1602 * (pow(voltage, 5)) + 2.2934 * (pow(voltage, 4)) +
               11.869 * (pow(voltage, 3)) + 25.009 * (pow(voltage, 2)) +
               46.013 * voltage + 3.8124;
  return temp;
}

// From
// https://github.com/Tuckie/max31855/blob/71b2724f6280a0970ff094290c63e01c889126c3/max31855.py#L111

// Calculate internal raw counts, given the register value
static void convert_thermocouple(TempMax31855Ctrl_s *dev, uint32_t raw) {
  struct {
    int raw_temp : 14;
  } reg;
  reg.raw_temp = raw;
  dev->data.rawThermocouple = reg.raw_temp;
  dev->data.thermocoupleTemp = reg.raw_temp * 0.25;
  float thermocoupleVoltage =
      K_THERMOCOUPLE_SENSITIVITY * dev->data.thermocoupleTemp;
  if (thermocoupleVoltage < 0) {
    // only recalibrate if voltage is negative
    dev->data.thermocoupleTemp =
        voltage_to_temperature(thermocoupleVoltage / 1000);
  }
}

// Calculate internal raw counts, given the register value
static void convert_reference_junction(TempMax31855Ctrl_s *dev, uint32_t raw) {
  struct {
    int raw_temp : 12;
  } reg;
  reg.raw_temp = raw;
  dev->data.rawInternal = reg.raw_temp;
  dev->data.internalTemp = reg.raw_temp * 0.0625;
}

void tempMax31855_read(TempMax31855Ctrl_s *dev) {
  // Read 32 bits from SPI
  uint8_t rxBuff[4] = {0};
  HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET);
  HAL_SPI_Receive(dev->hspi, rxBuff, 4, SPI_TIMEOUT_MS);
  HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET);

  uint32_t d;
  d = rxBuff[0];
  d <<= 8;
  d |= rxBuff[1];
  d <<= 8;
  d |= rxBuff[2];
  d <<= 8;
  d |= rxBuff[3];

  // Fun pointer wizardry to convert to our struct
  Max31855Raw_s *raw = (Max31855Raw_s *)&d;

  // Convert to real numbers
  convert_reference_junction(dev, d >> 4);
  convert_thermocouple(dev, d >> 18);
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
