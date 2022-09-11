#ifndef SYSTEM_STATES_STATE_INITIALIZE_H_
#define SYSTEM_STATES_STATE_INITIALIZE_H_

#include "states_interface.h"

class InitializeState : public State {
 public:
  using State::getID;
  using State::getPeriodMS;
  using State::State;

  void init(void) override;

  EndCondition_e run(void) override;

  void cleanup(void) override;
};

#endif  // SYSTEM_STATES_STATE_INITIALIZE_H_
