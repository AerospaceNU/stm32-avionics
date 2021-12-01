#ifndef STATE_CLI_CHOOSE_FLIGHT_H_
#define STATE_CLI_CHOOSE_FLIGHT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class CliChooseFlightState : public State {

	public:
		using State::State;
		using State::getID;
		using State::getPeriodMS;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_CLI_CALIBRATE_H */

