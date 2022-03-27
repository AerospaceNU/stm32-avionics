/*
 * buzzer.c
 */

#include "buzzer.h"

#ifdef HAS_BUZZER

#define TWO_POWER_SIXTEEN 65535

void buzzerInit(BuzzerCtrl_t *buzzer, TIM_HandleTypeDef *htim, uint32_t channel,
                float minFrequency) {
  // Set buzzer struct values
  buzzer->htim = htim;
  buzzer->channel = channel;

  // Set up timer prescaler for fastest counting possible that can still produce
  // a signal at (minFrequency / 2) Division by 2 is a safety factor, with the
  // tradeoff being inability to produce very high frequency signals
  buzzer->htim->Init.Prescaler =
      (uint32_t)((SystemCoreClock / minFrequency / TWO_POWER_SIXTEEN) -
                 1);  // * 2 and / 2 cancel out

  // Calculation of actually max frequency needed because prescaler could be
  // rounded a lot, resulting in different number than input
  buzzer->minFrequency =
      SystemCoreClock / ((htim->Init.Prescaler + 1) * TWO_POWER_SIXTEEN * 2);
}

void buzzerSetFrequency(BuzzerCtrl_t *buzzer, float fHz) {
  // Calculate period based on minimum and given frequencies
  if (fHz < buzzer->minFrequency) {
    buzzer->htim->Init.Period = 0;
  } else {
    buzzer->htim->Init.Period = TWO_POWER_SIXTEEN * buzzer->minFrequency / fHz;
  }

  // Ensure prescaler and period are set
  TIM_Base_SetConfig(buzzer->htim->Instance, &(buzzer->htim->Init));

  // Set timer pulse
  __HAL_TIM_SET_COMPARE(buzzer->htim, buzzer->channel,
                        buzzer->htim->Init.Period / 2);
}

void buzzerStart(BuzzerCtrl_t *buzzer) {
  // Start PWM generation
  HAL_TIM_PWM_Start(buzzer->htim, buzzer->channel);
}

void buzzerStop(BuzzerCtrl_t *buzzer) {
  // Stop PWM generation
  HAL_TIM_PWM_Stop(buzzer->htim, buzzer->channel);
}

#endif
