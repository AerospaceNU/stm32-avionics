/*
 * pyro_digital.h
 *
 *  Created on: Mar 2, 2022
 *      Author: matth
 */

#ifndef DEVICES_PYRO_DIGITAL_H_
#define DEVICES_PYRO_DIGITAL_H_

#include "board_config_common.h"

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
  uint32_t expireTime;
} PyroDigitalCtrl_t;

void PyroDigital_init(PyroDigitalCtrl_t *pyro);
void PyroDigital_start(PyroDigitalCtrl_t *pyro, uint32_t duration);
void PyroDigital_tick(PyroDigitalCtrl_t *pyro);

#endif  // DEVICES_PYRO_DIGITAL_H_
