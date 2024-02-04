#pragma once

#include "hardware_manager.h"

/**
 * @brief Simple timer convienence wrapper around a timer. Counts up from
 * construction time (or when reset), and provides an easy way to tell if the
 * period has expired or how long has elapsed.
 */
class HmTimer {
 private:
  uint32_t periodMs;
  uint32_t startTime;

 public:
  /**
   * @brief Construct a HmTimer, and reset the start time to now
   *
   * @param period_ How long from when the timer has started to when it is
   * "expired", in ms
   */
  explicit HmTimer(uint32_t period = 0) : periodMs(period) { Reset(); }

  /**
   * Reset the start time of the timer. Affects expired/elapsed
   */
  void Reset() { startTime = hm_millis(); }

  /**
   * Check if at least our period has elapsed since last reset
   */
  inline const bool Expired() const {
    return (hm_millis() - startTime) > periodMs;
  }

  /**
   * Check if at least our period has elapsed since last reset
   */
  inline const bool Expired(uint32_t timestamp) const {
    return (timestamp - startTime) > periodMs;
  }

  /**
   * How long it's been in ms since the timer was last reset/constructed
   */
  inline const uint32_t Elapsed() const { return (hm_millis() - startTime); }
};

/**
 * @brief Same as HmTimer, but with a constructor given frequency instead of
 * period.
 */
class HmTickTimer : public HmTimer {
 public:
  explicit HmTickTimer(unsigned int frequencyHz) : HmTimer(1.0 / frequencyHz) {}
};
