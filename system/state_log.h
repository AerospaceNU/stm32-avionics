/*
 * state_log.h
 */

#ifndef SYSTEM_STATE_LOG_H_
#define SYSTEM_STATE_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

void state_log_reload_flight();

/**
 * @brief Writes an integer state id to the state log
 * @param currentState: int representation of the state id
 */
void state_log_write(int currentState);

/**
 * @brief mark that a flight has completed and no states should be resumed
 */
void state_log_write_complete();

/**
 * @brief reads the most recent state from the state log
 * @return int representing the state id
 */
int state_log_read();

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_STATE_LOG_H_
