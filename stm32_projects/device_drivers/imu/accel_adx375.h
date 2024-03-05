/*
 * accel_adx375.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_IMU_ACCEL_ADX375_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_IMU_ACCEL_ADX375_H_

#include <math.h>

#include "board_config_common.h"
#include "spi_driver.h"

#define G_TO_MPS2(x) x * 9.80665

class AccelAdx375 {
 public:
  // Page 10 specifies rate/bandwidth. Bandwidth is the -3db frequency (page 3)
  // for a sinusoidal forcing function. Note that "The output reaches"
  enum class DataRate : uint8_t {
    RATE_3200HZ_BW_1600HZ = 0b1111,
    RATE_200HZ_BW_100HZ = 0b1011
  };

  bool begin(SpiCtrl_t spidev);
  void newData();
  double getAccelFullscaleMps2();

  /**
   * Change the output data rate of the sensor. Bandwidth is the -3db frequency
   * (page 3) for a sinusoidal forcing function. Note that "the output reaches
   * its final value after 4 * (1/(data rate))" due to internal filtering
   * (whatever that means).
   */
  void setDataRate(DataRate accelRate);

  AccelData_s data;

 private:
  SpiCtrl_t spi;
};

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_IMU_ACCEL_ADX375_H_
