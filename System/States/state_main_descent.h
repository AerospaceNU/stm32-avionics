#ifndef STATE_MAIN_DESCENT_H_
#define STATE_MAIN_DESCENT_H_

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

  uint32_t transitionResetTimer;

  double altitude;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_MAIN_DESCENT_H_ */
