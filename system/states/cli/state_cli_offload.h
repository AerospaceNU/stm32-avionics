#ifndef SYSTEM_STATES_CLI_STATE_CLI_OFFLOAD_H_
#define SYSTEM_STATES_CLI_STATE_CLI_OFFLOAD_H_

#include "states_interface.h"

class CliOffloadState : public State {
 public:
  using State::getID;
  using State::getPeriodMS;
  using State::State;

  void init(void) override;

  EndCondition_e run(void) override;

  void cleanup(void) override;

 private:
  bool initSuccess_;
};

#endif  // SYSTEM_STATES_CLI_STATE_CLI_OFFLOAD_H_
