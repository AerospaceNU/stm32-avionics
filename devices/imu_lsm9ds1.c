/*
 * imu_lsm9ds1.c
 */

#include "imu_lsm9ds1.h"

#if HAS_DEV(IMU_LSM9DS1)

#define LSM9DS1_SPI_REG_MASK (1 << 7)

static uint8_t agWhoAmI(Lsm9ds1AgCtrl_s *sensor) {
  return spi_readRegister(&sensor->lsm9ds1Spi,
                          LSM9DS1_SPI_REG_MASK | REG_WHO_AM_I_AG);
}

static uint8_t mWhoAmI(Lsm9ds1MCtrl_s *sensor) {
  return spi_readRegister(&sensor->lsm9ds1Spi,
                          LSM9DS1_SPI_REG_MASK | REG_WHO_AM_I_M);
}

static void lsm9ds1_getDataRaw(ImuLsm9ds1Ctrl_s *sensor) {
  // Takes x, y, and z axis readings
  uint8_t x_l_xl = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                    LSM9DS1_SPI_REG_MASK | OUT_X_L_XL);
  uint8_t x_h_xl = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                    LSM9DS1_SPI_REG_MASK | OUT_X_H_XL);
  uint8_t y_l_xl = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                    LSM9DS1_SPI_REG_MASK | OUT_Y_L_XL);
  uint8_t y_h_xl = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                    LSM9DS1_SPI_REG_MASK | OUT_Y_H_XL);
  uint8_t z_l_xl = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                    LSM9DS1_SPI_REG_MASK | OUT_Z_L_XL);
  uint8_t z_h_xl = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                    LSM9DS1_SPI_REG_MASK | OUT_Z_H_XL);

  uint8_t x_l_g = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                   LSM9DS1_SPI_REG_MASK | OUT_X_L_G);
  uint8_t x_h_g = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                   LSM9DS1_SPI_REG_MASK | OUT_X_H_G);
  uint8_t y_l_g = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                   LSM9DS1_SPI_REG_MASK | OUT_Y_L_G);
  uint8_t y_h_g = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                   LSM9DS1_SPI_REG_MASK | OUT_Y_H_G);
  uint8_t z_l_g = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                   LSM9DS1_SPI_REG_MASK | OUT_Z_L_G);
  uint8_t z_h_g = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                   LSM9DS1_SPI_REG_MASK | OUT_Z_H_G);

  uint8_t x_l_m =
      spi_readRegister(&sensor->m.lsm9ds1Spi, LSM9DS1_SPI_REG_MASK | OUT_X_L_M);
  uint8_t x_h_m =
      spi_readRegister(&sensor->m.lsm9ds1Spi, LSM9DS1_SPI_REG_MASK | OUT_X_H_M);
  uint8_t y_l_m =
      spi_readRegister(&sensor->m.lsm9ds1Spi, LSM9DS1_SPI_REG_MASK | OUT_Y_L_M);
  uint8_t y_h_m =
      spi_readRegister(&sensor->m.lsm9ds1Spi, LSM9DS1_SPI_REG_MASK | OUT_Y_H_M);
  uint8_t z_l_m =
      spi_readRegister(&sensor->m.lsm9ds1Spi, LSM9DS1_SPI_REG_MASK | OUT_Z_L_M);
  uint8_t z_h_m =
      spi_readRegister(&sensor->m.lsm9ds1Spi, LSM9DS1_SPI_REG_MASK | OUT_Z_H_M);

  uint8_t t_l = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                 LSM9DS1_SPI_REG_MASK | OUT_TEMP_L);
  uint8_t t_h = spi_readRegister(&sensor->ag.lsm9ds1Spi,
                                 LSM9DS1_SPI_REG_MASK | OUT_TEMP_H);

  // Writes combined h and l byte to struct
  sensor->data.accelRaw.x = ((int16_t)x_h_xl << 8) | (x_l_xl);
  sensor->data.accelRaw.y = ((int16_t)y_h_xl << 8) | (y_l_xl);
  sensor->data.accelRaw.z = ((int16_t)z_h_xl << 8) | (z_l_xl);

  sensor->data.angVelRaw.x = ((int16_t)x_h_g << 8) | (x_l_g);
  sensor->data.angVelRaw.y = ((int16_t)y_h_g << 8) | (y_l_g);
  sensor->data.angVelRaw.z = ((int16_t)z_h_g << 8) | (z_l_g);

  sensor->data.magRaw.x = ((int16_t)x_h_m << 8) | (x_l_m);
  sensor->data.magRaw.y = ((int16_t)y_h_m << 8) | (y_l_m);
  sensor->data.magRaw.z = ((int16_t)z_h_m << 8) | (z_l_m);

  sensor->ag.tRawVal = ((int16_t)t_h << 8) | (t_l);
}

