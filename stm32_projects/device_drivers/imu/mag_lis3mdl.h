/*
 * accel_adx375.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_IMU_MAG_LIS3MDL_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_IMU_MAG_LIS3MDL_H_

#include <math.h>

#include "board_config_common.h"
#include "spi_driver.h"

#define G_TO_MPS2(x) x * 9.80665

class MagLis3mdl {
 public:
  explicit MagLis3mdl(SpiCtrl_t spidev);
  bool begin();
  void newData();

  MagData_s data;

 private:
  SpiCtrl_t spi;
};

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_IMU_MAG_LIS3MDL_H_
