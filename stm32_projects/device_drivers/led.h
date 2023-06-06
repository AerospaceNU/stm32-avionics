#ifndef STM32_PROJECTS_DEVICE_DRIVERS_LED_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_LED_H_

#include <stdbool.h>

#include "board_config_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
} LedCtrl_s;

void led_init(LedCtrl_s *led);

void led_set(LedCtrl_s *led, bool set);

void led_toggle(LedCtrl_s *led);

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_LED_H_
