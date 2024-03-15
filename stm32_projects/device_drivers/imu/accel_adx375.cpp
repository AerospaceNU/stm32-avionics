#include "accel_adx375.h"

#include "math_utils.h"
#include "reg_helper.h"

#if HAS_DEV(ACCEL_ADX375)

struct RegisterAddress {
  uint8_t address : 6;
  // if we want the read/write pointer to auto advance
  bool multiByteRequest : 1;
  // set - request is a read. Reset - request is a write
  bool isRead : 1;
};

#define REG_WHOAMI 0
#define REG_INT_ENABLE 46
#define REG_DATA_FORMAT 0x31
#define REG_POWER_CTL 0x2d
#define REG_DATAX0 0x32

// TODO the datasheet says this is +-11%, def needs calibration per-chip. at
// least it doesn't change all that much vs temperature (.02%/deg C for
// sensitivity, 10mG/deg C for 0g offset)
// Units are lsb per G
constexpr float ACCEL_SENSITIVITY = 20.5 / G_TO_MPS2(1);
#define ACCEL_FS G_TO_MPS2(200)

#include <cstdarg>

#include "usb_std.h"
void CustomPrintf(const char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  int len = vsprintf(buffer, format, args);
  usbStd_transmit((uint8_t *)buffer, len);
  va_end(args);
}

bool AccelAdx375::begin(SpiCtrl_t spi_) {
  spi = spi_;

  HAL_GPIO_WritePin(spi.port, spi.pin, GPIO_PIN_SET);
  HAL_Delay(1);

  // adapted from
  // https://github.com/adafruit/Adafruit_ADX375/blob/master/Adafruit_ADX375.cpp

  auto wireID = spi_readRegister(
      &spi,
      to_size_type(RegisterAddress{
          .address = REG_WHOAMI, .multiByteRequest = false, .isRead = 1}));

  if (wireID != 0xe5) {
    return false;
  }

  // Interrupts default to off, but just to be sure
  spi_writeRegister(
      &spi,
      to_size_type(RegisterAddress{
          .address = REG_INT_ENABLE, .multiByteRequest = false, .isRead = 0}),
      0);

  // data format set to right justified/LSB first/sign extended; 3 wire spi
  // enabled; interrupts active high
  spi_writeRegister(
      &spi,
      to_size_type(RegisterAddress{
          .address = REG_DATA_FORMAT, .multiByteRequest = false, .isRead = 0}),
      0b00001011);

  // And power up
  spi_writeRegister(
      &spi,
      to_size_type(RegisterAddress{
          .address = REG_POWER_CTL, .multiByteRequest = false, .isRead = 0}),
      0b0001000);

  return true;
}

void AccelAdx375::newData() {
  spi_readRegisters(
      &spi,
      to_size_type(RegisterAddress{
          .address = REG_DATAX0, .multiByteRequest = true, .isRead = true}),
      reinterpret_cast<uint8_t *>(&data.raw), sizeof(data.raw));

  constexpr Axis3dReal_s accelOffset = {.x = -2.76, .y = 6.31, .z = -5.63};

  // and convert to real units. Note that ticks / (ticks / unit) = unit
  data.realMps2.x = data.raw.x / ACCEL_SENSITIVITY;
  data.realMps2.y = data.raw.y / ACCEL_SENSITIVITY;
  data.realMps2.z = data.raw.z / ACCEL_SENSITIVITY;

  data.realMps2.x += accelOffset.x;
  data.realMps2.y += accelOffset.y;
  data.realMps2.z += accelOffset.z;
}

double AccelAdx375::getAccelFullscaleMps2() { return ACCEL_FS; }

#endif  // HAS_DEV(ACCEL_ADX375)
