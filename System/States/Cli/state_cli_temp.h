#ifndef SYSTEM_STATES_CLI_STATE_CLI_TEMP_H_
#define SYSTEM_STATES_CLI_STATE_CLI_TEMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "states_interface.h"

class CliTempState : public State {
 public:
  using State::getID;
  using State::getPeriodMS;
  using State::State;

  EndCondition_t run(void) override;
  void init(void) override {}
  void cleanup(void) override {}
};

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_STATES_CLI_STATE_CLI_TEMP_H_
