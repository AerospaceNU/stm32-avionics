/*
 * states_interface.cpp
 *
 *  Created on: Mar 21, 2022
 *      Author: matth
 */

#include "states_interface.h"

EndCondition_t State::run_state() {
  EndCondition_t result = run();

  // Refresh watchdog
  HM_WatchdogRefresh();

  run_counter_++;
  return result;
}
