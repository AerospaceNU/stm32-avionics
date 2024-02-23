/*
 * imu_icm42688.cpp
 */

#include "imu_icm42688.h"

// registers in bank 0
#define REG_GYRO_CONFIG0 79
#define REG_ACCEL_CONFIG0 80
#define REG_TEMP_DATA1 29
#define REG_BANK_SEL 118
#define REG_WHO_AM_I 117
#define REG_PWR_MGMT0 78
#define REG_DEVICE_CONFIG 17

// Helper macros for read/write
// SPI register address MSB is read/write (1 for read 0 for write, then 7 bit
// address)
#define REG_RW_MASK ~(1 << 7)
#define REG_READ(x) (x & REG_RW_MASK) | 1
#define REG_WRITE(x) (x & REG_RW_MASK)

#define WHO_AM_I 0x47

#if HAS_DEV(IMU_ICM42688)

ImuIcm42688::ImuIcm42688(SpiCtrl_t spidev) : spi(spidev) {}

void ImuIcm42688::setGyroConfig(GyroFullscale range, GyroDataRate gyroRate) {
  struct GYRO_CONFIG0 {
    GyroDataRate odr : 4;
    uint8_t reserved : 1;
    uint8_t fs : 3;
  };
  GYRO_CONFIG0 config0{
      .odr = gyroRate,
      .fs = range.regValue,
  };

  spi_writeRegisters(&spi, REG_GYRO_CONFIG0,
                     reinterpret_cast<uint8_t*>(&config0), 1);

  gyroFullscale = range;
}
void ImuIcm42688::setAccelConfig(AccelFullscale range,
                                 AccelDataRate accelRate) {
  struct ACCEL_CONFIG0 {
    AccelDataRate odr : 4;
    uint8_t reserved : 1;
    uint8_t fs : 3;
  };
  ACCEL_CONFIG0 config0{
      .odr = accelRate,
      .fs = range.regValue,
  };

  spi_writeRegisters(&spi, REG_WRITE(REG_GYRO_CONFIG0),
                     reinterpret_cast<uint8_t*>(&config0), 1);

  accelFullscale = range;
}

void ImuIcm42688::setBank(int bank) {
  spi_writeRegister(&spi, REG_WRITE(REG_BANK_SEL), bank);
}

bool ImuIcm42688::begin() {
  // force CS high
  HAL_GPIO_WritePin(spi.port, spi.pin, GPIO_PIN_SET);
  HAL_Delay(1);

  // From
  // https://github.com/finani/ICM42688/blob/245ddf8a3d2a6526278008cbc7edc1a04aae8041/src/ICM42688.cpp#L24

  // Reset the device. Sets to 2000dps/16g by default
  setBank(0);
  spi_writeRegister(&spi, REG_WRITE(REG_DEVICE_CONFIG), 1);
  HAL_Delay(1);  // Wait 1ms per datasheet

  // check whoami
  if (spi_readRegister(&spi, REG_READ(REG_WHO_AM_I)) != WHO_AM_I) {
    return false;
  }

  // turn on accel and gyro in low noise mode
  struct PWR_MGMT0 {
    uint8_t accelMode : 2;
    uint8_t gyroMode : 2;
    uint8_t idle : 1;
    uint8_t temp_disabled : 1;
    uint8_t reserved : 2;
  };
  PWR_MGMT0 pwrCfg{.accelMode = 0b11,
                   .gyroMode = 0b11,
                   .idle = 0,
                   .temp_disabled = 0,
                   .reserved = 0};
  spi_writeRegisters(&spi, REG_WRITE(REG_PWR_MGMT0),
                     reinterpret_cast<uint8_t*>(&pwrCfg), 1);

  return true;
}

void ImuIcm42688::newData() {
  // Read data in
  struct AccelTempRaw {
    int16_t temp;
    // depends on these orders being x,y,z
    Axis3dRaw_s accel;
    Axis3dRaw_s angVel;
  } __attribute__((packed));
  AccelTempRaw raw;

  spi_readRegisters(&spi, REG_READ(REG_TEMP_DATA1),
                    reinterpret_cast<uint8_t*>(&raw), sizeof(raw));

  data.accelRaw = raw.accel;
  data.angVelRaw = raw.angVel;

  // and convert to real units. Note that ticks / (ticks / unit) = unit
  data.accelRealMps2.x = raw.accel.x / accelFullscale.sensitivity;
  data.accelRealMps2.y = raw.accel.y / accelFullscale.sensitivity;
  data.accelRealMps2.z = raw.accel.z / accelFullscale.sensitivity;
  data.angVelRealRadps.x = raw.accel.x / gyroFullscale.sensitivity;
  data.angVelRealRadps.y = raw.accel.y / gyroFullscale.sensitivity;
  data.angVelRealRadps.z = raw.accel.z / gyroFullscale.sensitivity;

  // Convert temp per page 65
  tempC = raw.temp / 132.48 + 25;
}

#endif  // HAS_DEV(IMU_LSM9DS1)
