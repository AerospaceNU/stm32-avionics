/*
 * imu_icm20600.c
 */

#include "imu_icm20600.h"

#if HAS_DEV(IMU_ICM20600)

typedef struct {
  float accel_xout;
  float accel_yout;
  float accel_zout;
  int16_t temp_out;
  int16_t gyro_xout;
  int16_t gyro_yout;
  int16_t gyro_zout;
} imu_out_t;
#define ACCEL_OUT_SIZE \
  14  // No longer sizeOf sinze imu_out_t holds the output stuff, not just raw
      // int16s

// Registers
#define ACCEL_XOUT_H 0x3B
#define PWR_MGMT_1 0x6B
#define SPI_MAX_DELAY 10  // ms

static uint8_t REG_WHO_AM_I = 0x75;
static uint8_t WHOAMI = 0x11;

static float getGyroSensitivity(ImuICM20600GyroFullscale_e scale) {
  float f = 0.0;
  switch (scale) {
    case (ICM20602_GYRO_RANGE_250dps):
      f = 131;
      break;
    case (ICM20602_GYRO_RANGE_500dps):
      f = 65.5;
      break;
    case (ICM20602_GYRO_RANGE_1000dps):
      f = 32.8;
      break;
    case (ICM20602_GYRO_RANGE_2000dps):
      f = 16.4;
      break;
  }

  return f;
}

static void setGyroFullscale(ImuICM20600Ctrl_s *sensor,
                             ImuICM20600GyroFullscale_e range) {
  spi_writeRegister(&sensor->spi, 0x1B, 0x00 | range << 3);
  sensor->gyroSensitivity = getGyroSensitivity(range);
}

static float getAccelSensitivity(ImuICM20600AccelFullscale_e scale) {
  float f = 0.0;
  switch (scale) {
    case (ICM20600_ACCEL_RANGE_2G):
      f = 16384.0;
      break;
    case (ICM20600_ACCEL_RANGE_4G):
      f = 8192.0;
      break;
    case (ICM20600_ACCEL_RANGE_8G):
      f = 4096.0;
      break;
    case (ICM20600_ACCEL_RANGE_16G):
      f = 2048.0;
      break;
  }

  return f;
}

static void setAccelFullscale(ImuICM20600Ctrl_s *sensor,
                              ImuICM20600AccelFullscale_e range) {
  spi_writeRegister(&sensor->spi, 0x1C, 0x00 | range << 3);
  sensor->accelSensitivity = getAccelSensitivity(range);
}

static bool isConnected(ImuICM20600Ctrl_s *sensor) {
  uint8_t id = spi_readRegister(&sensor->spi, REG_WHO_AM_I);
  return (id == WHOAMI) || (id == 0xAF);  // icm 20608-g special case
                                          // page 21,
  // https://invensense.tdk.com/wp-content/uploads/2015/03/RM-000030-v1.0.pdf
}

bool icm20600_init(ImuICM20600Ctrl_s *sensor,
                   ImuICM20600AccelFullscale_e accelFullscale,
                   ImuICM20600GyroFullscale_e gyroFullscale) {
  HAL_GPIO_WritePin(sensor->spi.port, sensor->spi.pin, 1);
  HAL_Delay(1);

  // Reset the device
  // Serial.println("resetting");
  spi_writeRegister(&sensor->spi, PWR_MGMT_1, (1 << 7));
  HAL_Delay(2);  // Wait 2ms

  // Disable sleep, use the best clock source
  spi_writeRegister(&sensor->spi, PWR_MGMT_1, 0x00 | 0);

  if (!isConnected(sensor)) {
    return false;
  }

  setGyroFullscale(sensor, accelFullscale);
  setAccelFullscale(sensor, gyroFullscale);

  return true;
}

void icm20600_getData(ImuICM20600Ctrl_s *sensor) {
  // Allocate a buffer
  uint8_t txBuf[ACCEL_OUT_SIZE + 1] = {0};
  uint8_t rxBuf[ACCEL_OUT_SIZE + 1] = {0};
  txBuf[0] = ACCEL_XOUT_H;

  // bring CS pin low
  HAL_GPIO_WritePin(sensor->spi.port, sensor->spi.pin, GPIO_PIN_RESET);

  HAL_SPI_TransmitReceive(sensor->spi.hspi, txBuf, rxBuf, sizeof(txBuf),
                          SPI_MAX_DELAY);

  // bring CS pin high
  HAL_GPIO_WritePin(sensor->spi.port, sensor->spi.pin, GPIO_PIN_SET);

  sensor->data.accelRaw.x = ((int16_t)rxBuf[2] << 8) | (rxBuf[1]);
  sensor->data.accelRaw.y = ((int16_t)rxBuf[4] << 8) | (rxBuf[3]);
  sensor->data.accelRaw.z = ((int16_t)rxBuf[6] << 8) | (rxBuf[5]);
  sensor->tRawVal = ((int16_t)rxBuf[8] << 8) | (rxBuf[7]);
  sensor->data.angVelRaw.x = ((int16_t)rxBuf[10] << 8) | (rxBuf[9]);
  sensor->data.angVelRaw.y = ((int16_t)rxBuf[12] << 8) | (rxBuf[11]);
  sensor->data.angVelRaw.z = ((int16_t)rxBuf[14] << 8) | (rxBuf[13]);
}
//}
//

//
// uint8_t isConnected() {
// return Read8(REG_WHO_AM_I, address) == WHOAMI;
//}
//
// void printReg(uint8_t reg) {
// Serial.print(reg, HEX);
// Serial.print(": ");
// Serial.println(Read8(reg, address), DEC);
//}

#endif  // HAS_DEV(IMU_LSM9DS1)
