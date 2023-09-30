//
// Created by nathan on 9/29/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_LSM6DS3_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_LSM6DS3_H_

#include "imu.h"

class Lsm6ds3 : public Imu {
 public:
  Lsm6ds3();

  void init();

  void tick() override;
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_LSM6DS3_H_
