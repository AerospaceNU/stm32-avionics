#include "scheduler.h"

#include <cstdio>

#include "cli_tasks.h"
#include "hardware_manager.h"

namespace {}

Scheduler::Scheduler() {
  pCurrentState_ = nullptr;
  pNextState = &initialize;

  lastTime_ = hm_millis();
  tickIdx = 0;
}

uint32_t Scheduler::getNextExpirationTimeMillis() {
  // TODO what should we do if current state is null here somehow?
  if (!pCurrentState_) return 0;

  return lastTime_ + pCurrentState_->getPeriodMS();
}

bool Scheduler::hasTimerExpired() {
  // TODO what should we do if current state is null here somehow?
  if (!pCurrentState_) return true;

  return ((hm_millis() - lastTime_) > pCurrentState_->getPeriodMS());
}

void Scheduler::tick(void) {
  /* Create all necessary states initially and store in list */

  // Helper functions throughout infinite loop
  EndCondition_e endCondition = NoChange;

  lastTime_ = hm_millis();
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

  hm_observeTickComplete(++tickIdx);
}

Scheduler::StateId_e Scheduler::getNextState(EndCondition_e endCondition) {
  // Return the current state if there's no change
  if (endCondition == EndCondition_e::NoChange) {
    return static_cast<Scheduler::StateId_e>(pCurrentState_->getID());
  }

  if (endCondition == EndCondition_e::NewFlightCommand) {
    return StateId_e::PreFlight;
  }

  // Look for next state based on current state and end condition
  switch (pCurrentState_->getID()) {
    // Coming from any CLI state, we want to end up back in
    // preflight
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
      int storedStateInt = stateLog_read();
      // Resumes a flight from the state log if there is a state to resume
      if (storedStateInt >= 0) {
        StateId_e storedStateId_e =
            static_cast<Scheduler::StateId_e>(storedStateInt);
        // Only resume from the 2 flight states
        // (They *should* be the only states written in the state log
        // regardless)
        if (storedStateId_e == StateId_e::Ascent ||
            storedStateId_e == StateId_e::Descent) {
          stateLog_reloadFlight();
          CliTasks::configureForFlight();
          return storedStateId_e;
        }
      }

      return StateId_e::PreFlight;
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
