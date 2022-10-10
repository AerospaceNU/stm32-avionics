/*
 * radio_manager.h
 */

#ifndef SYSTEM_TASKS_RADIO_MANAGER_H_
#define SYSTEM_TASKS_RADIO_MANAGER_H_

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
  uint32_t hardwareStatusLastSent;
  uint32_t cliStringLastSent;
} DataTransmitState_s;

#define RADIO_MAX_CALLBACKS 10
typedef void (*RadioCallback_t)(RadioRecievedPacket_s*);

typedef struct {
  // The radio will enqueue packets here automatically
  CircularBuffer_s rxBuffer;
  uint8_t rxArray[RX_BUFF_LEN * sizeof(RadioRecievedPacket_s)];

  RadioCallback_t callbacks[RADIO_MAX_CALLBACKS];
  size_t numCallbacks;
} DataRecieveState_s;

void radioManager_init();
void radioManager_tick();

void radioManager_transmitData(int radioId, SensorData_s* sensorData,
                               FilterData_s* filterData, uint8_t state);

// Send a string over radio
// Note that this BLOCKS for up to 150ms!!
void radioManager_transmitString(int radioId, uint8_t* data, size_t len);
// Default function is same, but sends over CLI
void radioManager_transmitStringDefault(uint8_t* data, size_t len);

void radioManager_addMessageCallback(int radioId, RadioCallback_t callback);

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_TASKS_RADIO_MANAGER_H_
