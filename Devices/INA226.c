#include "board_config.h"

#ifdef HAS_INA226

#include "INA226.h"

bool INA226_init(INA226Ctrl_t* ina226) {
  ina226->avg = INA226_AVG_1;             // default: INA226_AVG_1
  ina226->vbusct = INA226_VBUSCT_1100us;  // default: INA226_VBUSCT_1100us
  ina226->vshct = INA226_VSHCT_1100us;    // default: INA226_VSHCT_1100us
  ina226->mode = INA226_MODE_SH_BUS_C;    // default: INA226_MODE_SH_BUS_C
  INA226_configure(ina226);
  INA226_calibrate(ina226);

  uint16_t mfg_id = INA226_readRegister16(ina226, INA226_MFG_ID);
  return (mfg_id == 0x5449);
}

// datasheet section 7.6.1
void INA226_configure(INA226Ctrl_t* ina226) {
  uint16_t config;

  config = (ina226->avg << 9 | ina226->vbusct << 6 | ina226->vshct << 3 |
            ina226->mode);

  INA226_writeRegister16(ina226, INA226_CONFIG, config);
}

void INA226_calibrate(INA226Ctrl_t* ina226) {
  float current_lsb = current_lsb = (ina226->iMaxExpected) / 32768;

  ina226->currentLSB = current_lsb;
  // The ratio between power lsb and current lsb is fixed at 25
  ina226->powerLSB = current_lsb * 25;

  float cal = 0.00512 / (current_lsb * ina226->rShuntVal);

  uint16_t cal_16 = (uint16_t)cal;

  INA226_writeRegister16(ina226, INA226_CALIBRATION, cal_16);
}

float INA226_getMaxCurrentPossible(INA226Ctrl_t* ina226) {
  return (INA226_VSHUNTMAX / ina226->rShuntVal);
}

float INA226_getMaxCurrent(INA226Ctrl_t* ina226) {
  float maxCurrent = (ina226->currentLSB * 32767);
  float maxPossible = INA226_getMaxCurrentPossible(ina226);

  return fmin(maxCurrent, maxPossible);
}

float INA226_getMaxShuntVoltage(INA226Ctrl_t* ina226) {
  float maxVoltage = INA226_getMaxCurrent(ina226) * ina226->rShuntVal;

  return fmin(INA226_VSHUNTMAX, maxVoltage);
}

float INA226_getMaxPower(INA226Ctrl_t* ina226) {
  return (INA226_getMaxCurrent(ina226) * INA226_VBUSMAX);
}

float INA226_readBusPower(INA226Ctrl_t* ina226) {
  return (INA226_readRegister16(ina226, INA226_POWER) * ina226->powerLSB);
}

float INA226_readShuntCurrent(INA226Ctrl_t* ina226) {
  return (INA226_readRegister16(ina226, INA226_CURRENT) * ina226->currentLSB);
}

float INA226_readShuntVoltage(INA226Ctrl_t* ina226) {
  float voltage;

  voltage = INA226_readRegister16(ina226, INA226_SHUNT_VOLTAGE);

  return (voltage * 0.0000025);
}

float INA226_readBusVoltage(INA226Ctrl_t* ina226) {
  int16_t voltage;

  voltage = INA226_readRegister16(ina226, INA226_BUS_VOLTAGE);

  return (voltage * 0.00125);
}

INA226_avg_t INA226_getAverages(INA226Ctrl_t* ina226) {
  uint16_t value;

  value = INA226_readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000111000000000;
  value >>= 9;

  return (INA226_avg_t)value;
}

INA226_vbusct_t INA226_getBusConversionTime(INA226Ctrl_t* ina226) {
  uint16_t value;

  value = INA226_readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000000111000000;
  value >>= 6;

  return (INA226_vbusct_t)value;
}

INA226_vshct_t INA226_getShuntConversionTime(INA226Ctrl_t* ina226) {
  uint16_t value;

  value = INA226_readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000000000111000;
  value >>= 3;

  return (INA226_vshct_t)value;
}

INA226_mode_t INA226_getMode(INA226Ctrl_t* ina226) {
  uint16_t value;

  value = INA226_readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000000000000111;

  return (INA226_mode_t)value;
}

// We can implement the alert pin functions here if needed

// Read from a register over I2C
uint16_t INA226_readRegister16(INA226Ctrl_t* ina226, uint8_t reg) {
  uint8_t txBuf[1] = {reg};
  uint8_t rxBuf[2] = {0};

  // send the device the register you want to read:
  HAL_I2C_Master_Transmit(ina226->hi2c, (ina226->addr << 1) | 0x01, txBuf, 1,
                          INA226_I2CTIMEOUT);

  // receive register value
  if (HAL_I2C_Master_Receive(ina226->hi2c, (ina226->addr << 1) | 0x01, rxBuf, 2,
                             INA226_I2CTIMEOUT) != HAL_OK) {
    return 0xFF;
  } else {
    return ((uint16_t)rxBuf[0] << 8 | rxBuf[1]);
  }
}

// Sends a write command to I2C device
void INA226_writeRegister16(INA226Ctrl_t* ina226, uint8_t reg, uint16_t msg) {
  // send the device the register you want to read:
  // send a value to write

  uint8_t txBuf[3];

  txBuf[0] = reg;
  txBuf[1] = (msg & 0xFF00) >> 8;
  txBuf[2] = (msg & 0x00FF);

  HAL_I2C_Master_Transmit(ina226->hi2c, ina226->addr << 1, txBuf, 3,
                          INA226_I2CTIMEOUT);
}

#endif
