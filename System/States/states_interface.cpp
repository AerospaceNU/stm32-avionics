/*
 * states_interface.cpp
 *
 *  Created on: Mar 21, 2022
 *      Author: matth
 */

#include "states_interface.h"

#include "cli.h"

namespace cli_tasks {
EndCondition_t cliTick();
}

EndCondition_t State::run_state() {
  EndCondition_t result = run();

  auto endCon = cli_tasks::cliTick();
  if (endCon != NoChange) {
    return endCon;
  }

  // Refresh watchdog
  HM_IWDG_Refresh();

  run_counter_++;
  return result;
}
