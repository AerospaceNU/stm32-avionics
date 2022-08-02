#ifndef SYSTEM_STATES_CLI_STATE_CLI_ERASE_FLASH_H_
#define SYSTEM_STATES_CLI_STATE_CLI_ERASE_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "states_interface.h"

class CliEraseFlashState : public State {
 public:
  using State::getID;
  using State::getPeriodMS;
  using State::State;

  void init(void) override;

  EndCondition_t run(void) override;

  void cleanup(void) override;

 private:
  static constexpr uint32_t SEND_PERIOD = 990;
  uint32_t last_time = 0;
};

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_STATES_CLI_STATE_CLI_ERASE_FLASH_H_
