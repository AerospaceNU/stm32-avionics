#ifndef SYSTEM_STATES_STATE_POST_FLIGHT_H_
#define SYSTEM_STATES_STATE_POST_FLIGHT_H_

#include "flight_state.h"

class PostFlightState : public FlightState {
 public:
  PostFlightState(uint8_t id, uint32_t period_ms)
      : FlightState(id, period_ms, true, true, true) {}

  void init(void) override;

  EndCondition_e run(void) override;

  void cleanup(void) override;

 private:
  uint32_t lastSimDataTime = 0;
};

#endif  // SYSTEM_STATES_STATE_POST_FLIGHT_H_
