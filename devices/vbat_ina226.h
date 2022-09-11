#ifndef DEVICES_VBAT_INA226_H_
#define DEVICES_VBAT_INA226_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "board_config_common.h"

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
} VbatIna226Avg_e;

typedef enum {
  INA226_VBUSCT_140us = 0b000,
  INA226_VBUSCT_204us = 0b001,
  INA226_VBUSCT_332us = 0b010,
  INA226_VBUSCT_588us = 0b011,
  INA226_VBUSCT_1100us = 0b100,
  INA226_VBUSCT_2116us = 0b101,
  INA226_VBUSCT_4156us = 0b110,
  INA226_VBUSCT_8244us = 0b111
} VbatIna226Vbusct_e;

typedef enum {
  INA226_VSHCT_140us = 0b000,
  INA226_VSHCT_204us = 0b001,
  INA226_VSHCT_332us = 0b010,
  INA226_VSHCT_588us = 0b011,
  INA226_VSHCT_1100us = 0b100,
  INA226_VSHCT_2116us = 0b101,
  INA226_VSHCT_4156us = 0b110,
  INA226_VSHCT_8244us = 0b111
} VbatIna226Vshct_e;

typedef enum {
  INA226_MODE_PWDN = 0b000,
  INA226_MODE_VSH_T = 0b001,
  INA226_MODE_VBUS_T = 0b010,
  INA226_MODE_SH_BUS_T = 0b011,
  INA226_MODE_PWDN2 = 0b100,
  INA226_MODE_VSH_C = 0b101,
  INA226_MODE_VBUS_C = 0b110,
  INA226_MODE_SH_BUS_C = 0b111
} VbatIna226Mode_e;

/* INA226 Control Struct */
typedef struct {
  I2C_HandleTypeDef* hi2c;
  uint8_t addr;               // A0,A1 -> GND,GND : 1000000b
  float rShuntVal;            // 20 mOhm -> 0.02
  float iMaxExpected;         // 10 amps?
  float currentLSB;           // do not set, computed in calibrate
  float powerLSB;             // do not set, computer in calibrate
  VbatIna226Avg_e avg;        // default: INA226_AVG_1
  VbatIna226Vbusct_e vbusct;  // default: INA226_VBUSCT_1100us
  VbatIna226Vshct_e vshct;    // default: INA226_VSHCT_1100us
  VbatIna226Mode_e mode;      // default: INA226_MODE_SH_BUS_C
} VbatIna226Ctrl_s;

bool vbatIna226_init(VbatIna226Ctrl_s* ina226);
void vbatIna226_calibrate(VbatIna226Ctrl_s* ina226);               // static
void vbatIna226_configure(VbatIna226Ctrl_s* ina226);               // static
float vbatIna226_getMaxCurrentPossible(VbatIna226Ctrl_s* ina226);  // static
float vbatIna226_getMaxCurrent(VbatIna226Ctrl_s* ina226);          // static
float vbatIna226_getMaxShuntVoltage(VbatIna226Ctrl_s* ina226);     // static
float vbatIna226_getMaxPower(VbatIna226Ctrl_s* ina226);            // static

float vbatIna226_readBusPower(VbatIna226Ctrl_s* ina226);
float vbatIna226_readShuntCurrent(VbatIna226Ctrl_s* ina226);
float vbatIna226_readShuntVoltage(VbatIna226Ctrl_s* ina226);
float vbatIna226_readBusVoltage(VbatIna226Ctrl_s* ina226);

VbatIna226Avg_e vbatIna226_getAverages(VbatIna226Ctrl_s* ina226);
VbatIna226Vbusct_e vbatIna226_getBusConversionTime(VbatIna226Ctrl_s* ina226);
VbatIna226Vshct_e vbatIna226_getShuntConversionTime(VbatIna226Ctrl_s* ina226);
VbatIna226Mode_e vbatIna226_getMode(VbatIna226Ctrl_s* ina226);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_VBAT_INA226_H_
