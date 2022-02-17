/*
 * filters.h
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "hardware_manager.h"

typedef struct {
	double heading;
	double vtg;
	double pos_x;
	double pos_y;
	double pos_z;
	double vel_x;
	double vel_y;
	double vel_z;
	double acc_x;
	double acc_y;
	double acc_z;
	double qx;
	double qy;
	double qz;
	double qw;
} FilterData_t;

void filterInit(double dt);

/**
 * @brief Applies filters based on current sensor data
 * @param curSensorVals: Current measured sensor data
 * @param sensorProperties: Current sensor non-measured properties
 * @param hasPassedApogee: Whether rocket has passed apogee (true) or not (false)
 */
void filterApplyData(SensorData_t* curSensorVals, SensorProperties_t* sensorProperties, bool hasPassedApogee);

/**
 * @brief Sets reference pressure used for converting pressure to altitude
 * @param pres: Must be same unit as sensor data pressure
 */
void filterSetPressureRef(double pres);

/**
 * @brief Returns reference pressure
 */
double filterGetPressureRef();

/**
 * @brief Returns most recent filtered data
 */
FilterData_t* filterGetData();

#ifdef __cplusplus
}
#endif

#endif /* FILTERS_H_ */
