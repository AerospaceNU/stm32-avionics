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

class MagLis3mdl {
 public:
  bool begin(SpiCtrl_t spidev);
  void newData();

  MagData_s data;

 private:
  SpiCtrl_t spi;
};

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_IMU_MAG_LIS3MDL_H_
