#include <cstdio>
#include <cstdlib>

#include "cli.h"
#include "cli_tasks.h"

void CliTasks::triggerFire() {
  CliOptionVals_s options = cli_getOptions();
  if (options.t) {
    char* endPtr;
    int triggerNum = strtol(options.t, &endPtr, 10);
    if (*endPtr != '\0' || triggerNum < 0 || triggerNum >= MAX_TRIGGER) {
      cli_sendAck(false, "Invalid trigger number");
      return;
    }

    TriggerConfig_s* triggerConfig =
        cli_getConfigs()->triggerConfiguration + triggerNum;
    if (!triggerConfig->allowedManual) {
      cli_sendAck(false, "Manual ignition of that trigger is disabled");
      return;
    }

    char msg[70 + 4] = {0};
    snprintf(msg, sizeof(msg), "Initializing trigger %i\r\n", triggerNum);
    cli_send(msg);
    hm_radioUpdate();

    triggerManager_triggerFire(triggerNum, false);

    // Send success ack to CLI
    cli_sendAck(true, nullptr);
    return;
  }
  cli_sendAck(false, "You must specify a trigger number to fire with -t");
}
