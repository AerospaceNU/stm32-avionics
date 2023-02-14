#ifndef DEVICES_BAROMETER_MS5607_H_
#define DEVICES_BAROMETER_MS5607_H_

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
  uint8_t convertState;
  uint8_t rawPres[4];
  uint8_t rawTemp[4];
  uint32_t lastConvReqTime;
} BarometerMs5607Ctrl_s;

bool barometerMs5607_init(BarometerMs5607Ctrl_s *ms5607,
                          SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort,
                          uint16_t csPin);

/**
 * @brief Get data from the MS5607 barometer. Should be called until data
 * collection is complete.
 * @param ms5607: Sensor device
 * @return Whether data collection has completed (true) or is still ongoing
 * (false)
 */
bool barometerMs5607_getData(BarometerMs5607Ctrl_s *ms5607);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_BAROMETER_MS5607_H_
