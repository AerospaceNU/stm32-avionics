#include <stdlib.h>

#include <cstdio>

#include "cli.h"
#include "cli_tasks.h"

void cli_tasks::cliPyroFire() {
  CliOptionVals_t options = cliGetOptions();
  if (options.p) {
    char* endPtr;
    int pyroNum = strtol(options.p, &endPtr, 10) - 1;
    if (*endPtr != '\0' || pyroNum < 0 || pyroNum >= NUM_PYRO) {
      cliSendAck(false, "Invalid pyro number");
      return;
    }

    char msg[70 + 4] = {0};
    snprintf(msg, sizeof(msg), "Firing Pyro %i for 1s\r\n", pyroNum);
    cliSend(msg);
    HM_RadioUpdate();

    PyroManager_PyroFire(pyroNum, 1000, false);

    // Send success ack to CLI
    cliSendAck(true, nullptr);
    return;
  }
  cliSendAck(false, "You must specify a pyro number to fire with -p");
}
