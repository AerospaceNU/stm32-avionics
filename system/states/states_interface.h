#ifndef SYSTEM_STATES_STATES_INTERFACE_H_
#define SYSTEM_STATES_STATES_INTERFACE_H_

#include <stdint.h>

#include "hardware_manager.h"

typedef enum {
  NoChange,
  Apogee,
  CliCommandComplete,
  EraseFlashCommand,
  Initialized,
  Launch,
  NewFlightCommand,
  OffloadCommand,
  SimCommand,
  Touchdown,
} EndCondition_e;

class State {
 public:
  State(int id, uint32_t period_ms) : id_(id), period_ms_(period_ms) {}
  virtual ~State() {}

  /**
   * @brief Returns given ID of this state so higher level can track State
   * pointers instead of IDs
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
  virtual EndCondition_e run_state();

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
  virtual EndCondition_e run(void) = 0;

  int id_;
  uint32_t run_counter_ = 0;

 protected:
  int period_ms_;
};

#endif  // SYSTEM_STATES_STATES_INTERFACE_H_
