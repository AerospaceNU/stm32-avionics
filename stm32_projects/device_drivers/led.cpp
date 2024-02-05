#include "led.h"

#if HAS_DEV(LED_DIGITAL)

void led_init(LedCtrl_s* led) {
  if (led->port) {
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
  }
}

void led_set(LedCtrl_s* led, bool set) {
  if (led->port) {
    HAL_GPIO_WritePin(led->port, led->pin, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
}

void led_toggle(LedCtrl_s* led) {
  if (led->port) {
    HAL_GPIO_TogglePin(led->port, led->pin);
  }
}

#endif
