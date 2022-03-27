/*
 * H3LIS331DL.c
 *
 *  Created on: May 26, 2020
 *      Author: John
 */

#include "H3LIS331DL.h"

#ifdef HAS_H3LIS331DL

uint8_t whoAmI(H3LIS331DLCtrl_t *sensor) {
  return SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_WHO_AM_I);
}

uint8_t readCtrl1(H3LIS331DLCtrl_t *sensor) {
  return SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_CTRL1);
}

void H3LIS331DL_init(H3LIS331DLCtrl_t *sensor) {
  HAL_GPIO_WritePin(sensor->H3LIS331DLSPI.port, sensor->H3LIS331DLSPI.pin,
                    GPIO_PIN_SET);
  SPI_WriteRegister(&sensor->H3LIS331DLSPI, REG_CTRL1,
                    PWR_MODE_ON | DATA_RATE_100HZ | Z_AXIS_ENABLE |
                        Y_AXIS_ENABLE | X_AXIS_ENABLE);
  H3LIS331DL_get_gain(sensor);
  H3LIS331DL_get_adj(sensor);
}

void H3LIS331DL_get_data_raw(H3LIS331DLCtrl_t *sensor) {
  // Takes x, y, and z axis readings
  uint8_t x_l =
      SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_X_L);
  uint8_t x_h =
      SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_X_H);
  uint8_t y_l =
      SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Y_L);
  uint8_t y_h =
      SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Y_H);
  uint8_t z_l =
      SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Z_L);
  uint8_t z_h =
      SPI_ReadRegister(&sensor->H3LIS331DLSPI, (1 << 7) | REG_OUT_Z_H);

  // Writes combined h and l byte to struct
  sensor->rawVal.x = ((int16_t)x_h << 8) | (x_l);
  sensor->rawVal.y = ((int16_t)y_h << 8) | (y_l);
  sensor->rawVal.z = ((int16_t)z_h << 8) | (z_l);
}

void H3LIS331DL_get_data(H3LIS331DLCtrl_t *sensor) {
  H3LIS331DL_get_data_raw(sensor);
  sensor->val.x = (sensor->gain * sensor->rawVal.x) - sensor->adjVal.x;
  sensor->val.y = (sensor->gain * sensor->rawVal.y) - sensor->adjVal.y;
  sensor->val.z = (sensor->gain * sensor->rawVal.z) - sensor->adjVal.z;
}

void H3LIS331DL_get_adj(H3LIS331DLCtrl_t *sensor) {
  int sampleCount = 100;
  int adjx = 0;
  int adjy = 0;
  int adjz = 0;
  for (int i = 0; i < sampleCount; i++) {
    H3LIS331DL_get_data_raw(sensor);
    adjx += sensor->rawVal.x;
    adjy += sensor->rawVal.y;
    adjz += sensor->rawVal.z;
    HAL_Delay(5);
  }
  sensor->adjVal.x = (adjx / sampleCount) * sensor->gain;
  sensor->adjVal.y = (adjy / sampleCount) * sensor->gain;
  sensor->adjVal.z = (adjz / sampleCount) * sensor->gain - 9.80665;
}

void H3LIS331DL_get_gain(H3LIS331DLCtrl_t *sensor) { sensor->gain = 0.02942; }

#endif
