#include "circular_buffer.h"
#include "cli.h"
#include "cli_tasks.h"
#include "hardware_manager.h"
#include "state_log.h"

static bool simModeStarted = false;
static CliComms_e simModeSource;
static bool allowedTransitions[CliCommand_e::NUM_CLI_COMMANDS] = {0};

#define ILLEGAL_TRANSITION                     \
  cliSendAck(false, "Transition disallowed!"); \
  break;

void cli_tasks::ConfigureForFlight() {
  // Allow all transitions, then mask out
  for (int i = 0; i < NUM_CLI_COMMANDS; i++) {
    allowedTransitions[i] = true;
  }

  // Disable state transition commands in flight
  allowedTransitions[CliCommand_e::ERASE_FLASH] = false;
  allowedTransitions[CliCommand_e::SIM] = false;
  allowedTransitions[CliCommand_e::OFFLOAD] = false;
}
void cli_tasks::ConfigureForGround() {
  // allow all transitions on ground
  for (int i = 0; i < NUM_CLI_COMMANDS; i++) {
    allowedTransitions[i] = true;
  }
}

EndCondition_e cli_tasks::cliTick() {
  // Check if sim is ready to start, if we're waiting for it to
  if (simModeStarted &&
      cbCount(cliGetRxBufferFor(simModeSource)) >= sizeof(SensorData_s)) {
    HM_EnableSimMode(cliGetRxBuffer());
    simModeStarted = false;
    return EndCondition_e::SimCommand;
  }

  // Check for CLI input, and parse the first that has a command for us to parse
  for (int i = 0; i < CliComms_e::NUM_CLI_COMMS; i++) {
    // Special case: in sim mode, don't parse data coming in over USB
    if (HM_InSimMode() && i == CliComms_e::CLI_USB) continue;

    CliCommand_e command = cliParse((CliComms_e)i);
    if (command == NONE) continue;

    // If the command is a task, do it
    // If not, return the appropriate end condition to get us
    // into the correct State (if we can)
    switch (command) {
      case CliCommand_e::CONFIG:
        if (allowedTransitions[CliCommand_e::CONFIG]) cliConfig();
        return EndCondition_e::NoChange;
      case CliCommand_e::ERASE_FLASH:
        if (allowedTransitions[CliCommand_e::ERASE_FLASH]) {
          return EndCondition_e::EraseFlashCommand;
        } else {
          ILLEGAL_TRANSITION
        }
      case CliCommand_e::HELP:
        if (allowedTransitions[CliCommand_e::HELP]) cliHelp();
        return EndCondition_e::NoChange;
      case CliCommand_e::OFFLOAD:
        if (allowedTransitions[CliCommand_e::OFFLOAD]) {
          return EndCondition_e::OffloadCommand;
        } else {
          ILLEGAL_TRANSITION
        }
      case CliCommand_e::SIM:
        if (allowedTransitions[CliCommand_e::SIM]) {
          // We still need to wait for all the data to come in
          simModeStarted = true;
          simModeSource = static_cast<CliComms_e>(i);
          cliSendAck(true, nullptr);
        } else {
          ILLEGAL_TRANSITION
        }

        // If enabled, we'll transition into Sim later once we get data
        return EndCondition_e::NoChange;
      case CliCommand_e::SENSE:
        if (allowedTransitions[CliCommand_e::SENSE]) cliSense();
        return EndCondition_e::NoChange;
      case CliCommand_e::CREATE_NEW_FLIGHT:
        if (allowedTransitions[CliCommand_e::CREATE_NEW_FLIGHT]) {
          // Clear state log so that future runs won't try to resume
          state_log_write_complete();
          cliSendAck(true, nullptr);
          return EndCondition_e::NewFlightCommand;
        } else {
          ILLEGAL_TRANSITION
        }
      case CliCommand_e::PYROFIRE:
        if (allowedTransitions[CliCommand_e::PYROFIRE]) {
          cliPyroFire();
        } else {
          ILLEGAL_TRANSITION
        }
        break;
      case CliCommand_e::VERSION:
        if (allowedTransitions[CliCommand_e::VERSION]) {
          cliVersion();
        } else {
          ILLEGAL_TRANSITION
        }
        break;
      case CliCommand_e::LINECUTTER:
        if (allowedTransitions[CliCommand_e::LINECUTTER])
          cliSendLineCutterstring();
        break;
      default:
        break;
    }
  }

  return EndCondition_e::NoChange;
}
