/*
 * board_config.h
 *
 *  Created on: Feb 6, 2022
 *      Author: matth
 */

#ifndef INC_BOARD_CONFIG_H_
#define INC_BOARD_CONFIG_H_

#include "stm32h7xx.h"

#define HAS_CC1200
#define HAS_GPS
#define HAS_USB
#define HAS_UART
#define HAS_SPI
#define HAS_MS5607

/*
 * Baro Types: (up to 2 baros)
 * 0: No Baro
 * 1: MS5607 over SPI
 */
#define BARO_1 1
// #define BARO_2 0

/*
 * Radio. We assume we can only have 433 or 915, and they're both
 * either cc1200 or cc1120
 */

#define HAS_RADIO_433
//#define HAS_RADIO_915 // TODO commented out for now

#endif /* INC_BOARD_CONFIG_H_ */
