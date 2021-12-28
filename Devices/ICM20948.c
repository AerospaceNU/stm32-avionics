#include "ICM20948.h"
#include "ICM20948_defs.h"
#include <stdbool.h> // Not sure this is necessary?

static bool set_bank(ICM20948Ctrl_t *sensor, uint8_t bank)
{
  return SPI_WriteRegister(&sensor->spiconfig, BANK_SELECT, bank);
}

static uint8_t spi_read(ICM20948Ctrl_t *sensor, uint8_t bank, uint8_t reg)
{
  if (bank != sensor->last_bank)
  {
    set_bank(sensor, bank);
    sensor->last_bank = bank;
  }
  return SPI_ReadRegister(&sensor->spiconfig, reg);
}

static uint8_t spi_read_array(ICM20948Ctrl_t *sensor, uint8_t bank, uint8_t reg, uint8_t *array, uint8_t len)
{
  reg = reg | 0x80;
  if (bank != sensor->last_bank)
  {
    set_bank(sensor, bank);
    sensor->last_bank = bank;
  }
  return SPI_ReadArray(&sensor->spiconfig, reg, array, len);
}

static void spi_write(ICM20948Ctrl_t *sensor, uint8_t bank, uint8_t reg, uint8_t val)
{
  reg = reg | 0x80;
  if (bank != sensor->last_bank)
  {
    set_bank(sensor, bank);
    sensor->last_bank = bank;
  }
  SPI_WriteRegister(&sensor->spiconfig, reg, val);
}

// from Sparkfun driver
// Transact directly with an I2C device, one byte at a time
// Used to configure a device before it is setup into a normal 0-3 peripheral slot
static bool ICM_20948_i2c_controller_periph4_txn(ICM20948Ctrl_t *sensor, uint8_t addr, uint8_t reg, uint8_t *data,
                                          uint8_t len, bool Rw, bool send_reg_addr)
{
  // Thanks MikeFair! // https://github.com/kriswiner/MPU9250/issues/86
  ICM_20948_Status_e retval = ICM_20948_Stat_Ok;

  addr = (((Rw) ? 0x80 : 0x00) | addr);

  spi_write(sensor, 3, AGB3_REG_I2C_PERIPH4_ADDR, addr);
  spi_write(sensor, 3, AGB3_REG_I2C_PERIPH4_REG, reg);

  ICM_20948_I2C_PERIPH4_CTRL_t ctrl;
  ctrl.EN = 1;
  ctrl.INT_EN = false;
  ctrl.DLY = 0;
  ctrl.REG_DIS = !send_reg_addr;

  ICM_20948_I2C_MST_STATUS_t i2c_mst_status;
  bool txn_failed = false;
  uint16_t nByte = 0;

  while (nByte < len)
  {
    if (!Rw)
    {
      spi_write(sensor, 3, AGB3_REG_I2C_PERIPH4_DO, data[nByte]);
    }

    // Kick off txn
    spi_write(sensor, 3, AGB3_REG_I2C_PERIPH4_CTRL, *((uint8_t *)&ctrl));
    if (retval != ICM_20948_Stat_Ok)
    {
      return retval;
    }

    uint32_t max_cycles = 1000;
    uint32_t count = 0;
    bool peripheral4Done = false;
    while (!peripheral4Done)
    {
      uint8_t temp = spi_read(sensor, 0, AGB0_REG_I2C_MST_STATUS);
      i2c_mst_status = *((ICM_20948_I2C_MST_STATUS_t *)&temp);

      peripheral4Done = (i2c_mst_status.I2C_PERIPH4_DONE /*| (millis() > tsTimeout) */); // Avoid forever-loops
      peripheral4Done |= (count >= max_cycles);
      count++;
    }
    txn_failed = (i2c_mst_status.I2C_PERIPH4_NACK /*| (millis() > tsTimeout) */);
    txn_failed |= (count >= max_cycles);
    if (txn_failed)
      break;

    if (Rw)
    {
      //   ICM_20948_set_bank(pdev, 3);
      data[nByte] = spi_read(sensor, 3, AGB3_REG_I2C_PERIPH4_DI);
    }

    nByte++;
  }

  if (txn_failed)
  {
    // We often fail here if mag is stuck
    return false;
  }
  return true;
}

