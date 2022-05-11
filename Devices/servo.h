/*
 * servo.h
 */

#ifndef DEVICES_SERVO_H_
#define DEVICES_SERVO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config.h"
#include HAL_HEADER

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  uint32_t periodMS;
  float minPulseMS;
  float maxPulseMS;
  float minAngle;
  float maxAngle;
} ServoCtrl_t;

bool servoInit(ServoCtrl_t *servo, TIM_HandleTypeDef *htim, uint32_t channel,
               uint32_t periodMS, float minPulseMS, float maxPulseMS,
               float minAngle, float maxAngle);
void servoSetAngle(ServoCtrl_t *servo, float degrees);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_SERVO_H_
