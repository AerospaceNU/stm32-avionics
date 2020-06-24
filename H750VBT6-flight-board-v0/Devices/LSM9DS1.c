/*
 * LSM9DS1.c
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#include "SPIDriver.h"
#include "LSM9DS1.h"

uint8_t AG_whoAmI(AG_LSM9DS1Ctrl_t* sensor) {
	return SPI_ReadRegister(&sensor->LSM9DS1SPI, (1 << 7) | REG_WHO_AM_I_AG);
}

uint8_t M_whoAmI(M_LSM9DS1Ctrl_t* sensor) {
	return SPI_ReadRegister(&sensor->LSM9DS1SPI, (1 << 7) | REG_WHO_AM_I_M);
}

void LSM9DS1_init(LSM9DS1Ctrl_t* sensor) {
	//There are a lot of freaking ways to configure this sensor, I just kinda picked values at random
	//and wrote to the bare minimum number of registers to get it working.

	// Initialize Magnetometer
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG1_M, 0x30 << 1);
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG2_M, 0x00 << 1);
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG3_M, 0x04 << 1);
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG4_M, 0x04 << 1);
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG5_M, 0x00 << 1);

	// Initialize Accelerometer
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG5_XL, 0x38);
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG6_XL, 0x80);
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG7_XL, 0x00);

	// Initialize Gyroscope
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG1_G, 0x60);
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG2_G, 0x00);
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG3_G, 0x00);
}

void LSM9DS1_get_data_raw (LSM9DS1Ctrl_t* sensor) {
	// Takes x, y, and z axis readings
	uint8_t x_l_xl = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_X_L_XL);
	uint8_t x_h_xl = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_X_H_XL);
	uint8_t y_l_xl = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Y_L_XL);
	uint8_t y_h_xl = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Y_H_XL);
	uint8_t z_l_xl = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Z_L_XL);
	uint8_t z_h_xl = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Z_H_XL);

	uint8_t x_l_g = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_X_L_G);
	uint8_t x_h_g = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_X_H_G);
	uint8_t y_l_g = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Y_L_G);
	uint8_t y_h_g = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Y_H_G);
	uint8_t z_l_g = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Z_L_G);
	uint8_t z_h_g = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_Z_H_G);

	uint8_t x_l_m = SPI_ReadRegister(&sensor->m.LSM9DS1SPI, (1 << 7) | OUT_X_L_M);
	uint8_t x_h_m = SPI_ReadRegister(&sensor->m.LSM9DS1SPI, (1 << 7) | OUT_X_H_M);
	uint8_t y_l_m = SPI_ReadRegister(&sensor->m.LSM9DS1SPI, (1 << 7) | OUT_Y_L_M);
	uint8_t y_h_m = SPI_ReadRegister(&sensor->m.LSM9DS1SPI, (1 << 7) | OUT_Y_H_M);
	uint8_t z_l_m = SPI_ReadRegister(&sensor->m.LSM9DS1SPI, (1 << 7) | OUT_Z_L_M);
	uint8_t z_h_m = SPI_ReadRegister(&sensor->m.LSM9DS1SPI, (1 << 7) | OUT_Z_H_M);

	uint8_t t_l = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_TEMP_L);
	uint8_t t_h = SPI_ReadRegister(&sensor->ag.LSM9DS1SPI, (1 << 7) | OUT_TEMP_H);

	// Writes combined h and l byte to struct
	sensor->ag.aRawVal.x = ((int16_t)x_h_xl << 8) | (x_l_xl);
	sensor->ag.aRawVal.y = ((int16_t)y_h_xl << 8) | (y_l_xl);
	sensor->ag.aRawVal.z = ((int16_t)z_h_xl << 8) | (z_l_xl);

	sensor->ag.gRawVal.x = ((int16_t)x_h_g << 8) | (x_l_g);
	sensor->ag.gRawVal.y = ((int16_t)y_h_g << 8) | (y_l_g);
	sensor->ag.gRawVal.z = ((int16_t)z_h_g << 8) | (z_l_g);

	sensor->m.mRawVal.x = ((int16_t)x_h_m << 8) | (x_l_m);
	sensor->m.mRawVal.y = ((int16_t)y_h_m << 8) | (y_l_m);
	sensor->m.mRawVal.z = ((int16_t)z_h_m << 8) | (z_l_m);

	sensor->ag.tRawVal = ((int16_t)t_h << 8) | (t_l);
}

void LSM9DS1_get_data (LSM9DS1Ctrl_t* sensor) {

}
