/*
 * flight_state.h
 *
 *  Created on: Mar 12, 2022
 *      Author: matth
 */

#ifndef STATES_FLIGHT_STATES_INTERFACE_H_
#define STATES_FLIGHT_STATES_INTERFACE_H_

#include "states_interface.h"

class FlightState : public State {
 public:
  FlightState(int id, uint32_t period_ms, bool hasPastApogee)
      : State(id, period_ms), m_hasPastApogee(hasPastApogee) {}
  ~FlightState() = default;

  EndCondition_t run_state() override;

 protected:
  bool m_hasPastApogee;
};

#endif /* STATES_FLIGHT_STATES_INTERFACE_H_ */
