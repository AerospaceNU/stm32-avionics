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
} BuzzerPwmCtrl_s;

void buzzerPwmInit(BuzzerPwmCtrl_s *buzzer, TIM_HandleTypeDef *htim,
                   uint32_t channel, float minFrequency);
void buzzerPwmSetFrequency(BuzzerPwmCtrl_s *buzzer, float fHz);
void buzzerPwmStart(BuzzerPwmCtrl_s *buzzer);
void buzzerPwmStop(BuzzerPwmCtrl_s *buzzer);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_BUZZER_PWM_H_
