/*
 * ICM20948.c
 *
 *  Created on: Oct 26, 2018
 *      Author: cory
 * From: https://raw.githubusercontent.com/CoryCline/ICM20948/master/ICM20948.c
 */

// *** Three asterisks to the side of a line means this may change based on platform
// #include "main.h" // ***
// #include "gpio.h" // ***
// #include "spi.h"  // ***
// #include "usart.h"// ***
// #include "dma.h"  // ***
// #include <string.h>

#include "ICM20948.h"
#include "SPIDriver.h"

/*
 *
 * SPI abstraction
 *
 */
void ICM_readBytes(ICM20948Ctrl_t *sensor, uint8_t reg, uint8_t *pData, uint16_t len) // ***
{
  reg = reg | 0x80;
  SPI_ReadArray(&sensor->spictrl, reg, pData, len);
}

void ICM_ReadOneByte(ICM20948Ctrl_t *sensor, uint8_t reg, uint8_t *pData) // ***
{
  reg = reg | 0x80;
  *pData = SPI_ReadRegister(&sensor->spictrl, reg);
}

void ICM_WriteOneByte(ICM20948Ctrl_t *sensor, uint8_t reg, uint8_t Data) // ***
{
  reg = reg & 0x7F;
  SPI_WriteRegister(&sensor->spictrl, reg, Data);
}

/*
 *
 * AUX I2C abstraction for magnetometer
 *
 */
void i2c_Mag_write(ICM20948Ctrl_t *sensor, uint8_t reg, uint8_t value)
{
  ICM_WriteOneByte(sensor, 0x7F, 0x30);

  HAL_Delay(1);
  ICM_WriteOneByte(sensor, 0x03, 0x0C); // mode: write

  HAL_Delay(1);
  ICM_WriteOneByte(sensor, 0x04, reg); // set reg addr

  HAL_Delay(1);
  ICM_WriteOneByte(sensor, 0x06, value); // send value

  HAL_Delay(1);
}

static uint8_t ICM_Mag_Read(ICM20948Ctrl_t *sensor, uint8_t reg)
{
  uint8_t Data;
  ICM_WriteOneByte(sensor, 0x7F, 0x30);
  HAL_Delay(1);
  ICM_WriteOneByte(sensor, 0x03, 0x0C | 0x80);
  HAL_Delay(1);
  ICM_WriteOneByte(sensor, 0x04, reg); // set reg addr
  HAL_Delay(1);
  ICM_WriteOneByte(sensor, 0x06, 0xff); // read
  HAL_Delay(1);
  ICM_WriteOneByte(sensor, 0x7F, 0x00);
  ICM_ReadOneByte(sensor, 0x3B, &Data);
  HAL_Delay(1);
  return Data;
}

