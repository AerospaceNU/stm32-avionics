/*
 * ble_client_std.h
 *
 *  Created on: Jan 23, 2022
 *      Author: matth
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_BLE_CLIENT_STD_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_BLE_CLIENT_STD_H_

#include <stdint.h>

#include "ble_chip.h"
#include "board_config_common.h"

#define DATA_BUFFER_SIZE 512

typedef struct {
  // Reference to the BLE chip driver "class"
  BleChip_s* bleChip;

  // BLE "address" data comes from
  uint8_t address;

  // Backing array and incoming data circular buffer
  // This is changed by DMA!
  uint8_t packetArray[DATA_BUFFER_SIZE];
  CircularBuffer_s dmaBuffer;

  // This is _not_ changed by DMA, and does _not_ have length bytes
  uint8_t parsedArray[DATA_BUFFER_SIZE];
  CircularBuffer_s parsedBuffer;
} BleClientStdCtrl_s;

void bleClientStd_init(BleClientStdCtrl_s* ctrl, BleChip_s* bleChip,
                       uint8_t address);
void bleClientStd_tick(BleClientStdCtrl_s* ctrl);

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_BLE_CLIENT_STD_H_
