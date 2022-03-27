#ifndef SYSTEM_STATES_STATE_DROGUE_DESCENT_N_H_
#define SYSTEM_STATES_STATE_DROGUE_DESCENT_N_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "flight_state.h"

class DrogueDescentNState : public FlightState {
 public:
  DrogueDescentNState(int id, uint32_t period_ms)
      : FlightState(id, period_ms, true) {}

  void init(void) override;

  EndCondition_t run(void) override;

  void cleanup(void) override;

 private:
  static constexpr uint32_t kTransitionResetTimeThreshold =
      500;  // 500 ms transition timer

  uint32_t transitionResetTimer = 0;

  uint8_t completeDrogueCuts_ = 0;
};

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_STATES_STATE_DROGUE_DESCENT_N_H_
