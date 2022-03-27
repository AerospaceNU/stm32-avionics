/*
 * servo.c
 */

#include "servo.h"

#ifdef HAS_SERVO

bool servoInit(ServoCtrl_t *servo, TIM_HandleTypeDef *htim, uint32_t channel,
               uint32_t periodMS, float minPulseMS, float maxPulseMS,
               float minAngle, float maxAngle) {
  // Set servo struct values
  servo->htim = htim;
  servo->channel = channel;
  servo->periodMS = periodMS;
  servo->minPulseMS = minPulseMS;
  servo->maxPulseMS = maxPulseMS;
  servo->minAngle = minAngle;
  servo->maxAngle = maxAngle;

  // Set up timer prescaler for fastest counting possible that can still count
  // through one period of (periodMS * 2 safety factor)
  htim->Init.Prescaler =
      (uint32_t)((SystemCoreClock / 1000 * periodMS / 65535) -
                 1);  // * 2 and / 2 cancel out

  // Period should be half the max count of the clock, given how the prescaler
  // was set up
  htim->Init.Period = 32767;

  // Ensure prescaler and period are set
  if (HAL_TIM_PWM_Init(htim) != HAL_OK) return false;

  // Set the initial pulse to 0 so servo doesn't move
  __HAL_TIM_SET_COMPARE(servo->htim, servo->channel, 0);

  // Start PWM generation
  HAL_TIM_PWM_Start(htim, channel);

  return true;
}

void servoSetAngle(ServoCtrl_t *servo, float degrees) {
  // Calculate pulse from degrees
  float pulsePercent =
      (degrees - servo->minAngle) / (servo->maxAngle - servo->minAngle);
  float pulseLengthMS = (servo->maxPulseMS - servo->minPulseMS) * pulsePercent +
                        servo->minPulseMS;
  float dutyCycle = pulseLengthMS / servo->periodMS;
  uint32_t pulse = (uint32_t)(servo->htim->Init.Period * dutyCycle);

  // Set timer pulse
  __HAL_TIM_SET_COMPARE(servo->htim, servo->channel, pulse);
}

#endif
