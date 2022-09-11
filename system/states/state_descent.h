#ifndef SYSTEM_STATES_STATE_DESCENT_H_
#define SYSTEM_STATES_STATE_DESCENT_H_

#include "flight_state.h"

class DescentState : public FlightState {
 public:
  DescentState(int id, uint32_t period_ms) : FlightState(id, period_ms, true) {}

  void init(void) override;

  EndCondition_e run(void) override;

  void cleanup(void) override;

 private:
  static constexpr double kTouchdownZPosChangeThreshold = 3;  // meters
  static constexpr uint32_t kTransitionResetTimeThreshold =
      5000;  // 5 second transition timer

  uint32_t transitionResetTimer = 0;

  double altitude = 0;

  bool sentCut1 = false;
  bool sentCut2 = false;
};

#endif  // SYSTEM_STATES_STATE_DESCENT_H_
