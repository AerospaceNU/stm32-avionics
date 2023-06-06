//
// Created by nathan on 5/15/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_ACCELEROMETER_ACCELEROMETER_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_ACCELEROMETER_ACCELEROMETER_H_

#include <array>

#include "base_device.h"

class Accelerometer : public virtual BaseDevice {
 public:
  Accelerometer();

  std::array<double, 3> getAccelerometerData();

  double getAccelerometerFullScale() const;

 protected:
  double fullScaleAccelerometer = 0;

  std::array<double, 3> accelerometerData{};
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_ACCELEROMETER_ACCELEROMETER_H_
