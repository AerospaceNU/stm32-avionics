/*
 * flight_state.h
 *
 *  Created on: Mar 12, 2022
 *      Author: matth
 */

#ifndef SYSTEM_STATES_FLIGHT_STATE_H_
#define SYSTEM_STATES_FLIGHT_STATE_H_

#include "states_interface.h"

class FlightState : public State {
 public:
  FlightState(int id, uint32_t period_ms, bool hasPastApogee)
      : State(id, period_ms), m_hasPastApogee{hasPastApogee} {}

  ~FlightState() = default;

  EndCondition_t run_state() override;

 protected:
  bool m_hasPastApogee;
};

#endif  // SYSTEM_STATES_FLIGHT_STATE_H_
