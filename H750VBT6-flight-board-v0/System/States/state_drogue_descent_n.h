#ifndef STATE_DROGUE_DESCENT_N_H_
#define STATE_DROGUE_DESCENT_N_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class DrogueDescentNState : public State {

	public:
		using State::State;
		using State::getID;
		using State::getPeriodMS;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;

	private:
		static constexpr double kTransitionResetTimeThreshold = 5000; // 5 second transition timer

		double transitionResetTimer;

		uint8_t completeDrogueCuts_ = 0;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_DROGUE_DESCENT_N_H_ */

