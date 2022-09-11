
#include "state_cli_calibrate.h"

#include "cli.h"

void CliCalibrateState::init() { cliSendAck(true, nullptr); }

EndCondition_e CliCalibrateState::run() {
  return EndCondition_e::CliCommandComplete;
}

void CliCalibrateState::cleanup() { cliSendComplete(true, nullptr); }
