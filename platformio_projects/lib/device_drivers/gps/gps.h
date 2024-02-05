//
// Created by nathan on 5/15/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GPS_GPS_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GPS_GPS_H_

#include <Arduino.h>

#include "base_device.h"
#include "data_structures.h"

class Gps : public virtual BaseDevice {
 public:
  Gps();

  bool hasNewFix() const;

  GpsData_s* getLastFix();

  void fillTimeStamp();

  static uint64_t timestampSecondsFromDate(int year, int month, int day,
                                           int hour, int minute, int second);

 protected:
  bool newFix = false;
  GpsData_s gpsData{};

  void onNewFix();
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GPS_GPS_H_
