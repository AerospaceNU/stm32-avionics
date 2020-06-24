/*
 * H3LIS331DL.c
 *
 *  Created on: May 26, 2020
 *      Author: John
 */

#include "SPIDRiver.h"
#include "H3LIS331DL.h"

uint8_t whoAmI(H3LIS331DLCtrl_t* sensor) {
	return SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_WHO_AM_I);
}

uint8_t readCtrl1(H3LIS331DLCtrl_t* sensor) {
	return SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_CTRL1);
}

void H3LIS331DL_init(H3LIS331DLCtrl_t* sensor) {
	SPI_WriteRegister(&sensor->H3LIS331DLSPI, REG_CTRL1, PWR_MODE_ON | DATA_RATE_100HZ | Z_AXIS_ENABLE | Y_AXIS_ENABLE | X_AXIS_ENABLE);
}

void H3LIS331DL_get_data_raw(H3LIS331DLCtrl_t* sensor) {
	// Takes x, y, and z axis readings
	uint8_t x_l = SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_X_L);
	uint8_t x_h = SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_X_H);
	uint8_t y_l = SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Y_L);
	uint8_t y_h = SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Y_H);
	uint8_t z_l = SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Z_L);
	uint8_t z_h = SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Z_H);

	// Writes combined h and l byte to struct
	sensor->rawVal.x = ((int16_t)x_h << 8) | (x_l);
	sensor->rawVal.y = ((int16_t)y_h << 8) | (y_l);
	sensor->rawVal.z = ((int16_t)z_h << 8) | (z_l);
}

void H3LIS331DL_get_data(H3LIS331DLCtrl_t* sensor) {
	H3LIS331DL_get_data_raw(sensor);
}
