/*
 * BleConsole.h
 *
 *  Created on: Jan 23, 2022
 *      Author: matth
 */

#ifndef DEVICES_BLE_CONSOLE_H_
#define DEVICES_BLE_CONSOLE_H_

#include <ble_interface.h>

#define LINE_CUTTER_CB_SIZE 512

typedef struct {
  // Reference to the bluetooth driver
  BluetoothInterface_t *ble;

  // BLE "address" data comes from
  uint8_t address;

  // Backing array and incoming data circular buffer
  // This is changed by DMA!
  uint8_t packetArray[LINE_CUTTER_CB_SIZE];
  CircularBuffer_t dmaBuffer;

  // This is _not_ changed by DMA, and does _not_ have length bytes
  uint8_t parsedArray[LINE_CUTTER_CB_SIZE];
  CircularBuffer_t parsedBuffer;
} BleConsole_t;

void BleConsole_Init(BleConsole_t *console, BluetoothInterface_t *ble,
                     uint8_t address);
void BleConsole_Tick(BleConsole_t *console);

#endif  // DEVICES_BLE_CONSOLE_H_
