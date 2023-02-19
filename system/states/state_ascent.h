#ifndef SYSTEM_STATES_STATE_ASCENT_H_
#define SYSTEM_STATES_STATE_ASCENT_H_

#include "flight_state.h"

class AscentState : public FlightState {
 public:
  AscentState(int id, uint32_t period_ms)
      : FlightState(id, period_ms, true, false, false) {}

  void init(void) override;

  EndCondition_e run(void) override;

  void cleanup(void) override;

 private:
  double maxPosZ = 0;
  static constexpr double kPosDiffThreshold = 3;
  static constexpr double kBurnoutAccThreshold = 10;

  static constexpr uint32_t kTransitionResetTimeThreshold =
      150;  // 150 ms transition timer

  uint32_t transitionResetTimer = 0;
};

#endif  // SYSTEM_STATES_STATE_ASCENT_H_
