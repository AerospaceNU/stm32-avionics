/*
 * state_log.h
 */

#ifndef SYSTEM_TASKS_STATE_LOG_H_
#define SYSTEM_TASKS_STATE_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

void stateLog_reloadFlight();

/**
 * @brief Writes an integer state id to the state log
 * @param currentState: int representation of the state id
 */
void stateLog_write(int currentState);

/**
 * @brief mark that a flight has completed and no states should be resumed
 */
void stateLog_writeComplete();

/**
 * @brief reads the most recent state from the state log
 * @return int representing the state id
 */
int stateLog_read();

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_TASKS_STATE_LOG_H_
