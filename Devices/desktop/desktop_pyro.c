#include "stdint.h"
#define GPIO_TypeDef uint32_t
#include "pyro.h"
#undef GPIO_TypeDef
#include "hardware_manager.h"
#include "stdio.h"
#define HAL_GetTick HM_Millis

void Pyro_init(PyroCtrl_t *pyro) {
  // Make sure our pyros are OFF
  if (pyro->port) {
    printf("Writing port %u and pin %u to OFF\n", *(pyro->port), pyro->pin);
  }
}

void Pyro_start(PyroCtrl_t *pyro, uint32_t duration) {
  printf("Writing port %u and pin %u to ON\n", *(pyro->port), pyro->pin);
  pyro->expireTime = HAL_GetTick() + duration;
}

void Pyro_tick(PyroCtrl_t *pyro) {
  if (pyro->expireTime != 0 && HAL_GetTick() >= pyro->expireTime &&
      pyro->port != NULL) {
    printf("Writing port %u and pin %u to OFF\n", *(pyro->port), pyro->pin);
    pyro->expireTime = 0;
  }
}
