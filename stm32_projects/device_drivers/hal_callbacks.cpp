/*
 * hal_callbacks.c
 */

#include "hal_callbacks.h"

#include <stdint.h>

#define MAX_SPI_HANDLES 6   // SPI 1-6
#define MAX_ADC_HANDLES 3   // ADC 1-3
#define MAX_UART_HANDLES 5  // UART 1-5
#define MAX_EXTI_HANDLES 6
#define MAX_TIM_HANDLES 8  // TIM 1-8

#ifdef HAL_SPI_MODULE_ENABLED
typedef struct {
  SPI_HandleTypeDef *hspi;
  void (*txCallback)(void *);
  void (*txRxCallback)(void *);
  void *txCallbackUserData;
  void *txRxCallbackUserData;
} SpiCallbackProperty_s;

static SpiCallbackProperty_s spiCallbacks[MAX_SPI_HANDLES];
static int numSpiCallbacksRegistered = 0;

void halCallbacks_registerSpiTxCpltCallback(SPI_HandleTypeDef *hspi,
                                            void (*callback)(void *),
                                            void *userData) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numSpiCallbacksRegistered; i++) {
    if (spiCallbacks[i].hspi == hspi) {
      spiCallbacks[i].txCallback = callback;
      spiCallbacks[i].txCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
    }
  }

  // If reached, handle hasn't been registered, so create a new association
  spiCallbacks[numSpiCallbacksRegistered].hspi = hspi;
  spiCallbacks[numSpiCallbacksRegistered].txCallback = callback;
  spiCallbacks[numSpiCallbacksRegistered].txCallbackUserData = userData;
  numSpiCallbacksRegistered++;
}

void halCallbacks_registerSpiTxRxCpltCallback(SPI_HandleTypeDef *hspi,
                                              void (*callback)(void *),
                                              void *userData) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numSpiCallbacksRegistered; i++) {
    if (spiCallbacks[i].hspi == hspi) {
      spiCallbacks[i].txRxCallback = callback;
      spiCallbacks[i].txRxCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
    }
  }

  // If reached, handle hasn't been registered, so create a new association
  spiCallbacks[numSpiCallbacksRegistered].hspi = hspi;
  spiCallbacks[numSpiCallbacksRegistered].txRxCallback = callback;
  spiCallbacks[numSpiCallbacksRegistered].txRxCallbackUserData = userData;
  numSpiCallbacksRegistered++;
}

// The actuall callback functions
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  for (int i = 0; i < numSpiCallbacksRegistered; i++) {
    if (spiCallbacks[i].hspi == hspi) {
      if (spiCallbacks[i].txCallback != NULL) {
        spiCallbacks[i].txCallback(spiCallbacks[i].txCallbackUserData);
      }
      return;  // No need to keep searching if callback was found
    }
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  for (int i = 0; i < numSpiCallbacksRegistered; i++) {
    if (spiCallbacks[i].hspi == hspi) {
      if (spiCallbacks[i].txRxCallback != NULL) {
        spiCallbacks[i].txRxCallback(spiCallbacks[i].txRxCallbackUserData);
      }
      return;  // No need to keep searching if callback was found
    }
  }
}
#endif  // HAL_SPI_MODULE_ENABLED

#ifdef HAL_UART_MODULE_ENABLED
typedef struct {
  UART_HandleTypeDef *huart;
  void (*rxHalfCallback)(void *);
  void (*rxCallback)(void *);
  void (*rxIdleCallback)(void *, size_t);  // user data, size
  void *rxHalfCallbackUserData;
  void *rxCallbackUserData;
  void *rxIdleCallbackUserData;
} UartCallbackProperty_s;

static UartCallbackProperty_s uartCallbacks[MAX_UART_HANDLES];
static int numUartCallbacksRegistered = 0;

void halCallbacks_registerUartRxHalfCpltCallback(UART_HandleTypeDef *huart,
                                                 void (*callback)(void *),
                                                 void *userData) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      uartCallbacks[i].rxHalfCallback = callback;
      uartCallbacks[i].rxHalfCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
    }
  }

  // If reached, handle hasn't been registered, so create a new association
  uartCallbacks[numUartCallbacksRegistered].huart = huart;
  uartCallbacks[numUartCallbacksRegistered].rxHalfCallback = callback;
  uartCallbacks[numUartCallbacksRegistered].rxHalfCallbackUserData = userData;
  numUartCallbacksRegistered++;
}

void halCallbacks_registerUartRxCpltCallback(UART_HandleTypeDef *huart,
                                             void (*callback)(void *),
                                             void *userData) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      uartCallbacks[i].rxCallback = callback;
      uartCallbacks[i].rxCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
    }
  }

  // If reached, handle hasn't been registered, so create a new association
  uartCallbacks[numUartCallbacksRegistered].huart = huart;
  uartCallbacks[numUartCallbacksRegistered].rxCallback = callback;
  uartCallbacks[numUartCallbacksRegistered].rxCallbackUserData = userData;
  numUartCallbacksRegistered++;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      if (uartCallbacks[i].rxHalfCallback != NULL) {
        uartCallbacks[i].rxHalfCallback(
            uartCallbacks[i].rxHalfCallbackUserData);
      }
      return;  // No need to keep searching if callback was found
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      if (uartCallbacks[i].rxCallback != NULL) {
        uartCallbacks[i].rxCallback(uartCallbacks[i].rxCallbackUserData);
      }
      return;  // No need to keep searching if callback was found
    }
  }
}

