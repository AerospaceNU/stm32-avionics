
#include "state_cli_main.h"

#include "buzzer_heartbeat.h"
#include "cli.h"
#include "data_log.h"
#include "hardware_manager.h"

void CliMainState::init() {
  cliInit();
  simModeStarted = false;
}

EndCondition_t CliMainState::run() {
  // Run buzzer heartbeat
  buzzerHeartbeat();

  // Check if USB is disconnected
  if (!HM_UsbIsConnected()) {
    return EndCondition_t::UsbDisconnect;
  }

  // Wait for sim data
  if (simModeStarted) {
    if (cbCount(cliGetRxBuffer()) >= sizeof(SensorData_t)) {
      return EndCondition_t::SimCommand;
    } else {
      return EndCondition_t::NoChange;
    }
  }

  // Check for CLI input over USB
  CliCommand_t command = cliParse(CliComms_t::CLI_USB);

  switch (command) {
    case CliCommand_t::CALIBRATE:
      return EndCondition_t::CalibrateCommand;
    case CliCommand_t::CONFIG:
      return EndCondition_t::ConfigCommand;
    case CliCommand_t::ERASE_FLASH:
      return EndCondition_t::EraseFlashCommand;
    case CliCommand_t::HELP:
      return EndCondition_t::HelpCommand;
    case CliCommand_t::OFFLOAD:
      return EndCondition_t::OffloadCommand;
    case CliCommand_t::SIM:
      HM_EnableSimMode(cliGetRxBuffer());
      simModeStarted = true;
      data_log_assign_flight();
      cliSendAck(true, nullptr);
      return EndCondition_t::NoChange;
    case CliCommand_t::SENSE:
      return EndCondition_t::SenseCommand;
    case CliCommand_t::SHUTDOWN:
      return EndCondition_t::ShutdownCommand;
    default:
      break;
  }
  return EndCondition_t::NoChange;
}

void CliMainState::cleanup() {}
