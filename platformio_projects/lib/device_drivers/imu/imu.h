//
// Created by nathan on 5/15/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_IMU_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_IMU_H_

#include <array>

#include "accelerometer/accelerometer.h"
#include "base_device.h"
#include "gyroscope/gyroscope.h"
#include "magnetometer/magnetometer.h"

class Imu : public Accelerometer, public Gyroscope, public Magnetometer {
 public:
  Imu();
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_IMU_H_
