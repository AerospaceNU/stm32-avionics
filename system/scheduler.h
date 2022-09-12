#ifndef SYSTEM_SCHEDULER_H_
#define SYSTEM_SCHEDULER_H_

#include "states_interface.h"

class Scheduler {
 public:
  Scheduler() = default;
  void run(void);

 private:
  typedef enum {
    CliEraseFlash = 0,
    CliOffload,
    Ascent,
    Descent,
    Initialize,
    PostFlight,
    PreFlight,
    SimTempState,
    NUM_STATES,
    UNKNOWN
  } StateId_e;

  StateId_e getNextState(EndCondition_e endCondition);

  State* pCurrentState_ = nullptr;
};

#endif  // SYSTEM_SCHEDULER_H_
