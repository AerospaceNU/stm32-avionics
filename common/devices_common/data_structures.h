/*
 * data_structures.h
 *
 *  Created on: Mar 7, 2022
 *      Author: matth
 */

#ifndef COMMON_DEVICES_COMMON_DATA_STRUCTURES_H_
#define COMMON_DEVICES_COMMON_DATA_STRUCTURES_H_

#include <stdbool.h>
#include <stdint.h>

#define RADIO_MAX_LEN 128
typedef struct __attribute__((packed)) {
  uint8_t data[RADIO_MAX_LEN];
  uint8_t radioId;
  int8_t rssi;
  bool crc;
  uint8_t lqi;
} RadioRecievedPacket_s;

typedef enum { AXIS_X = 0, AXIS_Y, AXIS_Z } Axis_e;

typedef struct {
  Axis_e axis;
  int8_t direction;
} Orientation_s;

typedef struct __attribute__((packed)) {
  int16_t x;
  int16_t y;
  int16_t z;
} Axis3dRaw_s;

typedef struct __attribute__((packed)) {
  double x;
  double y;
  double z;
} Axis3dReal_s;

typedef struct __attribute__((packed)) {
  Axis3dRaw_s raw;
  Axis3dReal_s realMps2;
} AccelData_s;

typedef struct __attribute__((packed)) {
  Axis3dRaw_s raw;
  Axis3dReal_s realGauss;
} MagData_s;

typedef struct __attribute__((packed)) {
  double temperatureC;
  double pressureAtm;
} BarometerData_s;

typedef struct __attribute__((packed)) {
  float latitude;
  float longitude;
  float altitude;
  uint8_t fixQuality;
  uint8_t satsTracked;
  float hdop;
} GpsGeneralData_s;

typedef struct __attribute__((packed)) {
  uint64_t timestamp;
  int seconds;
  int microseconds;
  int minutes;
  int hours;
  int day;
  int month;
  int year;
} GpsTimeData_s;

typedef struct __attribute__((packed)) {
  GpsGeneralData_s generalData;
  GpsTimeData_s timeData;
} GpsData_s;

typedef struct __attribute__((packed)) {
  Axis3dRaw_s accel;
  Axis3dRaw_s angVel;
  //  Axis3dRaw_s mag;
} ImuDataRaw_s;

// IMU Sensor Reading Data struct
typedef struct __attribute__((packed)) {
  // Raw acceleration data from IMU
  Axis3dRaw_s accelRaw;
  // Acceleration converted to meters per seconds squared
  Axis3dReal_s accelRealMps2;
  // Raw angular velocity data from IMU
  Axis3dRaw_s angVelRaw;
  // Angular velocity data converted to radians per second
  Axis3dReal_s angVelRealRadps;
  //  Axis3dRaw_s magRaw;
  //  Axis3dReal_s magRealG;
} ImuData_s;

// The data struct from the line cutter
typedef struct __attribute__((__packed__)) {
  uint8_t lineCutterNumber;  // ID programmed into the line cutter. Maybe CRC of
                             // name?
  uint8_t state;
  uint32_t timestamp;
  uint32_t pressure;
  float altitude;
  float deltaAltitude;
  float temperature;
  float accelNorm;
  float battery;
  uint16_t cutSense1;
  uint16_t cutSense2;
  uint16_t currentSense;
  uint16_t photoresistor;
} LineCutterData_s;

// For writing data to flash & flight variable log
typedef struct __attribute__((packed)) {
  uint8_t lineCutterNumber;  // See above
  float limitVel;
  uint16_t altitude1;
  uint16_t altitude2;
  uint32_t disreefDelay1;
  uint32_t disreefDelay2;
  float pwmVoltage1;
  float pwmVoltage2;
  uint16_t pwmDuration;
  uint16_t lightThreshold;
  uint32_t lightTriggerTime;
  uint32_t seaLevelPressure;
} LineCutterFlightVars_s;

#endif  // COMMON_DEVICES_COMMON_DATA_STRUCTURES_H_
