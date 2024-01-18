//
// Created by nathan on 5/14/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_NEW_HAL_DEVICE_MANAGER_H_
#define PLATFORMIO_PROJECTS_LIB_NEW_HAL_DEVICE_MANAGER_H_

#include <Arduino.h>
#include <Array/Array.h>
#include <board_config_common.h>

#include "accelerometer/accelerometer.h"
#include "barometer/barometer.h"
#include "base_device.h"
#include "gps/gps.h"
#include "gyroscope/gyroscope.h"
#include "imu/imu.h"
#include "magnetometer/magnetometer.h"
#include "radio/radio.h"

// TODO Could this be better?
//Maybe device manager could be templated?
#define MAX_ACCELEROMETERS 2
#define MAX_BAROMETERS 2
#define MAX_GPS 2
#define MAX_GYROSCOPES 2
#define MAX_IMUS 2
#define MAX_MAGNETOMETERS 2
#define MAX_RADIOS 2

#define MAX_DEVICES (MAX_ACCELEROMETERS + MAX_BAROMETERS + MAX_GPS + MAX_GYROSCOPES + MAX_IMUS + MAX_MAGNETOMETERS + MAX_RADIOS)

// TODO: Patrick or someone, is there a way to template this class to make it
// easier to add more sensor types later?

class DeviceManager {
 public:
  // Driver objects
  Array<Accelerometer *, MAX_ACCELEROMETERS> accelerometers{};
  Array<Barometer *, MAX_BAROMETERS> barometers{};
  Array<Gps *, MAX_GPS> gps{};
  Array<Gyroscope *, MAX_GYROSCOPES> gyroscopes{};
  //  Array<Imu *, MAX_IMUS> imus{};
  Array<Magnetometer *, MAX_MAGNETOMETERS> magnetometers{};
  Array<Radio *, MAX_RADIOS> radios{};

  // Also store all the driver objects as an array of base class pointers
  // IDK how useful this really is, but we'll see
  Array<BaseDevice *, MAX_DEVICES> devices{};

  DeviceManager();

  bool addBatteryMonitorAdc(int pin, double multiplier);

  bool addRadio(Radio *radio);

  bool addBarometer(Barometer *barometer);

  bool addGps(Gps *gps);

  bool addImu(Imu *imu, bool has_mag = false);

  bool addMagnetometer(Magnetometer *mag);

  void init();

  void readSensors();

  uint getNumberBatteryMonitorAdcs() const;

  double readBatteryVoltage(uint battery_num);

  Accelerometer *getAccelerometer(uint index);

  Barometer *getBarometer(uint index);

  Gps *getGps(uint index);

  Gyroscope *getGyroscope(uint index);

  Magnetometer *getMagnetometer(uint index);

  Radio *getRadio(uint index);

  //  Imu *getImu(int index);

 protected:
  // ADC battery monitor stuff
  uint numBatteryMonitorADCs = 0;
  std::array<int, NUM_VBAT> batteryMonitorADCPins{};
  std::array<double, NUM_VBAT> batteryAdcMultipliers{};
};

#endif  // PLATFORMIO_PROJECTS_LIB_NEW_HAL_DEVICE_MANAGER_H_
