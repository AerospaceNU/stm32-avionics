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

const double CM_LENGTH_TO_DEGREES = -76;


struct LengthEntry {
	uint32_t timestamp;
	double lengthCm;
};

static LengthEntry motor1Control[] = {
    {3000, 15},
    {15000, 13},
    {17000, 15},
    {31000, 9},
    {36000, 15},
    {59000, 5},
    {67000, 15},
    {99000, 1},
    {110000, 15},
    {151000, 13},
    {153000, 15},
    {167000, 9},
    {172000, 15},
    {195000, 5},
    {203000, 15},
    {235000, 1},
    {246000, 15},
    {287000, 13},
    {289000, 15},
    {303000, 9},
    {308000, 15},
    {331000, 5},
    {339000, 15},
    {371000, 1},
    {382000, 15},
    {423000, 13},
    {425000, 15},
    {439000, 9},
    {444000, 15},
    {467000, 5},
    {475000, 15},
    {507000, 1},
    {518000, 15},
    {0xFFFFFFFF, 0} // Sentinel value indicating end of the control list
};

static LengthEntry motor2Control[] = {
    {3000, 15},
    {23000, 13},
    {25000, 15},
    {45000, 9},
    {50000, 15},
    {79000, 5},
    {87000, 15},
    {125000, 1},
    {136000, 15},
    {159000, 13},
    {161000, 15},
    {181000, 9},
    {186000, 15},
    {215000, 5},
    {223000, 15},
    {261000, 1},
    {272000, 15},
    {295000, 13},
    {297000, 15},
    {317000, 9},
    {322000, 15},
    {351000, 5},
    {359000, 15},
    {397000, 1},
    {408000, 15},
    {431000, 13},
    {433000, 15},
    {453000, 9},
    {458000, 15},
    {487000, 5},
    {495000, 15},
    {533000, 1},
    {544000, 15},
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
	motor1_idx = 0;
	motor2_idx = 0;
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

void guided_descent::collapse(SensorData_s* sensorData) {
	runMotors = false;
	setMotor(sensorData, 0, 1);
	setMotor(sensorData, 1, 1);
}

void guided_descent::setZeroOffset(uint32_t motorIdx, double offset) {
	if (motorIdx >= NUM_DYNAMIXEL) {
		return;
	}

	CliConfigs_s* configs = cli_getConfigs();
	configs->dynamixelZeroOffset[motorIdx] = offset;
}




