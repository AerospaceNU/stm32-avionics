#ifndef SYSTEM_SCHEDULER_H_
#define SYSTEM_SCHEDULER_H_

#include "state_ascent.h"
#include "state_cli_erase_flash.h"
#include "state_cli_offload.h"
#include "state_cli_temp.h"
#include "state_descent.h"
#include "state_initialize.h"
#include "state_log.h"
#include "state_post_flight.h"
#include "state_pre_flight.h"
#include "states_interface.h"

class Scheduler {
 public:
  /**
   * @brief Construct a new Scheduler, including all the states and state
   * transitions it manages
   */
  Scheduler();

  /**
   * @brief Determine if it's time to call tick() yet
   *
   * @return true if tick() should be called
   * @return false if not expired
   */
  bool hasTimerExpired(void);

  /**
   * @brief Get the time the next iteration should be run at, in the same
   * timebase as HM_Millis.
   *
   * @return uint32_t
   */
  uint32_t getNextExpirationTimeMillis();

  /**
   * @brief Run one iteration of the Scheduler. Does NOT block
   *
   */
  void tick(void);

 private:
  typedef enum {
    CliEraseFlash = 0,
    CliOffload,
    Ascent,
    Descent,
    Initialize,
    PostFlight,
    PreFlight,
    SimTempState,
    NUM_STATES,
    UNKNOWN,
  } StateId_e;

  StateId_e getNextState(EndCondition_e endCondition);

  uint32_t defaultPeriod = 15;
  CliEraseFlashState cliEraseFlash =
      CliEraseFlashState(StateId_e::CliEraseFlash, defaultPeriod);
  CliOffloadState cliOffload =
      CliOffloadState(StateId_e::CliOffload, defaultPeriod);
  AscentState ascent = AscentState(StateId_e::Ascent, defaultPeriod);
  InitializeState initialize =
      InitializeState(StateId_e::Initialize, defaultPeriod);
  DescentState descent = DescentState(StateId_e::Descent, defaultPeriod);
  PostFlightState postFlight =
      PostFlightState(StateId_e::PostFlight, defaultPeriod);
  PreFlightState preFlight =
      PreFlightState(StateId_e::PreFlight, defaultPeriod);
  CliTempState tempState = CliTempState(StateId_e::SimTempState, defaultPeriod);

  State* states[NUM_STATES] = {&cliEraseFlash, &cliOffload, &ascent,
                               &descent,       &initialize, &postFlight,
                               &preFlight,     &tempState};

  // Pointers for current and next states
  State* pCurrentState_ = nullptr;
  State* pNextState = &initialize;

  // Start time of the last loop
  uint32_t lastTime_;

  uint64_t tickIdx;
};

#endif  // SYSTEM_SCHEDULER_H_
