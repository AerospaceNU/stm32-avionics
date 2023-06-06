#ifndef COMMON_SYSTEM_TASKS_CLI_CLI_TASKS_H_
#define COMMON_SYSTEM_TASKS_CLI_CLI_TASKS_H_

#include "states_interface.h"

namespace CliTasks {
EndCondition_e tick();

// Change allowed transitions based on flight state
void configureForFlight();
void configureForGround();

void config();
void sense();
void help();
void triggerFire();
void sendLineCutterString();  // Send a string to a line cutter
void version();

}  // namespace CliTasks

#endif  // COMMON_SYSTEM_TASKS_CLI_CLI_TASKS_H_
