
#include "state_cli_erase_flash.h"

#include "board_config_common.h"
#include "buzzer_heartbeat.h"
#include "cli.h"
#include "data_log.h"
#include "hardware_manager.h"

void CliEraseFlashState::init() {
  // Send initial ack to CLI and start erasing chip
  cliSendAck(true, nullptr);
  last_time = HM_Millis();
  _curFlashId = 0;
  data_log_flash_erase(_curFlashId);
}

EndCondition_e CliEraseFlashState::run() {
  // Run buzzer heartbeat
  buzzerHeartbeat();

  // Only send data or check end condition once per second
  if (!HM_FlashIsEraseComplete(_curFlashId)) {
    if ((HM_Millis() - last_time) > SEND_PERIOD) {
      // Inform user that erase operation is still in progress
      cliSend("Erasing...");
      last_time = HM_Millis();
    }
  } else {
    _curFlashId++;
    if (_curFlashId == NUM_FLASH) {
      return EndCondition_e::CliCommandComplete;
    }
    data_log_flash_erase(_curFlashId);
  }
  return EndCondition_e::NoChange;
}

void CliEraseFlashState::cleanup() {
  // Re-write current configs so that they persist
  data_log_write_cli_configs();
  // Send complete message to CLI. No way of knowing if there is a failure
  // currently.
  cliSendComplete(true, nullptr);
}
