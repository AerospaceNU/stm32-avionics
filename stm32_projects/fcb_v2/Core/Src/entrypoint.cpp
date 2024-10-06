/*
` * entrypoint.cpp
 *
 *  Created on: Mar 29, 2023
 *      Author: mmorley@na.jnj.com
 */

#include "scheduler.h"
#include "dynamixel_command_queue.h"
#include "dynamixel_motor.h"

#include "usart.h"

extern "C" {

uint32_t lastTimestamp;


void main_cpp() {
	DynamixelCommandQueue commandQueue{&huart2};
	  DynamixelMotor m1{1, &commandQueue};
	  DynamixelMotor m2{2, &commandQueue};
	  m1.setOperatingMode(OperatingMode::EXT_POSITION);
	  m2.setOperatingMode(OperatingMode::EXT_POSITION);
	  m1.torqueEnable(Toggle::ON);
	  m2.torqueEnable(Toggle::ON);
	  double degrees = 0;
	  while (1) {
	    m1.goalPosition(degrees);
	    m2.goalPosition(degrees);
	    lastTimestamp = HAL_GetTick();
	    while (HAL_GetTick() - lastTimestamp < 5000) {
	    	commandQueue.tick();
	    }
	    degrees += 60;
	  }

}
}
