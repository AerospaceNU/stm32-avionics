/*
 * radio_manager.h
 */

#ifndef SYSTEM_RADIO_MANAGER_H_
#define SYSTEM_RADIO_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_config_common.h"
#include "circular_buffer.h"
#include "data_structures.h"
#include "filters.h"
#include "hardware_manager.h"
#include "radio_packet_types.h"

// Struct to keep track of when messages were last sent
typedef struct {
  uint32_t propStuffLastSent;
  uint32_t orientationLastSent;
  uint32_t positionLastSent;
  uint32_t lineCutterLastSent;
  uint32_t lineCutterVarsLastSent;
  uint32_t altInfoLastSent;
  uint32_t pyroInfoLastSent;
  uint32_t cliStringLastSent;
} DataTransmitState_t;

#define RADIO_MAX_CALLBACKS 10
typedef void (*RadioCallback_t)(RecievedPacket_t*);

typedef struct {
  // The radio will enqueue packets here automatically
  CircularBuffer_t rxBuffer;
  uint8_t rxArray[RX_BUFF_LEN * sizeof(RecievedPacket_t)];

  RadioCallback_t callbacks[RADIO_MAX_CALLBACKS];
  size_t numCallbacks;
} DataRecieveState_t;

void RadioManager_init();
void RadioManager_tick();

void RadioManager_transmitData(int radioId, SensorData_t* sensorData,
                               FilterData_t* filterData, uint8_t state);

// Send a string over radio
// Note that this BLOCKS for up to 150ms!!
void RadioManager_transmitString(int radioId, uint8_t* data, size_t len);
// Default function is same, but sends over CLI
void RadioManager_transmitStringDefault(uint8_t* data, size_t len);

void RadioManager_addMessageCallback(int radioId, RadioCallback_t callback);

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_RADIO_MANAGER_H_
