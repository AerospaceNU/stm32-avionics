//
// Created by nathan on 5/16/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_BNO055_IMU_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_BNO055_IMU_H_

#include <Arduino.h>
#include <Wire.h>

#include <new>

#include "Adafruit_BNO055.h"
#include "imu.h"

/**
 * I know the sensor fusion is bad, we're turning that off
 *
 * Its all I have at my house right now
 * I hate it too
 *
 *  - Nathan
 */
class Bno055Imu : public Imu {
 public:
  Bno055Imu();

  void init(TwoWire *wire, uint8_t address);

  void tick() override;

 private:
  uint8_t driver_memory[sizeof(Adafruit_BNO055)]{};
  Adafruit_BNO055 *bno055{};
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_IMU_BNO055_IMU_H_
