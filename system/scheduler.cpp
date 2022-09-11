#include "scheduler.h"

#include "cli_tasks.h"
#include "hardware_manager.h"
#include "state_ascent.h"
#include "state_cli_calibrate.h"
#include "state_cli_erase_flash.h"
#include "state_cli_offload.h"
#include "state_cli_temp.h"
#include "state_descent.h"
#include "state_initialize.h"
#include "state_log.h"
#include "state_post_flight.h"
#include "state_pre_flight.h"

void Scheduler::run(void) {
  /* Create all necessary states initially and store in list */
  uint32_t defaultPeriod = 15;
  CliCalibrateState cliCalibrate =
      CliCalibrateState(StateId_e::CliCalibrate, defaultPeriod);
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

  State* states[] = {&cliCalibrate, &cliEraseFlash, &cliOffload,
                     &ascent,       &initialize,    &descent,
                     &postFlight,   &preFlight,     &tempState};

  // Initialize the current and next states
  pCurrentState_ = nullptr;
  State* pNextState = &initialize;

  // Helper functions throughout infinite loop
  uint32_t lastTime_ = HM_Millis();
  EndCondition_e endCondition = NoChange;

  // Keep running scheduler forever
  while (1) {
    // Limit rate scheduler runs at
    if (pCurrentState_) {
      while ((HM_Millis() - lastTime_) < pCurrentState_->getPeriodMS()) {
      }
    }

    lastTime_ = HM_Millis();
    // Visually show how fast scheduler is running using LED
    HM_LedToggle(1);

    // Cleanup current state and initialize next state if changing states
    if (pNextState != pCurrentState_) {
      if (pCurrentState_) pCurrentState_->cleanup();
      if (pNextState) pNextState->init();
      pCurrentState_ = pNextState;
    }

    // Run the current state
    if (pCurrentState_) endCondition = pCurrentState_->run_state();

    // Find and set the next state
    StateId_e nextState = getNextState(endCondition);
    for (State* state : states) {
      if (state->getID() == nextState) {
        pNextState = state;
        break;
      }
    }
  }
}

Scheduler::StateId_e Scheduler::getNextState(EndCondition_e endCondition) {
  // Return the current state if there's no change
  if (endCondition == EndCondition_e::NoChange) {
    return static_cast<Scheduler::StateId_e>(pCurrentState_->getID());
  }

  if (endCondition == EndCondition_e::NewFlight) {
    return StateId_e::PreFlight;
  }

  // Look for next state based on current state and end condition
  switch (pCurrentState_->getID()) {
    // Coming from any CLI state, we want to end up back in
    // preflight
    case StateId_e::CliCalibrate:
    case StateId_e::CliEraseFlash:
    case StateId_e::CliOffload:
      switch (endCondition) {
        case EndCondition_e::CliCommandComplete: {
          return StateId_e::PreFlight;  // TODO should we go back to initialize?
        }
        default:
          break;
      }
      break;
    case StateId_e::Ascent:
      switch (endCondition) {
        case EndCondition_e::Apogee:
          return StateId_e::Descent;
        default:
          break;
      }
      break;
    case StateId_e::Initialize: {
      int storedStateInt = state_log_read();
      // Resumes a flight from the state log if there is a state to resume
      if (storedStateInt >= 0) {
        StateId_e storedStateId_e =
            static_cast<Scheduler::StateId_e>(storedStateInt);
        // Only resume from the 2 flight states
        // (They *should* be the only states written in the state log
        // regardless)
        if (storedStateId_e == StateId_e::Ascent ||
            storedStateId_e == StateId_e::Descent) {
          state_log_reload_flight();
          cli_tasks::ConfigureForFlight();
          return storedStateId_e;
        }
      }

      return StateId_e::PreFlight;

      break;
    }
    case StateId_e::Descent:
      switch (endCondition) {
        case EndCondition_e::Touchdown:
          return StateId_e::PostFlight;
        default:
          break;
      }
      break;
    case StateId_e::PostFlight:
    case StateId_e::PreFlight:
      switch (endCondition) {
        case EndCondition_e::Launch:
          return StateId_e::Ascent;
        case EndCondition_e::SimCommand:
          return StateId_e::SimTempState;
        case EndCondition_e::CalibrateCommand:
          return StateId_e::CliCalibrate;
        case EndCondition_e::OffloadCommand:
          return StateId_e::CliOffload;
        case EndCondition_e::EraseFlashCommand:
          return StateId_e::CliEraseFlash;
        default:
          break;
      }
      break;
    case StateId_e::SimTempState:
      return StateId_e::PreFlight;
    default:
      break;
  }
  return StateId_e::UNKNOWN;
}
