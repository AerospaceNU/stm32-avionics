#include "I2C_driver.h"

#ifdef HAS_I2C

uint8_t I2C_ReadRegister(I2CCtrld_t *sensor, uint8_t reg) {
  uint8_t txBuf = reg;
  uint8_t rxBuf = 0;

  // send the device the register you want to read:
  HAL_I2C_Master_Transmit(sensor->hi2c, (sensor->addr << 1) | 0x01, &txBuf, 1,
                          sensor->timeout);

  // receive register value
  if (HAL_OK !=
      (HAL_I2C_Master_Receive(sensor->hi2c, (sensor->addr << 1) | 0x01, &rxBuf,
                              1, sensor->timeout) != HAL_OK)) {
    return 0;  // TODO what to do on fail?
  } else {
    return rxBuf;
  }
}

// Sends a write command to I2C device
void I2C_WriteRegister(I2CCtrld_t *sensor, uint8_t reg, uint8_t val) {
  // send the device the register you want to read:
  // send a value to write

  uint8_t txBuf[2];

  txBuf[0] = reg;
  txBuf[1] = val;

  HAL_I2C_Master_Transmit(sensor->hi2c, sensor->addr << 1, txBuf, 2,
                          sensor->timeout);
}

#endif
