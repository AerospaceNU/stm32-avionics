/*
 * internal_flash.c
 */

#include "internal_flash.h"

/*
 * Writes to the internal flash of the H750VBT6
 * @param RelFlashAddress - Relative address to write to, where 0 means the defined start of writable internal flash
 * @param data - Array of uint8_t to write
 * @param numBytes - Number of bytes to be written from data
 */
bool internal_flash_write(uint32_t RelFlashAddress, uint8_t *data, uint32_t numBytes) {

	if (RelFlashAddress % 32) { //Relative address must be a multiple of 32
		return false;
	};

	uint32_t writeAddress = FLASH_START_ADDRESS + RelFlashAddress;

	if (writeAddress + numBytes > FLASH_END_ADDRESS) {
		return false;
	}

	uint8_t flashWriteBuffer[32]; //Flash writes only in increments of 32 bytes

	HAL_FLASH_Unlock();
	uint32_t sofar = 0;
	uint32_t index;
	while (sofar < numBytes) {
		index = 0;
		while (index + sofar < numBytes && index < 32) {
			flashWriteBuffer[index] = data[index + sofar];
			++index;
		}
		sofar += index;
		while (index < 32) {
			flashWriteBuffer[index++] = 0xFF; //Fill remaining bytes with FF
		}

		HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, writeAddress, (uint32_t)&flashWriteBuffer); // Write to the flash
		writeAddress += 32;
	}
	HAL_FLASH_Lock();
	return true;
}

/*
 * Reads from the internal flash of the H750VBT6
 * @param RelFlashAddress - Relative address to read from, where 0 means the defined start of readable internal flash
 * @param pData - Array of uint8_t to read into
 * @param numBytes - Number of bytes to be read from flash
 */
bool internal_flash_read(uint32_t RelFlashAddress, uint8_t *pData, uint32_t numBytes) {
	uint32_t readAddress = FLASH_START_ADDRESS + RelFlashAddress;
	if (readAddress + numBytes > FLASH_END_ADDRESS) {
		return false;
	}

	uint32_t sofar = 0;
	while (sofar < numBytes) {
		pData[sofar++] = *(uint8_t *)readAddress++;
	}

	return true;
}
