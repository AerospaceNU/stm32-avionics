/*
 * filters.h
 */

#ifndef SYSTEM_FILTERING_FILTERS_H_
#define SYSTEM_FILTERING_FILTERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hardware_manager.h"

typedef struct {
  double heading;
  double vtg;
  double pos_x;
  double pos_y;
  double pos_z;
  double rocket_vel_x;
  double rocket_vel_y;
  double rocket_vel_z;
  double rocket_acc_x;
  double rocket_acc_y;
  double rocket_acc_z;
  double world_vel_x;
  double world_vel_y;
  double world_vel_z;
  double world_acc_x;
  double world_acc_y;
  double world_acc_z;
  float ang_vel_x;
  float ang_vel_y;
  float ang_vel_z;
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
 * @param hasPassedApogee: Whether rocket has passed apogee (true) or not
 * (false)
 */
void filterApplyData(SensorData_t* curSensorVals,
                     SensorProperties_t* sensorProperties,
                     bool hasPassedApogee);

/**
 * @brief Adds reference pressure to the running list of pressures
 * @param pres: Must be same unit as sensor data pressure
 */
void filterAddPressureRef(double currentPres);

/**
 * @brief Adds a reference acceleration value to calculate gravity direction
 * (board orientation) on the pad
 * Does not take in any values, just controls when the filter fetches them
 */

void filterAddGravityRef();

/**
 * @brief Adds a reference angular velocity reading from all gyroscopes
 * Used to calculate zero offset to correctly bias the gyroscopes
 */

void filterAddGyroRef();

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

#endif  // SYSTEM_FILTERING_FILTERS_H_
