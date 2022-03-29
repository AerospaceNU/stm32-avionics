#pragma once

#include "states_interface.h"

namespace cli_tasks {
EndCondition_t cliTick();

// Change allowed transitions based on flight state
void ConfigureForFlight();
void ConfigureForGround();

void cliConfig();
void cliSense();
void cliHelp();
void cliPyroFire();
}  // namespace cli_tasks
