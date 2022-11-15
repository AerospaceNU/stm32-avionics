/*
 * pyro_pwm.c
 */

#include "pyro_pwm.h"

#if HAS_DEV(PYRO_PWM)

#define TWO_POWER_SIXTEEN 65535

void pyroPwm_init(PyroPwmCtrl_s *pyro, TIM_HandleTypeDef *htim,
                  uint32_t channel, float minFrequency) {
  // Set pyro struct values
  pyro->htim = htim;
  pyro->channel = channel;

  // Set up timer prescaler for fastest counting possible that can still produce
  // a signal at (minFrequency / 2) Division by 2 is a safety factor, with the
  // tradeoff being inability to produce very high frequency signals
  pyro->htim->Init.Prescaler =
      (uint32_t)((SystemCoreClock / minFrequency / TWO_POWER_SIXTEEN) -
                 1);  // * 2 and / 2 cancel out

  // Calculation of actually max frequency needed because prescaler could be
  // rounded a lot, resulting in different number than input
  pyro->minFrequency =
      SystemCoreClock / ((htim->Init.Prescaler + 1) * TWO_POWER_SIXTEEN * 2);
}

void pyroPwm_setFrequency(PyroPwmCtrl_s *pyro, float fHz) {
  // Calculate period based on minimum and given frequencies
  if (fHz < pyro->minFrequency) {
    pyro->htim->Init.Period = 0;
  } else {
    pyro->htim->Init.Period = TWO_POWER_SIXTEEN * pyro->minFrequency / fHz;
  }

  // Ensure prescaler and period are set
  TIM_Base_SetConfig(pyro->htim->Instance, &(pyro->htim->Init));

  // Set timer pulse
  __HAL_TIM_SET_COMPARE(pyro->htim, pyro->channel, pyro->htim->Init.Period / 2);
}

void pyroPwm_start(PyroPwmCtrl_s *pyro, uint32_t duration) {
  // Start PWM generation
  HAL_TIM_PWM_Start(pyro->htim, pyro->channel);
  pyro->expireTime = hm_millis() + duration;
}

void pyroPwm_stop(PyroPwmCtrl_s *pyro) {
  // Stop PWM generation
  HAL_TIM_PWM_Stop(pyro->htim, pyro->channel);
}

void pyroPWM_tick(PyroPwmCtrl_s *pyro) {
  // Stop PWM generation if enough time has passed
  if (hm_millis() >= pyro->expireTime) {
    pyroPwm_stop(pyro);
  }
}

#endif  // HAS_DEV(PYRO_PWM)
