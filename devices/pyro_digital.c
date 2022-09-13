/*
 * pyro_digital.c
 *
 *  Created on: Mar 2, 2022
 *      Author: matth
 */

#include "pyro_digital.h"

#if HAS_DEV(PYRO_DIGITAL)

void pyroDigital_init(PyroDigitalCtrl_s *pyro) {
  // Make sure our pyros are OFF
  if (pyro->port) {
    HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_RESET);
  }
  pyro->stayEnabled = false;
}

void pyroDigital_start(PyroDigitalCtrl_s *pyro, uint32_t duration) {
  HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_SET);
  pyro->expireTime = HAL_GetTick() + duration;
  pyro->stayEnabled = false;
}

void pyroDigital_set(PyroDigitalCtrl_s *pyro, bool enable) {
  if (pyro->port) {
    HAL_GPIO_WritePin(pyro->port, pyro->pin,
                      enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
    pyro->stayEnabled = enable;
  }
}

void pyroDigital_tick(PyroDigitalCtrl_s *pyro) {
  if (pyro->expireTime != 0 && HAL_GetTick() >= pyro->expireTime &&
      pyro->port != NULL && !pyro->stayEnabled) {
    HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_RESET);
    pyro->expireTime = 0;
  }
}

#endif  // HAS_DEV(PYRO_DIGITAL)
