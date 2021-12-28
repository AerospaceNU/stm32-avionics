#ifndef DEVICES_INA226_H_
#define DEVICES_INA226_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "stdbool.h"

#define INA226_VBUSMAX 36
#define INA226_VSHUNTMAX 0.08192f
#define INA226_I2CTIMEOUT 10

#define INA226_CONFIG 0x00
#define INA226_SHUNT_VOLTAGE 0x01
#define INA226_BUS_VOLTAGE 0x02
#define INA226_POWER 0x03
#define INA226_CURRENT 0x04
#define INA226_CALIBRATION 0x05
#define INA226_MASK 0x06
#define INA226_ALERT 0x07
#define INA226_MFG_ID 0xFE
#define INA226_DIE_ID 0xFF

typedef enum {
  INA226_AVG_1 = 0b000,
  INA226_AVG_4 = 0b001,
  INA226_AVG_16 = 0b010,
  INA226_AVG_64 = 0b011,
  INA226_AVG_128 = 0b100,
  INA226_AVG_256 = 0b101,
  INA226_AVG_512 = 0b110,
  INA226_AVG_1024 = 0b111
} INA226_avg_t;

typedef enum {
  INA226_VBUSCT_140us = 0b000,
  INA226_VBUSCT_204us = 0b001,
  INA226_VBUSCT_332us = 0b010,
  INA226_VBUSCT_588us = 0b011,
  INA226_VBUSCT_1100us = 0b100,
  INA226_VBUSCT_2116us = 0b101,
  INA226_VBUSCT_4156us = 0b110,
  INA226_VBUSCT_8244us = 0b111
} INA226_vbusct_t;

typedef enum {
  INA226_VSHCT_140us = 0b000,
  INA226_VSHCT_204us = 0b001,
  INA226_VSHCT_332us = 0b010,
  INA226_VSHCT_588us = 0b011,
  INA226_VSHCT_1100us = 0b100,
  INA226_VSHCT_2116us = 0b101,
  INA226_VSHCT_4156us = 0b110,
  INA226_VSHCT_8244us = 0b111
} INA226_vshct_t;

typedef enum {
  INA226_MODE_PWDN = 0b000,
  INA226_MODE_VSH_T = 0b001,
  INA226_MODE_VBUS_T = 0b010,
  INA226_MODE_SH_BUS_T = 0b011,
  INA226_MODE_PWDN2 = 0b100,
  INA226_MODE_VSH_C = 0b101,
  INA226_MODE_VBUS_C = 0b110,
  INA226_MODE_SH_BUS_C = 0b111
} INA226_mode_t;

/* INA226 Control Struct */
typedef struct INA226Ctrl_s {
  I2C_HandleTypeDef* hi2c;
  uint8_t addr;            // A0,A1 -> GND,GND : 1000000b
  float rShuntVal;         // 20 mOhm -> 0.02
  float iMaxExpected;      // 10 amps?
  float currentLSB;        // do not set, computed in calibrate
  float powerLSB;          // do not set, computer in calibrate
  INA226_avg_t avg;        // default: INA226_AVG_1
  INA226_vbusct_t vbusct;  // default: INA226_VBUSCT_1100us
  INA226_vshct_t vshct;    // default: INA226_VSHCT_1100us
  INA226_mode_t mode;      // default: INA226_MODE_SH_BUS_C
} INA226Ctrl_t;

bool INA226_init(INA226Ctrl_t* ina226);
void INA226_calibrate(INA226Ctrl_t* ina226);
void INA226_configure(INA226Ctrl_t* ina226);
float INA226_getMaxCurrentPossible(INA226Ctrl_t* ina226);
float INA226_getMaxCurrent(INA226Ctrl_t* ina226);
float INA226_getMaxShuntVoltage(INA226Ctrl_t* ina226);
float INA226_getMaxPower(INA226Ctrl_t* ina226);

float INA226_readBusPower(INA226Ctrl_t* ina226);
float INA226_readShuntCurrent(INA226Ctrl_t* ina226);
float INA226_readShuntVoltage(INA226Ctrl_t* ina226);
float INA226_readBusVoltage(INA226Ctrl_t* ina226);

INA226_avg_t INA226_getAverages(INA226Ctrl_t* ina226);
INA226_vbusct_t INA226_getBusConversionTime(INA226Ctrl_t* ina226);
INA226_vshct_t INA226_getShuntConversionTime(INA226Ctrl_t* ina226);
INA226_mode_t INA226_getMode(INA226Ctrl_t* ina226);

uint16_t INA226_readRegister16(INA226Ctrl_t* ina226, uint8_t reg);
void INA226_writeRegister16(INA226Ctrl_t* ina226, uint8_t reg, uint16_t msg);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_INA226_H_
