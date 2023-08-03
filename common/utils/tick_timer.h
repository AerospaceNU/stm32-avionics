#pragma once

#include <stdint.h>
#include "hardware_manager.h"

/**
 * Helper class for keeping track of time between events
*/
class Timer {
private:
    uint32_t m_lastTickMs;
public:
    Timer() : m_lastTickMs(hm_millis()) {}

    /**
     * Number of ms since the last call to reset();
    */
    uint32_t getTicksElapsed() {
        auto now = hm_millis();
        auto dt = (now - m_lastTickMs);
        return dt;
    }

    /**
     * Reset the timer to zero
    */
    void reset() {
        m_lastTickMs = hm_millis();
    }

    /**
     * Get the dt since the last call, and reset the timer to zero
    */
    uint32_t getTicksAndReset() {
        auto now = hm_millis();
        auto dt = (now - m_lastTickMs);
        m_lastTickMs = now;
        return dt;
    }
}
