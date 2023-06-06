#ifndef COMMON_SYSTEM_STATES_CLI_STATE_CLI_TEMP_H_
#define COMMON_SYSTEM_STATES_CLI_STATE_CLI_TEMP_H_

#include "states_interface.h"

class CliTempState : public State {
 public:
  using State::getID;
  using State::getPeriodMS;
  using State::State;

  EndCondition_e run(void) override;
  void init(void) override {}
  void cleanup(void) override {}
};

#endif  // COMMON_SYSTEM_STATES_CLI_STATE_CLI_TEMP_H_
