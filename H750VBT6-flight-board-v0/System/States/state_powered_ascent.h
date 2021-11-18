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
		static constexpr double kTransitionResetTimeThreshold = 500; // 500 ms transition timer

		uint32_t transitionResetTimer;
		double maxAccelZ = 0;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_POWERED_ASCENT_H_ */

