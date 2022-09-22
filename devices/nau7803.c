/*
 * nau7802.c
 *
 *  Created on: Jul 16, 2022
 *      Author: matth
 */

/*

After the DVDD supply is valid, and after the internal power-on reset is
completed, the NAU7802 is ready for host program control access. The following
steps apply to most applications.
1. Set the RR bit to 1 in R0x00, to guarantee a reset of all register values.
2. Set the RR bit to 0 and PUD bit 1, in R0x00, to enter normal operation
3. After about 200 microseconds, the PWRUP bit will be Logic=1 indicating the
device is ready for the remaining programming setup.
4. At this point, all appropriate device selections and configuration can be
made. a. For example R0x00 = 0xAE b. R0x15 = 0x30
5. No conversion will take place until the R0x00 bit 4 “CS” is set Logic=1
6. Enter the low power standby condition by setting PUA and PUD bits to 0, in
R0x00
7. Resume operation by setting PUA and PUD bits to 1, in R0x00. This sequence is
the same for powering up from the standby condition, except that from standby
all of the information in the configuration and calibration registers will be
retained if the power supply is stable. Depending on conditions and the
application, it may be desirable to perform calibration again to update the
calibration registers for the best possible accuracy.

 */

#include "bit_helper.h"
#include "nau7802.h"

static void NAU7802_SetBit(NAU7802Ctrl_t *sensor, uint8_t reg, uint8_t offset) {
  uint8_t reg_val = I2C_ReadRegister(sensor->dev, reg);
  I2C_WriteRegister(sensor->dev, reg, set_bit(reg_val, offset));
}

static void NAU7802_ClearBit(NAU7802Ctrl_t *sensor, uint8_t reg,
                             uint8_t offset) {
  uint8_t reg_val = I2C_ReadRegister(sensor->dev, reg);
  I2C_WriteRegister(sensor->dev, reg, clear_bit(reg_val, offset));
}

bool NAU7802_Init(NAU7802Ctrl_t *sensor) {
  if ((I2C_ReadRegister(sensor->dev, NAU7802_REVISION_ID) & 0xf) == 0xf) {
    return false;
  }

  return true;
}

bool NAU7802_Reset(NAU7802Ctrl_t *sensor) {
  // write 1 to RR field of PU_CTRL to enter, and 0 to exit, reset
  NAU7802_SetBit(sensor, NAU7802_PU_CTRL, 0);
  NAU7802_ClearBit(sensor, NAU7802_PU_CTRL, 0);

  // power up digital
  NAU7802_SetBit(sensor, NAU7802_PU_CTRL, 1);

  // Wait about 200uS (or well, 1ms is OK)
  HAL_Delay(1);

  return get_bit(I2C_ReadRegister(sensor->dev, NAU7802_PU_CTRL), 3) == 1;
}

bool NAU7802_Enable(NAU7802Ctrl_t *sensor, bool is_on) {
  if (!is_on) {
    NAU7802_ClearBit(sensor, NAU7802_PU_CTRL, 2);
    NAU7802_ClearBit(sensor, NAU7802_PU_CTRL, 1);
    return true;
  } else {
    // power up digital
    NAU7802_SetBit(sensor, NAU7802_PU_CTRL, 1);
    // power up analog
    NAU7802_SetBit(sensor, NAU7802_PU_CTRL, 2);

    // Takes ~600ms to turn on, apparently. Don't like huge sleeps
    // but not sure what else to do
    HAL_Delay(600);
    NAU7802_SetBit(sensor, NAU7802_PU_CTRL, 4);

    return get_bit(I2C_ReadRegister(sensor->dev, NAU7802_PU_CTRL), 3) == 1;
  }
}
