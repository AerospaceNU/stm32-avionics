/*
 * ble_chip.h
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_BLE_CHIP_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_BLE_CHIP_H_

#include <stdint.h>

#include "circular_buffer.h"

#define BT_MAX_PACKET_SIZE 0xff  // or 256

typedef struct {
  void (*registerAddress)(void *ctrl, uint8_t address, CircularBuffer_s *cb);
  bool (*sendRequest)(void *ctrl, uint8_t address, const void *pdata,
                      uint16_t len);
  bool (*isAddressConnected)(void *ctrl, uint8_t address);
  bool (*pollConnectedClients)(void *ctrl);
  size_t (*dequeuePacket)(CircularBuffer_s *buffer, uint8_t *pdata);
} BleChip_s;

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_BLE_CHIP_H_
