/*
 * hal_callbacks.c
 */

#include <stdint.h>
#include "hal_callbacks.h"

#define MAX_SPI_HANDLES	6	// SPI 1-6
#define MAX_ADC_HANDLES 3   // ADC 1-3
#define MAX_UART_HANDLES 5  // UART 1-5

#ifdef HAS_SPI
typedef struct {
	SPI_HandleTypeDef *hspi;
	void (*txCallback)(void *);
	void (*txRxCallback)(void *);
	void *txCallbackUserData;
	void *txRxCallbackUserData;
} SPICallbackProperty_t;

static SPICallbackProperty_t spiCallbacks[MAX_SPI_HANDLES];
static int numSpiCallbacksRegistered = 0;

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

//The actuall callback functions
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
#endif

#ifdef HAS_UART
typedef struct {
	UART_HandleTypeDef *huart;
	void (*rxHalfCallback)(void *);
	void (*rxCallback)(void *);
	void *rxHalfCallbackUserData;
	void *rxCallbackUserData;
} UARTCallbackProperty_t;

static UARTCallbackProperty_t uartCallbacks[MAX_UART_HANDLES];
static int numUartCallbacksRegistered = 0;


void register_HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart, void (*callback)(void *), void *userData) {

	// See if handle already has callback registered to it
	for (int i = 0; i < numUartCallbacksRegistered; i++) {
		if (uartCallbacks[i].huart == huart) {
			uartCallbacks[i].rxHalfCallback = callback;
			uartCallbacks[i].rxHalfCallbackUserData = userData;
			return; // No need to keep going if handle already found to be registered
		}
	}

	// If reached, handle hasn't been registered, so create a new association
	uartCallbacks[numUartCallbacksRegistered].huart = huart;
	uartCallbacks[numUartCallbacksRegistered].rxHalfCallback = callback;
	uartCallbacks[numUartCallbacksRegistered].rxHalfCallbackUserData = userData;
	numUartCallbacksRegistered++;
}

void register_HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart, void (*callback)(void *), void *userData) {

	// See if handle already has callback registered to it
	for (int i = 0; i < numUartCallbacksRegistered; i++) {
		if (uartCallbacks[i].huart == huart) {
			uartCallbacks[i].rxCallback = callback;
			uartCallbacks[i].rxCallbackUserData = userData;
			return; // No need to keep going if handle already found to be registered
		}
	}

	// If reached, handle hasn't been registered, so create a new association
	uartCallbacks[numUartCallbacksRegistered].huart = huart;
	uartCallbacks[numUartCallbacksRegistered].rxCallback = callback;
	uartCallbacks[numUartCallbacksRegistered].rxCallbackUserData = userData;
	numUartCallbacksRegistered++;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	for (int i = 0; i < numUartCallbacksRegistered; i++) {
		if (uartCallbacks[i].huart == huart) {
			if (uartCallbacks[i].rxHalfCallback != NULL) {
				uartCallbacks[i].rxHalfCallback(
						uartCallbacks[i].rxHalfCallbackUserData);
			}
			return; // No need to keep searching if callback was found
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	for (int i = 0; i < numUartCallbacksRegistered; i++) {
		if (uartCallbacks[i].huart == huart) {
			if (uartCallbacks[i].rxCallback != NULL) {
				uartCallbacks[i].rxCallback(
						uartCallbacks[i].rxCallbackUserData);
			}
			return; // No need to keep searching if callback was found
		}
	}
}

#endif


#ifdef HAS_ADC
typedef struct {
	ADC_HandleTypeDef *hadc;
	void (*convCallback)(void *);
	void *convCallbackUserData;
} ADCCallbackProperty_t;

static ADCCallbackProperty_t adcCallbacks[MAX_ADC_HANDLES];
static int numAdcCallbacksRegistered = 0;

void register_HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc, void(*callback)(void *), void *userData) {

	// See if handle already has callback registered to it
	for (int i = 0; i < numAdcCallbacksRegistered; i++) {
		if (adcCallbacks[i].hadc == hadc) {
			adcCallbacks[i].convCallback = callback;
			adcCallbacks[i].convCallbackUserData = userData;
			return; // No need to keep going if handle already found to be registered
		}
	}

	// If reached, handle hasn't been registered, so create a new association
	adcCallbacks[numAdcCallbacksRegistered].hadc = hadc;
	adcCallbacks[numAdcCallbacksRegistered].convCallback = callback;
	adcCallbacks[numAdcCallbacksRegistered].convCallbackUserData = userData;
	numAdcCallbacksRegistered++;
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {

	for (int i = 0; i < numAdcCallbacksRegistered; i++) {
		if (adcCallbacks[i].hadc == hadc) {
			if (adcCallbacks[i].convCallback != NULL) {
				adcCallbacks[i].convCallback(adcCallbacks[i].convCallbackUserData);
			}
			return; // No need to keep searching if callback was found
		}
	}
}

#endif

