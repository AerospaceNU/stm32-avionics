#include "print_pyro.h"

#include <stdint.h>
#include <stdio.h>

#include <chrono>  // NOLINT

void printPyro_init(PrintPyroCtrl_t *pyro, int id) {
  pyro->expireTime = 0;
  pyro->id = id;
}

void printPyro_start(PrintPyroCtrl_t *pyro, uint32_t duration) {
  pyro->expireTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count() +
                     duration;
  printf("Writing pyro %i to ON\n", pyro->id);
}

void printPyro_tick(PrintPyroCtrl_t *pyro) {
  if (pyro->expireTime != 0 &&
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
              .count() >= pyro->expireTime) {
    printf("Writing pyro %i OFF\n", pyro->id);
    pyro->expireTime = 0;
  }
}
