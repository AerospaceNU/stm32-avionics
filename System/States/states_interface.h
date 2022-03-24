#ifndef STATE_INTERFACE_H_
#define STATE_INTERFACE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <cstdint>

typedef enum {
	NoChange,
	Apogee,
	CalibrateCommand,
	CliCommandComplete,
	ConfigCommand,
	EraseFlashCommand,
	HelpCommand,
	Launch,
	MainCutAltitude,
	MotorBurnout,
	OffloadCommand,
	SenseCommand,
	SimCommand,
	ShutdownCommand,
	Touchdown,
	UsbConnect,
	UsbDisconnect,
} EndCondition_t;

class State {

    public:
		State(int id, uint32_t period_ms): id_(id), period_ms_(period_ms) { }
		~State() = default;

		/**
		 * @brief Returns given ID of this state so higher level can track State pointers instead of IDs
		 * @return Given ID of state
		 */
		int getID() { return id_; }

		/**
		 * @brief Gets the current value of the run counter
		 * @return Run counter value
		 */
		uint32_t getRunCounter() { return run_counter_; }

		/**
		 * @brief Runs this state and increments the counter
		 */
		virtual EndCondition_t run_state() {
			EndCondition_t result = run();
			run_counter_++;
			return result;
		};

		/**
		 * @brief Actions that occur on initialization of state
		 */
        virtual void init(void) = 0;

        /**
         * @brief Actions that occur at the end of
         */
        virtual void cleanup(void) = 0;

        /**
         * @brief Returns rate state should run as a period in MS
         */
        uint32_t getPeriodMS(void) { return period_ms_; }



    private:

        /**
         * @brief Actions that occur on every loop of state
         * @return End condition of the current loop
         */
        virtual EndCondition_t run(void) = 0;

        int id_;
        uint32_t run_counter_ = 0;

    protected:
        int period_ms_;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_INTERFACE_H_ */
