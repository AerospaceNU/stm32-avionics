/*
 * H3LIS331DL.h
 *
 *  Created on: May 26, 2020
 *      Author: John
 */

#ifndef SRC_H3LIS331DL_H_
#define SRC_H3LIS331DL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

static const uint8_t HIGH_G_ACC_ADDR = 0x19 << 1;
static const uint8_t REG_WHO_AM_I = 0x0F;
static const uint8_t REG_OUT_X_L = 0x28;
static const uint8_t REG_OUT_X_H = 0x29;
static const uint8_t REG_OUT_Y_L = 0x2A;
static const uint8_t REG_OUT_Y_H = 0x2B;
static const uint8_t REG_OUT_Z_L = 0x2C;
static const uint8_t REG_OUT_Z_H = 0x2D;
static const uint8_t REG_CTRL1 = 0x20;

// CTRL_REG1 Values
static const uint8_t PWR_MODE_ON = (1 << 5);
static const uint8_t DATA_RATE_50HZ = (0 << 3);
static const uint8_t DATA_RATE_100HZ = (1 << 3);
static const uint8_t DATA_RATE_400HZ = (2 << 3);
static const uint8_t DATA_RATE_1000HZ = (3 << 3);
static const uint8_t Z_AXIS_ENABLE = (1 << 2);
static const uint8_t Y_AXIS_ENABLE = (1 << 1);
static const uint8_t X_AXIS_ENABLE = (1 << 0);


typedef struct raw_data_s{
    int16_t x;
    int16_t y;
    int16_t z;
}raw_data_t;

typedef struct data_s{
    uint32_t t;
    double x;
    double y;
    double z;
}data_t;

typedef struct H3LIS331DLCtrl_s{
    SPICtrl_t H3LIS331DLSPI;
    raw_data_t rawVal;
    data_t val;
}H3LIS331DLCtrl_t;

uint8_t whoAmI(H3LIS331DLCtrl_t* sensor);
uint8_t readCtrl1(H3LIS331DLCtrl_t* sensor);
void H3LIS331DL_init(H3LIS331DLCtrl_t* sensor);
void H3LIS331DL_get_data_raw(H3LIS331DLCtrl_t* sensor);
void H3LIS331DL_get_data(H3LIS331DLCtrl_t* sensor);

#ifdef __cplusplus
}
#endif

#endif /* SRC_H3LIS331DL_H_ */
