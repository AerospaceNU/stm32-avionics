#ifndef DEVICES_LED_H_
#define DEVICES_LED_H_

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

#endif  // DEVICES_LED_H_
