/*
 * servo_pwm.h
 */

#ifndef DEVICES_SERVO_PWM_H_
#define DEVICES_SERVO_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  uint32_t periodMS;
  float minPulseMS;
  float maxPulseMS;
  float minAngle;
  float maxAngle;
} ServoPwmCtrl_t;

bool servoPwmInit(ServoPwmCtrl_t *servo, TIM_HandleTypeDef *htim,
                  uint32_t channel, uint32_t periodMS, float minPulseMS,
                  float maxPulseMS, float minAngle, float maxAngle);
void servoPwmSetAngle(ServoPwmCtrl_t *servo, float degrees);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_SERVO_PWM_H_
