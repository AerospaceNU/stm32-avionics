/*
 * imu_iis2mdc.c
 */

#include "mag_iis2mdc.h"

#if HAS_DEV(IMU_IIS2MDC)

bool iis2mdc_init(ImuIIS2MDCCtrl_s* sensor) { return true; }

void iis2mdc_getData(ImuIIS2MDCCtrl_s* sensor) {}

#endif  // HAS_DEV(IMU_LSM9DS1)
