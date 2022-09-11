/*
 * pyro_digital.c
 *
 *  Created on: Mar 2, 2022
 *      Author: matth
 */

#include "pyro_digital.h"

#if HAS_DEV(PYRO_DIGITAL)

void PyroDigital_init(PyroDigitalCtrl_s *pyro) {
  // Make sure our pyros are OFF
  if (pyro->port) {
    HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_RESET);
  }
}

void PyroDigital_start(PyroDigitalCtrl_s *pyro, uint32_t duration) {
  HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_SET);
  pyro->expireTime = HAL_GetTick() + duration;
}

void PyroDigital_tick(PyroDigitalCtrl_s *pyro) {
  if (pyro->expireTime != 0 && HAL_GetTick() >= pyro->expireTime &&
      pyro->port != NULL) {
    HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_RESET);
    pyro->expireTime = 0;
  }
}

#endif  // HAS_DEV(PYRO_DIGITAL)
