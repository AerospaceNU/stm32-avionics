/*
 * imu_lsm9ds1.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef DEVICES_IMU_LSM9DS1_H_
#define DEVICES_IMU_LSM9DS1_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config_common.h"
#include "spi_driver.h"

// Accelerometer-Gyroscope Registers
#define REG_WHO_AM_I_AG 0x0F

#define CTRL_REG1_G 0x10
#define CTRL_REG2_G 0x11
#define CTRL_REG3_G 0x12

#define CTRL_REG5_XL 0x1F
#define CTRL_REG6_XL 0x20
#define CTRL_REG7_XL 0x21

#define OUT_X_L_G 0x18
#define OUT_X_H_G 0x19
#define OUT_Y_L_G 0x1A
#define OUT_Y_H_G 0x1B
#define OUT_Z_L_G 0x1C
#define OUT_Z_H_G 0x1D
#define OUT_X_L_XL 0x28
#define OUT_X_H_XL 0x29
#define OUT_Y_L_XL 0x2A
#define OUT_Y_H_XL 0x2B
#define OUT_Z_L_XL 0x2C
#define OUT_Z_H_XL 0x2D

#define OUT_TEMP_L 0x15
#define OUT_TEMP_H 0x16

#define G_TO_MPS 9.80665
#define DEG_TO_RAD 3.14158265358979 / 180

// Sensor Sensitivity Constants
// Values set according to the typical specifications provided in
// table 3 of the LSM9DS1 datasheet. (pg 12)
#define SENSITIVITY_ACCELEROMETER_2 0.000061 * G_TO_MPS
#define SENSITIVITY_ACCELEROMETER_4 0.000122 * G_TO_MPS
#define SENSITIVITY_ACCELEROMETER_8 0.000244 * G_TO_MPS
#define SENSITIVITY_ACCELEROMETER_16 0.000732 * G_TO_MPS
#define SENSITIVITY_GYROSCOPE_245 0.00875 * DEG_TO_RAD
#define SENSITIVITY_GYROSCOPE_500 0.0175 * DEG_TO_RAD
#define SENSITIVITY_GYROSCOPE_2000 0.07 * DEG_TO_RAD
#define SENSITIVITY_MAGNETOMETER_4 0.00014
#define SENSITIVITY_MAGNETOMETER_8 0.00029
#define SENSITIVITY_MAGNETOMETER_12 0.00043
#define SENSITIVITY_MAGNETOMETER_16 0.00058

// CTRL_REG1_G Values
#define ODR_G_14_9 1 << 5
#define ODR_G_59_5 2 << 5
#define ODR_G_119 3 << 5
#define ODR_G_238 4 << 5
#define ODR_G_476 5 << 5
#define ODR_G_952 6 << 5
#define FS_G_245 0 << 3
#define FS_G_500 1 << 3
#define FS_G_2000 3 << 3
#define BW_G_0 0
#define BW_G_1 1
#define BW_G_2 2
#define BW_G_3 3

// CTRL_REG5_XL Values
#define ODR_XL_10 1 << 5
#define ODR_XL_50 2 << 5
#define ODR_XL_119 3 << 5
#define ODR_XL_238 4 << 5
#define ODR_XL_476 5 << 5
#define ODR_XL_952 6 << 5
#define FS_XL_2 0 << 3
#define FS_XL_4 2 << 3
#define FS_XL_8 3 << 3
#define FS_XL_16 1 << 3
#define BW_XL_SCAL_ODR_0 0 << 2
#define BW_XL_SCAL_ODR_1 1 << 2
#define BW_XL_408 0
#define BW_XL_211 1
#define BW_XL_105 2
#define BW_XL_50 3

// Magnetic Sensor Registers
#define REG_WHO_AM_I_M 0x0F
#define CTRL_REG1_M 0x20
#define CTRL_REG2_M 0x21
#define CTRL_REG3_M 0x22
#define CTRL_REG4_M 0x23
#define CTRL_REG5_M 0x24
#define OUT_X_L_M 0x28
#define OUT_X_H_M 0x29
#define OUT_Y_L_M 0x2A
#define OUT_Y_H_M 0x2B
#define OUT_Z_L_M 0x2C
#define OUT_Z_H_M 0x2D

// CTRL_REG1_M Values
#define TEMP_COMP_M_0 0 << 7
#define TEMP_COMP_M_1 1 << 7
#define OM_M_LP 0 << 5
#define OM_M_MP 1 << 5
#define OM_M_HP 2 << 5
#define OM_M_UP 3 << 5
#define DO_M_0_625 0 << 2
#define DO_M_1_25 1 << 2
#define DO_M_2_5 2 << 2
#define DO_M_5 3 << 2
#define DO_M_10 4 << 3
#define DO_M_20 5 << 3
#define DO_M_40 6 << 3
#define DO_M_80 7 << 3

// CTRL_REG2_M Values
#define FS_M_4 0 << 5
#define FS_M_8 1 << 5
#define FS_M_12 2 << 5
#define FS_M_16 3 << 5

// CTRL_REG3_M Values
#define I2C_DISABLE_M_0 0 << 7
#define I2C_DISABLE_M_1 1 << 7
#define LP_M_0 0 << 5
#define LP_M_1 1 << 5
#define SIM_M_0 0 << 2
#define SIM_M_1 1 << 2
#define MD_M_CC 0
#define MD_M_SC 1

// CTRL_REG4_M Values
#define OMZ_M_LP 0 << 2
#define OMZ_M_MP 1 << 2
#define OMZ_M_HP 2 << 2
#define OMZ_M_UP 3 << 2

typedef struct {
  SpiCtrl_t lsm9ds1Spi;
  uint8_t aFs;
  uint8_t gFs;
  double aRes;
  double gRes;
  int16_t tRawVal;
  double tVal;
} Lsm9ds1AgCtrl_s;

typedef struct {
  SpiCtrl_t lsm9ds1Spi;
  uint8_t mFs;
  double mRes;
} Lsm9ds1MCtrl_s;

typedef struct LSM9DS1Ctrl_s {
  ImuData_s data;
  Lsm9ds1AgCtrl_s ag;
  Lsm9ds1MCtrl_s m;
} ImuLsm9ds1Ctrl_s;

bool lsm9ds1_init(ImuLsm9ds1Ctrl_s *sensor);
void lsm9ds1_getData(ImuLsm9ds1Ctrl_s *sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_IMU_LSM9DS1_H_
