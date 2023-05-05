/*
 * ICM20948.c
 *
 *  Created on: Oct 26, 2018
 *      Author: cory
 */

#include <string.h>
#include "imu_icm20948.h"

#define PI 3.14159265358979323

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

bool ICM_PowerOn(ImuIcm20948Ctrl_s *sensor);
uint8_t ICM_WHOAMI(ImuIcm20948Ctrl_s *sensor);
void ICM_SelectBank(ImuIcm20948Ctrl_s *sensor, uint8_t bank);
void ICM_ReadAccelGyro(ImuIcm20948Ctrl_s *sensor);
void ICM_ReadMag(ImuIcm20948Ctrl_s *sensor);
uint16_t ICM_Initialize(ImuIcm20948Ctrl_s *sensor);
void ICM_SelectBank(ImuIcm20948Ctrl_s *sensor, uint8_t bank);
void ICM_Disable_I2C(ImuIcm20948Ctrl_s *sensor);
void ICM_SetClock(ImuIcm20948Ctrl_s *sensor, uint8_t clk);
void ICM_AccelGyroOff(ImuIcm20948Ctrl_s *sensor);
void ICM_AccelGyroOn(ImuIcm20948Ctrl_s *sensor);
void ICM_SetGyroRateLPF(ImuIcm20948Ctrl_s *sensor, uint8_t rate, uint8_t lpf);
void ICM_SetGyroLPF(ImuIcm20948Ctrl_s *sensor, uint8_t lpf);

/*
 *
 * I2C abstraction
 *
 */
void ICM_readBytes(ImuIcm20948Ctrl_s *sensor, uint8_t reg, uint8_t *pData,
		uint16_t Size) // ***
{
	reg = reg | 0x80;

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(sensor->hi2c,
			sensor->i2cAddr, &reg, 1, HAL_MAX_DELAY);
	ret |= HAL_I2C_Master_Receive(sensor->hi2c, sensor->i2cAddr, pData, Size,
	HAL_MAX_DELAY);
	(void) ret;
}

void ICM_WriteBytes(ImuIcm20948Ctrl_s *sensor, uint8_t reg, uint8_t *pData,
		uint16_t Size) // ***
{
	reg = reg & 0x7F;

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(sensor->hi2c,
			sensor->i2cAddr, &reg, 1, HAL_MAX_DELAY);
	ret |= HAL_I2C_Master_Transmit(sensor->hi2c, sensor->i2cAddr, pData, Size,
	HAL_MAX_DELAY);
	(void) ret;
}

void ICM_ReadOneByte(ImuIcm20948Ctrl_s *sensor, uint8_t reg, uint8_t *pData) // ***
{
	ICM_readBytes(sensor, reg, pData, 1);
}

void ICM_WriteOneByte(ImuIcm20948Ctrl_s *sensor, uint8_t reg, uint8_t Data) // ***
{
	ICM_WriteBytes(sensor, reg, &Data, 1);
}

/*
 *
 * AUX I2C abstraction for magnetometer
 *
 */
void i2c_Mag_write(ImuIcm20948Ctrl_s *sensor, uint8_t reg, uint8_t value) {
	ICM_WriteOneByte(sensor, 0x7F, 0x30);

	HAL_Delay(1);
	ICM_WriteOneByte(sensor, 0x03, 0x0C); //mode: write

	HAL_Delay(1);
	ICM_WriteOneByte(sensor, 0x04, reg); //set reg addr

	HAL_Delay(1);
	ICM_WriteOneByte(sensor, 0x06, value); //send value

	HAL_Delay(1);
}

static uint8_t ICM_Mag_Read(ImuIcm20948Ctrl_s *sensor, uint8_t reg) {
	uint8_t Data;
	ICM_WriteOneByte(sensor, 0x7F, 0x30);
	HAL_Delay(1);
	ICM_WriteOneByte(sensor, 0x03, 0x0C | 0x80);
	HAL_Delay(1);
	ICM_WriteOneByte(sensor, 0x04, reg); // set reg addr
	HAL_Delay(1);
	ICM_WriteOneByte(sensor, 0x06, 0xff); //read
	HAL_Delay(1);
	ICM_WriteOneByte(sensor, 0x7F, 0x00);
	ICM_ReadOneByte(sensor, 0x3B, &Data);
	HAL_Delay(1);
	return Data;
}

