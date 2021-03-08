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
		static constexpr int kMotorBurnoutJerkThreshold = 0; // TODO: Find this value
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_POWERED_ASCENT_H_ */

