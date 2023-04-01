/*
 * imu_iis2mdc.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef DEVICES_IMU_IIS2MDC_H_
#define DEVICES_IMU_IIS2MDC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config_common.h"
#include "i2c.h"

typedef struct IIS2MDCCtrl_s {
  // The most recent sensor reading
  MagData_s data;

  I2C_HandleTypeDef *hi2c;
  uint8_t address;

} ImuIIS2MDCCtrl_s;

bool iis2mdc_init(ImuIIS2MDCCtrl_s *sensor, uint8_t address7bit);
void iis2mdc_getData(ImuIIS2MDCCtrl_s *sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_IMU_IIS2MDC_H_
