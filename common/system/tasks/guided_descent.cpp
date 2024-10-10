/*
 * guided_descent.cpp
 *
 *  Created on: Oct 7, 2024
 *      Author: trhol
 */
#include "guided_descent.h"

#include "dynamixel_motor.h"

const double LENGTH_TO_DEGREES;
const double RPM;
const double VELOCITY;

const DynamixelCommandQueue commandQueue = DynamixelCommandQueue();
const DynamixelMotor motor = DynamixelMotor();

int setup() {
  motor.init(1, &commandQueue);
  motor.setDriveMode(ProfileConfig.VELOCITY_BASED, DirectionMode.NORMAL);
  motor.setOperatingMode(OperatingMode.EXT_POSITION);
}

int run() {
  // set drive and operating modes

  // set max velocity
  // set infinite acceleration

  // while true
  //   if past time for new goal position
  //     set goal position

  // goal length change to extended position
}

double goalLengthToExtendedPos(double goalLength) {
  return goalLength * LENGTH_TO_DEGREES;
}