void ICM20948_READ_MAG(ICM20948Ctrl_t *sensor, int16_t magn[3])
{
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
void ICM_ReadMagData(ICM20948Ctrl_t *sensor)
{
  uint8_t mag_buffer[10];

  mag_buffer[0] = ICM_Mag_Read(sensor, 0x01);

  mag_buffer[1] = ICM_Mag_Read(sensor, 0x11);
  mag_buffer[2] = ICM_Mag_Read(sensor, 0x12);
  sensor->rawData.mag_x_raw = mag_buffer[1] | mag_buffer[2] << 8;
  mag_buffer[3] = ICM_Mag_Read(sensor, 0x13);
  mag_buffer[4] = ICM_Mag_Read(sensor, 0x14);
  sensor->rawData.mag_y_raw = mag_buffer[3] | mag_buffer[4] << 8;
  mag_buffer[5] = ICM_Mag_Read(sensor, 0x15);
  mag_buffer[6] = ICM_Mag_Read(sensor, 0x16);
  sensor->rawData.mag_z_raw = mag_buffer[5] | mag_buffer[6] << 8;

  i2c_Mag_write(sensor, 0x31, 0x01);

  // LSB * uT/LSB =uT
  sensor->imuData.mag_x = sensor->rawData.mag_x_raw * MAG_SENSITIVITY_ICM20948;
  sensor->imuData.mag_y = sensor->rawData.mag_y_raw * MAG_SENSITIVITY_ICM20948;
  sensor->imuData.mag_z = sensor->rawData.mag_z_raw * MAG_SENSITIVITY_ICM20948;
}

/*
 *
 * Sequence to setup ICM290948 as early as possible after power on
 * This calls initialize internally!
 *
 */
bool ICM_PowerOn(ICM20948Ctrl_t *sensor, ICM_20948_ACCEL_CONFIG_t accelConfig, ICM_20948_GYRO_CONFIG_t gyroConfig)
{
  uint8_t whoami = 0xEA;
  uint8_t test = ICM_WHOAMI(sensor);
  if (test != whoami)
    return false;

  ICM_SelectBank(sensor, USER_BANK_0);
  HAL_Delay(10);
  ICM_Disable_I2C(sensor);
  HAL_Delay(10);
  ICM_SetClock(sensor, (uint8_t)CLK_BEST_AVAIL);
  HAL_Delay(10);
  ICM_AccelGyroOff(sensor);
  HAL_Delay(20);
  ICM_AccelGyroOn(sensor);
  HAL_Delay(10);
  ICM_Initialize(sensor, accelConfig, gyroConfig);
  //} else {
  // sprintf(uart_buffer, "Failed WHO_AM_I.  %i is not 0xEA\r\n", test);
  // HAL_UART_Transmit_DMA(UART_BUS, (uint8_t*) uart_buffer, strlen(uart_buffer));
  // HAL_Delay(100);
  //}

  return true;
}

uint16_t ICM_Initialize(ICM20948Ctrl_t *sensor, ICM_20948_ACCEL_CONFIG_t accelConfig, ICM_20948_GYRO_CONFIG_t gyroConfig)
{
  ICM_SelectBank(sensor, USER_BANK_2);
  HAL_Delay(20);
  // Set Gyro low-pass, full scale filter settings
  ICM_WriteOneByte(sensor, GYRO_CONFIG_1, *((uint8_t*) &gyroConfig));
  HAL_Delay(10);

  // Set gyroscope sample rate to 100hz (0x0A) in GYRO_SMPLRT_DIV register (0x00)
  // TODO do we ever need to change this
  // Math: ODR = 1.1khz / (1+value)
  ICM_WriteOneByte(sensor, 0x00, 10);
  HAL_Delay(10);

  // Set accelerometer dlpf, full-scale
  ICM_WriteOneByte(sensor, 0x14, *((uint8_t*) &accelConfig));

  // Set accelerometer sample rate to 100hz (0x00) in ACCEL_SMPLRT_DIV_1 register (0x10) and DIV_2 (0x11)
  // TODO do we ever need to change this
  // Math: ODR = 1.1khz / (1+value)
  ICM_WriteOneByte(sensor, 0x10, 0x00);
  HAL_Delay(10);
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
  i2c_Mag_write(sensor, 0x31, 0b1000); // use i2c to set AK8963 working on Continuous measurement mode at 100hz

  return 1337;
}

void ICM_ReadAccelGyroData(ICM20948Ctrl_t *sensor)
{
  uint8_t raw_data[12];
  ICM_readBytes(sensor, 0x2D, raw_data, 12);

  sensor->rawData.accel_x_raw = (raw_data[0] << 8) | raw_data[1];
  sensor->rawData.accel_y_raw = (raw_data[2] << 8) | raw_data[3];
  sensor->rawData.accel_z_raw = (raw_data[4] << 8) | raw_data[5];

  sensor->rawData.gyro_x_raw = (raw_data[6] << 8) | raw_data[7];
  sensor->rawData.gyro_y_raw = (raw_data[8] << 8) | raw_data[9];
  sensor->rawData.gyro_z_raw = (raw_data[10] << 8) | raw_data[11];

  // TODO abstract fullscale
  sensor->imuData.accel_x = sensor->rawData.accel_x_raw / 8;
  sensor->imuData.accel_y = sensor->rawData.accel_y_raw / 8;
  sensor->imuData.accel_z = sensor->rawData.accel_z_raw / 8;

  // TODO abstract fullscale
  sensor->imuData.gyro_x = sensor->rawData.gyro_x_raw / 250;
  sensor->imuData.gyro_y = sensor->rawData.gyro_y_raw / 250;
  sensor->imuData.gyro_z = sensor->rawData.gyro_z_raw / 250;
}
void ICM_SelectBank(ICM20948Ctrl_t *sensor, uint8_t bank)
{
  ICM_WriteOneByte(sensor, USER_BANK_SEL, bank);
}
void ICM_Disable_I2C(ICM20948Ctrl_t *sensor)
{
  ICM_WriteOneByte(sensor, 0x03, 0x78);
}
void ICM_SetClock(ICM20948Ctrl_t *sensor, uint8_t clk)
{
  ICM_WriteOneByte(sensor, PWR_MGMT_1, clk);
}
void ICM_AccelGyroOff(ICM20948Ctrl_t *sensor)
{
  ICM_WriteOneByte(sensor, PWR_MGMT_2, (0x38 | 0x07));
}
void ICM_AccelGyroOn(ICM20948Ctrl_t *sensor)
{
  ICM_WriteOneByte(sensor, 0x07, (0x00 | 0x00));
}
uint8_t ICM_WHOAMI(ICM20948Ctrl_t *sensor)
{
  ICM_SelectBank(sensor, USER_BANK_0);
  uint8_t spiData = 0x01;
  ICM_ReadOneByte(sensor, 0x00, &spiData);
  return spiData;
}
void ICM_SetGyroRateLPF(ICM20948Ctrl_t *sensor, uint8_t rate, uint8_t lpf)
{
  ICM_WriteOneByte(sensor, GYRO_CONFIG_1, (rate | lpf));
}

void ICM20948_read(ICM20948Ctrl_t *sensor)
{
  ICM_ReadAccelGyroData(sensor);
  ICM_ReadMagData(sensor);
}
