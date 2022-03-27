/*
 * usb.c
 *
 *  Created on: Dec 23, 2021
 *      Author: Ben
 */

#include "usb.h"

#ifdef HAS_USB

#include <stdbool.h>

#include "usbd_cdc_if.h"

#define TIME_BETWEEN_TRANSMITS_MS 1

static uint8_t rxBuffer[APP_RX_DATA_SIZE];
static CircularBuffer_t rxCircBuffer;
static int8_t (*cdcRxReceive)(uint8_t *Buf, uint32_t *Len);
static uint32_t lastTransmit;

extern USBD_HandleTypeDef hUsbDeviceFS;

static int8_t usbReceive(uint8_t *buf, uint32_t *len) {
  // First, call original CDC receive function to receive new packet
  int8_t ret = cdcRxReceive(buf, len);

  // Next, add received packet to circular buffer
  USBD_CDC_HandleTypeDef *pClassData =
      (USBD_CDC_HandleTypeDef *)hUsbDeviceFS.pClassData;
  for (uint32_t i = 0; i < pClassData->RxLength; i++) {
    cbEnqueue(&rxCircBuffer, pClassData->RxBuffer + i);
  }
  return ret;
}

void usbInit() {
  // Initialize circular buffer
  cbInit(&rxCircBuffer, rxBuffer, APP_RX_DATA_SIZE, sizeof(uint8_t));

  // Re-route CDC receive
  cdcRxReceive = USBD_Interface_fops_FS.Receive;
  USBD_Interface_fops_FS.Receive = usbReceive;
}

bool usbTransmit(uint8_t *buf, uint16_t len) {
  // Wait for time since last transmit
  while (HAL_GetTick() - lastTransmit < TIME_BETWEEN_TRANSMITS_MS)
    ;
  lastTransmit = HAL_GetTick();
  return CDC_Transmit_FS(buf, len) == USBD_OK;
}

CircularBuffer_t *usbGetRxBuffer() { return &rxCircBuffer; }

bool usbIsConnected() {
  return hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED;
}

// Overwrite _write so printf prints to USB
int _write(int file, char *ptr, int len) {
  if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) return 0;
  if (USBD_OK == CDC_Transmit_FS((uint8_t *)ptr, len)) return len;
  return 0;
}

#endif
