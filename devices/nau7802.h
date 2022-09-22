/*
 * nau7802.h
 *
 *  Created on: Jul 16, 2022
 *      Author: matth
 */

#ifndef DEVICES_NAU7802_H_
#define DEVICES_NAU7802_H_

#include "board_config.h"
#include HAL_HEADER
#include "board_config_common.h"

#if HAS_DEV(NAU7902)

#include "I2C_driver.h"
#include "stdbool.h"

typedef struct {
  I2CCtrld_t *dev;
} NAU7802Ctrl_t;

// From Adafruit
/** Default NAU7802 I2C address. */
#define NAU7802_I2CADDR_DEFAULT 0x2A  ///< I2C address
#define NAU7802_PU_CTRL 0x00          ///< Power control register
#define NAU7802_CTRL1 0x01            ///< Control/config register #1
#define NAU7802_CTRL2 0x02            ///< Control/config register #2
#define NAU7802_ADCO_B2 0x12          ///< ADC ouput LSB
#define NAU7802_ADC 0x15              ///< ADC / chopper control
#define NAU7802_PGA 0x1B              ///< PGA control
#define NAU7802_POWER 0x1C            ///< power control
#define NAU7802_REVISION_ID 0x1F      ///< Chip revision ID

//! Init, true if no error
bool NAU7802_Init(NAU7802Ctrl_t *sensor);
//! reset, true if no error
bool NAU7802_Reset(NAU7802Ctrl_t *sensor);
bool NAU7802_Enable(NAU7802Ctrl_t *sensor, bool is_on);

#endif

#endif  // DEVICES_NAU7802_H_
