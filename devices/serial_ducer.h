/*
 * ble_chip_nrf.h
 *
 *  Created on: Jan 3, 2022
 *      Author: matth
 */

#ifndef DEVICES_BLE_CHIP_NRF_H_
#define DEVICES_BLE_CHIP_NRF_H_

#include "board_config_common.h"
#include "circular_buffer.h"

#if HAS_DEV(SERIAL_DUCER)

typedef struct __attribute__((packed)) {
	uint8_t status;
	float temp;
	float pressure;
} PressureSensorData_t;

typedef struct {
	uint32_t timestamp;
	PressureSensorData_t pressureData;
} PressureSensorTimestampedData_t;

#define SERIAL_DUCER_INCOMING_PACKET_SIZE sizeof(PressureSensorData_t)

typedef struct {
  UART_HandleTypeDef *uart;

  PressureSensorTimestampedData_t latestResult;

  PressureSensorData_t rx_firstBuff, rx_secondBuff;
  bool firstBuf;
  bool data_available;
} SerialDucerCtrl_t;

void serialDucer_init(SerialDucerCtrl_t *ctrl, UART_HandleTypeDef *uart);
void serialDucer_tick(SerialDucerCtrl_t *ctrl);

#endif

#endif  // DEVICES_BLE_CHIP_NRF_H_
