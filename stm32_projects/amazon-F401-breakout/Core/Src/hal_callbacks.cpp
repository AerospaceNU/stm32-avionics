/*
 * hal_callbacks.c
 */

#include "hal_callbacks.h"

#include <stdint.h>

#define MAX_UART_HANDLES 5  // UART 1-5



typedef struct {
  UART_HandleTypeDef *huart;
  std::function<void (uint16_t)> rxIdleCallback;
} UartCallbackProperty_s;

static UartCallbackProperty_s uartCallbacks[MAX_UART_HANDLES];
static int numUartCallbacksRegistered = 0;

void halCallbacks_registerUartRxIdleCallback(UART_HandleTypeDef *huart,
											std::function<void (uint16_t)> callback) {
  // See if handle already has callback registered to it
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      uartCallbacks[i].rxIdleCallback = callback;
      return;  // No need to keep going if handle already found to be registered
    }
  }

  // If reached, handle hasn't been registered, so create a new association
  uartCallbacks[numUartCallbacksRegistered].huart = huart;
  uartCallbacks[numUartCallbacksRegistered].rxIdleCallback = callback;
  numUartCallbacksRegistered++;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
  for (int i = 0; i < numUartCallbacksRegistered; i++) {
    if (uartCallbacks[i].huart == huart) {
      if (uartCallbacks[i].rxIdleCallback != NULL) {
        uartCallbacks[i].rxIdleCallback(size);
      }
      return;  // No need to keep searching if callback was found
    }
  }
}


