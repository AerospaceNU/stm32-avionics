#ifndef COMMON_SYSTEM_TASKS_BUZZER_HEARTBEAT_H_
#define COMMON_SYSTEM_TASKS_BUZZER_HEARTBEAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hardware_manager.h"
enum class BuzzerState { PYRO };

void buzzerHeartbeat_setBuzzer(int buzzerId);
// update the buzzer state machine
void buzzerHeartbeat_tick(BuzzerState state);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_SYSTEM_TASKS_BUZZER_HEARTBEAT_H_
