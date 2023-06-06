
#ifndef COMMON_SYSTEM_TASKS_DATA_OFFLOAD_H_
#define COMMON_SYSTEM_TASKS_DATA_OFFLOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Offloads the next batch of data
 * @return Whether offload has completed (true) or is still ongoing (false)
 */
bool dataOffload_tick();

/**
 * @brief Set flight ID to offload
 * @param flightId: ID of flight to offload
 */
void dataOffload_setFlightId(uint32_t flightId);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_SYSTEM_TASKS_DATA_OFFLOAD_H_
