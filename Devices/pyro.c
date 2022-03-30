/*
 * pyro.c
 *
 *  Created on: Mar 2, 2022
 *      Author: matth
 */

#include "pyro.h"

#ifdef HAS_PYRO

void Pyro_init(PyroCtrl_t *pyro) {
  // Make sure our pyros are OFF
  if (pyro->port) {
    HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_RESET);
  }
}

void Pyro_start(PyroCtrl_t *pyro, uint32_t duration) {
  HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_SET);
  pyro->expireTime = HAL_GetTick() + duration;
}

void Pyro_tick(PyroCtrl_t *pyro) {
  if (pyro->expireTime != 0 && HAL_GetTick() >= pyro->expireTime &&
      pyro->port != NULL) {
    HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_RESET);
    pyro->expireTime = 0;
  }
}

#endif
