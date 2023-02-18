/**
 * @file sim_timing.h
 * @brief Thread-safe simulated timing controls. Allows pausing and resuming
 * timing. Modeled off of WPILib's sim timing
 */

// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license in the root directory of this project.

#include <stdint.h>

namespace timing {

/**
 * Restart the simulator time.
 */
void RestartTiming();

/**
 * Pause the simulator time.
 */
void PauseTiming();

/**
 * Resume the simulator time.
 */
void ResumeTiming();

/**
 * Check if the simulator time is paused.
 *
 * @return true if paused
 */
bool IsTimingPaused();

/**
 * Advance the simulator time
 *
 * @param deltaMiros the timestep in uS
 */
void StepTimingAsync(uint64_t uS);

/**
 * @brief Advance time and await loop completion
 * 
 * @param deltsMicros 
 */
void StepTiming(uint64_t deltsMicros);


void SetTickCompleted(uint64_t tick);
uint64_t GetTickNumber();

uint64_t GetProgramTimeMicros();
uint64_t GetProgramTimeMillis();

}
