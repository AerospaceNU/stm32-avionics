#ifndef STATE_POWERED_ASCENT_H_
#define STATE_POWERED_ASCENT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class PoweredAscentState : public State {

	public:
		using State::State;
		using State::getID;
		using State::getPeriodMS;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;

	private:
		static constexpr double kMotorBurnoutZAccelDiffThreshold = 20.0; // m/s^2

		double maxAccelZ = 0;

		int thresholdCounter;
		int thresholdLimit = 5;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_POWERED_ASCENT_H_ */

