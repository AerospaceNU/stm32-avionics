/*
 * states_interface.cpp
 *
 *  Created on: Mar 21, 2022
 *      Author: matth
 */

#include "states_interface.h"

EndCondition_e State::runState() {
  EndCondition_e result = run();

  // Refresh watchdog
  hm_watchdogRefresh();

  run_counter_++;
  return result;
}
