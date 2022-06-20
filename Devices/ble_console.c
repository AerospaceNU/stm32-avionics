/*
 * ble_console.c
 *
 *  Created on: Jan 23, 2022
 *      Author: matth
 */

#include "board_config.h"

#ifdef HAS_BLE

#include <ble_console.h>
#include <string.h>

#define MAX_PACKETS_PER_TICK 10
static uint8_t bleConsole_temp[LINE_CUTTER_CB_SIZE] = {0};

void BleConsole_Init(BleConsole_t *console, BluetoothInterface_t *ble,
                     uint8_t address) {
  console->ble = ble;
  console->address = address;
  cbInit(&console->parsedBuffer, console->parsedArray, LINE_CUTTER_CB_SIZE,
         sizeof(uint8_t));
  cbInit(&console->dmaBuffer, console->packetArray, LINE_CUTTER_CB_SIZE,
         sizeof(uint8_t));
  Bluetooth_RegisterAddress(ble, address, &console->dmaBuffer);
}

void BleConsole_Tick(BleConsole_t *console) {
  uint16_t dequeuedLen;
  uint8_t packetCount = 0;
  do {
    // Enqueue the message into the bleConsole_temp array
    dequeuedLen = Bluetooth_DequeuePacket(&console->dmaBuffer, bleConsole_temp);
    packetCount++;

    // Add it to our parsed buffer CB, which will be used by CLI
    for (int i = 0; i < dequeuedLen; i++) {
      cbEnqueue(&console->parsedBuffer, &bleConsole_temp[i]);
    }
  } while (dequeuedLen > 0 && packetCount < MAX_PACKETS_PER_TICK);
}

#endif
