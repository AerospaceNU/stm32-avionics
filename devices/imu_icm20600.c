/*
 * imu_icm20600.c
 */

#include "imu_icm20600.h"

#if HAS_DEV(IMU_ICM20600)

bool icm20600_init(ImuICM20600Ctrl_s *sensor) {
	return true;
}

void icm20600_getData(ImuICM20600Ctrl_s *sensor) {
}


#endif  // HAS_DEV(IMU_LSM9DS1)
