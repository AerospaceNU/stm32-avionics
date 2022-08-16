/*
 * accel_h3lis331dl.c
 *
 *  Created on: May 26, 2020
 *      Author: John
 */

#include "accel_h3lis331dl.h"

#if HAS_DEV(ACCEL_H3LIS331DL)

#define H3LIS331DL_SPI_REG_MASK (1 << 7)
#define HIGH_G_ACC_ADDR 0x19 << 1
#define REG_WHO_AM_I 0x0F
#define REG_OUT_X_L 0x28
#define REG_OUT_X_H 0x29
#define REG_OUT_Y_L 0x2A
#define REG_OUT_Y_H 0x2B
#define REG_OUT_Z_L 0x2C
#define REG_OUT_Z_H 0x2D
#define REG_CTRL1 0x20

// CTRL_REG1 Values
#define PWR_MODE_ON (1 << 5)
#define DATA_RATE_50HZ (0 << 3)
#define DATA_RATE_100HZ (1 << 3)
#define DATA_RATE_400HZ (2 << 3)
#define DATA_RATE_1000HZ (3 << 3)
#define Z_AXIS_ENABLE (1 << 2)
#define Y_AXIS_ENABLE (1 << 1)
#define X_AXIS_ENABLE (1 << 0)

static uint8_t whoAmI(AccelH3lis331dlCtrl_t *sensor) {
  return SPI_ReadRegister(&sensor->spi, H3LIS331DL_SPI_REG_MASK | REG_WHO_AM_I);
}

static void accelH3lis331dl_getDataRaw(AccelH3lis331dlCtrl_t *sensor) {
  // Takes x, y, and z axis readings
  uint8_t x_l =
      SPI_ReadRegister(&sensor->spi, H3LIS331DL_SPI_REG_MASK | REG_OUT_X_L);
  uint8_t x_h =
      SPI_ReadRegister(&sensor->spi, H3LIS331DL_SPI_REG_MASK | REG_OUT_X_H);
  uint8_t y_l =
      SPI_ReadRegister(&sensor->spi, H3LIS331DL_SPI_REG_MASK | REG_OUT_Y_L);
  uint8_t y_h =
      SPI_ReadRegister(&sensor->spi, H3LIS331DL_SPI_REG_MASK | REG_OUT_Y_H);
  uint8_t z_l =
      SPI_ReadRegister(&sensor->spi, H3LIS331DL_SPI_REG_MASK | REG_OUT_Z_L);
  uint8_t z_h =
      SPI_ReadRegister(&sensor->spi, H3LIS331DL_SPI_REG_MASK | REG_OUT_Z_H);

  // Writes combined h and l byte to struct
  sensor->val.raw.x = ((int16_t)x_h << 8) | (x_l);
  sensor->val.raw.y = ((int16_t)y_h << 8) | (y_l);
  sensor->val.raw.z = ((int16_t)z_h << 8) | (z_l);
}

static void accelH3lis331dl_getAdj(AccelH3lis331dlCtrl_t *sensor) {
  int sampleCount = 100;
  int adjx = 0;
  int adjy = 0;
  int adjz = 0;
  for (int i = 0; i < sampleCount; i++) {
    accelH3lis331dl_getDataRaw(sensor);
    adjx += sensor->val.raw.x;
    adjy += sensor->val.raw.y;
    adjz += sensor->val.raw.z;
    HAL_Delay(5);
  }
  sensor->adjVal.x = (adjx / sampleCount) * sensor->gain;
  sensor->adjVal.y = (adjy / sampleCount) * sensor->gain;
  sensor->adjVal.z = (adjz / sampleCount) * sensor->gain - 9.80665;
}

static void accelH3lis331dl_getGain(AccelH3lis331dlCtrl_t *sensor) {
  sensor->gain = 0.02942;
}

bool accelH3lis331dl_init(AccelH3lis331dlCtrl_t *sensor, SpiCtrl_t spi) {
  sensor->spi = spi;
  HAL_GPIO_WritePin(sensor->spi.port, sensor->spi.pin, GPIO_PIN_SET);

  uint8_t me = whoAmI(sensor);
  if (me == 0x00 || me == 0xFF) {
    return false;
  }
  SPI_WriteRegister(&sensor->spi, REG_CTRL1,
                    PWR_MODE_ON | DATA_RATE_100HZ | Z_AXIS_ENABLE |
                        Y_AXIS_ENABLE | X_AXIS_ENABLE);
  accelH3lis331dl_getGain(sensor);
  accelH3lis331dl_getAdj(sensor);
  return true;
}

void accelH3lis331dl_getData(AccelH3lis331dlCtrl_t *sensor) {
  accelH3lis331dl_getDataRaw(sensor);
  sensor->val.realMps2.x =
      (sensor->gain * sensor->val.raw.x) - sensor->adjVal.x;
  sensor->val.realMps2.y =
      (sensor->gain * sensor->val.raw.y) - sensor->adjVal.y;
  sensor->val.realMps2.z =
      (sensor->gain * sensor->val.raw.z) - sensor->adjVal.z;
}

#endif  // HAS_DEV(ACCEL_H3LIS331DL)
