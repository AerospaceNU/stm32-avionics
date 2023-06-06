//
// Created by nathan on 5/15/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BAROMETER_BAROMETER_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BAROMETER_BAROMETER_H_

#include "base_device.h"

class Barometer : public virtual BaseDevice {
 public:
  Barometer();

  double getPressureAtm() const;

  double getTemperatureC() const;

 protected:
  double pressureAtm = 0;
  double temperatureC = 0;
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BAROMETER_BAROMETER_H_
