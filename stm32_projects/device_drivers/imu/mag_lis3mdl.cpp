#include "mag_lis3mdl.h"

#if HAS_DEV(MAG_LIS3MDL)

#include "reg_helper.h"

struct RegisterAddress {
  uint8_t address : 6;
  // if we want the read/write pointer to auto advance
  bool multiByteRequest : 1;
  // set - request is a read. Reset - request is a write
  bool isRead : 1;
};

// register locations
#define REG_CTRL_REG1 0x20
#define REG_CTRL_REG2 0x21
#define REG_CTRL_REG3 0x22
#define REG_CTRL_REG4 0x23
#define REG_CTRL_REG5 0x24
#define REG_WHOAMI 0x0f
#define REG_OUT_X_L 0x28

#define WHOAMI_VAL 0b00111101

// fixed sensitivity
constexpr float MAG_SENSITIVITY = 6842;  // lsb/gauss at +-4 gauss

// tiny helper
#define WRITE_DEV_REG(addr, val)                                     \
  spi_writeRegister(                                                 \
      &spi,                                                          \
      to_size_type(RegisterAddress{                                  \
          .address = addr, .multiByteRequest = false, .isRead = 0}), \
      val);

bool MagLis3mdl::begin(SpiCtrl_t spi_) {
  spi = spi_;

  // adapted from
  // https://github.com/pololu/lis3mdl-arduino/blob/master/LIS3MDL.cpp

  auto wireID = spi_readRegister(
      &spi,
      to_size_type(RegisterAddress{
          .address = REG_WHOAMI, .multiByteRequest = false, .isRead = 1}));

  if (wireID != WHOAMI_VAL) {
    return false;
  }

  // OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
  WRITE_DEV_REG(REG_CTRL_REG1, 0b01110000);
  // FS = 00 (+/- 4 gauss full scale)
  WRITE_DEV_REG(REG_CTRL_REG2, 0);
  // MD = 00 (continuous-conversion mode)
  WRITE_DEV_REG(REG_CTRL_REG3, 0);
  // OMZ = 11 (ultra-high-performance mode for Z)
  WRITE_DEV_REG(REG_CTRL_REG4, 0b00001100);
  // BDU = 1 (block data update)
  WRITE_DEV_REG(REG_CTRL_REG5, 0b01000000);

  return true;
}

void MagLis3mdl::newData() {
  spi_readRegisters(
      &spi,
      to_size_type(RegisterAddress{
          .address = REG_OUT_X_L, .multiByteRequest = true, .isRead = 1}),
      reinterpret_cast<uint8_t*>(&data.raw), sizeof(data.raw));

  // and convert to real units. Note that ticks / (ticks / unit) = unit
  data.realGauss.x = data.raw.x / MAG_SENSITIVITY;
  data.realGauss.y = data.raw.y / MAG_SENSITIVITY;
  data.realGauss.z = data.raw.z / MAG_SENSITIVITY;
}

#endif
