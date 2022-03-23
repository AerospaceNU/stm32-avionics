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

#endif  // DEVICES_DATA_STRUCTURES_H_
