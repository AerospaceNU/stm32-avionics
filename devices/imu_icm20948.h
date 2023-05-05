/*
 * ICM20948.h
 *
 *  Created on: Oct 26, 2018
 *      Author: cory
 * 
 * From: https://github.com/therealwokewok/ICM20948/blob/master
 * Licensed under the GNU General Public License v3.0
 */

#ifndef IMU_ICM20948_H_
#define IMU_ICM20948_H_

#include "stdint.h"
#include "board_config_common.h"
#include "i2c.h"
#include HAL_HEADER

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t i2cAddr;
	ImuData_s agData;
	MagData_s mData;
} ImuIcm20948Ctrl_s;

bool icm20948_init(ImuIcm20948Ctrl_s *sensor, I2C_HandleTypeDef *hi2c, uint8_t i2cAddr);
void icm20948_getData(ImuIcm20948Ctrl_s *sensor);

#endif /* IMU_ICM20948_H_ */
