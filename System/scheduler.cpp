#include "scheduler.h"

#include "hardware_manager.h"
#include "state_ascent.h"
#include "state_cli_calibrate.h"
#include "state_cli_config.h"
#include "state_cli_erase_flash.h"
#include "state_cli_help.h"
#include "state_cli_main.h"
#include "state_cli_offload.h"
#include "state_cli_pyro_fire.h"
#include "state_cli_sense.h"
#include "state_descent.h"
#include "state_initialize.h"
#include "state_log.h"
#include "state_post_flight.h"
#include "state_pre_flight.h"
#include "state_shutdown.h"

void Scheduler::run(void) {
  /* Create all necessary states initially and store in list */
  uint32_t defaultPeriod = 15;
  CliCalibrateState cliCalibrate =
      CliCalibrateState(StateId::CliCalibrate, defaultPeriod);
  CliConfigState cliConfig = CliConfigState(StateId::CliConfig, defaultPeriod);
  CliEraseFlashState cliEraseFlash =
      CliEraseFlashState(StateId::CliEraseFlash, defaultPeriod);
  CliHelpState cliHelp = CliHelpState(StateId::CliHelp, defaultPeriod);
  CliPyroFireState cliPyroFire =
      CliPyroFireState(StateId::CliPyroFire, defaultPeriod);
  CliMainState cliMain = CliMainState(StateId::CliMain, defaultPeriod);
  CliSenseState cliSense = CliSenseState(StateId::CliSense, defaultPeriod);
  CliOffloadState cliOffload =
      CliOffloadState(StateId::CliOffload, defaultPeriod);
  AscentState ascent = AscentState(StateId::Ascent, defaultPeriod);
  InitializeState initialize =
      InitializeState(StateId::Initialize, defaultPeriod);
  DescentState descent = DescentState(StateId::Descent, defaultPeriod);
  PostFlightState postFlight =
      PostFlightState(StateId::PostFlight, defaultPeriod);
  PreFlightState preFlight = PreFlightState(StateId::PreFlight, defaultPeriod);
  ShutdownState shutdown = ShutdownState(StateId::Shutdown, defaultPeriod);

  State* states[] = {&cliCalibrate, &cliConfig,  &cliEraseFlash, &cliHelp,
                     &cliPyroFire,  &cliMain,    &cliOffload,    &cliSense,
                     &ascent,       &initialize, &descent,       &postFlight,
                     &preFlight,    &shutdown};

  // Initialize the current and next states
  pCurrentState_ = nullptr;
  State* pNextState = &initialize;

  // Helper functions throughout infinite loop
  uint32_t lastTime_ = HM_Millis();
  EndCondition_t endCondition = NoChange;

  // Keep running scheduler forever
  while (1) {
    // Limit rate scheduler runs at
    if (pCurrentState_)
      while ((HM_Millis() - lastTime_) < pCurrentState_->getPeriodMS()) {
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
    StateId nextState = getNextState(endCondition);
    for (State* state : states) {
      if (state->getID() == nextState) {
        pNextState = state;
        break;
      }
    }
  }
}

Scheduler::StateId Scheduler::getNextState(EndCondition_t endCondition) {
  // Return the current state if there's no change
  if (endCondition == EndCondition_t::NoChange) {
    return static_cast<Scheduler::StateId>(pCurrentState_->getID());
  }
  // Look for next state based on current state and end condition
  switch (pCurrentState_->getID()) {
    case StateId::CliCalibrate:
    case StateId::CliConfig:
    case StateId::CliEraseFlash:
    case StateId::CliHelp:
    case StateId::CliPyroFire:
    case StateId::CliOffload:
    case StateId::CliSense:
      switch (endCondition) {
        case EndCondition_t::CliCommandComplete:
          return StateId::CliMain;
        default:
          break;
      }
      break;
    case StateId::CliMain:
      switch (endCondition) {
        case EndCondition_t::UsbDisconnect:
          return StateId::PreFlight;
        case EndCondition_t::CalibrateCommand:
          return StateId::CliCalibrate;
        case EndCondition_t::ConfigCommand:
          return StateId::CliConfig;
        case EndCondition_t::EraseFlashCommand:
          return StateId::CliEraseFlash;
        case EndCondition_t::HelpCommand:
          return StateId::CliHelp;
        case EndCondition_t::PyroFireCommand:
          return StateId::CliPyroFire;
        case EndCondition_t::OffloadCommand:
          return StateId::CliOffload;
        case EndCondition_t::SenseCommand:
          return StateId::CliSense;
        case EndCondition_t::SimCommand:
          return StateId::PreFlight;
        case EndCondition_t::ShutdownCommand:
          return StateId::Shutdown;
        default:
          break;
      }
      break;
    case StateId::Ascent:
      switch (endCondition) {
        case EndCondition_t::Apogee:
          return StateId::Descent;
        default:
          break;
      }
      break;
    case StateId::Initialize: {
      if (endCondition == EndCondition_t::UsbConnect) {
        return StateId::CliMain;
      }
      int storedStateInt = state_log_read();
      // Resumes a flight from the state log if there is a state to resume
      if (storedStateInt >= 0) {
        StateId storedStateId = static_cast<Scheduler::StateId>(storedStateInt);
        // Only resume from the 4 flight states
        // (They *should* be the only states written in the state log
        // regardless)
        if (storedStateId == StateId::Ascent ||
            storedStateId == StateId::Descent) {
          state_log_reload_flight();
          return storedStateId;
        }
      }
      if (endCondition == EndCondition_t::UsbDisconnect) {
        return StateId::PreFlight;
      }
      break;
    }
    case StateId::Descent:
      switch (endCondition) {
        case EndCondition_t::Touchdown:
          return StateId::PostFlight;
        default:
          break;
      }
      break;
    case StateId::PostFlight:
      switch (endCondition) {
        case EndCondition_t::UsbConnect:
        case EndCondition_t::CliCommandComplete:
          return StateId::CliMain;
        default:
          break;
      }
      break;
    case StateId::PreFlight:
      switch (endCondition) {
        case EndCondition_t::Launch:
          return StateId::Ascent;
        case EndCondition_t::UsbConnect:
          return StateId::CliMain;
        default:
          break;
      }
      break;
    case StateId::Shutdown:
      switch (endCondition) {
        default:
          break;
      }
      break;
    default:
      break;
  }
  return StateId::UNKNOWN;
}
