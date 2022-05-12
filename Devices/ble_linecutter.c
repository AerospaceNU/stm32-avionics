/*
 * ble_linecutter.c
 *
 *  Created on: Jan 23, 2022
 *      Author: matth
 */

#include "board_config.h"

#ifdef HAS_BLE

#include <ble_linecutter.h>
#include <stdio.h>
#include <string.h>

#include "radio_manager.h"

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

bool LineCutter_SendString(LineCutterCtrl_t *lineCutter, const char *cmd) {
  // We used to have more checks here, but IMO we really just wanna send it
  // regardless
  bool ret =
      (HAL_OK == Bluetooth_SendRequest(lineCutter->ble, lineCutter->address,
                                       cmd, strlen((cmd)) + 1));
  if (ret) lineCutter->lastRequestTimestamp = HAL_GetTick();
  return ret;
}

#define BT_CMD_DEF(NAME, CMD)                            \
  bool LineCutter_##NAME(LineCutterCtrl_t *lineCutter) { \
    return LineCutter_SendString(lineCutter, CMD);       \
  }

BT_CMD_DEF(RequestData, REQ_DATA_CMD);
BT_CMD_DEF(RequestConfig, REQ_CFG_CMD);
BT_CMD_DEF(Cut1, REQ_CUT_1_CMD);
BT_CMD_DEF(Cut2, REQ_CUT_2_CMD);
BT_CMD_DEF(Reset, REQ_RESET_CMD);

#undef BT_CMD_DEF

#define LC_DATA_INTERVAL_MS 700 - 62  // Small offset

// We don't really need to ask for this periodically,
// but it doesn't hurt
#define LC_VARS_INTERVAL_MS 10000 + 124

void LineCutter_Tick(LineCutterCtrl_t *lineCutter) {
  uint16_t dequeuedLen;
  uint8_t packetCount = 0;
  do {
    dequeuedLen = Bluetooth_DequeuePacket(&lineCutter->buffer, tempBuffer);
    packetCount++;
    if (dequeuedLen > 0) LineCutter_Parse(lineCutter, dequeuedLen, tempBuffer);
  } while (dequeuedLen > 0 && packetCount < MAX_PACKETS_PER_TICK);

  volatile HAL_StatusTypeDef ec = 0;

  // Ask for flight data periodically
  if (HAL_GetTick() > (lineCutter->lastDataTimestamp + LC_DATA_INTERVAL_MS)) {
    // Bluetooth_PollConnectedClients(lineCutter->ble);
    lineCutter->lastDataTimestamp = HAL_GetTick();
    // If this gives us a bad exit code, the timestamp will still be old
    // and we'll try again next tick
    ec |= LineCutter_RequestData(lineCutter);
  }

  // Ask for flight vars sometimes too
  if (HAL_GetTick() >
      (lineCutter->lastFlightVarsTimestamp + LC_VARS_INTERVAL_MS)) {
    // Bluetooth_PollConnectedClients(lineCutter->ble);

    lineCutter->lastFlightVarsTimestamp = HAL_GetTick();
    ec |= LineCutter_RequestConfig(lineCutter);
  }
}

void LineCutter_Parse(LineCutterCtrl_t *cutter, uint16_t len, uint8_t *arr) {
  // If the first character is an invalid ASCII code, parse as a binary message
  if (len && arr[0] == 128) {
    // Skip first non-valid byte indicating byte-packed data
    len -= 1;
    arr += 1;

    bool success = false;
    // Len==1 is an ACK

    if (len == 1) {
      cutter->lastAckTimestamp = HAL_GetTick();
      success = true;
    } else if (len == sizeof(LineCutterData_t)) {
      cutter->lastData = *((LineCutterData_t *)arr);
      cutter->lastDataTimestamp = HAL_GetTick();
      success = true;
    } else if (len == sizeof(LineCutterFlightVars_t)) {
      cutter->flightVars = *((LineCutterFlightVars_t *)arr);
      cutter->lastFlightVarsTimestamp = HAL_GetTick();
      success = true;
    }

    if (success) cutter->lastRequestTimestamp = 0;
  } else {
    // Otherwise, we got a string -- we should just send it over radio
#ifdef TELEMETRY_RADIO
    RadioManager_transmitString(TELEMETRY_RADIO, (uint8_t *)arr, len);
#endif
  }
}

void LineCutter_Init(LineCutterCtrl_t *lineCutter, BluetoothInterface_t *ble,
                     uint8_t address) {
  lineCutter->ble = ble;
  lineCutter->address = address;

  cbInit(&lineCutter->buffer, lineCutter->packetArray, LINE_CUTTER_CB_SIZE,
         sizeof(uint8_t));
  Bluetooth_RegisterAddress(ble, lineCutter->address, &lineCutter->buffer);
}

bool LineCutter_IsConnected(LineCutterCtrl_t *ctrl) {
  return Bluetooth_IsAddressConnected(ctrl->ble, ctrl->address);
}

bool LineCutter_IsAwaitingReply(LineCutterCtrl_t *ctrl) {
  return ctrl->lastRequestTimestamp > 0;
}

#endif
