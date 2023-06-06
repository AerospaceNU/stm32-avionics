/*
 * usb_std.c
 *
 *  Created on: Dec 23, 2021
 *      Author: Ben
 */

#include "usb_cdc_composite.h"

#if HAS_DEV(USB_CDC_COMPOSITE)

#include <stdbool.h>

#include "hal_callbacks.h"
#include "usbd_cdc_acm_if.h"
#include "usbd_def.h"

#define TIME_BETWEEN_TRANSMITS_MS 2

#ifdef USB_IS_COMPOSITE
extern USBD_HandleTypeDef hUsbDevice;
#else
extern USBD_HandleTypeDef hUsbDeviceFS;
#endif

// Need to have a C style list here, this does leak instance members which is
// gross
static CircularBuffer_s *rxCircBuffer[NUM_USB_CDC_COMPOSITE] = {0};

static void usbReceive(void *userData, uint8_t cdc_ch, uint8_t *buf,
                       uint32_t *len) {
  for (uint32_t i = 0; i < *len; i++) {
    cb_enqueue(rxCircBuffer[cdc_ch], buf + i);
  }
}

void UsbCDCCtrl::init(uint8_t cdc_ch) {
  m_cdcCh = cdc_ch;
  // Initialize circular buffer
  cb_init(&m_rxCircBuffer, m_rxBuffer, APP_RX_DATA_SIZE, sizeof(uint8_t));
  rxCircBuffer[cdc_ch] = &m_rxCircBuffer;
  halCallbacks_registerUsbCdcReceiveCallback(cdc_ch, usbReceive, NULL);
}

bool UsbCDCCtrl::busy() {
  extern USBD_CDC_ACM_HandleTypeDef CDC_ACM_Class_Data[];
  USBD_CDC_ACM_HandleTypeDef *hcdc = NULL;
  hcdc = &CDC_ACM_Class_Data[m_cdcCh];
  return (hcdc && hcdc->TxState != 0);
}

bool UsbCDCCtrl::transmit(uint8_t *buf, uint16_t len) {
  // Wait for time since last transmit
  while (UsbCDCCtrl::busy() &&
         HAL_GetTick() - m_lastTransmit < TIME_BETWEEN_TRANSMITS_MS) {
  }
  m_lastTransmit = HAL_GetTick();
  return CDC_Transmit(m_cdcCh, buf, len) == USBD_OK;
}

CircularBuffer_s *UsbCDCCtrl::getRxBuffer() { return &m_rxCircBuffer; }

bool UsbCDCCtrl::isConnected() {
  return
#ifdef USB_IS_COMPOSITE
      hUsbDevice
#else
      hUsbDeviceFS
#endif
          .dev_state == USBD_STATE_CONFIGURED;
}

#endif  // HAS_DEV(USB_STD)
