
#include "state_cli_erase_flash.h"

#include "cli.h"
#include "data_log.h"
#include "hardware_manager.h"

void CliEraseFlashState::init() {
  // Send initial ack to CLI and start erasing chip
  cliSendAck(true, nullptr);
  last_time = HM_Millis();
  data_log_flash_erase();
}

EndCondition_t CliEraseFlashState::run() {
  // Only send data or check end condition once per second
  if (!HM_FlashIsEraseComplete()) {
    if ((HM_Millis() - last_time) > SEND_PERIOD) {
      // Inform user that erase operation is still in progress
      cliSend("Erasing...");
      last_time = HM_Millis();
    }
  } else {
    // Re-write current configs so that they persist
    data_log_write_cli_configs();
    return EndCondition_t::CliCommandComplete;
  }
  return EndCondition_t::NoChange;
}

void CliEraseFlashState::cleanup() {
  // Send complete message to CLI. No way of knowing if there is a failure
  // currently.
  cliSendComplete(true, nullptr);
}
