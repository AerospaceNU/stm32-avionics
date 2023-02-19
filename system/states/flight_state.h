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
  FlightState(int id, uint32_t period_ms, bool hasPassedLaunch,
              bool hasPassedApogee, bool hasPassedTouchdown)
      : State(id, period_ms),
        m_hasPassedLaunch{hasPassedLaunch},
        m_hasPassedApogee{hasPassedApogee},
        m_hasPassedTouchdown{hasPassedTouchdown} {}

  ~FlightState() = default;

  EndCondition_e runState() override;

 protected:
  bool m_hasPassedLaunch;
  bool m_hasPassedApogee;
  bool m_hasPassedTouchdown;

 public:
  static EndCondition_e m_lastCliEndConn;
};

#endif  // SYSTEM_STATES_FLIGHT_STATE_H_
