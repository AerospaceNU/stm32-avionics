
#include "state_cli_temp.h"

EndCondition_t CliTempState::run() {
  return EndCondition_t::CliCommandComplete;
}
