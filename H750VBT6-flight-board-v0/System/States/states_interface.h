#ifndef STATE_INTERFACE_H_
#define STATE_INTERFACE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <cstdint>

typedef enum {
	NoChange,
	Apogee,
	Launch,
	MainCutAltitude,
	MotorBurnout,
	Touchdown,
	UsbConnect,
	UsbDisconnect,
	CliCommandComplete
} EndCondition_t;

class State {

    public:
		State(int id, uint32_t period_ms): id_(id), period_ms_(period_ms) { }
		virtual ~State() = default;

		/**
		 * @brief Returns given ID of this state so higher level can track State pointers instead of IDs
		 * @return Given ID of state
		 */
		int getID() { return id_; }

		/**
		 * @brief Gets the current value of the run counter
		 * @return Run counter value
		 */
		uint32_t getRunCounter() { return run_counter_; };

		/**
		 * @brief Runs this state and increments the counter
		 */
		EndCondition_t run_state() {
			EndCondition_t result = run();
			run_counter_++;
			return result;
		};

		/**
		 * @brief Actions that occur on initialization of state
		 */
        virtual void init(void);

        /**
         * @brief Actions that occur at the end of
         */
        virtual void cleanup(void);

        /**
         * @brief Returns rate state should run as a period in MS
         */
        virtual uint32_t getPeriodMS(void) { return period_ms_; }

    private:
        /**
         * @brief Actions that occur on every loop of state
         * @return End condition of the current loop
         */
        virtual EndCondition_t run(void);

        int id_;
        int period_ms_;
        uint32_t run_counter_ = 0;
};

#ifdef __cplusplus
}
#endif

#endif /* STATE_INTERFACE_H_ */
