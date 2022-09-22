/*
 * I2C_driver.h
 *
 *  Created on: Jul 16, 2022
 *      Author: matth
 */

#ifndef DEVICES_I2C_DRIVER_H_
#define DEVICES_I2C_DRIVER_H_

#include "board_config.h"

#ifdef HAS_I2C

#include HAL_HEADER

typedef struct SPICtrl_s {
  I2C_HandleTypeDef *hi2c;
  uint8_t addr;
  uint32_t timeout;
} I2CCtrld_t;

uint8_t I2C_ReadRegister(I2CCtrld_t *sensor, uint8_t reg);
void I2C_WriteRegister(I2CCtrld_t *sensor, uint8_t reg, uint8_t val);

#endif

#endif  // DEVICES_I2C_DRIVER_H_
