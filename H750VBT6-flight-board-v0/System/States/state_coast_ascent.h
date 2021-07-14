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

	private:
		// Amount of time required for z position to be below the maximum to detect apogee
		static constexpr uint32_t kTimeUnderApogeeThreshold = 1000; // ms

		double maxPosZ = 0;
		uint32_t maxPosZTimeHit = 0; // What system time was when the max Z position was hit in ms
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_COAST_ASCENT_H_ */

