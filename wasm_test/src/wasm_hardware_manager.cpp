#include "hardware_manager.h"

static uint32_t time = 0;

void setTime(uint32_t time_) {
    time = time_;
}

extern "C" {

uint32_t hm_millis() {
    return time;
}

void hm_radioUpdate() {}
void hm_watchdogRefresh() {}
void hm_hardwareInit() {}
bool hm_bleClientSend(int bleClientId, const uint8_t* data, uint16_t numBytes) { 
    return true;
}
bool hm_usbTransmit(int usbId, uint8_t *data, uint16_t numBytes) {
    return true;
}
bool hm_radioSend(int radioNum, uint8_t* data, uint16_t numBytes) {
    return true;
}

}