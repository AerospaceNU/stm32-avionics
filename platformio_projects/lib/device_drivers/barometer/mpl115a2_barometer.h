//
// Created by nathan on 5/16/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BAROMETER_MPL115A2_BAROMETER_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BAROMETER_MPL115A2_BAROMETER_H_

#include <Adafruit_MPL115A2.h>

#include "barometer.h"

class Mpl115a2Barometer : public Barometer {
 public:
  Mpl115a2Barometer();

  void init();

  void tick() override;

 private:
  Adafruit_MPL115A2 mpl115A2;
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BAROMETER_MPL115A2_BAROMETER_H_
