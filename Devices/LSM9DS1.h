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

#include "board_config.h"
#include "SPIDriver.h"

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

#define G_TO_MPS	9.80665
#define DEG_TO_RAD	3.14158265358979/180

// Sensor Sensitivity Constants
// Values set according to the typical specifications provided in
// table 3 of the LSM9DS1 datasheet. (pg 12)
#define SENSITIVITY_ACCELEROMETER_2  0.000061*G_TO_MPS
#define SENSITIVITY_ACCELEROMETER_4  0.000122*G_TO_MPS
#define SENSITIVITY_ACCELEROMETER_8  0.000244*G_TO_MPS
#define SENSITIVITY_ACCELEROMETER_16 0.000732*G_TO_MPS
#define SENSITIVITY_GYROSCOPE_245    0.00875*DEG_TO_RAD
#define SENSITIVITY_GYROSCOPE_500    0.0175*DEG_TO_RAD
#define SENSITIVITY_GYROSCOPE_2000   0.07*DEG_TO_RAD
#define SENSITIVITY_MAGNETOMETER_4   0.00014
#define SENSITIVITY_MAGNETOMETER_8   0.00029
#define SENSITIVITY_MAGNETOMETER_12  0.00043
#define SENSITIVITY_MAGNETOMETER_16  0.00058


//CTRL_REG1_G Values
static const uint8_t ODR_G_14_9 = 1 << 5;
static const uint8_t ODR_G_59_5 = 2 << 5;
static const uint8_t ODR_G_119 = 3 << 5;
static const uint8_t ODR_G_238 = 4 << 5;
static const uint8_t ODR_G_476 = 5 << 5;
static const uint8_t ODR_G_952 = 6 << 5;
static const uint8_t FS_G_245 = 0 << 3;
static const uint8_t FS_G_500 = 1 << 3;
static const uint8_t FS_G_2000 = 3 << 3;
static const uint8_t BW_G_0 = 0;
static const uint8_t BW_G_1 = 1;
static const uint8_t BW_G_2 = 2;
static const uint8_t BW_G_3 = 3;


//CTRL_REG5_XL Values
static const uint8_t ODR_XL_10 = 1 << 5;
static const uint8_t ODR_XL_50 = 2 << 5;
static const uint8_t ODR_XL_119 = 3 << 5;
static const uint8_t ODR_XL_238 = 4 << 5;
static const uint8_t ODR_XL_476 = 5 << 5;
static const uint8_t ODR_XL_952 = 6 << 5;
static const uint8_t FS_XL_2 = 0 << 3;
static const uint8_t FS_XL_4 = 2 << 3;
static const uint8_t FS_XL_8 = 3 << 3;
static const uint8_t FS_XL_16 = 1 << 3;
static const uint8_t BW_XL_SCAL_ODR_0 = 0 << 2;
static const uint8_t BW_XL_SCAL_ODR_1 = 1 << 2;
static const uint8_t BW_XL_408 = 0;
static const uint8_t BW_XL_211 = 1;
static const uint8_t BW_XL_105 = 2;
static const uint8_t BW_XL_50 = 3;


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

//CTRL_REG1_M Values
static const uint8_t TEMP_COMP_M_0 = 0 << 7;
static const uint8_t TEMP_COMP_M_1 = 1 << 7;
static const uint8_t OM_M_LP = 0 << 5;
static const uint8_t OM_M_MP = 1 << 5;
static const uint8_t OM_M_HP = 2 << 5;
static const uint8_t OM_M_UP = 3 << 5;
static const uint8_t DO_M_0_625 = 0 << 2;
static const uint8_t DO_M_1_25 = 1 << 2;
static const uint8_t DO_M_2_5 = 2 << 2;
static const uint8_t DO_M_5 = 3 << 2;
static const uint8_t DO_M_10 = 4 << 3;
static const uint8_t DO_M_20 = 5 << 3;
static const uint8_t DO_M_40 = 6 << 3;
static const uint8_t DO_M_80 = 7 << 3;

//CTRL_REG2_M Values
static const uint8_t FS_M_4 = 0 << 5;
static const uint8_t FS_M_8 = 1 << 5;
static const uint8_t FS_M_12 = 2 << 5;
static const uint8_t FS_M_16 = 3 << 5;

//CTRL_REG3_M Values
static const uint8_t I2C_DISABLE_M_0 = 0 << 7;
static const uint8_t I2C_DISABLE_M_1 = 1 << 7;
static const uint8_t LP_M_0 = 0 << 5;
static const uint8_t LP_M_1 = 1 << 5;
static const uint8_t SIM_M_0 = 0 << 2;
static const uint8_t SIM_M_1 = 1 << 2;
static const uint8_t MD_M_CC = 0;
static const uint8_t MD_M_SC = 1;

//CTRL_REG4_M Values
static const uint8_t OMZ_M_LP = 0 << 2;
static const uint8_t OMZ_M_MP = 1 << 2;
static const uint8_t OMZ_M_HP = 2 << 2;
static const uint8_t OMZ_M_UP = 3 << 2;

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
    SPICtrld_t LSM9DS1SPI;
    LSM9DS1_raw_data_t aRawVal;
    LSM9DS1_raw_data_t gRawVal;
    LSM9DS1_data_t aVal;
    LSM9DS1_data_t gVal;
    LSM9DS1_data_t gAdj;
    uint8_t aFs;
    uint8_t gFs;
    double aRes;
    double gRes;
    int16_t tRawVal;
    double tVal;
}AG_LSM9DS1Ctrl_t;

typedef struct M_LSM9DS1Ctrl_s{
    SPICtrld_t LSM9DS1SPI;
    LSM9DS1_raw_data_t mRawVal;
    LSM9DS1_data_t mVal;
    LSM9DS1_data_t mAdj;
    uint8_t mFs;
    double mRes;
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
void LSM9DS1_get_adj(LSM9DS1Ctrl_t* sensor);
void LSM9DS1_calcRes(LSM9DS1Ctrl_t* sensor);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LSM9DS1_H_ */
