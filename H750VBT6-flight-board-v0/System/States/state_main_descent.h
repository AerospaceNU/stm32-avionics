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
		static constexpr double kTouchdownZAccelMagThreshold = 1.0; // m/s^2
		static constexpr double kTouchdownNoAccelTime = 1000.0; // ms

		double touchdownResetTime = 0; // Time when touchdown detection had to reset due to being outside threshold

		int thresholdCounter;
		int thresholdLimit = 5;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_MAIN_DESCENT_H_ */

