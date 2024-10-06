//
// Created by nathan on 5/15/23.
//

#include "gps.h"

#include <ctime>

Gps::Gps() = default;

bool Gps::hasNewFix() const { return newFix; }

GpsData_s *Gps::getLastFix() {
  newFix = false;
  return &gpsData;
}

void Gps::fillTimeStamp() {
  gpsData.timeData.timestamp = timestampSecondsFromDate(
      gpsData.timeData.year, gpsData.timeData.month, gpsData.timeData.day,
      gpsData.timeData.hours, gpsData.timeData.minutes,
      gpsData.timeData.seconds);
}

uint64_t Gps::timestampSecondsFromDate(int year, int month, int day, int hour,
                                       int minute, int second) {
  struct tm t{};
  memset(&t, 0, sizeof(tm));  // Initialize to all 0's
  t.tm_year = year - 1900;    // This is year-1900, so 112 = 2012
  t.tm_mon = month - 1;       // January is 0
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = second;
  time_t time_since_epoch = mktime(&t);

  return time_since_epoch;
}

void Gps::onNewFix() { newFix = true; }
