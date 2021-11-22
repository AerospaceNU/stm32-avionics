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
    int16_t mag_x_raw;
    int16_t mag_y_raw;
    int16_t mag_z_raw;
    int16_t temp_raw;
} ICM20948_RawData_t;
#define RAW_DATA_SIZE = sizeof(ICM20948_RAWData_t);

// Accelerometer fullscale
typedef enum {
    ACCEL_2G = 0,
    ACCEL_4G,
    ACCEL_8G,
    ACCEL_16G
} ICM20948_AccelFullscale_t;

// Gyro fullscale options
typedef enum {
    GYRO_250_DPS = 0,
    GYRO_500_DPS,
    GYRO_1000_DPS,
    GYRO_2000_DPS
} ICM20948_GyroFullscale_t;

// Filtering options

// Option registers
// Note that these are ordered backwards of the datasheet
// So the last element of the enum is the top bit
#define PWR_MGMT_1 0x6

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
	ICM20948Data_t imuData;
    uint8_t last_bank;
} ICM20948Ctrl_t;

bool ICM20948_is_connected(ICM20948Ctrl_t* sensor);
bool ICM20948_init(ICM20948Ctrl_t* sensor);
bool ICM20948_read(ICM20948Ctrl_t* sensor);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LSM9DS1_H_ */