void lsm9ds1_getData(ImuLsm9ds1Ctrl_s *sensor) {
  lsm9ds1_getDataRaw(sensor);
  sensor->data.accelRealMps2.x = sensor->ag.aRes * sensor->data.accelRaw.x;
  sensor->data.accelRealMps2.y = sensor->ag.aRes * sensor->data.accelRaw.y;
  sensor->data.accelRealMps2.z = sensor->ag.aRes * sensor->data.accelRaw.z;

  sensor->data.angVelRealRadps.x = sensor->ag.gRes * sensor->data.angVelRaw.x;
  sensor->data.angVelRealRadps.y = sensor->ag.gRes * sensor->data.angVelRaw.y;
  sensor->data.angVelRealRadps.z = sensor->ag.gRes * sensor->data.angVelRaw.z;

  sensor->data.magRealG.x = sensor->m.mRes * sensor->data.magRaw.x;
  sensor->data.magRealG.y = sensor->m.mRes * sensor->data.magRaw.y;
  sensor->data.magRealG.z = sensor->m.mRes * sensor->data.magRaw.z;
}

static void lsm9ds1_calcRes(ImuLsm9ds1Ctrl_s *sensor) {
  switch (sensor->ag.aFs) {
    case 0 << 3:
      sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_2;
      break;
    case 2 << 3:
      sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_4;
      break;
    case 3 << 3:
      sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_8;
      break;
    case 1 << 3:
      sensor->ag.aRes = SENSITIVITY_ACCELEROMETER_16;
      break;
    default:
      sensor->ag.aRes = 0;
      break;
  }

  switch (sensor->ag.gFs) {
    case 0 << 3:
      sensor->ag.gRes = SENSITIVITY_GYROSCOPE_245;
      break;
    case 1 << 3:
      sensor->ag.gRes = SENSITIVITY_GYROSCOPE_500;
      break;
    case 3 << 3:
      sensor->ag.gRes = SENSITIVITY_GYROSCOPE_2000;
      break;
    default:
      sensor->ag.gRes = 0;
      break;
  }
  switch (sensor->m.mFs) {
    case 0 << 5:
      sensor->m.mRes = SENSITIVITY_MAGNETOMETER_4;
      break;
    case 1 << 5:
      sensor->m.mRes = SENSITIVITY_MAGNETOMETER_8;
      break;
    case 2 << 5:
      sensor->m.mRes = SENSITIVITY_MAGNETOMETER_12;
      break;
    case 3 << 5:
      sensor->m.mRes = SENSITIVITY_MAGNETOMETER_16;
      break;
    default:
      sensor->m.mRes = 0;
      break;
  }
}

bool lsm9ds1_init(ImuLsm9ds1Ctrl_s *sensor) {
  // Pull CS High
  HAL_GPIO_WritePin(sensor->ag.lsm9ds1Spi.port, sensor->ag.lsm9ds1Spi.pin,
                    GPIO_PIN_SET);
  HAL_GPIO_WritePin(sensor->m.lsm9ds1Spi.port, sensor->m.lsm9ds1Spi.pin,
                    GPIO_PIN_SET);

  // Verify whoAmI
  uint8_t iam = agWhoAmI(&sensor->ag);
  if (iam == 0xFF || iam == 0x00) {
    return false;
  }
  iam = mWhoAmI(&sensor->m);
  if (iam == 0xFF || iam == 0x00) {
    return false;
  }

  // Initialize Gyroscope
  spi_writeRegister(&sensor->ag.lsm9ds1Spi, CTRL_REG1_G,
                    ODR_G_238 | sensor->ag.gFs | BW_G_2);

  // Initialize Accelerometer
  spi_writeRegister(&sensor->ag.lsm9ds1Spi, CTRL_REG6_XL,
                    ODR_XL_50 | sensor->ag.aFs);

  // Initialize Magnetometer
  spi_writeRegister(&sensor->m.lsm9ds1Spi, CTRL_REG3_M,
                    I2C_DISABLE_M_0 | SIM_M_0 | MD_M_CC);
  spi_writeRegister(&sensor->m.lsm9ds1Spi, CTRL_REG1_M, OM_M_MP | DO_M_5);
  spi_writeRegister(&sensor->m.lsm9ds1Spi, CTRL_REG2_M, sensor->m.mFs);
  spi_writeRegister(&sensor->m.lsm9ds1Spi, CTRL_REG4_M, OMZ_M_MP);

  lsm9ds1_calcRes(sensor);

  return true;
}

#endif  // HAS_DEV(IMU_LSM9DS1)
