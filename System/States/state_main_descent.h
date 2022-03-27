#ifndef SYSTEM_STATES_STATE_MAIN_DESCENT_H_
#define SYSTEM_STATES_STATE_MAIN_DESCENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "flight_state.h"

class MainDescentState : public FlightState {
 public:
  MainDescentState(int id, uint32_t period_ms)
      : FlightState(id, period_ms, true) {}

  void init(void) override;

  EndCondition_t run(void) override;

  void cleanup(void) override;

 private:
  static constexpr double kTouchdownZPosChangeThreshold = 3;  // meters
  static constexpr uint32_t kTransitionResetTimeThreshold =
      5000;  // 5 second transition timer

  uint32_t transitionResetTimer = 0;

  double altitude = 0;
};

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_STATES_STATE_MAIN_DESCENT_H_
