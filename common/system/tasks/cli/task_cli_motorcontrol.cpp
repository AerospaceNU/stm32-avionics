#include <cstdio>
#include <cstdlib>

#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "board_config_common.h"
#include "guided_descent.h"
#include "hardware_manager.h"

void CliTasks::motorControl() {
  CliOptionVals_s options = cli_getOptions();
  if (!options.m) {
    cli_sendAck(false, "Motor number (-m) and degree setting (-d) required but not provided");
    return;  // verify non-void pointers
  }

  char* endPtr;
  int id = strtol(options.m, &endPtr, 0);
  if (*endPtr != '\0' || id < 0 || id >= NUM_DYNAMIXEL) {
    cli_sendAck(false, "Got invalid motor number");
    return;
  }

  if (options.d) {
	  int degrees = strtol(options.d, &endPtr, 0);
	  if (*endPtr != '\0') {
		cli_sendAck(false, "Got invalid motor number");
		return;
	  }

	  guided_descent::setMotor(hm_getSensorData(), id, degrees);
  } else if (options.N) {
	  guided_descent::setZeroOffset(id, hm_getSensorData()->dynamixelSetDegrees[id]);
	  dataLog_writeCliConfigs();
  } else {
	    cli_sendAck(false, "-N or -d required!");
	    return;  // verify non-void pointers
  }

  cli_sendAck(true, nullptr);
  cli_sendComplete(true, nullptr);
}
