/*
 * ble_linecutter.h
 *
 *  Created on: Jan 6, 2022
 *      Author: matth
 */

#ifndef DEVICES_BLE_LINECUTTER_H_
#define DEVICES_BLE_LINECUTTER_H_

#include <ble_interface.h>
#include <stdint.h>

#include "data_structures.h"

#define LINE_CUTTER_CB_SIZE 512

typedef struct {
  // The address assigned by the FCB bluetooth chip
  uint8_t address;

  // Reference to the bluetooth driver
  BluetoothInterface_t *ble;

  // Backing array and incoming data circular buffer
  uint8_t packetArray[LINE_CUTTER_CB_SIZE];
  CircularBuffer_t buffer;

  // The last data packet we've gotten
  uint32_t lastDataTimestamp;
  uint32_t lastFlightVarsTimestamp;
  LineCutterData_t lastData;
  LineCutterFlightVars_t flightVars;

  // If we've gotten an ack, at all
  uint32_t lastAckTimestamp;

  // The timestamp when we last sent a request
  // 0 if it's been acked
  uint32_t lastRequestTimestamp;
} LineCutterCtrl_t;

void LineCutter_Init(LineCutterCtrl_t *lineCutter, BluetoothInterface_t *ble,
                     uint8_t address);
void LineCutter_Parse(LineCutterCtrl_t *cutter, uint16_t len, uint8_t *arr);
void LineCutter_Tick(LineCutterCtrl_t *lineCutter);

// Send a string to the line cutter over Bluetooth
bool LineCutter_SendString(LineCutterCtrl_t *lineCutter, const char *cmd);

// Send a request for the latest data packet from a line cutter at a specified
// clientAddress
bool LineCutter_RequestData(LineCutterCtrl_t *ctrl);

// Request the line cutter variables struct
bool LineCutter_RequestConfig(LineCutterCtrl_t *ctrl);

// Send a request for the line cutter at clientAddress to cut the first line
bool LineCutter_Cut1(LineCutterCtrl_t *ctrl);

// Send a request for the line cutter at clientAddress to cut the second line
bool LineCutter_Cut2(LineCutterCtrl_t *ctrl);

// Send a request for the line cutter at clientAddress reset itself. Note that
// this may change the address of the other line cutter if the first line cutter
// disconnects as line cutter 1 is the first connected, cutter 2 is second, etc
// (see the fcb ble code for details)
bool LineCutter_Reset(LineCutterCtrl_t *ctrl);

// Returns if this line cutter is connected
bool LineCutter_IsConnected(LineCutterCtrl_t *ctrl);

// Returns if this line cutter is awaiting a reply to a command
bool LineCutter_IsAwaitingReply(LineCutterCtrl_t *ctrl);

#endif  // DEVICES_BLE_LINECUTTER_H_
