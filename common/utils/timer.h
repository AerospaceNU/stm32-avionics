/*
 * timer.h
 *
 *  Created on: Jun 4, 2023
 *      Author: matt
 */

#ifndef UTILS_TIMER_H_
#define UTILS_TIMER_H_

#include "hardware_manager.h"

class Timer {
 public:
  Timer() { reset(); }

  inline void reset() { m_start = hm_millis(); }

  inline uint32_t elapsed_ms() const { return hm_millis() - m_start; }

 private:
  uint32_t m_start;
};

#endif  // UTILS_TIMER_H_