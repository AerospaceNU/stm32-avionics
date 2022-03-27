#ifndef SCHEDULER_H_
#define SCHEDULER_H_

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
    CliMain,
    CliOffload,
    CliSense,
    Ascent,
    DrogueDescentN,
    Initialize,
    MainDescent,
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

#endif /* SCHEDULER_H_ */
