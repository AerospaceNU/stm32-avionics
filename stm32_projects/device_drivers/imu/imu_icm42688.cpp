/*
 * imu_icm42688.cpp
 */

#include "imu_icm42688.h"

#include "const_map.h"
#include "bit_helper.h"

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
constexpr uint8_t REG_RW_MASK = (1 << 7);
#define REG_READ(x) (x | REG_RW_MASK)
#define REG_WRITE(x) (x & ~REG_RW_MASK)

#define WHO_AM_I 0x47

#if HAS_DEV(IMU_ICM42688)

// ================================= //

using FullscaleEntry = ImuIcm42688::FullscaleEntry;

using AccelFullscaleEntry =
    std::pair<ImuIcm42688::AccelFullscale, FullscaleEntry>;
using GyroFullscaleEntry =
    std::pair<ImuIcm42688::GyroFullscale, FullscaleEntry>;

constexpr ConstMap<
    ImuIcm42688::AccelFullscale, FullscaleEntry,
    static_cast<size_t>(ImuIcm42688::AccelFullscale::NUM_ACCEL_FULLSCALE)>
    accelFullscales({AccelFullscaleEntry{ImuIcm42688::AccelFullscale::FS_8G,
                                         {1, G_TO_MPS2(8), 4096}},
                     AccelFullscaleEntry{ImuIcm42688::AccelFullscale::FS_16G,
                                         {0, G_TO_MPS2(16), 2048}}});

constexpr ConstMap<
    ImuIcm42688::GyroFullscale, FullscaleEntry,
    static_cast<size_t>(ImuIcm42688::GyroFullscale::NUM_GYRO_FULLSCALE)>
    gyroFullscales({GyroFullscaleEntry{ImuIcm42688::GyroFullscale::FS_2000_DPS,
                                       {0, DEG_TO_RAD(2000), 16.4}},
                    GyroFullscaleEntry{ImuIcm42688::GyroFullscale::FS_1000_DPS,
                                       {1, DEG_TO_RAD(1000), 32.8}}});

// ================================= //

void ImuIcm42688::setGyroConfig(const GyroFullscale range,
                                GyroDataRate gyroRate) {
  auto fsSetting = gyroFullscales[range];
  struct GYRO_CONFIG0 {
    GyroDataRate odr : 4;
    uint8_t reserved : 1;
    uint8_t fs : 3;
  };
  GYRO_CONFIG0 config0{
      .odr = gyroRate,
    		  .reserved=0,
      .fs = fsSetting.regValue,
  };

  spi_writeRegisters(&spi, REG_GYRO_CONFIG0,
                     reinterpret_cast<uint8_t*>(&config0), 1);

  gyroFS = fsSetting;
}
void ImuIcm42688::setAccelConfig(const AccelFullscale range,
                                 AccelDataRate accelRate) {
  auto fsSetting = accelFullscales[range];
  struct ACCEL_CONFIG0 {
    AccelDataRate odr : 4;
    uint8_t reserved : 1;
    uint8_t fs : 3;
  };
  ACCEL_CONFIG0 config0{
      .odr = accelRate,
    		  .reserved=0,
      .fs = fsSetting.regValue,
  };

  spi_writeRegisters(&spi, REG_WRITE(REG_ACCEL_CONFIG0),
                     reinterpret_cast<uint8_t*>(&config0), 1);

  accelFS = fsSetting;
}

void ImuIcm42688::setBank(int bank) {
  spi_writeRegister(&spi, REG_WRITE(REG_BANK_SEL), bank);
}

bool ImuIcm42688::begin(SpiCtrl_t spi_) {
  spi = spi_;

  // force CS high
  HAL_GPIO_WritePin(spi.port, spi.pin, GPIO_PIN_SET);
  HAL_Delay(1);

  // From
  // https://github.com/finani/ICM42688/blob/245ddf8a3d2a6526278008cbc7edc1a04aae8041/src/ICM42688.cpp#L24

  // Reset the device. Sets to 2000dps/16g by default
  setBank(0);
  volatile auto bank = spi_readRegister(&spi, REG_READ(REG_BANK_SEL));
  if (bank != 0) {
	  return false;
  }
  spi_writeRegister(&spi, REG_WRITE(REG_DEVICE_CONFIG), 1);
  HAL_Delay(1);  // Wait 1ms per datasheet

  // check whoami
  auto id = spi_readRegister(&spi, REG_READ(REG_WHO_AM_I));

  if (id != WHO_AM_I) {
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

  // randoms ane defaults
  setAccelConfig(AccelFullscale::FS_16G, AccelDataRate::RATE_2KHZ);
  setGyroConfig(GyroFullscale::FS_2000_DPS, GyroDataRate::RATE_2KHZ);

  return true;
}

void ImuIcm42688::newData() {

	struct [[gnu::packed]] AccelTempRaw {
		int16_t temp;
		Axis3dRaw_s accel;
		Axis3dRaw_s angVel;
	};
	AccelTempRaw raw;
	spi_readRegisters(&spi, REG_READ(REG_TEMP_DATA1),
			reinterpret_cast<uint8_t*>(&raw), sizeof(raw));

	// and swap all the numbers around
	if constexpr (std::endian::native == std::endian::little) {
		auto arr = std::bit_cast<std::array<uint16_t, sizeof(AccelTempRaw)/sizeof(uint16_t)>>(raw);
		for (auto& elem : arr) {
			elem = std::byteswap(elem);
		}
	}

	data.accelRaw = raw.accel;
	data.angVelRaw = raw.angVel;

	// and convert to real units. Note that ticks / (ticks / unit) = unit
	float accelSensitivity = G_TO_MPS2(1.0) / accelFS.sensitivity;
	float gyroSensitivity = DEG_TO_RAD(1.0) / gyroFS.sensitivity;
	data.accelRealMps2.x = raw.accel.x * accelSensitivity;
	data.accelRealMps2.y = raw.accel.y * accelSensitivity;
	data.accelRealMps2.z = raw.accel.z * accelSensitivity;
	data.angVelRealRadps.x = raw.angVel.x * gyroSensitivity;
	data.angVelRealRadps.y = raw.angVel.y * gyroSensitivity;
	data.angVelRealRadps.z = raw.angVel.z * gyroSensitivity;

	// Convert temp per page 65
	tempC = raw.temp / 132.48 + 25;
}

double ImuIcm42688::getAccelFullscaleMps2() { return accelFS.fullscale; }

#endif  // HAS_DEV(IMU_LSM9DS1)
