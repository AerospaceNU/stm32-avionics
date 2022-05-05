#ifndef SYSTEM_STATES_CLI_STATE_CLI_CONFIG_H_
#define SYSTEM_STATES_CLI_STATE_CLI_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "states_interface.h"

class CliConfigState : public State {
 public:
  using State::getID;
  using State::getPeriodMS;
  using State::State;

  void init(void) override;

  EndCondition_t run(void) override;

  void cleanup(void) override;
};

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_STATES_CLI_STATE_CLI_CONFIG_H_