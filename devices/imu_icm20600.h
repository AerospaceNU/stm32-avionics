/*
 * imu_icm20600.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef DEVICES_IMU_ICM20600_H_
#define DEVICES_IMU_ICM20600_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config_common.h"
#include "spi_driver.h"

typedef struct ICM20600Ctrl_s {
  ImuData_s data;
  SpiCtrl_t spi;
  uint8_t aFs;
  uint8_t gFs;
  double aRes;
  double gRes;
  int16_t tRawVal; // raw temp

  double tVal;
} ImuICM20600Ctrl_s;

bool icm20600_init(ImuICM20600Ctrl_s *sensor);
void icm20600_getData(ImuICM20600Ctrl_s *sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_IMU_ICM20600_H_
