/*
 * usb_std.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Ben Fox
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_USB_STD_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_USB_STD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config.h"
#include "board_config_common.h"
#include "circular_buffer.h"

/**
 * @brief Initialize USB device. RX occurs automatically
 * @param cdc_ch: USB CDC channel to redirect printf to, up to NUM_USB_CDC
 */
void usbStd_init(uint8_t cdc_ch);

/**
 * @brief Transmit data over USB
 * @param cdc_ch: USB CDC channel to transmit on, up to NUM_USB_CDC
 * @param[in] buf: Data to transmit
 * @param[in] len: Length of data to transmit
 * @return True on success, False on failure
 */
bool usbStd_transmit(uint8_t cdc_ch, uint8_t* buf, uint16_t len);

/**
 * @brief Retrieve circular RX buffer
 * @param cdc_ch: USB CDC channel to transmit on, up to NUM_USB_CDC
 * @return Pointer to circular buffer
 */
CircularBuffer_s* usbStd_getRxBuffer(uint8_t cdc_ch);

/**
 * @brief Whether USB is connected or not
 * @return Connected (true) or disconnected (false)
 */
bool usbStd_isConnected();

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_USB_STD_H_
