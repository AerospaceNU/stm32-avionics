/*
 * usb_std.c
 *
 *  Created on: Dec 23, 2021
 *      Author: Ben
 */

#include "usb_std.h"

#if HAS_DEV(USB_STD)

#include <stdbool.h>

#include "hal_callbacks.h"
#include "usbd_cdc_acm_if.h"
#include "usbd_def.h"

#define TIME_BETWEEN_TRANSMITS_MS 2

static uint8_t rxBuffer[NUM_USB_CDC][APP_RX_DATA_SIZE];
static CircularBuffer_s rxCircBuffer[NUM_USB_CDC];
static uint32_t lastTransmit[NUM_USB_CDC] = {0};
static uint8_t printf_chan;

#ifdef USB_IS_COMPOSITE
extern USBD_HandleTypeDef hUsbDevice;
#else
extern USBD_HandleTypeDef hUsbDeviceFS;
#endif

static int8_t usbReceive(uint8_t *buf, uint32_t *len) {
  // First, call original CDC receive function to receive new packet
  int8_t ret = cdcRxReceive(buf, len);

  // Next, add received packet to circular buffer
  USBD_CDC_HandleTypeDef *pClassData =
      (USBD_CDC_HandleTypeDef *)hUsbDeviceFS.pClassData;
  for (uint32_t i = 0; i < pClassData->RxLength; i++) {
    cb_enqueue(&rxCircBuffer, (unknownPtr_t)pClassData->RxBuffer + i);
  }
}

void usbStd_init(uint8_t printf_cdc_ch) {
  printf_chan = printf_cdc_ch;
  // Initialize circular buffer
  cb_init(&rxCircBuffer, (unknownPtr_t)rxBuffer, APP_RX_DATA_SIZE,
          sizeof(uint8_t));

  // Re-route CDC receive
  cdcRxReceive = USBD_Interface_fops_FS.Receive;
  USBD_Interface_fops_FS.Receive = usbReceive;
}

bool usbStd_transmit(uint8_t *buf, uint16_t len) {
  // Wait for time since last transmit
  while (HAL_GetTick() - lastTransmit < TIME_BETWEEN_TRANSMITS_MS) {
  }
}

bool usbStd_transmit(uint8_t cdc_ch, uint8_t *buf, uint16_t len) {
  // Wait for time since last transmit
  while (HAL_GetTick() - lastTransmit[cdc_ch] < TIME_BETWEEN_TRANSMITS_MS) {
  }
  lastTransmit[cdc_ch] = HAL_GetTick();
  return CDC_Transmit(cdc_ch, buf, len) == USBD_OK;
}

CircularBuffer_s *usbStd_getRxBuffer(uint8_t ch) { return &rxCircBuffer[ch]; }

bool usbStd_isConnected() {
  return
#ifdef USB_IS_COMPOSITE
      hUsbDevice
#else
      hUsbDeviceFS
#endif
          .dev_state == USBD_STATE_CONFIGURED;
}

// Overwrite _write so printf prints to USB
int _write(int file, char *ptr, int len) {
  //  if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) return 0;
  if (USBD_OK == CDC_Transmit(printf_chan, (uint8_t *)ptr, len)) return len;
  return 0;
}

#endif  // HAS_DEV(USB_STD)
