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
		static constexpr int kBufferSize = 7;
		SensorData_t sensorDataBuffer[kBufferSize];
		FilterData_t filterDataBuffer[kBufferSize];
		int bufferCounter = 0;

		static constexpr double kLaunchAccelThreshold = 20.0; // m/s**2
		static constexpr double kLaunchPosZDiffThreshold = 1.0; // m
		static constexpr uint32_t kTransitionResetTimeThreshold = 500; // 500 ms transition timer

		uint32_t transitionResetTimer;
		double minPosZ = 1000000;

		bool gpsTimestamp;

};

#ifdef __cplusplus
}
#endif

#endif /* STATE_PRE_FLIGHT_H_ */

