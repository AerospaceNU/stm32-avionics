
#include "state_cli_erase_flash.h"

#include "board_config_common.h"
#include "buzzer_heartbeat.h"
#include "cli.h"
#include "data_log.h"
#include "hardware_manager.h"

void CliEraseFlashState::init() {
  // Send initial ack to CLI and start erasing chip
  cli_sendAck(true, nullptr);
  last_time = hm_millis();
  _curFlashId = 0;
  dataLog_flashErase(_curFlashId);
}

EndCondition_e CliEraseFlashState::run() {
  // Run buzzer heartbeat
  buzzerHeartbeat_tick();

  // Only send data or check end condition once per second
  if (!hm_flashIsEraseComplete(_curFlashId)) {
    if ((hm_millis() - last_time) > SEND_PERIOD) {
      // Inform user that erase operation is still in progress
      cli_send("Erasing...\r\n");
      last_time = hm_millis();
    }
  } else {
    _curFlashId++;
    if (_curFlashId == FIRST_ID_FLASH_S25FLX + NUM_FLASH_S25FLX) {
      return EndCondition_e::CliCommandComplete;
    }
    dataLog_flashErase(_curFlashId);
  }
  return EndCondition_e::NoChange;
}

void CliEraseFlashState::cleanup() {
  // Assign new flight initially to have a place to start logging again
  dataLog_assignFlight();
  // Re-write current configs so that they persist
  dataLog_writeCliConfigs();
  // Send complete message to CLI. No way of knowing if there is a failure
  // currently.
  cli_sendComplete(true, nullptr);
}
