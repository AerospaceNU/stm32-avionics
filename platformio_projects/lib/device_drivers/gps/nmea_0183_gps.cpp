//
// Created by nathan on 5/16/23.
//

#include "nmea_0183_gps.h"

Nmea0183Gps::Nmea0183Gps() = default;

void Nmea0183Gps::init(HardwareSerial *serial_port, int baud_rate) {
  serialPort = serial_port;

  serialPort->begin(baud_rate);
  sensorStatus = true;
}

void Nmea0183Gps::tick() {
  while (serialPort->available()) {
    tinyGpsPlus.encode(serialPort->read());
  }

  if (tinyGpsPlus.location.isUpdated()) {
    gpsData.generalData.latitude = (float)tinyGpsPlus.location.lat();
    gpsData.generalData.longitude = (float)tinyGpsPlus.location.lng();
    gpsData.generalData.altitude = (float)tinyGpsPlus.altitude.meters();
    gpsData.generalData.satsTracked = tinyGpsPlus.satellites.value();

    gpsData.timeData.year = tinyGpsPlus.date.year();
    gpsData.timeData.month = tinyGpsPlus.date.month();
    gpsData.timeData.day = tinyGpsPlus.date.day();
    gpsData.timeData.hours = tinyGpsPlus.time.hour();
    gpsData.timeData.minutes = tinyGpsPlus.time.minute();
    gpsData.timeData.seconds = tinyGpsPlus.time.second();
    gpsData.timeData.microseconds = tinyGpsPlus.time.centisecond() * 10000;

    fillTimeStamp();

    onNewFix();
  }
}
