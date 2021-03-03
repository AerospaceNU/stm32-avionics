
#include "state_initialize.h"

#include "hardware_manager.h"

void InitializeState::init() {
	HM_HardwareInit();
}

EndCondition_t InitializeState::run() {
	if (HM_UsbIsConnected()) {
		return EndCondition_t::UsbConnect;
	}
	return EndCondition_t::NoChange;
}

void InitializeState::cleanup() {
	// Do nothing
}
