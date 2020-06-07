#include "I2C_Driver.h"


// global variables for i2c
enum{ I2C_BUFFER_LENGTH = 1,
      I2C_TRANSMIT_LENGTH };
uint8_t rxBuff[I2C_BUFFER_LENGTH];
uint8_t txBuff[I2C_TRANSMIT_LENGTH];

// Start I2C interface
// still must configure chip select pin as OUTPUT
void I2C_init(){
    // start I2C
    //>I2C.begin();

    // set to MSB first mode
    //>I2C.setBitOrder(MSBFIRST);
}

//Read from a register over I2C
uint8_t I2C_ReadRegister(I2CCtrl_t i2cCtrl, uint8_t reg) {
  
  rxBuff[0] = reg;

  // send the device the register you want to read:
  HAL_I2C_Master_Transmit(i2cCtrl.hi2c, i2cCtrl.addr, rxBuff, I2C_BUFFER_LENGTH, 1000000);

  // wait for transaction to occur
  HAL_Delay(20);

  // receive register value
  HAL_I2C_Master_Receive(i2cCtrl.hi2c, i2cCtrl.addr, rxBuff, I2C_BUFFER_LENGTH, 1000000);

  // return the result:
  return rxBuff[0];
}


//Sends a write command to I2C device
void I2C_WriteRegister(I2CCtrl_t i2cCtrl, uint8_t reg, uint8_t val) {

  txBuff[0] = reg;
	txBuff[1] = val;

	// send the device the register you want to read:
	// send a value to write
	HAL_I2C_Master_Transmit(i2cCtrl.hi2c, i2cCtrl.addr, txBuff, I2C_TRANSMIT_LENGTH, 1000000);

}
