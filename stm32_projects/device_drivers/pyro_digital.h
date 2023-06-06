/*
 * pyro_digital.h
 *
 *  Created on: Mar 2, 2022
 *      Author: matth
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_PYRO_DIGITAL_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_PYRO_DIGITAL_H_

#include "board_config_common.h"

#ifdef HAL_TIM_MODULE_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCK_FREQUENCY_HZ 1000

typedef struct {
  GPIO_TypeDef *port;
  TIM_HandleTypeDef *htim;
  uint16_t pin;
  bool stayEnabled;
  uint32_t expireTime;
  uint32_t counter;
  bool pwm;
  uint32_t counterThreshold;
  uint32_t counterMax;
} PyroDigitalCtrl_s;

void pyroDigital_init(PyroDigitalCtrl_s *pyro);
void pyroDigital_start(PyroDigitalCtrl_s *pyro, uint32_t duration);
void pyroDigital_set(PyroDigitalCtrl_s *pyro, bool enable);
void pyroDigital_tick(PyroDigitalCtrl_s *pyro);
void pyroDigital_pwmStart(PyroDigitalCtrl_s *pyro, uint32_t duration,
                          uint32_t frequency, uint32_t pulseWidth);

#ifdef __cplusplus
}
#endif

#endif  // HAL_TIM_MODULE_ENABLED

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_PYRO_DIGITAL_H_
