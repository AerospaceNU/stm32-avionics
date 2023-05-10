/*
 * board_config.c
 */

#include "board_config_common.h"
#include "main.h"
#include "spi.h"

#if HAS_DEV(RADIO_TI_433)
SPI_HandleTypeDef* radioTi433Hspi[NUM_RADIO_TI_433] = {&hspi1};
GPIO_TypeDef* radioTi433CsGpioPort[NUM_RADIO_TI_433] = {RAD_CS_GPIO_Port};
uint16_t radioTi433CsPin[NUM_RADIO_TI_433] = {RAD_CS_Pin};
GPIO_TypeDef* radioTi433RstGpioPort[NUM_RADIO_TI_433] = {RAD_RST_GPIO_Port};
uint16_t radioTi433RstPin[NUM_RADIO_TI_433] = {RAD_RST_Pin};
GPIO_TypeDef* radioTi433MisoGpioPort[NUM_RADIO_TI_433] = {GPIOA};
uint16_t radioTi433MisoPin[NUM_RADIO_TI_433] = {GPIO_PIN_6};
GPIO_TypeDef* radioTi433Gp0GpioPort[NUM_RADIO_TI_433] = {0};
uint16_t radioTi433Gp0Pin[NUM_RADIO_TI_433] = {0};
GPIO_TypeDef* radioTi433Gp2GpioPort[NUM_RADIO_TI_433] = {0};
uint16_t radioTi433Gp2Pin[NUM_RADIO_TI_433] = {0};
GPIO_TypeDef* radioTi433Gp3GpioPort[NUM_RADIO_TI_433] = {0};
uint16_t radioTi433Gp3Pin[NUM_RADIO_TI_433] = {0};
#endif  // HAS_DEV(RADIO_TI_433)
