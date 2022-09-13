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
  float rocket_ang_vel_x;
  float rocket_ang_vel_y;
  float rocket_ang_vel_z;
  double qx;
  double qy;
  double qz;
  double qw;
} FilterData_s;

void filter_init(double dt);

/**
 * @brief Applies filters based on current sensor data
 * @param curSensorVals: Current measured sensor data
 * @param sensorProperties: Current sensor non-measured properties
 * @param hasPassedApogee: Whether rocket has passed apogee (true) or not
 * (false)
 */
void filter_applyData(SensorData_s* curSensorVals,
                      SensorProperties_s* sensorProperties,
                      bool hasPassedApogee);

/**
 * @brief Get average pressure from sensor data
 * @param curSensorVals: Current sensor values to get average pressure of
 */
double filter_getAveragePressure(SensorData_s* curSensorVals);

/**
 * @brief Adds reference pressure to the running list of pressures
 * @param curSensorVals: Current sensor values to use pressure from
 */
void filter_addPressureRef(SensorData_s* curSensorVals);

/**
 * @brief Adds a reference acceleration value to calculate gravity direction
 * (board orientation) on the pad
 * Does not take in any values, just controls when the filter fetches them
 */

void filter_addGravityRef();

/**
 * @brief Adds a reference angular velocity reading from all gyroscopes
 * Used to calculate zero offset to correctly bias the gyroscopes
 */

void filter_addGyroRef();

/**
 * @brief Sets reference pressure used for converting pressure to altitude
 * @param pres: Must be same unit as sensor data pressure
 */
void filter_setPressureRef(double pres);

/**
 * @brief Returns reference pressure
 */
double filter_getPressureRef();

/**
 * @brief Returns most recent filtered data
 */
FilterData_s* filter_getData();

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_FILTERING_FILTERS_H_
