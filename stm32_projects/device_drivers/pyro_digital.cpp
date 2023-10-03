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
  pyro->pwm = false;
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
    pyro->pwm = false;
  }
  if (pyro->pwm) {
    // Tick PWM counters
    pyro->counter += 1;
    if (pyro->counter > pyro->counterMax) {
      pyro->counter = 0;
      HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_SET);
    }
    if (pyro->counter >= pyro->counterThreshold) {
      HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_RESET);
    }
  }
}

void pyroDigital_pwmStart(PyroDigitalCtrl_s *pyro, uint32_t duration,
                          uint32_t frequency, uint32_t pulseWidth) {
  if (pyro->port) {
    pyro->pwm = true;
    pyro->expireTime = HAL_GetTick() + duration;
    pyro->counterMax = static_cast<uint32_t>(
        static_cast<float>(CLOCK_FREQUENCY_HZ) / static_cast<float>(frequency));
    pyro->counterThreshold = static_cast<uint32_t>(
        (static_cast<float>(pulseWidth) / 100.0) * pyro->counterMax);
    HAL_GPIO_WritePin(pyro->port, pyro->pin, GPIO_PIN_SET);
  }
}

#endif  // HAS_DEV(PYRO_DIGITAL)
