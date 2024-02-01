/*
 * filters.h
 */

#ifndef COMMON_SYSTEM_FILTERING_FILTERS_H_
#define COMMON_SYSTEM_FILTERING_FILTERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "filterdata_types.h"
#include "hardware_manager.h"

void filter_init(float dt);

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
 * @brief Sets gravity reference direction
 * @param reference: Direction of gravity reference
 */
void filter_setGravityRef(int8_t reference);

/**
 * @brief Returns gravity reference value
 */
int8_t filter_getGravityRef();

/**
 * @brief Returns most recent filtered data
 */
FilterData_s* filter_getData();

/**
 * @brief Sets gyro offsets in unit of sensor data
 * 		(documented under ImuData_s)
 * @param xOffset: x-axis gyro offset
 * @param yOffset: y-axis gyro offset
 * @param zOffset: z-axis gyro offset
 */
void filter_setGyroOffsets(float xOffset, float yOffset, float zOffset);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_SYSTEM_FILTERING_FILTERS_H_
