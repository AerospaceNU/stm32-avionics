#ifndef STM32_PROJECTS_DEVICE_DRIVERS_BAROMETER_MS5607_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_BAROMETER_MS5607_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "board_config_common.h"
#include "data_structures.h"

typedef struct {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *csPort;
  uint16_t csPin;
  BarometerData_s data;
  uint16_t promVals[6];
} BarometerMs5607Ctrl_s;

bool barometerMs5607_init(BarometerMs5607Ctrl_s *ms5607,
                          SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort,
                          uint16_t csPin);
void barometerMs5607_getData(BarometerMs5607Ctrl_s *altCtrl);

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_BAROMETER_MS5607_H_
