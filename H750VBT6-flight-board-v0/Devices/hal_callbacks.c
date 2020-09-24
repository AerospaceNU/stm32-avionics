/*
 * hal_callbacks.c
 */

#include <stdint.h>
#include "hal_callbacks.h"

#define MAX_SPI_HANDLES	6	// SPI 1-6

typedef struct {
	SPI_HandleTypeDef *hspi;
	void (*txCallback)(void *);
	void (*txRxCallback)(void *);
	void *txCallbackUserData;
	void *txRxCallbackUserData;
} SPICallbackProperty_t;

static SPICallbackProperty_t spiCallbacks[MAX_SPI_HANDLES];
static uint8_t numSpiCallbacksRegistered = 0;

void register_HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi, void (*callback)(void *), void *userData) {

	// See if handle already has callback registered to it
	for (int i = 0; i < numSpiCallbacksRegistered; i++) {
		if (spiCallbacks[i].hspi == hspi) {
			spiCallbacks[i].txCallback = callback;
			spiCallbacks[i].txCallbackUserData = userData;
			return; // No need to keep going if handle already found to be registered
		}
	}

	// If reached, handle hasn't been registered, so create a new association
	spiCallbacks[numSpiCallbacksRegistered].hspi = hspi;
	spiCallbacks[numSpiCallbacksRegistered].txCallback = callback;
	spiCallbacks[numSpiCallbacksRegistered].txCallbackUserData = userData;
	numSpiCallbacksRegistered++;
}

void register_HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi, void (*callback)(void *), void *userData) {

	// See if handle already has callback registered to it
	for (int i = 0; i < numSpiCallbacksRegistered; i++) {
		if (spiCallbacks[i].hspi == hspi) {
			spiCallbacks[i].txRxCallback = callback;
			spiCallbacks[i].txRxCallbackUserData = userData;
			return; // No need to keep going if handle already found to be registered
		}
	}

	// If reached, handle hasn't been registered, so create a new association
	spiCallbacks[numSpiCallbacksRegistered].hspi = hspi;
	spiCallbacks[numSpiCallbacksRegistered].txRxCallback = callback;
	spiCallbacks[numSpiCallbacksRegistered].txRxCallbackUserData = userData;
	numSpiCallbacksRegistered++;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {

	for (int i = 0; i < numSpiCallbacksRegistered; i++) {
		if (spiCallbacks[i].hspi == hspi) {
			if (spiCallbacks[i].txCallback != NULL) {
				spiCallbacks[i].txCallback(spiCallbacks[i].txCallbackUserData);
			}
			return; // No need to keep searching if callback was found
		}
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

	for (int i = 0; i < numSpiCallbacksRegistered; i++) {
		if (spiCallbacks[i].hspi == hspi) {
			if (spiCallbacks[i].txRxCallback != NULL) {
				spiCallbacks[i].txRxCallback(spiCallbacks[i].txRxCallbackUserData);
			}
			return; // No need to keep searching if callback was found
		}
	}
}




