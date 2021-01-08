/*
 * buzzer.h
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "stm32h7xx_hal.h"

typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	float minFrequency;
} BuzzerCtrl_t;

void buzzerInit(BuzzerCtrl_t *buzzer, TIM_HandleTypeDef *htim, uint32_t channel, float minFrequency);
void buzzerSetFrequency(BuzzerCtrl_t *buzzer, float fHz);
void buzzerStart(BuzzerCtrl_t *buzzer);
void buzzerStop(BuzzerCtrl_t *buzzer);

#ifdef __cplusplus
}
#endif

#endif /* BUZZER_H_ */
