/*
 * state_cli_sense.h
 */

#ifndef STATES_CLI_STATE_CLI_SENSE_H_
#define STATES_CLI_STATE_CLI_SENSE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class CliSenseState : public State {

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

#endif /* STATES_CLI_STATE_CLI_SENSE_H_ */
