//
// Created by nathan on 5/16/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BASE_DEVICE_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BASE_DEVICE_H_

class BaseDevice {
 public:
  BaseDevice();

  /**
   * Returns true if the sensor is connected and functioning
   */
  bool getSensorStatus() const;

  virtual void tick();

 protected:
  bool sensorStatus;
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_BASE_DEVICE_H_
