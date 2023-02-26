#include "nt_pyro.h"

#include <stdint.h>
#include <stdio.h>

#include <chrono>  // NOLINT
#include <fmt/format.h>

void ntPyro_init(NtPyroCtrl_s *pyro, int id) {
  pyro->expireTime = 0;
  pyro->id = id;
  auto name = fmt::format("/rocket_sim/pyro_{}/state", id);
  pyro->stateTopic = NT_GetTopic(NT_GetDefaultInstance(), name.c_str(), name.length());
  NT_PubSubOptions opts {0};
  opts.structSize = sizeof(opts);
  pyro->statePublisher = NT_Publish(pyro->stateTopic, NT_STRING, "string", &opts);
}

void ntPyro_start(NtPyroCtrl_s *pyro, uint32_t duration) {
  pyro->expireTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count() +
                     duration;
  // ntf("Writing pyro %i to ON\n", pyro->id);
  std::string name = "ON_TIMED";
  NT_SetString(pyro->statePublisher, 0, name.c_str(), name.length());
}

void ntPyro_set(NtPyroCtrl_s *pyro, bool enable) {
  // ntf("Setting pyro %i to %s\n", pyro->id, enable ? "ON" : "OFF");
  std::string name = enable ? "ON_MANUAL" : "OFF_MANUAL";
  NT_SetString(pyro->statePublisher, 0, name.c_str(), name.length());
}

void ntPyro_pwmStart(NtPyroCtrl_s *pyro, uint32_t duration,
                        uint32_t frequency, uint32_t pulseWidth) {
  // ntf("Initializing PWM on pyro %i with %i duty cycle \n", pyro->id,
        //  pulseWidth);
  std::string name = fmt::format("PWM_{}_{}", frequency, pulseWidth);
  NT_SetString(pyro->statePublisher, 0, name.c_str(), name.length());
}

void ntPyro_tick(NtPyroCtrl_s *pyro) {
  if (pyro->expireTime != 0 &&
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
              .count() >= pyro->expireTime) {
    // ntf("Writing pyro %i OFF\n", pyro->id);
    pyro->expireTime = 0;
    std::string name = "OFF_TIMED_OUT";
    NT_SetString(pyro->statePublisher, 0, name.c_str(), name.length());
  }
}
