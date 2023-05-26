/*
 * ble_chip_nrf.h
 *
 *  Created on: Jan 3, 2022
 *      Author: matth
 */

#ifndef DEVICES_SERIAL_DUCER_H_
#define DEVICES_SERIAL_DUCER_H_

#include "board_config_common.h"
#include "circular_buffer.h"

typedef struct {
  uint32_t timestamp;
  SerialDucerData_t pressureData;
} PressureSensorTimestampedData_t;

#define SERIAL_DUCER_INCOMING_PACKET_SIZE sizeof(SerialDucerData_t)

typedef struct {
  UART_HandleTypeDef *uart;

  PressureSensorTimestampedData_t latestResult;

  SerialDucerData_t rx_firstBuff, rx_secondBuff;
  bool firstBuf;
  bool data_available;
} SerialDucerCtrl_s;

void serialDucer_init(SerialDucerCtrl_s *ctrl, UART_HandleTypeDef *uart);
void serialDucer_tick(SerialDucerCtrl_s *ctrl);

#endif  // DEVICES_SERIAL_DUCER_H_
