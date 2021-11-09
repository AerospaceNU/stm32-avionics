/*
 * state_log.h
 */

#ifndef STATE_LOG_H_
#define STATE_LOG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "filters.h"

void state_log_reload_flight();

void state_log_write(int currentState);

void state_log_write_complete();

int state_log_read();

#ifdef __cplusplus
}
#endif


#endif /* STATE_LOG_H_ */
