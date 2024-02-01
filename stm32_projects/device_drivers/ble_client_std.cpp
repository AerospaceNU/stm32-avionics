/*
 * ble_client_std.c
 *
 *  Created on: Jan 23, 2022
 *      Author: matth
 */

#include "ble_client_std.h"

#if HAS_DEV(BLE_CLIENT_STD)

#include <string.h>

#define MAX_PACKETS_PER_TICK 10
static uint8_t tempDataBuffer[DATA_BUFFER_SIZE] = {0};

void bleClientStd_init(BleClientStdCtrl_s *ctrl, BleChip_s *bleChip,
                       uint8_t address) {
  ctrl->bleChip = bleChip;
  ctrl->address = address;
  cb_init(&ctrl->parsedBuffer, (unknownPtr_t)ctrl->parsedArray,
          DATA_BUFFER_SIZE, sizeof(uint8_t));
  cb_init(&ctrl->dmaBuffer, (unknownPtr_t)ctrl->packetArray, DATA_BUFFER_SIZE,
          sizeof(uint8_t));
  ctrl->bleChip->registerAddress(ctrl->bleChip, address, &ctrl->dmaBuffer);
}

void bleClientStd_tick(BleClientStdCtrl_s *ctrl) {
  size_t dequeuedLen;
  uint8_t packetCount = 0;
  do {
    // Enqueue the message into the tempDataBuffer array
    dequeuedLen =
        ctrl->bleChip->dequeuePacket(&ctrl->dmaBuffer, tempDataBuffer);
    packetCount++;

    // Add it to our parsed buffer CB, which will be used by CLI
    for (size_t i = 0; i < dequeuedLen; i++) {
      cb_enqueue(&ctrl->parsedBuffer, (unknownPtr_t)&tempDataBuffer[i]);
    }
  } while (dequeuedLen > 0 && packetCount < MAX_PACKETS_PER_TICK);
}

#endif  // HAS_DEV(BLE_CLIENT_STD)
