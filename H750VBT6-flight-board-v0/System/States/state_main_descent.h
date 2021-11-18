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
		static constexpr double kTouchdownZPosChangeThreshold = 10; // meters
		static constexpr double kTransitionResetTimeThreshold = 5000; // 5 second transition timer

		uint32_t transitionResetTimer;

		double altitude = 0;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_MAIN_DESCENT_H_ */

