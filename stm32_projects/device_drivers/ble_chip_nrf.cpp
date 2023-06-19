/*
 * ble_chip_nrf.c
 *
 *  Created on: Jan 5, 2022
 *      Author: matth
 */
#include "ble_chip_nrf.h"

#if HAS_DEV(BLE_CHIP_NRF)

#include <stdio.h>
#include <string.h>

#include "hal_callbacks.h"

static bool bleChipNrf_txBusy(BleChipNrfCtrl_t *ctrl) {
  return HAL_UART_STATE_READY != ctrl->ble_uart->gState;
}

/**
 * Transmission function to write the desired bytes out to
 * the BLE chip.
 *
 * @param buf the buffer of bytes to be sent to the BLE
 * chip
 * @param bufLen the number of valid bytes in the given
 * buffer to be sent
 * @return the status code from the HAL library from
 * sending a transmission.
 */
static HAL_StatusTypeDef bleChipNrf_txInternal(BleChipNrfCtrl_t *ctrl,
                                               uint8_t *buf, uint16_t bufLen) {
  uint32_t start = HAL_GetTick();

  while (HAL_GetTick() - start <
         BLE_TX_TIMEOUT) {  // Wait for the last transmit to end, up
                            // to 10ms. TODO this seems really long
    if (!bleChipNrf_txBusy(ctrl)) {
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
static void bleChipNrf_rxEventCallback(void *userData, size_t Size) {
  BleChipNrfCtrl_t *ctrl = (BleChipNrfCtrl_t *)userData;

  // Our new data starts at dma_buff_begin
  // Let's just cast it to a pointer to a RecievedPacket and
  // check if the length is right
  uint8_t address = ctrl->dma_buff_begin[0];
  uint16_t len = static_cast<uint16_t>(ctrl->dma_buff_begin[1] |
                                       ctrl->dma_buff_begin[2] << 8);
  uint8_t *pdata = ctrl->dma_buff_begin + 3;

  // Dumb exception for "connected clients" packet
  if (address == BLEINTERFACE_SELF_ADDR && len == 1) {
    ctrl->connectedClients = pdata[0];
  }

  // 3 bytes of header, so we should get (Size + 3) bytes
  if (static_cast<size_t>(len + 3) == Size && address < MAX_ADDRESSES &&
      ctrl->circular_buffers[address] != NULL) {
    for (uint16_t i = 1; i < Size;
         i++) {  // Byte 1 is address, so just enqueue [1..end]
      cb_enqueue(ctrl->circular_buffers[address],
                 (unknownPtr_t)ctrl->dma_buff_begin + i);
    }
  }

  // We need to restart DMA at the end
  START_DMA;
}

static void bleChipNrf_rxCpltCallback(void *userData) {
  // The buffer should be full at this point
  bleChipNrf_rxEventCallback(userData, DMA_BUFF_SIZE);
}

// Associate a given circular buffer with an address. Bytes addressed to it will
// be added to the buffer.
static void bleChipNrf_registerAddress(void *ctrl, uint8_t address,
                                       CircularBuffer_s *cb) {
  BleChipNrfCtrl_t *realCtrl = (BleChipNrfCtrl_t *)ctrl;
  realCtrl->circular_buffers[address] = cb;
}

/**
 * Send a request to a given client, consisting of a address (held by the
 * client), length, and request The request must be an array of size [len],
 * which MUST be below MAX_PACKET Note that this blocks while transmission
 * happens
 */
static bool bleChipNrf_sendRequest(void *ctrl, uint8_t address,
                                   const void *pdata, uint16_t len) {
  BleChipNrfCtrl_t *realCtrl = (BleChipNrfCtrl_t *)ctrl;

  // We need to transmit 1 byte addr, 2 byte len, n bytes of data
  const uint16_t headerSize = sizeof(address) + sizeof(len);
  const uint16_t totalSize = headerSize + len;

  // Statically allocate buffer for requests
  static uint8_t arr[1024];

  arr[0] = address;

  // uint16_t to 2 uint8_t's
  arr[0 + sizeof(address)] = (uint8_t)(len & 0xff);
  arr[0 + sizeof(address) + 1] = (uint8_t)((len >> 8) & 0xff);

  // Stuff from headerSize up to sizeof(arr) should be set to the array pointed
  // to by pdata
  memcpy(arr + headerSize, pdata, len);

  // Transmit this all over UART -- this happens synchronously
  HAL_StatusTypeDef ret = bleChipNrf_txInternal(realCtrl, arr, totalSize);
  return ret == HAL_OK;
}

static size_t bleChipNrf_dequeuePacket(CircularBuffer_s *buffer,
                                       uint8_t *pdata) {
  // We should have at least 2 bytes (the len bytes) in our array
  if (cb_count(buffer) < 2) return 0;

  // Buffer should have two bytes length, then a bunch of bytes data
  uint16_t len;
  size_t two = 2;
  cb_peek(buffer, (unknownPtr_t)&len, &two);

  // If we have enough bytes to copy out a whole packet, do so
  if (static_cast<size_t>(len + 2) <= cb_count(buffer)) {
    cb_dequeue(buffer, 2);  // Dequeue length first

    // Then pop out len-many bytes
    size_t len_ = len;
    cb_peek(buffer, (unknownPtr_t)pdata, &len_);

    cb_dequeue(buffer, len_);

    return len_;
  } else {
    // If we don't, all we can do is flush the buffer
    cb_flush(buffer);
    return 0;
  }
}

// Check if a particular address was reported as connected. Note that this uses
// the cached connectedClients value! connectedClients should be updated
// periodically
static bool bleChipNrf_isAddressConnected(void *ctrl, uint8_t address) {
  BleChipNrfCtrl_t *realCtrl = (BleChipNrfCtrl_t *)ctrl;
  uint8_t mask = 0b1 << (address - 1);
  return (bool)(realCtrl->connectedClients & mask);
}

// Send a request to the Bluetooth chip on the FCB to tell us what clients are
// connected
static bool bleChipNrf_pollConnectedClients(void *ctrl) {
  uint8_t one = 0x01;
  return HAL_OK ==
         bleChipNrf_sendRequest(ctrl, BLEINTERFACE_SELF_ADDR, &one, 1);
}

void bleChipNrf_init(BleChipNrfCtrl_t *ctrl, UART_HandleTypeDef *ble_uart) {
  ctrl->bleChip.dequeuePacket = bleChipNrf_dequeuePacket;
  ctrl->bleChip.isAddressConnected = bleChipNrf_isAddressConnected;
  ctrl->bleChip.pollConnectedClients = bleChipNrf_pollConnectedClients;
  ctrl->bleChip.registerAddress = bleChipNrf_registerAddress;
  ctrl->bleChip.sendRequest = bleChipNrf_sendRequest;

  ctrl->ble_uart = ble_uart;

  for (int i = 0; i < MAX_ADDRESSES; i++) {
    ctrl->circular_buffers[i] = NULL;
  }

  ctrl->connectedClients = 0;

  // Set up our "Event" callback, which should only happen when idle happens
  halCallbacks_registerUartRxIdleCallback(ctrl->ble_uart,
                                          bleChipNrf_rxEventCallback, ctrl);
  halCallbacks_registerUartRxCpltCallback(ctrl->ble_uart,
                                          bleChipNrf_rxCpltCallback, ctrl);

  // This will trigger DMA stuff to happen until either the line goes idle
  START_DMA;
}

void bleChipNrf_tick(BleChipNrfCtrl_t *ctrl) {
  if (HAL_GetTick() > ctrl->lastPollTimestamp + BLE_POLL_INTERVAL) {
    bleChipNrf_pollConnectedClients(ctrl);
    ctrl->lastPollTimestamp = HAL_GetTick();
  }
}

#endif  // HAS_DEV(BLE_CHIP_NRF)
