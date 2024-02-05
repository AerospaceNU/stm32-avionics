//
// Created by nathan on 5/14/23.
//

#ifndef COMMON_GROUNDSTATION_GROUNDSTATION_H_
#define COMMON_GROUNDSTATION_GROUNDSTATION_H_

#include <board_config_common.h>
#include <circular_buffer.h>
#include <data_structures.h>
#include <hardware_manager.h>
#include <tasks/radio_manager.h>

#include <cstdint>

#define CHANNEL_COMMAND_ID 1
#define RADIO_HARDWARE_COMMAND_ID 2

typedef enum {
  RAD_433 = FIRST_ID_RADIO_TI_433,
  RAD_915 = FIRST_ID_RADIO_TI_915,
  GROUNDSTATION = 0xff
} MessageDestination_e;

typedef struct __attribute__((__packed__)) {
  uint8_t packetType;
  float latitude;
  float longitude;
  float gps_alt;
  double groundPressure;
  double groundTemp;
} HeartbeatData_s;

typedef struct __attribute__((packed)) {
  uint8_t destination;
  uint16_t len;
  uint8_t data[256];
} GroundstationUsbCommand_s;

class Groundstation {
 public:
  Groundstation();

  void init();

  void runOnce();

 protected:
  CircularBuffer_s* buffer{};

  uint32_t start = 0;
};

#endif  // COMMON_GROUNDSTATION_GROUNDSTATION_H_
