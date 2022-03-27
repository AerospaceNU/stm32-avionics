#ifndef STATE_CLI_OFFLOAD_H_
#define STATE_CLI_OFFLOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "states_interface.h"

class CliOffloadState : public State {
 public:
  using State::getID;
  using State::getPeriodMS;
  using State::State;

  void init(void) override;

  EndCondition_t run(void) override;

  void cleanup(void) override;

 private:
  bool initSuccess_;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_CLI_OFFLOAD_H_ */
