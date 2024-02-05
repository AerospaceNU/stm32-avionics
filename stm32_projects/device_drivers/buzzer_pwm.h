/*
 * buzzer_pwm.h
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_BUZZER_PWM_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_BUZZER_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_config_common.h"

#if HAS_DEV(BUZZER_PWM)

typedef struct {
  TIM_HandleTypeDef* htim;
  uint32_t channel;
  double minFrequency;
} BuzzerPwmCtrl_s;

void buzzerPwm_init(BuzzerPwmCtrl_s* buzzer, TIM_HandleTypeDef* htim,
                    uint32_t channel, float minFrequency);
void buzzerPwm_setFrequency(BuzzerPwmCtrl_s* buzzer, float fHz);
void buzzerPwm_start(BuzzerPwmCtrl_s* buzzer);
void buzzerPwm_stop(BuzzerPwmCtrl_s* buzzer);

#endif

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_BUZZER_PWM_H_
