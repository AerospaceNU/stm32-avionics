/*
 * ble_chip.h
 */

#ifndef DEVICES_BLE_CHIP_H_
#define DEVICES_BLE_CHIP_H_

#include <stdint.h>

#include "circular_buffer.h"

#define BT_MAX_PACKET_SIZE 0xff  // or 256

typedef struct {
  void (*registerAddress)(void *ctrl, uint8_t address, CircularBuffer_t *cb);
  bool (*sendRequest)(void *ctrl, uint8_t address, const void *pdata,
                      uint16_t len);
  bool (*isAddressConnected)(void *ctrl, uint8_t address);
  bool (*pollConnectedClients)(void *ctrl);
  uint16_t (*dequeuePacket)(CircularBuffer_t *buffer, uint8_t *pdata);
} BleChip_t;

#endif  // DEVICES_BLE_CHIP_H_
