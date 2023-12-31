#ifndef COMMON_SYSTEM_STATES_STATE_ASCENT_H_
#define COMMON_SYSTEM_STATES_STATE_ASCENT_H_

#include "flight_state.h"

class AscentState : public FlightState {
 public:
  AscentState(uint8_t id, uint32_t period_ms)
      : FlightState(id, period_ms, true, false, false) {}

  void init(void) override;

  EndCondition_e run(void) override;

  void cleanup(void) override;

 private:
  double maxPosZ = 0;
  static constexpr double kPosDiffThreshold = 3;
  static constexpr double kBurnoutAccThreshold = 10;

  static constexpr uint32_t kBurnoutResetTimeThreshold =
      150;  // 150 ms transition timer

  uint32_t burnoutResetTimer = 0;
};

#endif  // COMMON_SYSTEM_STATES_STATE_ASCENT_H_