static uint8_t readMag(ICM20948Ctrl_t *sensor, uint8_t reg)
{
  uint8_t data = 0;
  ICM_20948_i2c_controller_periph4_txn(sensor, MAG_AK09916_I2C_ADDR, reg, &data, 1, true, true);
  return data;
}

static uint8_t writeMag(ICM20948Ctrl_t *sensor, uint8_t reg, uint8_t data)
{
  return ICM_20948_i2c_controller_periph4_txn(sensor, MAG_AK09916_I2C_ADDR, reg, &data, 1, false, true);
}

bool ICM20948_init(ICM20948Ctrl_t *sensor, ICM_20948_ACCEL_CONFIG_t accel_config, ICM_20948_GYRO_CONFIG_t gyro_config)
{
  // Check to make sure we can see the sensor
  if (!ICM20948_is_connected(sensor))
  {
    return false;
  }

  // Reset (this should also disable sleep and LP mode)
  uint8_t reg = 0b1000000; // Internal 20MHz, set reset flag
  spi_write(sensor, 0, PWR_MGMT_1, reg);
  // Sparkfun driver sleeps for 50ms here
  HAL_Delay(50);

  // === Start the magnetometer ===

  // disable master passthrough and enable the i3c master
  spi_write(sensor, 0, INT_PIN_CFG, 0b0);                         // not strictly necessary, as the reset value is already 0x0
  spi_write(sensor, 3, AGB3_REG_I2C_MST_CTRL, 0b00010000 | 0x07); // TODO check if this config is correct -- from sparkfun
  spi_write(sensor, 0, USER_CTRL, 1 << 5);                        // Just enable the i2c master

  // Then, reset the magnetometer
  // This sends a soft-reset to the mag
  uint8_t data = 1;
  ICM_20948_i2c_controller_periph4_txn(sensor, MAG_AK09916_I2C_ADDR, AK09916_REG_CNTL3, &data, 1, false, true);

  // Keep trying until the magnetometer finally decides to start responding
  uint8_t i = 0;
  while (i++ < 10)
  {
    uint8_t whoiam1, whoiam2;
    whoiam1 = readMag(sensor, AK09916_REG_WIA1);
    whoiam2 = readMag(sensor, AK09916_REG_WIA2);
    if ((whoiam1 == (MAG_AK09916_WHO_AM_I >> 8)) && (whoiam2 == (MAG_AK09916_WHO_AM_I & 0xFF)))
    {
      break;
    }

    // Just reset and try again
    spi_write(sensor, 0, USER_CTRL, 1 << 1); // Just enable the i2c master
    HAL_Delay(10);
  }

  if (i == 10)
  {
    // Something went wrong, we should do something about this
    return false;
  }

  // Set up the magnetometer
  const uint8_t mag_mode_100hz = 0x04 << 1; // From sparkfun, apparently this is 100hz
  writeMag(sensor, AK09916_REG_CNTL2, mag_mode_100hz);

  const uint8_t periph_addr_reg = 3;
  const uint8_t periph_reg_reg = 4;
  const uint8_t periph_ctrl_reg = 5;
  // const uint8_t periph_do_reg = 6; // RW is false so this is unnedded

  uint8_t i2c_addr = MAG_AK09916_I2C_ADDR << 1 | 1;
  spi_write(sensor, 3, periph_addr_reg, i2c_addr);

  uint8_t subaddress = AK09916_REG_ST1;
  spi_write(sensor, 3, periph_reg_reg, subaddress);

  ICM_20948_I2C_PERIPHX_CTRL_t ctrl;
  ctrl.LENG = 9;
  ctrl.EN = true;
  ctrl.REG_DIS = false;
  ctrl.GRP = false;
  ctrl.BYTE_SW = false;
  spi_write(sensor, 3, periph_ctrl_reg, *((uint8_t *)&ctrl));

  // Set fullscale, Digital low pass config
  ICM20948_set_config(sensor, accel_config, gyro_config);

  return true;
}

