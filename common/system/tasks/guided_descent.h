/*
 * guided_descent.h
 *
 *  Created on: Oct 7, 2024
 *      Author: trhol
 */

#ifndef SYSTEM_TASKS_GUIDED_DESCENT_H_
#define SYSTEM_TASKS_GUIDED_DESCENT_H_

#include "hardware_manager.h"

namespace guided_descent {

	/**
	 * Mark the initialization timestamp and begin executing instructions.
	 *
	 * Should be called at apogee/when drogue parachute is expected to be out
	 */
	void setInitializationTimestamp(uint32_t timestamp);

	void setMotor(SensorData_s* sensorData, uint32_t motorIdx, double lengthCm);

	/**
	 * Update and potentially progress the state of the dynamixel motors
	 */
	void update(SensorData_s* sensorData);

	void setZeroOffset(uint32_t motorIdx, double offset);

	void collapse(SensorData_s* sensorData);

}
#endif /* SYSTEM_TASKS_GUIDED_DESCENT_H_ */
