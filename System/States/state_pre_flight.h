#ifndef SYSTEM_STATES_STATE_PRE_FLIGHT_H_
#define SYSTEM_STATES_STATE_PRE_FLIGHT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "filters.h"
#include "flight_state.h"
#include "hardware_manager.h"

class PreFlightState : public FlightState {
 public:
  PreFlightState(int id, uint32_t period_ms)
      : FlightState(id, period_ms, false) {}

  void init(void) override;

  EndCondition_t run(void) override;

  void cleanup(void) override;

 private:
  static constexpr int kBufferSize = 7;
  SensorData_t sensorDataBuffer[kBufferSize];
  FilterData_t filterDataBuffer[kBufferSize];
  int bufferCounter = 0;

  static constexpr double kLaunchAccelThreshold = 20.0;  // m/s**2
  static constexpr double kLaunchPosZDiffFailsafeThreshold =
      100.0;  // m failsafe
  static constexpr uint32_t kGravityRefInterval = 20;
  static constexpr uint32_t kTransitionResetTimeThreshold =
      150;  // 150 ms transition timer

  uint32_t transitionResetTimer = 0;

  uint32_t prevPressureLogTime = 0;
  uint32_t prefGravityRefTime = 0;

  bool simModeStarted = false;

  bool gpsTimestamp = false;

  bool doCleanup = false;
};

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_STATES_STATE_PRE_FLIGHT_H_
