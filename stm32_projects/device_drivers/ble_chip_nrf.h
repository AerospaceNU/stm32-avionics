/*
 * ble_chip_nrf.h
 *
 *  Created on: Jan 3, 2022
 *      Author: matth
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_BLE_CHIP_NRF_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_BLE_CHIP_NRF_H_

#include "ble_chip.h"
#include "board_config_common.h"
#include "circular_buffer.h"

#if HAS_DEV(BLE_CHIP_NRF)

#define BLEINTERFACE_SELF_ADDR 0
#define MAX_ADDRESSES \
  10  // The maximum number of addresses we could possibly have

// How many ms between polling the nRF for new clients
#define BLE_POLL_INTERVAL 1000

// MS to wait for last UART transmit to end and start ours
#define BLE_TX_TIMEOUT 10

/*
 * A single packet from the nRF on the FCB. Packed so we can read directly from
 * UART
 */
typedef struct __attribute__((__packed__)) {
  uint16_t len;
  uint8_t incomingPacketArray[BT_MAX_PACKET_SIZE];
} BleChipNrfRecievedPacket_s;

#define INCOMING_PACKET_BUFF_LEN 10
#define INCOMING_PACKET_SIZE (sizeof(BleChipNrfRecievedPacket_s) + 1)
#define DMA_BUFF_SIZE \
  INCOMING_PACKET_BUFF_LEN *INCOMING_PACKET_SIZE  // Buffer that DMA writes to

/*
 * A bluetooth client inplements the comm protocol defined here:
 * https://docs.google.com/document/d/1jiheXicNSWqpXipNPMICgTwhR5yaPUJfUuBSakWD2V8/edit
 * The TLDR is that each message starts with a 1 byte address, 2 byte length,
 * and N-many bytes of data The address is 0 for the FCB NRF, 1 for phone, 2 and
 * 3 for line cutters 1 and 2 Each instance of a client needs to hold an array
 * for incoming data, and a client number it represents. Each instance holds a
 * buffer for incoming data, since packets may come in multiple chunks. it's up
 * to user code to wait until the buffer is full
 */

// Holds our two line cutters
typedef struct {
  BleChip_s bleChip;

  UART_HandleTypeDef *ble_uart;
  uint8_t dma_buff_begin[INCOMING_PACKET_SIZE];

  // List of pointers to circular buffers for our 4 addresses. Set these from
  // device drivers
  CircularBuffer_s *circular_buffers[MAX_ADDRESSES];

  uint8_t connectedClients;  // bits: 0 for phone, 1 for LC1, 2 for LC2

  // When we last polled connected clients. Should happen ~1x/second
  uint32_t lastPollTimestamp;
} BleChipNrfCtrl_t;

void bleChipNrf_init(BleChipNrfCtrl_t *ctrl, UART_HandleTypeDef *ble_uart);
void bleChipNrf_tick(BleChipNrfCtrl_t *ctrl);

#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_BLE_CHIP_NRF_H_
