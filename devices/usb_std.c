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
#define CDC_CHANNEL 0

static uint8_t rxBuffer[APP_RX_DATA_SIZE];
static CircularBuffer_s rxCircBuffer;
static uint32_t lastTransmit;

#ifdef USB_IS_COMPOSITE
extern USBD_HandleTypeDef hUsbDevice;
#else
extern USBD_HandleTypeDef hUsbDeviceFS;
#endif

static void usbReceive(void *userData, uint8_t cdc_ch, uint8_t *buf,
                       uint32_t *len) {
  // add received packet to circular buffer
  //  USBD_CDC_ACM_HandleTypeDef *pClassData =
  //      (USBD_CDC_ACM_HandleTypeDef *)hUsbDeviceFS.pClassData;
  for (uint32_t i = 0; i < *len; i++) {
    cb_enqueue(&rxCircBuffer, buf + i);
  }
}

void usbStd_init() {
  // Initialize circular buffer
  cb_init(&rxCircBuffer, rxBuffer, CDC_DATA_HS_MAX_PACKET_SIZE,
          sizeof(uint8_t));

  halCallbacks_registerUsbCdcReceiveCallback(CDC_CHANNEL, usbReceive, NULL);
}

bool usbStd_transmit(uint8_t *buf, uint16_t len) {
  // Wait for time since last transmit
  while (HAL_GetTick() - lastTransmit < TIME_BETWEEN_TRANSMITS_MS) {
  }
  lastTransmit = HAL_GetTick();
  return CDC_Transmit(CDC_CHANNEL, buf, len) == USBD_OK;
}

CircularBuffer_s *usbStd_getRxBuffer() { return &rxCircBuffer; }

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
  if (USBD_OK == CDC_Transmit(CDC_CHANNEL, (uint8_t *)ptr, len)) return len;
  return 0;
}

// redirect scanf, too
#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

GETCHAR_PROTOTYPE
{
  uint8_t ch = 0;

  /* Wait for reception of a character on the USB RX and echo this
   * character on console */
  size_t count;
  do {
	  count = 1;
	  cb_peek(usbStd_getRxBuffer(), &ch, &count);
	  HAL_Delay(1);
  } while (count < 1);

  cb_dequeue(usbStd_getRxBuffer(), 1);

  CDC_Transmit_FS(&ch, 1);

  return ch;
}


#endif  // HAS_DEV(USB_STD)
