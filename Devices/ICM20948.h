/*
 * ICM20948.h
 *
 *  Created on: Oct 26, 2018
 *      Author: cory
 * From: https://github.com/CoryCline/ICM20948/blob/master/Examples/SW4STM32/F103C8xx/Core/Inc/ICM20948.h
 */

#ifndef ICM20948_H_
#define ICM20948_H_

#include <stdint.h>
#include "SPIDriver.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USER_BANK_SEL	(0x7F)
#define USER_BANK_0		(0x00)
#define USER_BANK_1		(0x10)
#define USER_BANK_2		(0x20)
#define USER_BANK_3		(0x30)

#define PWR_MGMT_1 		(0x06)
#define PWR_MGMT_2		(0x07)
#define GYRO_CONFIG_1	(0x01)

#define CLK_BEST_AVAIL	(0x01)

// Bank 3
#define GYRO_SMPLRT_DIV 0x00
#define I2C_MST_CTRL 0x01
#define I2C_MST_DELAY_CTRL 0x2
#define I2C_SLV0_CTRL 0x05
#define ACCEL_CONFIG 0x14
#define ACCEL_SMPLRT_DIV_1 0x10
#define ACCEL_SMPLRT_DIV_2 0x11

// Mag
#define MAG_CNTL3 0x32
#define MAG_CNTL2 0x31

// Accelerometer fullscale
typedef enum {
    ACCEL_2G = 0,
    ACCEL_4G,
    ACCEL_8G,
    ACCEL_16G
} ICM20948_AccelFullscale_e;

// Gyro fullscale options
typedef enum {
    GYRO_250_DPS = 0,
    GYRO_500_DPS,
    GYRO_1000_DPS,
    GYRO_2000_DPS
} ICM20948_GyroFullscale_e;

// Accelerometer low-pass filter options
typedef enum
{ // Format is dAbwB_nXbwZ - A is integer part of 3db BW, B is fraction. X is integer part of nyquist bandwidth, Y is fraction
    acc_d246bw_n265bw = 0x00,
    acc_d246bw_n265bw_1,
    acc_d111bw4_n136bw,
    acc_d50bw4_n68bw8,
    acc_d23bw9_n34bw4,
    acc_d11bw5_n17bw,
    acc_d5bw7_n8bw3,
    acc_d473bw_n499bw,
} ICM20948_ACCEL_CONFIG_DLPCFG_e;

// Gyro digital low-pass filter options
typedef enum
{ // Format is dAbwB_nXbwY - A is integer part of 3db BW, B is fraction. X is integer part of nyquist bandwidth, Y is fraction
    gyr_d196bw6_n229bw8 = 0x00,
    gyr_d151bw8_n187bw6,
    gyr_d119bw5_n154bw3,
    gyr_d51bw2_n73bw3,
    gyr_d23bw9_n35bw9,
    gyr_d11bw6_n17bw8,
    gyr_d5bw7_n8bw9,
    gyr_d361bw4_n376bw5,
} ICM20948_GYRO_CONFIG_1_DLPCFG_e;

typedef struct __attribute__((__packed__))
{
    uint8_t ACCEL_FCHOICE : 1; // If we should enable the digital low-pass filter
    uint8_t ACCEL_FS_SEL : 2; // An instance of ICM20948_AccelFullscale_e
    uint8_t ACCEL_DLPFCFG : 3; // an instance of ICM20948_ACCEL_CONFIG_DLPCFG_e
    uint8_t reserved_0 : 2; // Reserved, should be zeros
} ICM_20948_ACCEL_CONFIG_t;

typedef struct __attribute__((__packed__))
{
    uint8_t GYRO_FCHOICE : 1; // see above
    uint8_t GYRO_FS_SEL : 2;
    uint8_t GYRO_DLPFCFG : 3;
    uint8_t reserved_0 : 2;
} ICM_20948_GYRO_CONFIG_t;

// Fullscale is 0.15uT/LSB
#define MAG_SENSITIVITY_ICM20948 0.15; // uT/LSB

typedef struct ICM20948_RawData_t {
    int16_t accel_x_raw;
    int16_t accel_y_raw;
    int16_t accel_z_raw;
    int16_t gyro_x_raw;
    int16_t gyro_y_raw;
    int16_t gyro_z_raw;
    int16_t temp_raw;
    int16_t mag_x_raw;
    int16_t mag_y_raw;
    int16_t mag_z_raw;
    // uint8_t magStat1, magStat2;
} ICM20948_RawData_t;

typedef struct {
    double accel_x;
    double accel_y;
    double accel_z;
    double gyro_x;
    double gyro_y;
    double gyro_z;
    double mag_x;
    double mag_y;
    double mag_z;
    double temp;
} ICM20948Data_t;

typedef struct {
	SPICtrld_t spictrl;
	ICM20948_RawData_t rawData;
	ICM20948Data_t imuData;
} ICM20948Ctrl_t;


/**
 * Call PowerOn() to set up the IMU.
 * By default, the output data rate should be 1.1khz with DLPF disabled for the gyro and acceleromter
 */
bool ICM_PowerOn(ICM20948Ctrl_t *sensor, ICM_20948_ACCEL_CONFIG_t accelConfig, ICM_20948_GYRO_CONFIG_t gyroConfig);

uint8_t ICM_WHOAMI(ICM20948Ctrl_t *sensor);
void ICM_SelectBank(ICM20948Ctrl_t *sensor, uint8_t bank);
void ICM_ReadRawAccelGyro(ICM20948Ctrl_t *sensor);
void ICM_ReadMagData(ICM20948Ctrl_t *sensor);
uint16_t ICM_Initialize(ICM20948Ctrl_t *sensor, ICM_20948_ACCEL_CONFIG_t accelConfig, ICM_20948_GYRO_CONFIG_t gyroConfig);
void ICM_Disable_I2C(ICM20948Ctrl_t *sensor);
void ICM_CSHigh(ICM20948Ctrl_t *sensor);
void ICM_CSLow(ICM20948Ctrl_t *sensor);
void ICM_SetClock(ICM20948Ctrl_t *sensor, uint8_t clk);
void ICM_AccelGyroOff(ICM20948Ctrl_t *sensor);
void ICM_AccelGyroOn(ICM20948Ctrl_t *sensor);
void ICM_SetGyroRateLPF(ICM20948Ctrl_t *sensor, uint8_t rate, uint8_t lpf);
void ICM_SetGyroLPF(ICM20948Ctrl_t *sensor, uint8_t lpf);

void ICM20948_read(ICM20948Ctrl_t *sensor);

#ifdef __cplusplus
}
#endif

#endif /* ICM20948_H_ */
