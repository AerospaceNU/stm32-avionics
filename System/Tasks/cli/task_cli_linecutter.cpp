#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "cli_tasks.h"
#include "hardware_manager.h"

void cli_tasks::cliSendLineCutterstring() {
  CliOptionVals_t options = cliGetOptions();
  if (!options.lcId || !options.lcCmd) return;  // verify non-void pointers

  char* endPtr;
  int id = strtol(options.lcId, &endPtr, 0);
  if (*endPtr != '\0') {
    cliSendAck(false, "Got invalid LC ID");
    return;
  }

  if (*options.lcCmd == '\0') {
    cliSendAck(false, "Got invalid LC command");
    return;
  }

  cliSendAck(true, nullptr);
  HM_LineCutterSendString(id, options.lcCmd);
  cliSendComplete(true, nullptr);
}
