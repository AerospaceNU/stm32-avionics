/*
 * board_config.h
 *
 *  Created on: Mar 27, 2022
 *      Author: matth
 */

#ifndef INC_BOARD_CONFIG_H_
#define INC_BOARD_CONFIG_H_

#include "stm32g0xx.h"

#define HAS_GPS
#define HAS_CC1200
#define HAS_MS5607
#define HAS_USB

#define HAS_SPI
#define HAS_UART

#define HAS_RADIO_433
#define BARO1 BARO_MS5607_SPI
#define TELEMETRY_RADIO RADIO_915

#endif /* INC_BOARD_CONFIG_H_ */
