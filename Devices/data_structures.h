/*
 * data_structures.h
 *
 *  Created on: Mar 7, 2022
 *      Author: matth
 */

#ifndef DEVICES_DATA_STRUCTURES_H_
#define DEVICES_DATA_STRUCTURES_H_

#include <stdbool.h>
#include <stdint.h>

#define RADIO_MAX_LEN 128
typedef struct __attribute__((packed)) {
  uint8_t data[RADIO_MAX_LEN];
  uint8_t radioId;
  int8_t rssi;
  bool crc;
  uint8_t lqi;
} RecievedPacket_t;

typedef enum { AXIS_X = 0, AXIS_Y, AXIS_Z } Axis_t;

typedef struct {
  Axis_t axis;
  int8_t direction;
} Orientation_t;

typedef struct __attribute__((packed)) {
  int16_t x;
  int16_t y;
  int16_t z;
} Axis3dRaw_t;

typedef struct __attribute__((packed)) {
  double x;
  double y;
  double z;
} Axis3dReal_t;

typedef struct __attribute__((packed)) {
  Axis3dRaw_t raw;
  Axis3dReal_t realMps2;
} AccelData_t;

typedef struct __attribute__((packed)) {
  double temperatureC;
  double pressureAtm;
} BarometerData_t;

typedef struct __attribute__((packed)) {
  float latitude;
  float longitude;
  float altitude;
  float speedKnots;
  float courseDeg;
  float latitudeDeviation;
  float longitudeDeviation;
  float altitudeDeviation;
  float speedKph;
  float speedKnots2;
  uint64_t timestamp;
  int seconds;
  int microseconds;
  int minutes;
  int hours;
  int day;
  int month;
  int year;
  int num_sats;
  char status;
} GpsData_t;

typedef struct __attribute__((packed)) {
  Axis3dRaw_t accel;
  Axis3dRaw_t angVel;
  Axis3dRaw_t mag;
} ImuDataRaw_t;

typedef struct __attribute__((packed)) {
  Axis3dRaw_t accelRaw;
  Axis3dReal_t accelRealMps2;
  Axis3dRaw_t angVelRaw;
  Axis3dReal_t angVelRealRadps;
  Axis3dRaw_t magRaw;
  Axis3dReal_t magRealG;
} ImuData_t;

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
} LineCutterData_t;

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
} LineCutterFlightVars_t;

#endif  // DEVICES_DATA_STRUCTURES_H_
