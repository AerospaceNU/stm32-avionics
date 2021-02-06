#ifndef STATE_COAST_ASCENT_H_
#define STATE_COAST_ASCENT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class CoastAscentState : public State {

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

#endif /* STATE_COAST_ASCENT_H_ */

