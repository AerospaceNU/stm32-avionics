/*
 * timer_microsecond.c
 *
 *  Created on: Mar 21, 2023
 *      Author: matth
 */

#include <board_config.h>
#include HAL_HEADER
#include "hal_callbacks.h"

#define MICROS_PER_MILLI 1000

typedef struct {
  TIM_HandleTypeDef *pTimer;
  volatile uint64_t offset;
} TimerMicrosecondCtrl_s;

static void timerMicrosecond_callback(void *pData) {
  TimerMicrosecondCtrl_s *ctrl = (TimerMicrosecondCtrl_s *)pData;
  ctrl->offset += 3600000000;
}

void timerMicrosecond_init(TimerMicrosecondCtrl_s *ctrl) {
  // TODO so I have by default configured the IOC to count up at 1MHz, and
  // overrun every 1 hour
  ctrl->pTimer->Init.Prescaler = 240 - 1;
  ctrl->pTimer->Init.Period = 3600000000;

  ctrl->offset = 0;

  // Ensure prescaler and period are set
  TIM_Base_SetConfig(ctrl->pTimer->Instance, &(ctrl->pTimer->Init));

  halCallbacks_registerTimPeriodElapsedCallback(
      ctrl->pTimer, timerMicrosecond_callback, ctrl);
  HAL_TIM_Base_Start_IT(ctrl->pTimer);
}

uint64_t timerMicrosecond_getTimestampMicros(TimerMicrosecondCtrl_s *ctrl) {
  return ctrl->pTimer->Instance->CNT + ctrl->offset;
}

uint32_t timerMicrosecond_getTimestampMillis(TimerMicrosecondCtrl_s *ctrl) {
  return timerMicrosecond_getTimestampMicros(ctrl) / 1000;
}
