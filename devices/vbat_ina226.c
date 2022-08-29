#include "vbat_ina226.h"

#if HAS_DEV(VBAT_INA226)

// We can implement the alert pin functions here if needed

// Read from a register over I2C
static uint16_t readRegister16(VbatIna226Ctrl_t* ina226, uint8_t reg) {
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
static void writeRegister16(VbatIna226Ctrl_t* ina226, uint8_t reg,
                            uint16_t msg) {
  // send the device the register you want to read:
  // send a value to write

  uint8_t txBuf[3];

  txBuf[0] = reg;
  txBuf[1] = (msg & 0xFF00) >> 8;
  txBuf[2] = (msg & 0x00FF);

  HAL_I2C_Master_Transmit(ina226->hi2c, ina226->addr << 1, txBuf, 3,
                          INA226_I2CTIMEOUT);
}

bool vbatIna226_init(VbatIna226Ctrl_t* ina226) {
  ina226->avg = INA226_AVG_1;             // default: INA226_AVG_1
  ina226->vbusct = INA226_VBUSCT_1100us;  // default: INA226_VBUSCT_1100us
  ina226->vshct = INA226_VSHCT_1100us;    // default: INA226_VSHCT_1100us
  ina226->mode = INA226_MODE_SH_BUS_C;    // default: INA226_MODE_SH_BUS_C
  vbatIna226_configure(ina226);
  vbatIna226_calibrate(ina226);

  uint16_t mfg_id = readRegister16(ina226, INA226_MFG_ID);
  return (mfg_id == 0x5449);
}

// datasheet section 7.6.1
void vbatIna226_configure(VbatIna226Ctrl_t* ina226) {
  uint16_t config;

  config = (ina226->avg << 9 | ina226->vbusct << 6 | ina226->vshct << 3 |
            ina226->mode);

  writeRegister16(ina226, INA226_CONFIG, config);
}

void vbatIna226_calibrate(VbatIna226Ctrl_t* ina226) {
  float current_lsb = current_lsb = (ina226->iMaxExpected) / 32768;

  ina226->currentLSB = current_lsb;
  // The ratio between power lsb and current lsb is fixed at 25
  ina226->powerLSB = current_lsb * 25;

  float cal = 0.00512 / (current_lsb * ina226->rShuntVal);

  uint16_t cal_16 = (uint16_t)cal;

  writeRegister16(ina226, INA226_CALIBRATION, cal_16);
}

float vbatIna226_getMaxCurrentPossible(VbatIna226Ctrl_t* ina226) {
  return (INA226_VSHUNTMAX / ina226->rShuntVal);
}

float vbatIna226_getMaxCurrent(VbatIna226Ctrl_t* ina226) {
  float maxCurrent = (ina226->currentLSB * 32767);
  float maxPossible = vbatIna226_getMaxCurrentPossible(ina226);

  return fmin(maxCurrent, maxPossible);
}

float vbatIna226_getMaxShuntVoltage(VbatIna226Ctrl_t* ina226) {
  float maxVoltage = vbatIna226_getMaxCurrent(ina226) * ina226->rShuntVal;

  return fmin(INA226_VSHUNTMAX, maxVoltage);
}

float vbatIna226_getMaxPower(VbatIna226Ctrl_t* ina226) {
  return (vbatIna226_getMaxCurrent(ina226) * INA226_VBUSMAX);
}

float vbatIna226_readBusPower(VbatIna226Ctrl_t* ina226) {
  return (readRegister16(ina226, INA226_POWER) * ina226->powerLSB);
}

float vbatIna226_readShuntCurrent(VbatIna226Ctrl_t* ina226) {
  return (readRegister16(ina226, INA226_CURRENT) * ina226->currentLSB);
}

float vbatIna226_readShuntVoltage(VbatIna226Ctrl_t* ina226) {
  float voltage;

  voltage = readRegister16(ina226, INA226_SHUNT_VOLTAGE);

  return (voltage * 0.0000025);
}

float vbatIna226_readBusVoltage(VbatIna226Ctrl_t* ina226) {
  int16_t voltage;

  voltage = readRegister16(ina226, INA226_BUS_VOLTAGE);

  return (voltage * 0.00125);
}

VbatIna226Avg_t vbatIna226_getAverages(VbatIna226Ctrl_t* ina226) {
  uint16_t value;

  value = readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000111000000000;
  value >>= 9;

  return (VbatIna226Avg_t)value;
}

VbatIna226Vbusct_t vbatIna226_getBusConversionTime(VbatIna226Ctrl_t* ina226) {
  uint16_t value;

  value = readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000000111000000;
  value >>= 6;

  return (VbatIna226Vbusct_t)value;
}

VbatIna226Vshct_t vbatIna226_getShuntConversionTime(VbatIna226Ctrl_t* ina226) {
  uint16_t value;

  value = readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000000000111000;
  value >>= 3;

  return (VbatIna226Vshct_t)value;
}

VbatIna226Mode_t vbatIna226_getMode(VbatIna226Ctrl_t* ina226) {
  uint16_t value;

  value = readRegister16(ina226, INA226_CONFIG);
  value &= 0b0000000000000111;

  return (VbatIna226Mode_t)value;
}

#endif  // HAS_DEV(VBAT_INA226)
