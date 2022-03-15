#ifndef STATE_ASCENT_H_
#define STATE_ASCENT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class AscentState : public State {

	public:
		using State::State;
		using State::getID;
		using State::getPeriodMS;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;

	private:

		double maxPosZ = 0;
		static constexpr double kPosDiffThreshold = 3;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_ASCENT_H_ */

