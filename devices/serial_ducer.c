/*
 * ble_chip_nrf.c
 *
 *  Created on: Jan 5, 2022
 *      Author: matth
 */
#include "serial_ducer.h"

#if HAS_DEV(SERIAL_DUCER)

#include <stdio.h>
#include <string.h>

#include "hal_callbacks.h"

static SerialDucerData_t *getDmaBuff(SerialDucerCtrl_s *ctrl, bool swapBuff) {
  bool desiredBuff = swapBuff ? ctrl->firstBuf : !ctrl->firstBuf;
  return desiredBuff ? &ctrl->rx_firstBuff : &ctrl->rx_secondBuff;
}

// This will trigger DMA stuff to happen until either the line goes idle, or we
// get a maximally sized packet
#define START_DMA                                                             \
  HAL_UARTEx_ReceiveToIdle_DMA(ctrl->uart, (uint8_t *)getDmaBuff(ctrl, true), \
                               SERIAL_DUCER_INCOMING_PACKET_SIZE)

// A transmission Size bytes long just ended that began at the old dma_buff_head
static void serialDucer_rxEventCallback(void *userData, size_t Size) {
  SerialDucerCtrl_s *ctrl = (SerialDucerCtrl_s *)userData;

  ctrl->data_available = true;
  // todo swap buffers n shit

  // We need to restart DMA at the end
  START_DMA;
}

static void serialDucer_rxCpltCallback(void *userData) {
  // The buffer should be full at this point
  serialDucer_rxEventCallback(userData, SERIAL_DUCER_INCOMING_PACKET_SIZE);
}

void serialDucer_init(SerialDucerCtrl_s *ctrl, UART_HandleTypeDef *uart) {
  ctrl->uart = uart;
  ctrl->data_available = false;

  // Set up our "Event" callback, which should only happen when idle happens
  halCallbacks_registerUartRxIdleCallback(ctrl->uart,
                                          serialDucer_rxEventCallback, ctrl);
  halCallbacks_registerUartRxCpltCallback(ctrl->uart,
                                          serialDucer_rxCpltCallback, ctrl);

  // This will trigger DMA stuff to happen until either the line goes idle
  START_DMA;
}

bool serialDucer_newData(SerialDucerCtrl_s *ctrl) {
  if (ctrl->data_available) {
    SerialDucerData_t *buff = getDmaBuff(ctrl, false);
    ctrl->latestResult.pressureData = *buff;
    ctrl->latestResult.timestamp = HAL_GetTick();
    ctrl->data_available = false;
    return true;
  }
  return false;
}

#endif  // HAS_DEV(BLE_CHIP_NRF)