bool ICM20948_set_config(ICM20948Ctrl_t *sensor, ICM_20948_ACCEL_CONFIG_t accel_config, ICM_20948_GYRO_CONFIG_t gyro_config)
{
  spi_write(sensor, 2, AGB2_ACCEL_CONFIG, *((uint8_t *)&accel_config));
  spi_write(sensor, 2, AGB2_GYRO_CONFIG_1, *((uint8_t *)&gyro_config));
  return true; // TODO check if it actually worked
}

bool ICM20948_is_connected(ICM20948Ctrl_t *sensor)
{
  return spi_read(sensor, 0, IMU_WHO_AM_I) == ICM20948_WHOAMI_ID;
}

static bool ICM20948_readRaw(ICM20948Ctrl_t *sensor)
{
  static uint8_t buff[RAW_DATA_SIZE];
  bool ret = spi_read_array(sensor, 0, ACCEL_XOUT_H, buff, RAW_DATA_SIZE);
  if (!ret)
    return false; // TODO should we log this?

  // From sparkfun
  sensor->rawData.accel_x_raw = ((buff[0] << 8) | (buff[1] & 0xFF));
  sensor->rawData.accel_y_raw = ((buff[2] << 8) | (buff[3] & 0xFF));
  sensor->rawData.accel_z_raw = ((buff[4] << 8) | (buff[5] & 0xFF));
  sensor->rawData.gyro_x_raw = ((buff[6] << 8) | (buff[7] & 0xFF));
  sensor->rawData.gyro_y_raw = ((buff[8] << 8) | (buff[9] & 0xFF));
  sensor->rawData.gyro_z_raw = ((buff[10] << 8) | (buff[11] & 0xFF));
  sensor->rawData.magStat1 = buff[14];
  sensor->rawData.mag_x_raw = ((buff[16] << 8) | (buff[15] & 0xFF)); // Mag data is read little endian
  sensor->rawData.mag_y_raw = ((buff[18] << 8) | (buff[17] & 0xFF));
  sensor->rawData.mag_z_raw = ((buff[20] << 8) | (buff[19] & 0xFF));
  sensor->rawData.magStat2 = buff[22];

  return true; // TODO check if it worked
}

static void ICM20948_scale_result(ICM20948Ctrl_t *sensor)
{
  // From the datasheet
  float accel_scale;
  switch (sensor->accel_config.ACCEL_FS_SEL)
  {
  case 0:
    accel_scale = 16384; // lsb/gee
  case 1:
    accel_scale = 8192;
  case 2:
    accel_scale = 4096;
  case 3:
  default:
    accel_scale = 2048;
  }

  float gyro_scale;
  switch (sensor->accel_config.ACCEL_FS_SEL)
  {
  case 0:
    gyro_scale = 131; // lsb/deg-per-sec
  case 1:
    gyro_scale = 65.5;
  case 2:
    gyro_scale = 32.8;
  case 3:
  default:
    gyro_scale = 16.4;
  }

  float mag_scale = 0.15; // uT per LSB

  // Apply scale factors
  sensor->imuData.accel_x = sensor->rawData.accel_x_raw / accel_scale;
  sensor->imuData.accel_y = sensor->rawData.accel_y_raw / accel_scale;
  sensor->imuData.accel_z = sensor->rawData.accel_y_raw / accel_scale;
  sensor->imuData.gyro_x = sensor->rawData.gyro_x_raw / gyro_scale;
  sensor->imuData.gyro_y = sensor->rawData.gyro_y_raw / gyro_scale;
  sensor->imuData.gyro_z = sensor->rawData.gyro_y_raw / gyro_scale;
  sensor->imuData.mag_x = sensor->rawData.mag_x_raw * mag_scale;
  sensor->imuData.mag_y = sensor->rawData.mag_y_raw * mag_scale;
  sensor->imuData.mag_z = sensor->rawData.mag_y_raw * mag_scale;
  sensor->imuData.temp = ((sensor->rawData.temp_raw - 21) / 333.87) + 21;
}

bool ICM20948_read(ICM20948Ctrl_t *sensor)
{
  bool ret = ICM20948_readRaw(sensor);
  ICM20948_scale_result(sensor);

  return ret;
}
