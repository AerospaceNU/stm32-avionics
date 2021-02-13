/*
 * LSM9DS1.c
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#include "LSM9DS1.h"

uint8_t AG_whoAmI(AG_LSM9DS1Ctrl_t* sensor) {
	return SPI_ReadRegister(&sensor->LSM9DS1SPI, (1 << 7) | REG_WHO_AM_I_AG);
}

uint8_t M_whoAmI(M_LSM9DS1Ctrl_t* sensor) {
	return SPI_ReadRegister(&sensor->LSM9DS1SPI, (1 << 7) | REG_WHO_AM_I_M);
}

void LSM9DS1_init(LSM9DS1Ctrl_t* sensor) {
	// Pull CS High
	HAL_GPIO_WritePin(sensor->ag.LSM9DS1SPI.port, sensor->ag.LSM9DS1SPI.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(sensor->m.LSM9DS1SPI.port, sensor->m.LSM9DS1SPI.pin, GPIO_PIN_SET);

	// Initialize Gyroscope
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG1_G, ODR_G_238 | sensor->ag.gFs | BW_G_1);

	// Initialize Accelerometer
	SPI_WriteRegister(&sensor->ag.LSM9DS1SPI, CTRL_REG6_XL, ODR_XL_50 | sensor->ag.aFs);

	// Initialize Magnetometer
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG3_M, I2C_DISABLE_M_0 | SIM_M_0 | MD_M_CC);
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG1_M, OM_M_MP | DO_M_5);
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG2_M, sensor->m.mFs);
	SPI_WriteRegister(&sensor->m.LSM9DS1SPI, CTRL_REG4_M, OMZ_M_MP);

	LSM9DS1_calcRes(sensor);
	LSM9DS1_get_adj(sensor);
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
	LSM9DS1_get_data_raw(sensor);
	sensor->ag.aVal.x = (sensor->ag.aRes * sensor->ag.aRawVal.x) - sensor->ag.aAdj.x;
	sensor->ag.aVal.y = (sensor->ag.aRes * sensor->ag.aRawVal.y) - sensor->ag.aAdj.y;
	sensor->ag.aVal.z = ((sensor->ag.aRes * sensor->ag.aRawVal.z) - sensor->ag.aAdj.z);

	sensor->ag.gVal.x = (sensor->ag.gRes * sensor->ag.gRawVal.x) - sensor->ag.gAdj.x;
	sensor->ag.gVal.y = (sensor->ag.gRes * sensor->ag.gRawVal.y) - sensor->ag.gAdj.y;
	sensor->ag.gVal.z = (sensor->ag.gRes * sensor->ag.gRawVal.z) - sensor->ag.gAdj.z;

	sensor->m.mVal.x = (sensor->m.mRes * sensor->m.mRawVal.x) - sensor->m.mAdj.x;
	sensor->m.mVal.y = (sensor->m.mRes * sensor->m.mRawVal.y) - sensor->m.mAdj.y;
	sensor->m.mVal.z = (sensor->m.mRes * sensor->m.mRawVal.z) - sensor->m.mAdj.z;
}
void LSM9DS1_get_adj(LSM9DS1Ctrl_t* sensor) {
	int sampleCount = 100;
	int aAdjX = 0;
	int aAdjY = 0;
	int aAdjZ = 0;
	int gAdjX = 0;
	int gAdjY = 0;
	int gAdjZ = 0;
	int mAdjX = 0;
	int mAdjY = 0;
	int mAdjZ = 0;
	for (int i = 0; i < sampleCount; i++) {
		LSM9DS1_get_data_raw(sensor);
		aAdjX += sensor->ag.aRawVal.x;
		aAdjY += sensor->ag.aRawVal.y;
		aAdjZ += sensor->ag.aRawVal.z;
		gAdjX += sensor->ag.gRawVal.x;
		gAdjY += sensor->ag.gRawVal.y;
		gAdjZ += sensor->ag.gRawVal.z;
		mAdjX += sensor->m.mRawVal.x;
		mAdjY += sensor->m.mRawVal.y;
		mAdjZ += sensor->m.mRawVal.z;
		HAL_Delay(5);
	}
	sensor->ag.aAdj.x = (aAdjX / sampleCount) * sensor->ag.aRes;
	sensor->ag.aAdj.y = (aAdjY / sampleCount) * sensor->ag.aRes;
	sensor->ag.aAdj.z = (aAdjZ / sampleCount) * sensor->ag.aRes - G_TO_MPS;
	sensor->ag.gAdj.x = (gAdjX / sampleCount) * sensor->ag.gRes;
	sensor->ag.gAdj.y = (gAdjY / sampleCount) * sensor->ag.gRes;
	sensor->ag.gAdj.z = (gAdjZ / sampleCount) * sensor->ag.gRes;
	sensor->m.mAdj.x = (mAdjX / sampleCount) * sensor->m.mRes;
	sensor->m.mAdj.y = (mAdjY / sampleCount) * sensor->m.mRes;
	sensor->m.mAdj.z = (mAdjZ / sampleCount) * sensor->m.mRes;
}

void LSM9DS1_calcRes(LSM9DS1Ctrl_t* sensor) {
	switch (sensor->ag.aFs) {
		case 0 << 3:
			sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_2;
			break;
		case 2 << 3:
			sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_4;
			break;
		case 3 << 3:
			sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_8;
			break;
		case 1 << 3:
			sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_16;
			break;
	}

	switch (sensor->ag.gFs) {
		case 0 << 3:
			sensor->ag.gRes = SENSITIVITY_GYROSCOPE_245;
			break;
		case 1 << 3:
			sensor->ag.gRes = SENSITIVITY_GYROSCOPE_500;
			break;
		case 3 << 3:
			sensor->ag.gRes = SENSITIVITY_GYROSCOPE_2000;
			break;
	}
	switch (sensor->m.mFs) {
		case 0 << 5:
			sensor->m.mRes = SENSITIVITY_MAGNETOMETER_4;
			break;
		case 1 << 5:
			sensor->m.mRes = SENSITIVITY_MAGNETOMETER_8;
			break;
		case 2 << 5:
			sensor->m.mRes = SENSITIVITY_MAGNETOMETER_12;
			break;
		case 3 << 5:
			sensor->m.mRes = SENSITIVITY_MAGNETOMETER_16;
			break;
	}
}
