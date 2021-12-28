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

uint16_t accel_data[3];
uint16_t gyro_data[3];
int16_t mag_data[3];

#define SPI_BUS			(&hspi1) // ***
#define UART_BUS		(&huart1) // ***

#define USER_BANK_SEL	(0x7F)
#define USER_BANK_0		(0x00)
#define USER_BANK_1		(0x10)
#define USER_BANK_2		(0x20)
#define USER_BANK_3		(0x30)

#define PWR_MGMT_1 		(0x06)
#define PWR_MGMT_2		(0x07)
#define GYRO_CONFIG_1	(0x01)


#define CLK_BEST_AVAIL	(0x01)
#define GYRO_RATE_250	(0x00)
#define GYRO_LPF_17HZ 	(0x29)

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
  // ICM_20948_ACCEL_CONFIG_t accel_config;
  // ICM_20948_GYRO_CONFIG_t gyro_config;
} ICM20948Ctrl_t;


/**
 * Call PowerOn() to set up the IMU
 */
bool ICM_PowerOn();

uint8_t ICM_WHOAMI(ICM20948Ctrl_t *sensor);
void ICM_SelectBank(ICM20948Ctrl_t *sensor, uint8_t bank);
void ICM_ReadAccelGyroData(ICM20948Ctrl_t *sensor);
void ICM_ReadMagData(ICM20948Ctrl_t *sensor);
uint16_t ICM_Initialize(ICM20948Ctrl_t *sensor);
void ICM_SelectBank(ICM20948Ctrl_t *sensor, uint8_t bank);
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
