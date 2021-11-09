/*
 * state_log.c
 */
#include "state_log.h"
#include "data_log.h"

static uint32_t stateWriteAddress = 0x08100000; // Static flash location for the state log
static uint32_t maxFlashAddr = 0x081FFFE0; // Highest write address available on internal flash
uint8_t curRead;
uint8_t prevRead;

/*
 * @brief Reinitializes needed parameters for an in-flight restart
 */
void state_log_reload_flight() {
	double groundPress = data_log_get_stored_ground_pressure(); // Get stored pressure from data log metadata

	data_log_assign_flight(); // Assign a new flight number for the restart flight

	filterSetPressureRef(groundPress); // Set the ground pressure to the recovered value
}

/*
 * @brief Writes an integer state id to the state log
 * @param currentState: int representation of the state id
 */
void state_log_write(int currentState) {
	uint8_t stateWriteBuffer[32]; // Buffer to store the 32 bytes that need to be written to the flash

	uint32_t writeAddress = stateWriteAddress;
	while (1) {
		curRead = *(uint8_t *)writeAddress; // Read from the address
		if (curRead == 255) { // Find the first empty address
			break;
		}
		writeAddress += 32; // Increase the write address by 32 bytes to account for the flash write size
		if (writeAddress > maxFlashAddr) {
			return; // Return if the address is too high
		}
	}

	// If we're at the last write address, write 0xEE to indicate
	// that no state should be resumed, because we can't be sure
	// that the state it would resume is correct
	if (writeAddress == maxFlashAddr) {
		stateWriteBuffer[0] = (uint8_t)(0xEE);
	} else {
		stateWriteBuffer[0] = (uint8_t)(currentState); // Otherwise write the state
	}

	for (int i = 1; i < 32; ++i) {stateWriteBuffer[i] = (uint8_t)0xFF;} // Fill the remaining bytes as 0xFF

	HAL_FLASH_Unlock();

	uint32_t stateDataAddress = (uint32_t)&stateWriteBuffer; // Get address of the start of the buffer

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, writeAddress, stateDataAddress); // Write to the flash

	HAL_FLASH_Lock();

}

void state_log_write_complete() {
	state_log_write(0xEE); // Write 0xEE to the flash to indicate that the flight ended and nothing should be resumed
}

/*
 * @brief reads the most recent state from the state log
 * @return int representing the state id
 */
int state_log_read() {
	uint32_t readAddress = stateWriteAddress;
	prevRead = *(uint8_t *)readAddress;

	if (prevRead == 255) { // If we immediately read 255, the state log is empty, so return -1 for no state to resume
		return -1;
	}

	while (1) {
		curRead = *(uint8_t *)readAddress;
		readAddress += 32;

		// If we reach an empty state or are going to read past the last address
		if (curRead == 255 || readAddress > maxFlashAddr) {

			if (curRead == 0xEE) { // Nothing to resume if the last data is 0xEE
				return -1;
			}
			return prevRead; // Otherwise return the state id to resume
		}
		prevRead = curRead;
	}
}
