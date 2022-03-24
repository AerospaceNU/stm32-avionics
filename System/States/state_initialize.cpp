
#include "state_initialize.h"

#include "hardware_manager.h"
#include "filters.h"
#include "radio_manager.h"

void InitializeState::init() {
	HM_HardwareInit();

	// This is needed to tell the Kalman filter the nominal dt for prediction
	filterInit(this->period_ms_ / 1000.0);

	// Initiliaze radio circular buffers and things
	RadioManager_init();
}

EndCondition_t InitializeState::run() {
	if (HM_UsbIsConnected()) {
		return EndCondition_t::UsbConnect;
	}
	return EndCondition_t::UsbDisconnect;
}

void InitializeState::cleanup() {
	// Do nothing
}
