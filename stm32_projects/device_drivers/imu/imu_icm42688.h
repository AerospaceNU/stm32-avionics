/*
 * imu_icm20600.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_IMU_IMU_ICM42688_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_IMU_IMU_ICM42688_H_

#include "board_config_common.h"
#include "math_utils.h"
#include "spi_driver.h"

class ImuIcm42688 {
 public:
  enum class GyroFullscale { FS_2000_DPS = 0, FS_1000_DPS, NUM_GYRO_FULLSCALE };
  enum class AccelFullscale { FS_16G = 0, FS_8G, NUM_ACCEL_FULLSCALE };

  enum class GyroDataRate : uint8_t {
    RATE_32KHZ = 1,
    RATE_16KHZ,
    RATE_8KHZ,
    RATE_4KHZ,
    RATE_2KHZ,
    RATE_1KHZ,
    RATE_200HZ,
    RATE_100HZ
  };

  enum class AccelDataRate : uint8_t {
    RATE_32KHZ = 1,
    RATE_16KHZ,
    RATE_8KHZ,
    RATE_4KHZ,
    RATE_2KHZ,
    RATE_1KHZ,
    RATE_200HZ,
    RATE_100HZ
  };

  bool begin(SpiCtrl_t spidev);
  void newData();
  double getAccelFullscaleMps2();

  void setGyroConfig(const GyroFullscale range, GyroDataRate gyroRate);
  void setAccelConfig(const AccelFullscale range, AccelDataRate accelRate);

  ImuData_s data;
  double tempC;
  int16_t tRaw;

 private:
  void setBank(int bank);

  SpiCtrl_t spi;

  float accelSensitivity;
  float gyroSensitivity;
};

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_IMU_IMU_ICM42688_H_
