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

#if HAS_DEV(NUM_BUZZER_PWM)

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  float minFrequency;
} BuzzerPwmCtrl_s;

void buzzerPwm_init(BuzzerPwmCtrl_s *buzzer, TIM_HandleTypeDef *htim,
                    uint32_t channel, float minFrequency);
void buzzerPwm_setFrequency(BuzzerPwmCtrl_s *buzzer, float fHz);
void buzzerPwm_start(BuzzerPwmCtrl_s *buzzer);
void buzzerPwm_stop(BuzzerPwmCtrl_s *buzzer);

#endif

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_BUZZER_PWM_H_
