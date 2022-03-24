#ifndef STATE_ASCENT_H_
#define STATE_ASCENT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "flight_state.h"

class AscentState : public FlightState {

	public:
		using FlightState::FlightState;

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

