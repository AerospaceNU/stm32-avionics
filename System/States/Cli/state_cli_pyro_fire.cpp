
#include "state_cli_pyro_fire.h"

#include <stdlib.h>

#include "cli.h"
#include "hardware_manager.h"
#include "pyro_manager.h"

void CliPyroFireState::init() {}

EndCondition_t CliPyroFireState::run() {
  CliOptionVals_t options = cliGetOptions();
  if (options.p) {
    char* endPtr;
    int pyroNum = strtol(options.p, &endPtr, 10) - 1;
    if (*endPtr != '\0' || pyroNum < 0 || pyroNum >= MAX_PYRO) {
      cliSendAck(false, "Invalid pyro number");
      return EndCondition_t::CliCommandComplete;
    }
    PyroManager_PyroFire(pyroNum, 1000, false);
    // Send success ack to CLI
    cliSendAck(true, nullptr);
    return EndCondition_t::CliCommandComplete;
  }
  cliSendAck(false, "You must specify a pyro number to fire with -p");
  return EndCondition_t::CliCommandComplete;
}

void CliPyroFireState::cleanup() {
  // Send complete message to CLI
  cliSendComplete(true, nullptr);
}
