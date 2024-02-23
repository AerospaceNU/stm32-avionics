/*
 * imu_icm20600.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_IMU_ICM20600_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_IMU_ICM20600_H_

#include "board_config_common.h"
#include "spi_driver.h"

#define DEG_TO_RAD(x) x *M_PI / 180.0
#define G_TO_MPS2(x) x * 9.80665

class ImuIcm42688 {
 public:
  struct FullscaleEntry {
    uint8_t regValue;
    // sensitivity, in native units per physical unit (eg ticks/dps)
    float sensitivity;
  };
  struct AccelFullscale : FullscaleEntry {};
  struct GyroFullscale : FullscaleEntry {};

  static constexpr GyroFullscale GYRO_FS_2000_DPS{0, DEG_TO_RAD(2000)};
  static constexpr GyroFullscale GYRO_FS_1000_DPS{1, DEG_TO_RAD(1000)};
  static constexpr GyroFullscale GYRO_FS_500_DPS{2, DEG_TO_RAD(500)};
  static constexpr AccelFullscale ACCEL_FS_16G{0, G_TO_MPS2(16)};
  static constexpr AccelFullscale ACCEL_FS_8G{1, G_TO_MPS2(8)};

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

  ImuIcm42688(SpiCtrl_t spidev);
  bool begin();
  void newData();

  void setGyroConfig(GyroFullscale range, GyroDataRate gyroRate);
  void setAccelConfig(AccelFullscale range, AccelDataRate accelRate);

  ImuData_s data;
  double tempC;
  int16_t tRaw;

 private:
  void setBank(int bank);  

  SpiCtrl_t spi;

  AccelFullscale accelFullscale;
  GyroFullscale gyroFullscale;
};

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_IMU_ICM20600_H_
