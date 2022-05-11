#include "buzzer_heartbeat.h"
#include "cli.h"
#include "cli_tasks.h"
#include "hardware_manager.h"
#include "state_log.h"

static bool simModeStarted = false;
static CliComms_t simModeSource;
static bool allowedTransitions[CliCommand_t::NUM_CLI_COMMANDS] = {0};

#define ILLEGAL_TRANSITION                     \
  cliSendAck(false, "Transition disallowed!"); \
  break;

void cli_tasks::ConfigureForFlight() {
  // Allow all transitions, then mask out
  for (int i = 0; i < NUM_CLI_COMMANDS; i++) {
    allowedTransitions[i] = true;
  }

  // Disable state transition commands in flight
  allowedTransitions[CliCommand_t::ERASE_FLASH] = false;
  allowedTransitions[CliCommand_t::CALIBRATE] = false;
  allowedTransitions[CliCommand_t::SIM] = false;
  allowedTransitions[CliCommand_t::OFFLOAD] = false;
}
void cli_tasks::ConfigureForGround() {
  // allow all transitions on ground
  for (int i = 0; i < NUM_CLI_COMMANDS; i++) {
    allowedTransitions[i] = true;
  }
}

EndCondition_t cli_tasks::cliTick() {
  // Run buzzer heartbeat
  buzzerHeartbeat();

  // Check if sim is ready to start, if we're waiting for it to
  if (simModeStarted &&
      cbCount(cliGetRxBufferFor(simModeSource)) >= sizeof(SensorData_t)) {
    HM_EnableSimMode(cliGetRxBuffer());
    simModeStarted = false;
    return EndCondition_t::SimCommand;
  }

  // Check for CLI input, and parse the first that has a command for us to parse
  for (int i = 0; i < CliComms_t::NUM_CLI_COMMS; i++) {
    // Special case: in sim mode, don't parse data coming in over USB
    if (HM_InSimMode() && i == CliComms_t::CLI_USB) continue;

    CliCommand_t command = cliParse((CliComms_t)i);
    if (command == NONE) continue;

    // If the command is a task, do it
    // If not, return the appropriate end condition to get us
    // into the correct State (if we can)
    switch (command) {
      case CliCommand_t::CALIBRATE:
        if (allowedTransitions[CliCommand_t::CALIBRATE]) {
          return EndCondition_t::CalibrateCommand;
        } else {
          ILLEGAL_TRANSITION
        }
      case CliCommand_t::CONFIG:
        if (allowedTransitions[CliCommand_t::CONFIG]) cliConfig();
        return EndCondition_t::NoChange;
      case CliCommand_t::ERASE_FLASH:
        if (allowedTransitions[CliCommand_t::ERASE_FLASH]) {
          return EndCondition_t::EraseFlashCommand;
        } else {
          ILLEGAL_TRANSITION
        }
      case CliCommand_t::HELP:
        if (allowedTransitions[CliCommand_t::HELP]) cliHelp();
        return EndCondition_t::NoChange;
      case CliCommand_t::OFFLOAD:
        if (allowedTransitions[CliCommand_t::OFFLOAD]) {
          return EndCondition_t::OffloadCommand;
        } else {
          ILLEGAL_TRANSITION
        }
      case CliCommand_t::SIM:
        if (allowedTransitions[CliCommand_t::SIM]) {
          // We still need to wait for all the data to come in
          simModeStarted = true;
          simModeSource = static_cast<CliComms_t>(i);
          cliSendAck(true, nullptr);
        } else {
          ILLEGAL_TRANSITION
        }

        // If enabled, we'll transition into Sim later once we get data
        return EndCondition_t::NoChange;
      case CliCommand_t::SENSE:
        if (allowedTransitions[CliCommand_t::SENSE]) cliSense();
        return EndCondition_t::NoChange;
      case CliCommand_t::CREATE_NEW_FLIGHT:
        if (allowedTransitions[CliCommand_t::CREATE_NEW_FLIGHT]) {
          // Clear state log so that future runs won't try to resume
          state_log_write_complete();
          cliSendAck(true, nullptr);
          return EndCondition_t::NewFlight;
        } else {
          ILLEGAL_TRANSITION
        }
      case CliCommand_t::PYROFIRE:
        if (allowedTransitions[CliCommand_t::PYROFIRE]) {
          cliPyroFire();
        } else {
          ILLEGAL_TRANSITION
        }
        break;
      case CliCommand_t::LINECUTTER:
        if (allowedTransitions[CliCommand_t::LINECUTTER]) cliSendLineCutterstring();
        break;
      default:
        break;
    }
  }

  return EndCondition_t::NoChange;
}
