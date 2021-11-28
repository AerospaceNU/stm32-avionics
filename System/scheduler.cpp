#include "hardware_manager.h"
#include "scheduler.h"
#include "state_cli_calibrate.h"
#include "state_cli_choose_flight.h"
#include "state_cli_config.h"
#include "state_cli_erase_flash.h"
#include "state_cli_help.h"
#include "state_cli_main.h"
#include "state_cli_offload.h"
#include "state_coast_ascent.h"
#include "state_drogue_descent_n.h"
#include "state_initialize.h"
#include "state_main_descent.h"
#include "state_post_flight.h"
#include "state_powered_ascent.h"
#include "state_pre_flight.h"
#include "state_shutdown.h"
#include "stm32h7xx_hal.h"

void Scheduler::run(void) {

    /* Create all necessary states initially and store in list */
	uint32_t defaultPeriod = 15;
	CliCalibrateState cliCalibrate = CliCalibrateState(StateId::CliCalibrate, defaultPeriod);
	CliChooseFlightState cliChooseFlight = CliChooseFlightState(StateId::CliChooseFlight, defaultPeriod);
	CliConfigState cliConfig = CliConfigState(StateId::CliConfig, defaultPeriod);
	CliEraseFlashState cliEraseFlash = CliEraseFlashState(StateId::CliEraseFlash, defaultPeriod);
	CliHelpState cliHelp = CliHelpState(StateId::CliHelp, defaultPeriod);
	CliMainState cliMain = CliMainState(StateId::CliMain, defaultPeriod);
	CliOffloadState cliOffload = CliOffloadState(StateId::CliOffload, defaultPeriod);
	CoastAscentState coastAscent = CoastAscentState(StateId::CoastAscent, defaultPeriod);
	DrogueDescentNState drogueDescentN = DrogueDescentNState(StateId::DrogueDescentN, defaultPeriod);
	InitializeState initialize = InitializeState(StateId::Initialize, defaultPeriod);
	MainDescentState mainDescent = MainDescentState(StateId::MainDescent, defaultPeriod);
	PostFlightState postFlight = PostFlightState(StateId::PostFlight, defaultPeriod);
	PoweredAscentState poweredAscent = PoweredAscentState(StateId::PoweredAscent, defaultPeriod);
	PreFlightState preFlight = PreFlightState(StateId::PreFlight, defaultPeriod);
	ShutdownState shutdown = ShutdownState(StateId::Shutdown, defaultPeriod);

	State* states[] = {&cliCalibrate, &cliChooseFlight, &cliConfig, &cliEraseFlash, &cliHelp,
			&cliMain, &cliOffload, &coastAscent, &drogueDescentN, &initialize, &mainDescent,
			&postFlight, &poweredAscent, &preFlight, &shutdown};

	// Initialize the current and next states
	pCurrentState_ = nullptr;
    State* pNextState = &initialize;

    // Helper functions throughout infinite loop
    uint32_t lastTime_ = HM_Millis();
    EndCondition_t endCondition = NoChange;

    // Keep running scheduler forever
    while(1) {

        // Limit rate scheduler runs at
    	if (pCurrentState_)
    		while((HM_Millis() - lastTime_) < pCurrentState_->getPeriodMS());
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
	switch(pCurrentState_->getID()) {
	case StateId::CliCalibrate:
		switch(endCondition) {
		default:
			break;
		}
		break;
	case StateId::CliChooseFlight:
		switch(endCondition) {
		default:
			break;
		}
		break;
	case StateId::CliConfig:
		switch(endCondition) {
		default:
			break;
		}
		break;
	case StateId::CliEraseFlash:
		switch(endCondition) {
		default:
			break;
		}
		break;
	case StateId::CliHelp:
		switch(endCondition) {
		default:
			break;
		}
		break;
	case StateId::CliMain:
		switch(endCondition) {
		case EndCondition_t::UsbDisconnect:
			return StateId::PreFlight;
		default:
			break;
		}
		break;
	case StateId::CliOffload:
		switch(endCondition) {
		default:
			break;
		}
		break;
	case StateId::CoastAscent:
		switch(endCondition) {
		case EndCondition_t::Apogee:
			return StateId::DrogueDescentN;
		default:
			break;
		}
		break;
	case StateId::DrogueDescentN:
		switch(endCondition) {
		case EndCondition_t::MainCutAltitude:
			return StateId::MainDescent;
		default:
			break;
		}
		break;
	case StateId::Initialize:
		switch(endCondition) {
		case EndCondition_t::UsbConnect:
			return StateId::CliMain;
		case EndCondition_t::UsbDisconnect:
			return StateId::PreFlight;
		default:
			break;
		}
		break;
	case StateId::MainDescent:
		switch(endCondition) {
		case EndCondition_t::Touchdown:
			return StateId::PostFlight;
		default:
			break;
		}
		break;
	case StateId::PostFlight:
		switch(endCondition) {
		case EndCondition_t::UsbConnect:
			return StateId::CliMain;
		default:
			break;
		}
		break;
	case StateId::PoweredAscent:
		switch(endCondition) {
		case EndCondition_t::MotorBurnout:
			return StateId::CoastAscent;
		default:
			break;
		}
		break;
	case StateId::PreFlight:
		switch(endCondition) {
		case EndCondition_t::Launch:
			return StateId::PoweredAscent;
		case EndCondition_t::UsbConnect:
			return StateId::CliMain;
		default:
			break;
		}
		break;
	case StateId::Shutdown:
		switch(endCondition) {
		default:
			break;
		}
		break;
	default:
		break;
	}
	return StateId::UNKNOWN;
}