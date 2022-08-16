/*
 * accel_h3lis331dl.h
 *
 *  Created on: May 26, 2020
 *      Author: John
 */

#ifndef DEVICES_ACCEL_H3LIS331DL_H_
#define DEVICES_ACCEL_H3LIS331DL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config_common.h"
#include "data_structures.h"
#include "spi_driver.h"

typedef struct {
  SpiCtrl_t spi;
  Axis3dReal_t adjVal;
  double gain;
  AccelData_t val;
} AccelH3lis331dlCtrl_t;

bool accelH3lis331dl_init(AccelH3lis331dlCtrl_t *sensor, SpiCtrl_t spi);
void accelH3lis331dl_getData(AccelH3lis331dlCtrl_t *sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_ACCEL_H3LIS331DL_H_
