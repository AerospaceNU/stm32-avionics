/*
 * pyro_pwm.h
 */

#ifndef DEVICES_PYRO_PWM_H_
#define DEVICES_PYRO_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_config_common.h"

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  float minFrequency;
  uint32_t expireTime;
} PyroPwmCtrl_s;

void pyroPwm_init(PyroPwmCtrl_s *pyro, TIM_HandleTypeDef *htim,
                  uint32_t channel, float minFrequency);
void pyroPwm_setFrequency(PyroPwmCtrl_s *pyro, float fHz);
void pyroPwm_start(PyroPwmCtrl_s *pyro, uint32_t duration);
void pyroPwm_stop(PyroPwmCtrl_s *pyro);
void pyroPWM_tick(PyroPwmCtrl_s *pyro);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_PYRO_PWM_H_
