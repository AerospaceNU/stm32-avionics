
#include "data_offload.h"
#include "usbd_cdc_if.h"
#include "data_log.h"
#include  "hardware_manager.h"

static uint32_t last_time = 0;
static bool readComplete_ = false;
static uint32_t flightID;
uint8_t flashBuf_[2049];

//Reads data of current flight ID and transmits it over USB
bool dataOffload(){

	flightID = data_log_get_last_flight_num();

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
