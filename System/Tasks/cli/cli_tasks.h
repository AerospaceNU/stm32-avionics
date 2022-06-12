#ifndef SYSTEM_TASKS_CLI_CLI_TASKS_H_
#define SYSTEM_TASKS_CLI_CLI_TASKS_H_

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
void cliSendLineCutterstring();  // Send a string to a line cutter

}  // namespace cli_tasks

#endif  // SYSTEM_TASKS_CLI_CLI_TASKS_H_
