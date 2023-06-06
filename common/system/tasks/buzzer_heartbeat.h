
#ifndef COMMON_SYSTEM_TASKS_BUZZER_HEARTBEAT_H_
#define COMMON_SYSTEM_TASKS_BUZZER_HEARTBEAT_H_

#ifdef __cplusplus
extern "C" {
#endif

void buzzerHeartbeat_setBuzzer(int buzzerId);
void buzzerHeartbeat_tick();

#ifdef __cplusplus
}
#endif

#endif  // COMMON_SYSTEM_TASKS_BUZZER_HEARTBEAT_H_
