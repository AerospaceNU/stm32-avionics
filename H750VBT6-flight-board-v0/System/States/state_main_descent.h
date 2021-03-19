#ifndef STATE_MAIN_DESCENT_H_
#define STATE_MAIN_DESCENT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class MainDescentState : public State {

	public:
		using State::State;
		using State::getID;
		using State::getPeriodMS;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;

	private:
		static constexpr double kTouchdownVelocityThreshold = 0; // TODO: Figure out velocity threshold
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_MAIN_DESCENT_H_ */
