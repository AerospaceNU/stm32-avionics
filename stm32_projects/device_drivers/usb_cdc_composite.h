/*
 * usb_std.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Ben Fox
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_USB_CDC_COMPOSITE_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_USB_CDC_COMPOSITE_H_

#include <stdbool.h>
#include <stdint.h>

#include "board_config.h"
#include "board_config_common.h"
#include "circular_buffer.h"

#if HAS_DEV(USB_CDC_COMPOSITE)

class UsbCDCCtrl {
 public:
  UsbCDCCtrl() = default;

  /**
   * @brief Initialize USB device. RX occurs automatically
   * @param cdc_ch: USB CDC channel to redirect printf to, up to NUM_USB_CDC
   */
  void init(uint8_t cdc_ch);

  bool busy();

  /**
   * @brief Transmit data over USB
   * @param cdc_ch: USB CDC channel to transmit on, up to NUM_USB_CDC
   * @param[in] buf: Data to transmit
   * @param[in] len: Length of data to transmit
   * @return True on success, False on failure
   */
  bool transmit(uint8_t* buf, uint16_t len);

  /**
   * @brief Retrieve circular RX buffer
   * @param cdc_ch: USB CDC channel to transmit on, up to NUM_USB_CDC
   * @return Pointer to circular buffer
   */
  CircularBuffer_s* getRxBuffer();

  /**
   * @brief Whether a USB cable is connected or not.
   * @return Connected (true) or disconnected (false)
   */
  bool isConnected();

 private:
  uint8_t m_cdcCh;

  uint8_t m_rxBuffer[APP_RX_DATA_SIZE];
  CircularBuffer_s m_rxCircBuffer;
  uint32_t m_lastTransmit = {0};
};

#endif  // USB_CDC_COMPOSITE

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_USB_CDC_COMPOSITE_H_