void ICM20948_READ_MAG(ImuIcm20948Ctrl_s *sensor, int16_t magn[3]) {
	uint8_t mag_buffer[10];

	mag_buffer[0] = ICM_Mag_Read(sensor, 0x01);

	mag_buffer[1] = ICM_Mag_Read(sensor, 0x11);
	mag_buffer[2] = ICM_Mag_Read(sensor, 0x12);
	magn[0] = mag_buffer[1] | mag_buffer[2] << 8;
	mag_buffer[3] = ICM_Mag_Read(sensor, 0x13);
	mag_buffer[4] = ICM_Mag_Read(sensor, 0x14);
	magn[1] = mag_buffer[3] | mag_buffer[4] << 8;
	mag_buffer[5] = ICM_Mag_Read(sensor, 0x15);
	mag_buffer[6] = ICM_Mag_Read(sensor, 0x16);
	magn[2] = mag_buffer[5] | mag_buffer[6] << 8;

	i2c_Mag_write(sensor, 0x31, 0x01);
}

/*
 *
 * Read magnetometer
 *
 */
void ICM_ReadMag(ImuIcm20948Ctrl_s *sensor) {
	uint8_t mag_buffer[10];

	mag_buffer[0] = ICM_Mag_Read(sensor, 0x01);

	mag_buffer[1] = ICM_Mag_Read(sensor, 0x11);
	mag_buffer[2] = ICM_Mag_Read(sensor, 0x12);
	sensor->mData.raw.x = mag_buffer[1] | mag_buffer[2] << 8;
	mag_buffer[3] = ICM_Mag_Read(sensor, 0x13);
	mag_buffer[4] = ICM_Mag_Read(sensor, 0x14);
	sensor->mData.raw.y = mag_buffer[3] | mag_buffer[4] << 8;
	mag_buffer[5] = ICM_Mag_Read(sensor, 0x15);
	mag_buffer[6] = ICM_Mag_Read(sensor, 0x16);
	sensor->mData.raw.z = mag_buffer[5] | mag_buffer[6] << 8;

	i2c_Mag_write(sensor, 0x31, 0x01);
}

/*
 *
 * Sequence to setup ICM290948 as early as possible after power on
 *
 */
bool ICM_PowerOn(ImuIcm20948Ctrl_s *sensor) {
	uint8_t whoami = 0xEA;
	uint8_t test = ICM_WHOAMI(sensor);
	(void) whoami;
	(void) test;

	ICM_SelectBank(sensor, USER_BANK_0);
	HAL_Delay(10);
	ICM_Disable_I2C(sensor);
	HAL_Delay(10);
	ICM_SetClock(sensor, (uint8_t) CLK_BEST_AVAIL);
	HAL_Delay(10);
	ICM_AccelGyroOff(sensor);
	HAL_Delay(20);
	ICM_AccelGyroOn(sensor);
	HAL_Delay(10);
	ICM_Initialize(sensor);

	return whoami == test;

}
uint16_t ICM_Initialize(ImuIcm20948Ctrl_s *sensor) {
	ICM_SelectBank(sensor, USER_BANK_2);
	HAL_Delay(20);
	ICM_SetGyroRateLPF(sensor, GYRO_RATE_250, GYRO_LPF_17HZ);
	HAL_Delay(10);

	// Set gyroscope sample rate to 100hz (0x0A) in GYRO_SMPLRT_DIV register (0x00)
	ICM_WriteOneByte(sensor, 0x00, 0x0A);
	HAL_Delay(10);

	// Set accelerometer low pass filter to 136hz (0x11) and the rate to 8G (0x04) in register ACCEL_CONFIG (0x14)
	ICM_WriteOneByte(sensor, 0x14, (0x04 | 0x11));

	// Set accelerometer sample rate to 225hz (0x00) in ACCEL_SMPLRT_DIV_1 register (0x10)
	ICM_WriteOneByte(sensor, 0x10, 0x00);
	HAL_Delay(10);

	// Set accelerometer sample rate to 100 hz (0x0A) in ACCEL_SMPLRT_DIV_2 register (0x11)
	ICM_WriteOneByte(sensor, 0x11, 0x0A);
	HAL_Delay(10);

	ICM_SelectBank(sensor, USER_BANK_2);
	HAL_Delay(20);

	// Configure AUX_I2C Magnetometer (onboard ICM-20948)
	ICM_WriteOneByte(sensor, 0x7F, 0x00); // Select user bank 0
	ICM_WriteOneByte(sensor, 0x0F, 0x30); // INT Pin / Bypass Enable Configuration
	ICM_WriteOneByte(sensor, 0x03, 0x20); // I2C_MST_EN
	ICM_WriteOneByte(sensor, 0x7F, 0x30); // Select user bank 3
	ICM_WriteOneByte(sensor, 0x01, 0x4D); // I2C Master mode and Speed 400 kHz
	ICM_WriteOneByte(sensor, 0x02, 0x01); // I2C_SLV0 _DLY_ enable
	ICM_WriteOneByte(sensor, 0x05, 0x81); // enable IIC	and EXT_SENS_DATA==1 Byte

	// Initialize magnetometer
	i2c_Mag_write(sensor, 0x32, 0x01); // Reset AK8963
	HAL_Delay(1000);
	i2c_Mag_write(sensor, 0x31, 0x02); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output

	return 1337;
}

