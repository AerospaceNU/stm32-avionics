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
  cbInit(&ctrl->parsedBuffer, ctrl->parsedArray, DATA_BUFFER_SIZE,
         sizeof(uint8_t));
  cbInit(&ctrl->dmaBuffer, ctrl->packetArray, DATA_BUFFER_SIZE,
         sizeof(uint8_t));
  ctrl->bleChip->registerAddress(ctrl->bleChip, address, &ctrl->dmaBuffer);
}

void bleClientStd_tick(BleClientStdCtrl_s *ctrl) {
  uint16_t dequeuedLen;
  uint8_t packetCount = 0;
  do {
    // Enqueue the message into the tempDataBuffer array
    dequeuedLen =
        ctrl->bleChip->dequeuePacket(&ctrl->dmaBuffer, tempDataBuffer);
    packetCount++;

    // Add it to our parsed buffer CB, which will be used by CLI
    for (int i = 0; i < dequeuedLen; i++) {
      cbEnqueue(&ctrl->parsedBuffer, &tempDataBuffer[i]);
    }
  } while (dequeuedLen > 0 && packetCount < MAX_PACKETS_PER_TICK);
}

#endif  // HAS_DEV(BLE_CLIENT_STD)
