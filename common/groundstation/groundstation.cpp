//
// Created by nathan on 5/14/23.
//

#include "groundstation.h"

// Define out a couple of STM32 things (I could just change them, but I'd rather
// do it this way so its easy to see what needs to change)
#ifndef USE_STM_HARDWARE_MANAGER
#define HAL_GetTick hm_millis
#define HAL_Delay delay
#endif  // USE_STM_HARDWARE_MANAGER
#ifndef USE_STM32_HAL_HEADER
#include <Arduino.h>
#endif

static void OnDataRx(RadioRecievedPacket_s *packet) {
  hm_usbTransmit(FIRST_ID_USB_STD, (uint8_t *)packet, sizeof(*packet));
}

static void GroundstationParseCommand(GroundstationUsbCommand_s *command) {
  if (command->data[0] == CHANNEL_COMMAND_ID) {
    uint8_t radioHw = command->data[1];
    int8_t channel = command->data[2];

    hm_radioSetChannel((int)radioHw, channel);
  }
}

Groundstation::Groundstation() = default;

void Groundstation::init() {
  hm_hardwareInit();

  radioManager_init();
  for (int i = 0; i < NUM_RADIO; i++) {
    radioManager_addMessageCallback(i, OnDataRx);
  }

  buffer = hm_usbGetRxBuffer(FIRST_ID_USB_STD);

  start = HAL_GetTick();
}

#define min(a, b) (a < b) ? a : b

void Groundstation::runOnce() {
  // Update sensors
  hm_watchdogRefresh();
  hm_readSensorData();
  hm_radioUpdate();

  // Process incoming data
  radioManager_tick();

  // Send barometer ~1x/5sec
  if ((hm_millis() - start) >= 2000) {
    start = HAL_GetTick();

    static HeartbeatData_s heartbeat;
    heartbeat.packetType = 200;
#if HAS_DEV(GPS)
    heartbeat.latitude = hm_getSensorData()->gpsData->generalData.latitude;
    heartbeat.longitude = hm_getSensorData()->gpsData->generalData.longitude;
    heartbeat.gps_alt = hm_getSensorData()->gpsData->generalData.altitude;
#endif
#if HAS_DEV(BARO)
    heartbeat.groundPressure = hm_getSensorData()->barometerData[0].pressureAtm;
    heartbeat.groundTemp = hm_getSensorData()->barometerData[0].temperatureC;
#endif

    // Hack to make all packets the same length when sent over USB
    static uint8_t heartbeatArr[sizeof(RadioRecievedPacket_s)] = {0};
    memset(heartbeatArr, 0, sizeof(heartbeatArr));
    memcpy(heartbeatArr, &heartbeat, sizeof(heartbeat));
    hm_usbTransmit(FIRST_ID_USB_STD, (uint8_t *)&heartbeatArr,
                   sizeof(heartbeatArr));
  }

  // A packet must have at least a destination [1 byte] and a len [2 bytes],
  // and a non-zero quantity of data
  if (cb_count(buffer) > 3) {
    static GroundstationUsbCommand_s command;
    size_t count = min(cb_count(buffer), sizeof(command));
    cb_peek(buffer, (uint8_t *)&command, &count);

    // If we got at least enough bytes for one message to be done
    if (count >= static_cast<uint32_t>(command.len + 3)) {
      if (command.destination == GROUNDSTATION) {
        GroundstationParseCommand(&command);
      } else {
        int dest = command.destination == RAD_433 ? FIRST_ID_RADIO_TI_433
                                                  : FIRST_ID_RADIO_TI_915;

        radioManager_transmitString(dest, command.data, command.len);
      }
      cb_dequeue(buffer, count);
    } else if (command.destination != GROUNDSTATION ||
               command.destination != RAD_433 ||
               command.destination != RAD_915) {
      cb_flush(buffer);
    }
  }

  HAL_Delay(1);
}
