/*
 * temp_max31855.h
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#ifndef DEVICES_TEMP_MAX31855_H_
#define DEVICES_TEMP_MAX31855_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "board_config_common.h"

typedef struct {
  bool openCircuit : 1;
  bool gndFault : 1;
  bool vccFault : 1;
  uint8_t res : 1;  // Reserved
  int rawInternalTemp : 12;
  bool faulted : 1;
  uint8_t res2 : 1;  // Reserved
  int rawThermocoupleTemp : 14;
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

void tempMax31855_init(TempMax31855Ctrl_s *dev, SPI_HandleTypeDef *hspi,
                       GPIO_TypeDef *csPort, uint16_t csPin);
void tempMax31855_read(TempMax31855Ctrl_s *dev);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_TEMP_MAX31855_H_