void ICM_ReadAccelGyro(ImuIcm20948Ctrl_s *sensor) {
	uint8_t raw_data[12];
	ICM_readBytes(sensor, 0x2D, raw_data, 12);

	sensor->agData.accelRaw.x = (raw_data[0] << 8) | raw_data[1];
	sensor->agData.accelRaw.y = (raw_data[2] << 8) | raw_data[3];
	sensor->agData.accelRaw.z = (raw_data[4] << 8) | raw_data[5];

	sensor->agData.angVelRaw.x = (raw_data[6] << 8) | raw_data[7];
	sensor->agData.angVelRaw.y = (raw_data[8] << 8) | raw_data[9];
	sensor->agData.angVelRaw.z = (raw_data[10] << 8) | raw_data[11];

	sensor->agData.accelRealMps2.x = sensor->agData.accelRaw.x / 8.0;
	sensor->agData.accelRealMps2.y = sensor->agData.accelRaw.y / 8.0;
	sensor->agData.accelRealMps2.z = sensor->agData.accelRaw.z / 8.0;
	const float gyroScale = (1.0 / 250.0 * PI / 180.0);
	sensor->agData.angVelRealRadps.x = sensor->agData.angVelRaw.x * gyroScale;
	sensor->agData.angVelRealRadps.y = sensor->agData.angVelRaw.y * gyroScale;
	sensor->agData.angVelRealRadps.z = sensor->agData.angVelRaw.z * gyroScale;
}
void ICM_SelectBank(ImuIcm20948Ctrl_s *sensor, uint8_t bank) {
	ICM_WriteOneByte(sensor, USER_BANK_SEL, bank);
}
void ICM_Disable_I2C(ImuIcm20948Ctrl_s *sensor) {
	ICM_WriteOneByte(sensor, 0x03, 0x78);
}
void ICM_SetClock(ImuIcm20948Ctrl_s *sensor, uint8_t clk) {
	ICM_WriteOneByte(sensor, PWR_MGMT_1, clk);
}
void ICM_AccelGyroOff(ImuIcm20948Ctrl_s *sensor) {
	ICM_WriteOneByte(sensor, PWR_MGMT_2, (0x38 | 0x07));
}
void ICM_AccelGyroOn(ImuIcm20948Ctrl_s *sensor) {
	ICM_WriteOneByte(sensor, 0x07, (0x00 | 0x00));
}
uint8_t ICM_WHOAMI(ImuIcm20948Ctrl_s *sensor) {
	uint8_t spiData = 0x01;
	ICM_ReadOneByte(sensor, 0x00, &spiData);
	return spiData;
}
void ICM_SetGyroRateLPF(ImuIcm20948Ctrl_s *sensor, uint8_t rate, uint8_t lpf) {
	ICM_WriteOneByte(sensor, GYRO_CONFIG_1, (rate | lpf));
}

bool icm20948_init(ImuIcm20948Ctrl_s *sensor, I2C_HandleTypeDef *hi2c,
		uint8_t i2cAddr) {
	ICM_SelectBank(sensor, USER_BANK_0);
	HAL_Delay(10);
	return ICM_PowerOn(sensor);
}

void icm20948_getData(ImuIcm20948Ctrl_s *sensor) {
	ICM_SelectBank(sensor, USER_BANK_0);
	ICM_ReadAccelGyro(sensor);
	ICM_ReadMag(sensor);
}
