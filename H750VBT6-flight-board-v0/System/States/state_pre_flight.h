#ifndef STATE_PRE_FLIGHT_H_
#define STATE_PRE_FLIGHT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

#include "filters.h"
#include "hardware_manager.h"

class PreFlightState : public State {

	public:
		using State::State;
		using State::getID;
		using State::getPeriodMS;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;

	private:
		static constexpr int kBufferSize = 10;
		SensorData_t sensorDataBuffer[kBufferSize];
		FilterData_t filterDataBuffer[kBufferSize];
		int bufferCounter = 0;

		// TODO: What are vel and accel thresholds?
		static constexpr double kLaunchAccelThreshold = 0;
		static constexpr double kLaunchVelThreshold = 0;

};

#ifdef __cplusplus
}
#endif

#endif /* STATE_PRE_FLIGHT_H_ */

