/*
 * H3LIS331DL.h
 *
 *  Created on: May 26, 2020
 *      Author: John
 */

#ifndef DEVICES_H3LIS331DL_H_
#define DEVICES_H3LIS331DL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "SPIDriver.h"
#include "board_config.h"
#include HAL_HEADER

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

typedef struct raw_data_s {
  int16_t x;
  int16_t y;
  int16_t z;
} raw_data_t;

typedef struct data_s {
  uint32_t t;
  double x;
  double y;
  double z;
} data_t;

typedef struct H3LIS331DLCtrl_s {
  SPICtrld_t H3LIS331DLSPI;
  data_t adjVal;
  double gain;
  raw_data_t rawVal;
  data_t val;
} H3LIS331DLCtrl_t;

uint8_t whoAmI(H3LIS331DLCtrl_t *sensor);
uint8_t readCtrl1(H3LIS331DLCtrl_t *sensor);
void H3LIS331DL_init(H3LIS331DLCtrl_t *sensor);
void H3LIS331DL_get_data_raw(H3LIS331DLCtrl_t *sensor);
void H3LIS331DL_get_data(H3LIS331DLCtrl_t *sensor);
void H3LIS331DL_get_adj(H3LIS331DLCtrl_t *sensor);
void H3LIS331DL_get_gain(H3LIS331DLCtrl_t *sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_H3LIS331DL_H_
