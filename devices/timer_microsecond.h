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

void timerMicrosecond_init(TimerMicrosecondCtrl_s *ctrl);

uint64_t timerMicrosecond_getTimestampMicros(TimerMicrosecondCtrl_s *ctrl);

uint32_t timerMicrosecond_getTimestampMillis(TimerMicrosecondCtrl_s *ctrl);
