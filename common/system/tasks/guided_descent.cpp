/*
 * guided_descent.cpp
 *
 *  Created on: Oct 7, 2024
 *      Author: trhol
 */
#include "guided_descent.h"

#include "hardware_manager.h"

const double LENGTH_TO_DEGREES = 360;
const double RPM = 60;

//

//const DynamixelCommandQueue commandQueue = DynamixelCommandQueue();
//const DynamixelMotor motor = DynamixelMotor();

//int setup() {
//  motor.init(1, &commandQueue);
//  motor.setDriveMode(ProfileConfig::VELOCITY_BASED, DirectionMode::NORMAL);
//  motor.setOperatingMode(OperatingMode::EXT_POSITION);
//}

struct LengthEntry {
	uint32_t timestamp;
	double length;
};

static LengthEntry motor1Control[] = {
		{5000, 3},
    {12000, 8}
};

static LengthEntry motor2Control[] = {
		{5000, -5},
    {12000, -8}
};


static uint8_t motor1_idx = 0;
static uint8_t motor2_idx = 0;

static uint32_t startTimestamp = 0;
static bool runMotors = false;

void setStartTime(uint32_t timestamp) {
	startTimestamp = timestamp;
	runMotors = true;

}

void update() {
//	currentTimestamp = hm_millis();
//
//	if (!runMotors) {
//		return;
//	}
//
//	if (currentTimestamp - startTimestamp > motor1Control[motor1_idx].timestamp) {
//		hm_dynamixelSetGoalPosition(0, goalLengthToDegrees(motor1Control[motor1_idx].length))
//		motor1_idx++;
//	}
//
//	if (currentTimestamp - startTimestamp > motor2Control[motor2_idx].timestamp) {
//		hm_dynamixelSetGoalPosition(1, goalLengthToDegrees(motor2Control[motor2_idx].length))
//		motor2_idx++;
//	}
}

double goalLengthToDegrees(double goalLength) {
  return goalLength * LENGTH_TO_DEGREES;
}
