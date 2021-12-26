/*
 * internal_flash.c
 */

#include "internal_flash.h"
#include "stm32h7xx_hal.h"
#include <string.h>

#define FLASH_TIMEOUT_VALUE              50000U /* 50 s */

/**
  * @brief  Duplicate of HAL_FLASH_Program but without checking for a valid flash address
  * @param  TypeProgram Indicate the way to program at a specified address.
  *         This parameter can be a value of @ref FLASH_Type_Program
  * @param  FlashAddress specifies the address to be programmed.
  *         This parameter shall be aligned to the Flash word:
  *          - 256 bits for STM32H74x/5X devices (8x 32bits words)
  *          - 128 bits for STM32H7Ax/BX devices (4x 32bits words)
  *          - 256 bits for STM32H72x/3X devices (8x 32bits words)
  * @param  DataAddress specifies the address of data to be programmed.
  *         This parameter shall be 32-bit aligned
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef Internal_Flash_Program(uint32_t TypeProgram, uint32_t FlashAddress, uint32_t DataAddress)
{
	#if (FCB_VERSION == 1)
	return HAL_FLASH_Program(TypeProgram, FlashAddress, DataAddress);
	#else
	HAL_StatusTypeDef status;
	__IO uint32_t *dest_addr = (__IO uint32_t *)FlashAddress;
	__IO uint32_t *src_addr = (__IO uint32_t*)DataAddress;
	uint32_t bank;
	uint8_t row_index = FLASH_NB_32BITWORD_IN_FLASHWORD;

	/* Check the parameters */
	assert_param(IS_FLASH_TYPEPROGRAM(TypeProgram));
	// assert_param(IS_FLASH_PROGRAM_ADDRESS(FlashAddress));

	/* Process Locked */
	__HAL_LOCK(&pFlash);

	bank = FLASH_BANK_1;

	/* Reset error code */
	pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE, bank);


	SET_BIT(FLASH->CR1, FLASH_CR_PG);

	__ISB();
	__DSB();


	/* Program the flash word */
	do
	{
	*dest_addr = *src_addr;
	dest_addr++;
	src_addr++;
	row_index--;
	} while (row_index != 0U);


	__ISB();
	__DSB();

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE, bank);


		/* If the program operation is completed, disable the PG */
	CLEAR_BIT(FLASH->CR1, FLASH_CR_PG);


	/* Process Unlocked */
	__HAL_UNLOCK(&pFlash);

	return status;
	#endif
}

/*
 * Writes to the internal flash of the H750VBT6
 * @param RelFlashAddress - Relative address to write to, where 0 means the defined start of writable internal flash
 * @param data - Array of uint8_t to write
 * @param numBytes - Number of bytes to be written from data
 */
bool internal_flash_write(uint32_t RelFlashAddress, uint8_t *data, uint32_t numBytes) {
	if (RelFlashAddress % 32 || RelFlashAddress > MAX_FLASH_ADDRESS) { //Relative address must be a multiple of 32 and within bounds
		return false;
	};

	uint32_t writeAddress = INTERNAL_FLASH_START + RelFlashAddress;

	uint8_t flashWriteBuffer[32]; //Flash writes only in increments of 32 bytes

	HAL_FLASH_Unlock();
	uint32_t sofar = 0;
	uint32_t copyBytes;
	while (sofar < numBytes) {
		copyBytes = (numBytes - sofar < 32) ? numBytes - sofar : 32; // Copy either 32 bytes, or less if we don't need to copy that many
		memcpy(flashWriteBuffer, data + sofar, copyBytes);

		memset(flashWriteBuffer + copyBytes, 0xFF, 32 - copyBytes); // Set any remaining bytes to 0xFF

		sofar += copyBytes;

		Internal_Flash_Program(FLASH_TYPEPROGRAM_FLASHWORD, writeAddress, (uint32_t)&flashWriteBuffer); // Write to the flash
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
	if (RelFlashAddress > MAX_FLASH_ADDRESS) {
		return false;
	}

	uint32_t readAddress = INTERNAL_FLASH_START + RelFlashAddress;

	memcpy(pData, (uint8_t *)readAddress, numBytes);

	return true;
}
