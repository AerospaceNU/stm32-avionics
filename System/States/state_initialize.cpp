
#include "state_initialize.h"

#include "hardware_manager.h"
#include "filters.h"

void InitializeState::init() {
	HM_HardwareInit();

	// This is needed to tell the Kalman filter the nominal dt for prediction
	filterInit(this->period_ms_ / 1000.0);
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
