/*
 * pyro.h
 *
 *  Created on: Mar 2, 2022
 *      Author: matth
 */

#ifndef DEVICES_PYRO_H_
#define DEVICES_PYRO_H_

#include "board_config.h"

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
  uint32_t expireTime;
} PyroCtrl_t;

void Pyro_init(PyroCtrl_t *pyro);
void Pyro_start(PyroCtrl_t *pyro, uint32_t duration);
void Pyro_tick(PyroCtrl_t *pyro);

#endif  // DEVICES_PYRO_H_
