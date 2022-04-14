#ifndef SYSTEM_SCHEDULER_H_
#define SYSTEM_SCHEDULER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "states_interface.h"

class Scheduler {
 public:
  Scheduler() = default;
  void run(void);

 private:
  typedef enum StateId {
    CliCalibrate = 0,
    CliConfig,
    CliEraseFlash,
    CliHelp,
    CliPyroFire,
    CliMain,
    CliOffload,
    CliSense,
    Ascent,
    Initialize,
    Descent,
    PostFlight,
    PreFlight,
    Shutdown,
    NUM_STATES,
    UNKNOWN
  } StateId;

  StateId getNextState(EndCondition_t endCondition);

  State* pCurrentState_ = nullptr;
};

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_SCHEDULER_H_
