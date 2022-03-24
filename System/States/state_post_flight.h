#ifndef STATE_POST_FLIGHT_H_
#define STATE_POST_FLIGHT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "flight_state.h"

class PostFlightState : public FlightState {

	public:
		using FlightState::FlightState;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;

	private:
		uint32_t lastSimDataTime = 0;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_POST_FLIGHT_H_ */

