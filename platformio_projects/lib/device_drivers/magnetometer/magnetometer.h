//
// Created by nathan on 5/15/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_MAGNETOMETER_MAGNETOMETER_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_MAGNETOMETER_MAGNETOMETER_H_

#include <array>

#include "base_device.h"

class Magnetometer : public virtual BaseDevice {
 public:
  Magnetometer();

  std::array<double, 3> getMagnetometerData();

  double getMagnetometerFullScale() const;

 protected:
  double fullScaleMagnetometer = 0;

  std::array<double, 3> magnetometerData{};
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_MAGNETOMETER_MAGNETOMETER_H_
