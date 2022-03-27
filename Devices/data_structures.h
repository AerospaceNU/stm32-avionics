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

#endif  // DEVICES_DATA_STRUCTURES_H_
