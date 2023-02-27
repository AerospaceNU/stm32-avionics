#include "krpc_pyro.h"

#include <stdint.h>
#include <stdio.h>

#include <chrono>  // NOLINT

void krpcPyro_init(KRPCPyroCtrl_s *pyro, int id, std::function<void(int, bool)> callback) {
  pyro->expireTime = 0;
  pyro->id = id;
  pyro->callback = callback;
}

void krpcPyro_start(KRPCPyroCtrl_s *pyro, uint32_t duration) {
  pyro->expireTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count() +
                     duration;
  printf("Writing pyro %i to ON\n", pyro->id);
  pyro->callback(pyro->id, true);
}

void krpcPyro_set(KRPCPyroCtrl_s *pyro, bool enable) {
  printf("Setting pyro %i to %s\n", pyro->id, enable ? "ON" : "OFF");
  pyro->callback(pyro->id, enable);
}

void krpcPyro_pwmStart(KRPCPyroCtrl_s *pyro, uint32_t duration,
                        uint32_t frequency, uint32_t pulseWidth) {
  printf("Initializing PWM on pyro %i with %i duty cycle \n", pyro->id,
         pulseWidth);
  pyro->callback(pyro->id, true);
}

void krpcPyro_tick(KRPCPyroCtrl_s *pyro) {
  if (pyro->expireTime != 0 &&
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
              .count() >= pyro->expireTime) {
    printf("Writing pyro %i OFF\n", pyro->id);
    pyro->callback(pyro->id, false);
    pyro->expireTime = 0;
  }
}
