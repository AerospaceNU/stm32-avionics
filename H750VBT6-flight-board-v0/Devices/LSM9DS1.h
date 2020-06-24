/*
 * LSM9DS1.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef SRC_LSM9DS1_H_
#define SRC_LSM9DS1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

// Accelerometer-Gyroscope Registers
static const uint8_t REG_WHO_AM_I_AG = 0x0F;

static const uint8_t CTRL_REG1_G = 0x10;
static const uint8_t CTRL_REG2_G = 0x11;
static const uint8_t CTRL_REG3_G = 0x12;

static const uint8_t CTRL_REG5_XL = 0x1F;
static const uint8_t CTRL_REG6_XL = 0x20;
static const uint8_t CTRL_REG7_XL = 0x21;

static const uint8_t OUT_X_L_G = 0x18;
static const uint8_t OUT_X_H_G = 0x19;
static const uint8_t OUT_Y_L_G = 0x1A;
static const uint8_t OUT_Y_H_G = 0x1B;
static const uint8_t OUT_Z_L_G = 0x1C;
static const uint8_t OUT_Z_H_G = 0x1D;
static const uint8_t OUT_X_L_XL = 0x28;
static const uint8_t OUT_X_H_XL = 0x29;
static const uint8_t OUT_Y_L_XL = 0x2A;
static const uint8_t OUT_Y_H_XL = 0x2B;
static const uint8_t OUT_Z_L_XL = 0x2C;
static const uint8_t OUT_Z_H_XL = 0x2D;

static const uint8_t OUT_TEMP_L = 0x15;
static const uint8_t OUT_TEMP_H = 0x16;

// Magnetic Sensor Registers
static const uint8_t REG_WHO_AM_I_M = 0x0F;
static const uint8_t CTRL_REG1_M = 0x20;
static const uint8_t CTRL_REG2_M = 0x21;
static const uint8_t CTRL_REG3_M = 0x22;
static const uint8_t CTRL_REG4_M = 0x23;
static const uint8_t CTRL_REG5_M = 0x24;
static const uint8_t OUT_X_L_M = 0x28;
static const uint8_t OUT_X_H_M = 0x29;
static const uint8_t OUT_Y_L_M = 0x2A;
static const uint8_t OUT_Y_H_M = 0x2B;
static const uint8_t OUT_Z_L_M = 0x2C;
static const uint8_t OUT_Z_H_M = 0x2D;


typedef struct LSM9DS1_raw_data_s{
    int16_t x;
    int16_t y;
    int16_t z;
}LSM9DS1_raw_data_t;

typedef struct LSM9DS1_data_s{
    double x;
    double y;
    double z;
}LSM9DS1_data_t;

typedef struct AG_LSM9DS1Ctrl_s{
    SPICtrl_t LSM9DS1SPI;
    LSM9DS1_raw_data_t aRawVal;
    LSM9DS1_raw_data_t gRawVal;
    LSM9DS1_data_t aVal;
    LSM9DS1_data_t gVal;
    int16_t tRawVal;
    double tVal;
}AG_LSM9DS1Ctrl_t;

typedef struct M_LSM9DS1Ctrl_s{
    SPICtrl_t LSM9DS1SPI;
    LSM9DS1_raw_data_t mRawVal;
    LSM9DS1_data_t mVal;
}M_LSM9DS1Ctrl_t;

typedef struct LSM9DS1Ctrl_s{
	AG_LSM9DS1Ctrl_t ag;
	M_LSM9DS1Ctrl_t m;
}LSM9DS1Ctrl_t;

uint8_t AG_whoAmI(AG_LSM9DS1Ctrl_t* sensor);
uint8_t M_whoAmI(M_LSM9DS1Ctrl_t* sensor);
void LSM9DS1_init(LSM9DS1Ctrl_t* sensor);
void LSM9DS1_get_data_raw (LSM9DS1Ctrl_t* sensor);
void LSM9DS1_get_data (LSM9DS1Ctrl_t* sensor);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LSM9DS1_H_ */
