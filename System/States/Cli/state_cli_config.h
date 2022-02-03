#ifndef STATE_CLI_CONFIG_H_
#define STATE_CLI_CONFIG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "states_interface.h"

class CliConfigState : public State {

	public:
		using State::State;
		using State::getID;
		using State::getPeriodMS;

		void init(void) override;

		EndCondition_t run(void) override;

		void cleanup(void) override;

		static constexpr int MAX_DROGUE_CUTS = 4;

		struct Configs_t {
			uint8_t drogueCuts; // Excludes initial separation and main descent
			double drogueCutAltitudesM[MAX_DROGUE_CUTS];
			double mainCutAltitudeM;
			double groundElevationM;
			double groundTemperatureC;
		};

		static Configs_t* getConfigs();
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_CLI_CONFIG_H */

