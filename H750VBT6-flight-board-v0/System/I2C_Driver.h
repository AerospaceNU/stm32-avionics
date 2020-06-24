#ifndef I2C_DRIVER_H_

#define I2C_DRIVER_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "i2c.h"

/* I2C Control Struct */
typedef struct I2CCtrl_s{
    I2C_HandleTypeDef *hi2c;
    uint8_t addr;
}I2CCtrl_t;

// Start SPI interface
void I2C_init(void);

// read register function
uint8_t I2C_ReadRegister(I2CCtrl_t i2cCtrl, uint8_t reg);

// write value to register
void I2C_WriteRegister(I2CCtrl_t i2cCtrl, uint8_t reg, uint8_t val);

#ifdef __cplusplus
}
#endif

#endif
