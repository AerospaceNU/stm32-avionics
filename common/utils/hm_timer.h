#pragma once

#include "hardware_manager.h"

/**
 * @brief Simple timer convienenve wrapper around a timer
 * 
 */
class HmTimer {
private:
    uint32_t periodMs;
    uint32_t startTime;

public:
    /**
     * @brief Construct a HmTimer, and reset the start time to now
     * 
     * @param period_ How long from when the timer has started to when it is "expired", in ms
     */
    HmTimer(uint32_t period_ = 0) : periodMs(period_) {
        Reset();
    }

    void Reset() { startTime = hm_millis(); }

    inline const bool Expired() const { return (hm_millis() - startTime) > periodMs; }
    inline const bool Expired(uint32_t timestamp) const { return (timestamp - startTime) > periodMs; }
    inline const uint32_t Elapsed() const { return (hm_millis() - startTime); }
};

class HmTickTimer : public HmTimer {
public:
    HmTickTimer(unsigned int frequencyHz) : HmTimer(1.0 / frequencyHz) {}
};
