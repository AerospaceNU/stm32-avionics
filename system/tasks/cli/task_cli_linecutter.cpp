#include <cstdio>
#include <cstdlib>

#include "cli.h"
#include "cli_tasks.h"
#include "hardware_manager.h"

void CliTasks::sendLineCutterString() {
  CliOptionVals_s options = cli_getOptions();
  if (!options.lcId || !options.lcCmd) {
    cli_sendAck(false, "Command (-c) and ID (-i) required but not provided");
    return;  // verify non-void pointers
  }

  char* endPtr;
  int id = strtol(options.lcId, &endPtr, 0);
  if (*endPtr != '\0') {
    cli_sendAck(false, "Got invalid LC ID");
    return;
  }

  if (*options.lcCmd == '\0') {
    cli_sendAck(false, "Got invalid LC command");
    return;
  }

  cli_sendAck(true, nullptr);
  hm_lineCutterSendString(id, options.lcCmd);
  cli_sendComplete(true, nullptr);
}
