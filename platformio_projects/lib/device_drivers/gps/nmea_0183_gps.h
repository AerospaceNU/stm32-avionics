//
// Created by nathan on 5/16/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GPS_NMEA_0183_GPS_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GPS_NMEA_0183_GPS_H_

#include <Arduino.h>
#include <TinyGPS++.h>

#include "gps.h"

class Nmea0183Gps : public Gps {
 public:
  Nmea0183Gps();

  void init(Uart *serial_port, int baud_rate);

  void tick() override;

 protected:
  Uart *serialPort = nullptr;

  TinyGPSPlus tinyGpsPlus;
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_GPS_NMEA_0183_GPS_H_
