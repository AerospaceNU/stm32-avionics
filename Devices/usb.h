/*
 * usb.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Ben Fox
 */

#ifndef USB_H_
#define USB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config.h"
#include "circular_buffer.h"

/**
 * @brief Initialize USB device. RX occurs automatically
 */
void usbInit();

/**
 * @brief Transmit data over USB
 * @param[in] buf: Data to transmit
 * @param[in] len: Length of data to transmit
 * @return True on success, False on failure
 */
bool usbTransmit(uint8_t* buf, uint16_t len);

/**
 * @brief Retrieve circular RX buffer
 * @return Pointer to circular buffer
 */
CircularBuffer_t* usbGetRxBuffer();

/**
 * @brief Whether USB is connected or not
 * @return Connected (true) or disconnected (false)
 */
bool usbIsConnected();

#ifdef __cplusplus
}
#endif

#endif /* USB_H_ */
