//
// Created by nathan on 5/15/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GYROSCOPE_GYROSCOPE_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GYROSCOPE_GYROSCOPE_H_

#include <array>

#include "base_device.h"

class Gyroscope : public virtual BaseDevice {
 public:
  Gyroscope();

  std::array<double, 3> getGyroscopeData();

  double getGyroscopeFullScale() const;

 protected:
  double fullScaleGyro = 0;

  std::array<double, 3> gyroscopeData{};
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GYROSCOPE_GYROSCOPE_H_
