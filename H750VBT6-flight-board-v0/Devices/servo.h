/*
 * servo.h
 */

#ifndef SERVO_H_
#define SERVO_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"

typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	uint32_t periodMS;
	float minPulseMS;
	float maxPulseMS;
	float minAngle;
	float maxAngle;
} ServoCtrl_t;

bool servoInit(ServoCtrl_t *servo, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t periodMS, float minPulseMS, float maxPulseMS, float minAngle, float maxAngle, float initAngle);
void servoSetAngle(ServoCtrl_t *servo, float degrees);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_H_ */
