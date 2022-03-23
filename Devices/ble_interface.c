/*
 * BluetoothClient.c
 *
 *  Created on: Jan 5, 2022
 *      Author: matth
 */

#include <ble_interface.h>
#include <ble_linecutter.h>
#include <stdio.h>

#include "cli.h"
#include "hal_callbacks.h"
#include "string.h"

HAL_StatusTypeDef BLE_Tx_Internal(BluetoothInterface_t *ctrl, uint8_t *buf,
                                  int bufLen) {
  uint32_t start = HAL_GetTick();

  while (HAL_GetTick() - start < 10) {  // Wait for the last transmit to end, up
                                        // to 10ms. TODO this seems really long
    if (!Bluetooth_TxBusy(ctrl)) {
      return HAL_UART_Transmit(ctrl->ble_uart, buf, bufLen, HAL_MAX_DELAY);
    }
  }

  return HAL_BUSY;
}

// This will trigger DMA stuff to happen until either the line goes idle, or we
// get a maximally sized packet
#define START_DMA                                                    \
  HAL_UARTEx_ReceiveToIdle_DMA(ctrl->ble_uart, ctrl->dma_buff_begin, \
                               INCOMING_PACKET_SIZE)

// A transmission Size bytes long just ended that began at the old dma_buff_head
void Bluetooth_RxEventCallback(void *userData, size_t Size) {
  BluetoothInterface_t *ctrl = (BluetoothInterface_t *)userData;

  // Our new data starts at dma_buff_begin
  // Let's just cast it to a pointer to a RecievedPacket and
  // check if the length is right
  uint8_t address = ctrl->dma_buff_begin[0];
  uint16_t len = ctrl->dma_buff_begin[1] | ctrl->dma_buff_begin[2] << 8;
  uint8_t *pdata = ctrl->dma_buff_begin + 3;

  // Dumb exception for "connected clients" packet
  if (address == BLEINTERFACE_SELF_ADDR && len == 1) {
    ctrl->connectedClients = pdata[0];
  }

  // 3 bytes of header, so we should get (Size + 3) bytes
  if (len + 3 == Size && address < MAX_ADDRESSES &&
      ctrl->circular_buffers[address] != NULL) {
    for (uint16_t i = 1; i < Size;
         i++) {  // Byte 1 is address, so just enqueue [1..end]
      cbEnqueue(ctrl->circular_buffers[address], ctrl->dma_buff_begin + i);
    }
  }

  // We need to restart DMA at the end
  START_DMA;
}

void Bluetooth_RxCpltCallback(void *userData) {
  // The buffer should be full at this point
  Bluetooth_RxEventCallback(userData, DMA_BUFF_SIZE);
}

void Bluetooth_Init(BluetoothInterface_t *ctrl, UART_HandleTypeDef *ble_uart) {
  ctrl->ble_uart = ble_uart;

  for (int i = 0; i < MAX_ADDRESSES; i++) {
    ctrl->circular_buffers[i] = NULL;
  }

  ctrl->connectedClients = 0;

  // Set up our "Event" callback, which should only happen when idle happens
  register_HAL_UART_RxIdleCallback(ctrl->ble_uart, Bluetooth_RxEventCallback,
                                   ctrl);
  register_HAL_UART_RxCpltCallback(ctrl->ble_uart, Bluetooth_RxCpltCallback,
                                   ctrl);

  // This will trigger DMA stuff to happen until either the line goes idle
  START_DMA;
}

void Bluetooth_RegisterAddress(BluetoothInterface_t *ctrl, uint8_t address,
                               CircularBuffer_t *cb) {
  ctrl->circular_buffers[address] = cb;
}

bool Bluetooth_TxBusy(BluetoothInterface_t *ctrl) {
  return HAL_UART_STATE_READY != ctrl->ble_uart->gState;
}

/**
 * Send a request to a given client, consisting of a address (held by the
 * client), length, and request The request must be an array of size [len],
 * which MUST be below MAX_PACKET Note that this blocks while transmission
 * happens
 */
HAL_StatusTypeDef Bluetooth_SendRequest(BluetoothInterface_t *ctrl,
                                        const uint8_t address,
                                        const uint8_t *pdata,
                                        const uint16_t len) {
  // We need to transmit 1 byte addr, 2 byte len, n bytes of data
  const uint16_t headerSize = sizeof(address) + sizeof(len);
  const uint16_t totalSize = headerSize + len;

  // Statically allocate buffer for requests
  static uint8_t arr[1024];

  arr[0] = address;

  // uint16_t to 2 uint8_t's
  arr[0 + sizeof(address)] = len & 0xff;
  arr[0 + sizeof(address) + 1] = (len >> 8) & 0xff;

  // Stuff from headerSize up to sizeof(arr) should be set to the array pointed
  // to by pdata
  for (uint16_t i = 0; i < len; i++) {
    arr[i + headerSize] = pdata[i];
  }

  // Transmit this all over UART -- this happens asynchronously
  HAL_StatusTypeDef ret = BLE_Tx_Internal(ctrl, arr, totalSize);
  return ret;
}

void Bluetooth_PrintPacket(uint8_t addr, uint16_t len, uint8_t *pdata) {
  printf("Packet: addr [%u] len [%u]\nData:\n", addr, len);
  for (int i = 0; i < len; i++) {
    printf("0x%X ", pdata[i]);
    if (i % 8 == 0 && i != 0) printf("\n");
  }
  printf("\n");  // Flush
}

uint16_t Bluetooth_DequeuePacket(CircularBuffer_t *buffer, uint8_t *pdata) {
  // We should have at least 2 bytes (the len bytes) in our array
  if (cbCount(buffer) < 2) return 0;

  // Buffer should have two bytes length, then a bunch of bytes data
  uint16_t len;
  size_t two = 2;
  cbPeek(buffer, &len, &two);

  // If we have enough bytes to copy out a whole packet, do so
  if (len + 2 <= cbCount(buffer)) {
    cbDequeue(buffer, 2);  // Dequeue length first

    // Then pop out len-many bytes
    size_t len_ = len;
    cbPeek(buffer, pdata, &len_);

    cbDequeue(buffer, len_);

    return len;
  } else {
    // If we don't, all we can do is flush the buffer
    cbFlush(buffer);
    return 0;
  }
}

bool Bluetooth_IsAddressConnected(BluetoothInterface_t *ctrl, uint8_t address) {
  uint8_t mask = 0b1 << (address - 1);
  return (bool)(ctrl->connectedClients & mask);
}

bool Bluetooth_PollConnectedClients(BluetoothInterface_t *ctrl) {
  uint8_t one = 0x01;
  return HAL_OK == Bluetooth_SendRequest(ctrl, BLEINTERFACE_SELF_ADDR, &one, 1);
}

void Bluetooth_Tick(BluetoothInterface_t *ctrl) {
  if (HAL_GetTick() > ctrl->lastPollTimestamp + 900) {
    Bluetooth_PollConnectedClients(ctrl);
    ctrl->lastPollTimestamp = HAL_GetTick();
  }
}
