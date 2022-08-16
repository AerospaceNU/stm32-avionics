/*
 * buzzer_pwm.h
 */

#ifndef DEVICES_BUZZER_PWM_H_
#define DEVICES_BUZZER_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_config_common.h"

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  float minFrequency;
} BuzzerPwmCtrl_t;

void buzzerPwmInit(BuzzerPwmCtrl_t *buzzer, TIM_HandleTypeDef *htim,
                   uint32_t channel, float minFrequency);
void buzzerPwmSetFrequency(BuzzerPwmCtrl_t *buzzer, float fHz);
void buzzerPwmStart(BuzzerPwmCtrl_t *buzzer);
void buzzerPwmStop(BuzzerPwmCtrl_t *buzzer);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_BUZZER_PWM_H_