void halCallbacks_registerUartRxIdleCallback(UART_HandleTypeDef *huart,
                                             void (*callback)(void *, size_t),
                                             void *userData) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      uartCallbacks[i].rxIdleCallback = callback;
      uartCallbacks[i].rxIdleCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
    }
  }

  // If reached, handle hasn't been registered, so create a new association
  uartCallbacks[numUartCallbacksRegistered].huart = huart;
  uartCallbacks[numUartCallbacksRegistered].rxIdleCallback = callback;
  uartCallbacks[numUartCallbacksRegistered].rxIdleCallbackUserData = userData;
  numUartCallbacksRegistered++;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      if (uartCallbacks[i].rxIdleCallback != NULL) {
        uartCallbacks[i].rxIdleCallback(uartCallbacks[i].rxIdleCallbackUserData,
                                        size);
      }
      return;  // No need to keep searching if callback was found
    }
  }
}

#endif  // HAL_UART_MODULE_ENABLED

#ifdef HAL_ADC_MODULE_ENABLED
typedef struct {
  ADC_HandleTypeDef *hadc;
  void (*convCallback)(void *);
  void *convCallbackUserData;
} AdcCallbackProperty_s;

static AdcCallbackProperty_s adcCallbacks[MAX_ADC_HANDLES];
static int numAdcCallbacksRegistered = 0;

void halCallbacks_registerAdcConvCpltCallback(ADC_HandleTypeDef *hadc,
                                              void (*callback)(void *),
                                              void *userData) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numAdcCallbacksRegistered; i++) {
    if (adcCallbacks[i].hadc == hadc) {
      adcCallbacks[i].convCallback = callback;
      adcCallbacks[i].convCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
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
      return;  // No need to keep searching if callback was found
    }
  }
}

#endif  // HAL_ADC_MODULE_ENABLED

#ifdef HAL_EXTI_MODULE_ENABLED
typedef struct {
  uint16_t extInterruptGpioPin;
  void (*extInterruptCallback)(void *);
  void *extInterruptCallbackUserData;
} ExtInterruptCallbackProperty_s;

static ExtInterruptCallbackProperty_s extInterruptCallbacks[MAX_EXTI_HANDLES];
static int numExtInterruptCallbacksRegistered = 0;

void halCallbacks_registerExtInterruptCallback(uint16_t gpioPin,
                                               void (*callback)(void *),
                                               void *userData) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numExtInterruptCallbacksRegistered; i++) {
    if (extInterruptCallbacks[i].extInterruptGpioPin == gpioPin) {
      extInterruptCallbacks[i].extInterruptCallback = callback;
      extInterruptCallbacks[i].extInterruptCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
    }
  }

  // If reached, handle hasn't been registered, so create a new association
  extInterruptCallbacks[numExtInterruptCallbacksRegistered]
      .extInterruptGpioPin = gpioPin;
  extInterruptCallbacks[numExtInterruptCallbacksRegistered]
      .extInterruptCallback = callback;
  extInterruptCallbacks[numExtInterruptCallbacksRegistered]
      .extInterruptCallbackUserData = userData;
  numExtInterruptCallbacksRegistered++;
}

// The actual callback function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  for (int i = 0; i < numExtInterruptCallbacksRegistered; i++) {
    if (extInterruptCallbacks[i].extInterruptGpioPin == GPIO_Pin) {
      if (extInterruptCallbacks[i].extInterruptCallback != NULL) {
        extInterruptCallbacks[i].extInterruptCallback(
            extInterruptCallbacks[i].extInterruptCallbackUserData);
      }
    }
  }
}
#endif  // HAL_EXTI_MODULE_ENABLED

#ifdef HAL_TIM_MODULE_ENABLED

typedef struct {
  TIM_HandleTypeDef *htim;
  void (*irqCallback)(void *);
  void *irqCallbackUserData;
} TimPeriodElapsedCallbackProperty_s;

static TimPeriodElapsedCallbackProperty_s
    timPeriodElapsedCallbacks[MAX_TIM_HANDLES];
static int numTimPeriodElapsedCallbacksRegistered = 0;

void halCallbacks_registerTimPeriodElapsedCallback(TIM_HandleTypeDef *htim,
                                                   void (*callback)(void *),
                                                   void *userData) {
  for (int i = 0; i < numTimPeriodElapsedCallbacksRegistered; i++) {
    if (timPeriodElapsedCallbacks[i].htim == htim) {
      timPeriodElapsedCallbacks[i].irqCallback = callback;
      timPeriodElapsedCallbacks[i].irqCallbackUserData = userData;
      return;  // No need to keep going if handle already found to be registered
    }
  }
  // If reached, handle hasn't been registered, so create a new association
  timPeriodElapsedCallbacks[numTimPeriodElapsedCallbacksRegistered].htim = htim;
  timPeriodElapsedCallbacks[numTimPeriodElapsedCallbacksRegistered]
      .irqCallback = callback;
  timPeriodElapsedCallbacks[numTimPeriodElapsedCallbacksRegistered]
      .irqCallbackUserData = userData;
  numTimPeriodElapsedCallbacksRegistered++;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  for (int i = 0; i < numTimPeriodElapsedCallbacksRegistered; i++) {
    if (timPeriodElapsedCallbacks[i].htim == htim) {
      if (timPeriodElapsedCallbacks[i].irqCallback != NULL) {
        timPeriodElapsedCallbacks[i].irqCallback(
            timPeriodElapsedCallbacks[i].irqCallbackUserData);
      }
      return;  // No need to keep searching if callback was found
    }
  }
}

#endif  // HAL_TIM_MODULE_ENABLED
