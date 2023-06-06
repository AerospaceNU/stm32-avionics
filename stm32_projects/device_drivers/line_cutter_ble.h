/*
 * line_cutter_ble.h
 *
 *  Created on: Jan 6, 2022
 *      Author: matth
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_LINE_CUTTER_BLE_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_LINE_CUTTER_BLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "ble_chip.h"
#include "board_config_common.h"
#include "data_structures.h"

#define LINE_CUTTER_CB_SIZE 512

typedef void (*LineCutterForwardStringCb_t)(uint8_t *, size_t);

typedef struct {
  // Reference to the BLE chip driver
  BleChip_s *bleChip;

  // The address assigned by the FCB bluetooth chip
  uint8_t address;

  // Backing array and incoming data circular buffer
  uint8_t packetArray[LINE_CUTTER_CB_SIZE];
  CircularBuffer_s buffer;

  // The last data packet we've gotten
  uint32_t lastDataTimestamp;
  uint32_t lastFlightVarsTimestamp;
  LineCutterData_s lastData;
  LineCutterFlightVars_s flightVars;

  // If we've gotten an ack, at all
  uint32_t lastAckTimestamp;

  // The timestamp when we last sent a request
  // 0 if it's been acked
  uint32_t lastRequestTimestamp;
} LineCutterBleCtrl_t;

void lineCutterBle_init(LineCutterBleCtrl_t *lineCutter, BleChip_s *bleChip,
                        uint8_t address);
void lineCutterBle_parse(LineCutterBleCtrl_t *lineCutter, uint16_t len,
                         uint8_t *arr);
void lineCutterBle_tick(LineCutterBleCtrl_t *lineCutter);

// Send a string to the line cutter over Bluetooth
bool lineCutterBle_sendString(LineCutterBleCtrl_t *ctrl, const char *cmd);

// Send a request for the latest data packet from a line cutter at a specified
// clientAddress
bool lineCutterBle_requestData(LineCutterBleCtrl_t *ctrl);

// Request the line cutter variables struct
bool lineCutterBle_requestConfig(LineCutterBleCtrl_t *ctrl);

// Send a request for the line cutter at clientAddress to cut the first line
bool lineCutterBle_cut1(LineCutterBleCtrl_t *ctrl);

// Send a request for the line cutter at clientAddress to cut the second line
bool lineCutterBle_cut2(LineCutterBleCtrl_t *ctrl);

// Send a request for the line cutter at clientAddress reset itself. Note that
// this may change the address of the other line cutter if the first line cutter
// disconnects as line cutter 1 is the first connected, cutter 2 is second, etc
// (see the fcb ble code for details)
bool lineCutterBle_reset(LineCutterBleCtrl_t *ctrl);

// Returns if this line cutter is connected
bool lineCutterBle_isConnected(LineCutterBleCtrl_t *ctrl);

// Returns if this line cutter is awaiting a reply to a command
bool lineCutterBle_isAwaitingReply(LineCutterBleCtrl_t *ctrl);

// Registers a function to call to forward strings to
void lineCutterBle_registerForwardStringCb(LineCutterForwardStringCb_t cb);

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_LINE_CUTTER_BLE_H_
