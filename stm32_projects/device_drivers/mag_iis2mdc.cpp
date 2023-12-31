/*
 * imu_icm20600.c
 */

#include "mag_iis2mdc.h"

#include "bit_helper.h"

#if HAS_DEV(MAG_IIS2MDC)

static uint8_t readByte(ImuIIS2MDCCtrl_s *sensor, uint8_t addr) {
  //    HAL_I2C_Master_Transmit(sensor->hi2c, sensor->address, &addr, 1,
  // HAL_MAX_DELAY);
  uint8_t ret;
  //    HAL_I2C_Master_Receive(sensor->hi2c, sensor->address, &ret, 1,
  // HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(sensor->hi2c, sensor->address, addr, 1, &ret, 1,
                   HAL_MAX_DELAY);
  return ret;
}

static void readArray(ImuIIS2MDCCtrl_s *sensor, uint8_t addr, uint8_t *pData,
                      uint8_t len) {
  //    HAL_I2C_Master_Transmit(sensor->hi2c, sensor->address, &addr, 1,
  // HAL_MAX_DELAY);     HAL_I2C_Master_Receive(sensor->hi2c, sensor->address,
  // pData, len, HAL_MAX_DELAY);

  HAL_I2C_Mem_Read(sensor->hi2c, addr, sensor->address, 1, pData, len,
                   HAL_MAX_DELAY);
}

static void writeByte(ImuIIS2MDCCtrl_s *sensor, uint8_t addr, uint8_t val) {
  //    uint8_t pData[2] = {addr, val};
  //    HAL_I2C_Master_Transmit(sensor->hi2c, sensor->address, pData,
  // sizeof(pData), HAL_MAX_DELAY);

  HAL_I2C_Mem_Write(sensor->hi2c, sensor->address, addr, 1, &val, 1,
                    HAL_MAX_DELAY);
}

#define WHO_AM_I 0x4F
#define WHO_AM_I_VALUE 0b01000000

#define CFG_REG_A 0x60
#define CFG_REG_C 0x62

#define OUTX_L_REG 0x68

#include <stdio.h>

bool iis2mdc_init(ImuIIS2MDCCtrl_s *sensor, uint8_t address7bit) {
  for (int i = 1; i < 128; i++) {
    uint8_t ret = HAL_I2C_IsDeviceReady(sensor->hi2c, (uint16_t)(i << 1), 3, 5);
    if (ret != HAL_OK) /* No ACK Received At That Address */ {
      printf("No ack\n");
    } else if (ret == HAL_OK) {
      printf("Got %i\n", i);
    }
  }

  sensor->address = address7bit << 1;
  volatile uint8_t readWhoAmI = readByte(sensor, WHO_AM_I);
  if (WHO_AM_I_VALUE != readWhoAmI) {
    // return false;
  }

  // enable bdu, disable self test
  writeByte(sensor, CFG_REG_C, (1 << 3));

  // Change odr to 100hz, and enable continuous mode and temp comp
  writeByte(sensor, CFG_REG_A, (0b1 << 7) | (0b11 << 2) | (0b00 << 0));

  return true;
}

void iis2mdc_getData(ImuIIS2MDCCtrl_s *sensor) {
  // Currently still polling! TODO we can set up a latching drdy interrupt?
  uint8_t raw[6];
  readArray(sensor, OUTX_L_REG, raw, sizeof(raw));

  sensor->data.raw.x = combine_to_u16(raw[1], raw[0]);
  sensor->data.raw.y = combine_to_u16(raw[3], raw[2]);
  sensor->data.raw.z = combine_to_u16(raw[5], raw[4]);

  // Sensitivity can only ever be 1.5mGauss / LSB
  sensor->data.realGauss.x = sensor->data.raw.x * 1.5;
  sensor->data.realGauss.y = sensor->data.raw.y * 1.5;
  sensor->data.realGauss.z = sensor->data.raw.z * 1.5;
}

#endif  // HAS_DEV(IMU_LSM9DS1)
