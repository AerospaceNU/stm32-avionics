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
#include "spi_driver.h"

typedef struct IIS2MDCCtrl_s {
  // The most recent sensor reading
  MagData_s data;

  SpiCtrl_t spi;
  uint8_t aFs;
  uint8_t gFs;
  double aRes;
  double gRes;
  int16_t tRawVal; // raw temp

  double tVal;
} ImuIIS2MDCCtrl_s;

bool iis2mdc_init(ImuIIS2MDCCtrl_s *sensor);
void iis2mdc_getData(ImuIIS2MDCCtrl_s *sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_IMU_IIS2MDC_H_
