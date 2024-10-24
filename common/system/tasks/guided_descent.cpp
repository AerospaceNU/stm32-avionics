/*
 * guided_descent.cpp
 *
 *  Created on: Oct 7, 2024
 *      Author: trhol
 */
#include "guided_descent.h"

#include "data_log.h"
#include "hardware_manager.h"

using namespace guided_descent;

const double CM_LENGTH_TO_DEGREES = 1;


struct LengthEntry {
	uint32_t timestamp;
	double lengthCm;
};

static LengthEntry motor1Control[] = {
	{5000, 3},
    {12000, 8},
	{0xFFFFFFFF, 0} // Sentinel value indicating end of the control list
};

static LengthEntry motor2Control[] = {
	{5000, -5},
    {12000, -8},
	{0xFFFFFFFF, 0} // Sentinel value indicating end of the control list
};


static uint8_t motor1_idx = 0;
static uint8_t motor2_idx = 0;

static uint32_t startTimestamp = 0;
static bool runMotors = false;

double goalLengthToDegrees(double goalLength) {
  return goalLength * CM_LENGTH_TO_DEGREES;
}

void guided_descent::setInitializationTimestamp(uint32_t timestamp) {
	startTimestamp = timestamp;
	runMotors = true;
}

void guided_descent::setMotor(SensorData_s* sensorData, uint32_t motorIdx, double lengthCm) {
	if (motorIdx >= NUM_DYNAMIXEL) {
		return;
	}
	double goalDegrees = goalLengthToDegrees(lengthCm);
	goalDegrees += cli_getConfigs()->dynamixelZeroOffset[motorIdx];
	hm_dynamixelSetGoalPosition(motorIdx, goalDegrees);
	sensorData->dynamixelSetDegrees[motorIdx] = goalDegrees;
	sensorData->dynamixelLengthCm[motorIdx] = lengthCm;
}


void guided_descent::update(SensorData_s* sensorData){
	if (!runMotors) {
		return;
	}
	uint32_t currentTimestamp = hm_millis() - startTimestamp;
	// Motor 1
	if (motor1Control[motor1_idx].timestamp != 0xFFFFFFFF) {
		if (motor1Control[motor1_idx].timestamp <= currentTimestamp) {
			setMotor(sensorData, 0, motor1Control[motor1_idx].lengthCm);
			++motor1_idx;
		}
	}
	// Motor 2
	if (motor2Control[motor2_idx].timestamp != 0xFFFFFFFF) {
		if (motor2Control[motor2_idx].timestamp <= currentTimestamp) {
			setMotor(sensorData, 1, motor2Control[motor2_idx].lengthCm);
			++motor2_idx;
		}
	}
}

void guided_descent::setZeroOffset(uint32_t motorIdx, double offset) {
	if (motorIdx >= NUM_DYNAMIXEL) {
		return;
	}

	CliConfigs_s* configs = cli_getConfigs();
	configs->dynamixelZeroOffset[motorIdx] = offset;
}




