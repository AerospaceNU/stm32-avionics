/*
 * BluetoothClient.h
 *
 *  Created on: Jan 3, 2022
 *      Author: matth
 */

#ifndef BLE_INTERFACE_H_
#define BLE_INTERFACE_H_


#include "stm32h7xx_hal.h"
#include "circular_buffer.h"

#define BLEINTERFACE_SELF_ADDR 0
#define BT_MAX_PACKET_SIZE 0xff // or 256
#define MAX_ADDRESSES 10 // The maximum number of addresses we could possibly have

/*
 * A single packet from the nRF on the FCB. Packed so we can read directly from UART
 */
typedef struct __attribute__((__packed__)) {
	uint16_t len;
	uint8_t incomingPacketArray[BT_MAX_PACKET_SIZE];
} BleRecievedPacket_t;

#define INCOMING_PACKET_BUFF_LEN 10
#define INCOMING_PACKET_SIZE (sizeof(BleRecievedPacket_t) + 1)
#define DMA_BUFF_SIZE INCOMING_PACKET_BUFF_LEN * INCOMING_PACKET_SIZE // Buffer that DMA writes to

/*
 * A bluetooth client inplements the comm protocol defined here:
 * https://docs.google.com/document/d/1jiheXicNSWqpXipNPMICgTwhR5yaPUJfUuBSakWD2V8/edit
 * The TLDR is that each message starts with a 1 byte address, 2 byte length, and N-many bytes of data
 * The address is 0 for the FCB NRF, 1 for phone, 2 and 3 for line cutters 1 and 2
 * Each instance of a client needs to hold an array for incoming data, and a client number it represents.
 * Each instance holds a buffer for incoming data, since packets may come in multiple chunks. it's up to
 * user code to wait until the buffer is full
 */

// Holds our two line cutters
typedef struct {
	UART_HandleTypeDef *ble_uart;
	uint8_t dma_buff_begin[INCOMING_PACKET_SIZE];

	// List of pointers to circular buffers for our 4 addresses. Set these from device drivers
	CircularBuffer_t *circular_buffers[MAX_ADDRESSES];

	uint8_t connectedClients; // bits: 0 for phone, 1 for LC1, 2 for LC2

	// When we last polled connected clients. Should happen ~1x/second
	uint32_t lastPollTimestamp;
} BluetoothInterface_t;

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
HAL_StatusTypeDef BLE_Tx_Internal(BluetoothInterface_t *ctrl, uint8_t *buf, int bufLen);

// the DMA channel should be hdma_usart6_rx
void Bluetooth_Init(BluetoothInterface_t *ctrl, UART_HandleTypeDef *ble_uart);

// Associate a given circular buffer with an address. Bytes addressed to it will be added to the buffer.
void Bluetooth_RegisterAddress(BluetoothInterface_t *ctrl, uint8_t address, CircularBuffer_t *cb);

bool Bluetooth_TxBusy(BluetoothInterface_t *ctrl);

/**
 * Send a request to a given client, consisting of a address (held by the client), length, and request
 * The request must be an array of size [len], which MUST be below MAX_PACKET
 * Note that this blocks while transmission happens
 */
HAL_StatusTypeDef Bluetooth_SendRequest(BluetoothInterface_t *ctrl, const uint8_t address, const uint8_t* pdata, const uint16_t len);

// Check if a particular address was reported as connected. Note that this uses the cached connectedClients value!
// connectedClients should be updated periodically using Bluetooth_SendRequest.
bool Bluetooth_IsAddressConnected(BluetoothInterface_t *ctrl, uint8_t address);

// Send a request to the Bluetooth chip on the FCB to tell us what clients are connected
bool Bluetooth_PollConnectedClients(BluetoothInterface_t *ctrl);

// Helper to dequeue one packet from a circular buffer (where the buffer holds [2 bytes len, n bytes data])
// Note that pdata must be at least
uint16_t Bluetooth_DequeuePacket(CircularBuffer_t *buffer, uint8_t *pdata);

// Helper for printing packets
void Bluetooth_PrintPacket(uint8_t addr, uint16_t len, uint8_t* pdata);

void Bluetooth_Tick(BluetoothInterface_t *ctrl);

#endif /* BLE_INTERFACE_H_ */
