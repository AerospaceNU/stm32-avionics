/*
 * ICM20948.h
 *
 *  Created on: November 13, 2021
 *      Author: Matt Morley
 */

#ifndef SRC_ICM20948_H_
#define SRC_ICM20948_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "SPIDriver.h"
#include "stdint.h"
#include <stdbool.h>

typedef struct __attribute__((__packed__)) ICM20948_RawData_t {
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
    uint8_t magStat1, magStat2;
} ICM20948_RawData_t;
#define RAW_DATA_SIZE 14+9

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
    uint8_t ACCEL_FCHOICE : 1;
    uint8_t ACCEL_FS_SEL : 2;
    uint8_t ACCEL_DLPFCFG : 3;
    uint8_t reserved_0 : 2;
} ICM_20948_ACCEL_CONFIG_t;

typedef struct __attribute__((__packed__))
{
    uint8_t GYRO_FCHOICE : 1;
    uint8_t GYRO_FS_SEL : 2;
    uint8_t GYRO_DLPFCFG : 3;
    uint8_t reserved_0 : 2;
} ICM_20948_GYRO_CONFIG_t;

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
	SPICtrld_t spiconfig;
	ICM20948_RawData_t rawData;
	ICM20948Data_t imuData;
    uint8_t last_bank;
    ICM_20948_ACCEL_CONFIG_t accel_config;
    ICM_20948_GYRO_CONFIG_t gyro_config;
} ICM20948Ctrl_t;

bool ICM20948_is_connected(ICM20948Ctrl_t* sensor);
bool ICM20948_init(ICM20948Ctrl_t* sensor, ICM_20948_ACCEL_CONFIG_t accel_config, ICM_20948_GYRO_CONFIG_t gyro_config);
bool ICM20948_set_config(ICM20948Ctrl_t* sensor, ICM_20948_ACCEL_CONFIG_t accel_config, ICM_20948_GYRO_CONFIG_t gyro_config);
bool ICM20948_read(ICM20948Ctrl_t* sensor);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LSM9DS1_H_ */
