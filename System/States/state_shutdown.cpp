
#include "state_shutdown.h"

#include "cli.h"

void ShutdownState::init() {
  // Send acknowledgement that this command was reached
  cliSendAck(true, nullptr);
  // Notify CLI that shutdown has completed
  cliSendComplete(true, nullptr);
}

EndCondition_t ShutdownState::run() { return EndCondition_t::NoChange; }

void ShutdownState::cleanup() {}
