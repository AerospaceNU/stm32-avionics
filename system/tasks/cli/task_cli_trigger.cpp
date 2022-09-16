#include <cstdio>
#include <cstdlib>

#include "cli.h"
#include "cli_tasks.h"

void cli_tasks::cliTriggerFire() {
  CliOptionVals_s options = cliGetOptions();
  if (options.t) {
    char* endPtr;
    int triggerNum = strtol(options.t, &endPtr, 10);
    if (*endPtr != '\0' || triggerNum < 0 || triggerNum >= MAX_TRIGGER) {
      cliSendAck(false, "Invalid trigger number");
      return;
    }

    char msg[70 + 4] = {0};
    snprintf(msg, sizeof(msg), "Initializing trigger %i\r\n", triggerNum);
    cliSend(msg);
    HM_RadioUpdate();

    TriggerManager_TriggerFire(triggerNum, false);

    // Send success ack to CLI
    cliSendAck(true, nullptr);
    return;
  }
  cliSendAck(false, "You must specify a trigger number to fire with -t");
}
