/*
 * usb_std.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Ben Fox
 */

#ifndef DEVICES_USB_STD_H_
#define DEVICES_USB_STD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"
#include "circular_buffer.h"

/**
 * @brief Initialize USB device. RX occurs automatically
 */
void usbStdInit();

/**
 * @brief Transmit data over USB
 * @param[in] buf: Data to transmit
 * @param[in] len: Length of data to transmit
 * @return True on success, False on failure
 */
bool usbStdTransmit(uint8_t* buf, uint16_t len);

/**
 * @brief Retrieve circular RX buffer
 * @return Pointer to circular buffer
 */
CircularBuffer_s* usbStdGetRxBuffer();

/**
 * @brief Whether USB is connected or not
 * @return Connected (true) or disconnected (false)
 */
bool usbStdIsConnected();

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_USB_STD_H_
