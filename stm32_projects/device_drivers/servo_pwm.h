/*
 * servo_pwm.h
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_SERVO_PWM_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_SERVO_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"

#if HAS_DEV(SERVO_PWM)

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  uint32_t periodMS;
  float minPulseMS;
  float maxPulseMS;
  float minAngle;
  float maxAngle;
} ServoPwmCtrl_t;

bool servoPwm_init(ServoPwmCtrl_t *servo, TIM_HandleTypeDef *htim,
                   uint32_t channel, uint32_t periodMS, float minPulseMS,
                   float maxPulseMS, float minAngle, float maxAngle);
void servoPwm_setAngle(ServoPwmCtrl_t *servo, float degrees);

#endif

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_SERVO_PWM_H_
