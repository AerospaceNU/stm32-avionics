/*
 * buzzer.h
 */

#ifndef DEVICES_BUZZER_H_
#define DEVICES_BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_config.h"
#include HAL_HEADER

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t channel;
  float minFrequency;
} BuzzerCtrl_t;

void buzzerInit(BuzzerCtrl_t *buzzer, TIM_HandleTypeDef *htim, uint32_t channel,
                float minFrequency);
void buzzerSetFrequency(BuzzerCtrl_t *buzzer, float fHz);
void buzzerStart(BuzzerCtrl_t *buzzer);
void buzzerStop(BuzzerCtrl_t *buzzer);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_BUZZER_H_
