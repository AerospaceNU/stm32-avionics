/*
 * line_cutter_ble.c
 *
 *  Created on: Jan 23, 2022
 *      Author: matth
 */

#include "line_cutter_ble.h"

#include <stdio.h>
#include <string.h>

#if HAS_DEV(LINE_CUTTER_BLE)

// Temporary buffer used while dequeueing packets
#define PACKET_TEMP_BUFFER_SIZE BT_MAX_PACKET_SIZE
static uint8_t tempBuffer[PACKET_TEMP_BUFFER_SIZE];

#define MAX_PACKETS_PER_TICK 10

// Things we can ask the line cutter. note that strlen doesn't include the null
static const char *REQ_DATA_CMD = "!fcbdata\n\0";
static const char *REQ_CFG_CMD = "!fcbcfg\n\0";
static const char *REQ_CUT_1_CMD = "!cut 1\n\0";
static const char *REQ_CUT_2_CMD = "!cut 2\n\0";
static const char *REQ_RESET_CMD = "!reset\n\0";

static LineCutterForwardStringCb_t stringCallback = NULL;

bool lineCutterBle_sendString(LineCutterBleCtrl_t *lineCutter,
                              const char *cmd) {
  // We used to have more checks here, but IMO we really just wanna send it
  // regardless
  bool ret = lineCutter->bleChip->sendRequest(
      lineCutter->bleChip, lineCutter->address, cmd, strlen((cmd)) + 1);
  if (ret) lineCutter->lastRequestTimestamp = HAL_GetTick();
  return ret;
}

#define BT_CMD_DEF(NAME, CMD)                                  \
  bool lineCutterBle_##NAME(LineCutterBleCtrl_t *lineCutter) { \
    return lineCutterBle_sendString(lineCutter, CMD);          \
  }

BT_CMD_DEF(requestData, REQ_DATA_CMD)
BT_CMD_DEF(requestConfig, REQ_CFG_CMD)
BT_CMD_DEF(cut1, REQ_CUT_1_CMD)
BT_CMD_DEF(cut2, REQ_CUT_2_CMD)
BT_CMD_DEF(reset, REQ_RESET_CMD)

#undef BT_CMD_DEF

#define LC_DATA_INTERVAL_MS 700 - 62  // Small offset

// We don't really need to ask for this periodically,
// but it doesn't hurt
#define LC_VARS_INTERVAL_MS 10000 + 124

void lineCutterBle_tick(LineCutterBleCtrl_t *lineCutter) {
  size_t dequeuedLen;
  uint8_t packetCount = 0;
  do {
    dequeuedLen =
        lineCutter->bleChip->dequeuePacket(&lineCutter->buffer, tempBuffer);
    packetCount++;
    if (dequeuedLen > 0)
      lineCutterBle_parse(lineCutter, dequeuedLen, tempBuffer);
  } while (dequeuedLen > 0 && packetCount < MAX_PACKETS_PER_TICK);

  volatile int ec = 0;

  // Ask for flight data periodically
  if (HAL_GetTick() > (lineCutter->lastDataTimestamp + LC_DATA_INTERVAL_MS)) {
    // Bluetooth_PollConnectedClients(lineCutter->ble);
    lineCutter->lastDataTimestamp = HAL_GetTick();
    // If this gives us a bad exit code, the timestamp will still be old
    // and we'll try again next tick
    ec |= lineCutterBle_requestData(lineCutter);
  }

  // Ask for flight vars sometimes too
  if (HAL_GetTick() >
      (lineCutter->lastFlightVarsTimestamp + LC_VARS_INTERVAL_MS)) {
    // Bluetooth_PollConnectedClients(lineCutter->ble);

    lineCutter->lastFlightVarsTimestamp = HAL_GetTick();
    ec |= lineCutterBle_requestConfig(lineCutter);
  }
}

void lineCutterBle_parse(LineCutterBleCtrl_t *lineCutter, uint16_t len,
                         uint8_t *arr) {
  // If the first character is an invalid ASCII code, parse as a binary message
  if (len && arr[0] == 128) {
    // Skip first non-valid byte indicating byte-packed data
    len -= 1;
    arr += 1;

    bool success = false;
    // Len==1 is an ACK

    if (len == 1) {
      lineCutter->lastAckTimestamp = HAL_GetTick();
      success = true;
    } else if (len == sizeof(LineCutterData_s)) {
      lineCutter->lastData = *((LineCutterData_s *)arr);
      lineCutter->lastDataTimestamp = HAL_GetTick();
      success = true;
    } else if (len == sizeof(LineCutterFlightVars_s)) {
      lineCutter->flightVars = *((LineCutterFlightVars_s *)arr);
      lineCutter->lastFlightVarsTimestamp = HAL_GetTick();
      success = true;
    }

    if (success) lineCutter->lastRequestTimestamp = 0;
  } else {
    // Otherwise, we got a string -- we should just send it to the callback
    if (stringCallback) {
      stringCallback((uint8_t *)arr, len);
    }
  }
}

void lineCutterBle_init(LineCutterBleCtrl_t *lineCutter, BleChip_s *bleChip,
                        uint8_t address) {
  lineCutter->bleChip = bleChip;
  lineCutter->address = address;

  cb_init(&lineCutter->buffer, (unknownPtr_t)lineCutter->packetArray,
          LINE_CUTTER_CB_SIZE, sizeof(uint8_t));
  lineCutter->bleChip->registerAddress(lineCutter->bleChip, lineCutter->address,
                                       &lineCutter->buffer);
}

bool lineCutterBle_isConnected(LineCutterBleCtrl_t *ctrl) {
  return ctrl->bleChip->isAddressConnected(ctrl->bleChip, ctrl->address);
}

bool lineCutterBle_isAwaitingReply(LineCutterBleCtrl_t *ctrl) {
  return ctrl->lastRequestTimestamp > 0;
}

void lineCutterBle_registerForwardStringCb(LineCutterForwardStringCb_t cb) {
  stringCallback = cb;
}

#endif  // HAS_DEV(LINE_CUTTER_BLE)
