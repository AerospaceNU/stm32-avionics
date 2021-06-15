
#include "data_offload.h"
#include "usbd_cdc_if.h"
#include "data_log.h"
#include  "hardware_manager.h"

static bool readComplete_ = false;
static uint32_t flightID = 0;
uint8_t flashBuf_[2049];

//Reads data of current flight ID and transmits it over USB
bool dataOffload(){

	if (!readComplete_) {
			uint32_t bytesRead = data_log_read(flightID, 2048, flashBuf_);
			if (bytesRead == 0) {
				readComplete_ = true;
			}
			else {
				CDC_Transmit_FS(flashBuf_, bytesRead);
			}
		}
		return readComplete_;
}

void userSetFlightID(int userFlightID){
	flightID = userFlightID;
	readComplete_ = false;
}
