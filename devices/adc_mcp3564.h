/*
 * adc_mcp3564.h
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#ifndef ADC_MCP3564_H_
#define ADC_MCP3564_H_

#include "board_config.h"
#include HAL_HEADER

typedef struct {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *csPort;
  uint16_t csPin;
  GPIO_TypeDef *intPort;
  uint16_t intPin;

  int32_t result;
} AdcMcp3564Ctrl_s;

int mcp356x_read(AdcMcp3564Ctrl_s *dev);
int mcp356x_channel_setup(AdcMcp3564Ctrl_s *dev,
                          const int channel_id);
int mcp3564_init(AdcMcp3564Ctrl_s *dev, SPI_HandleTypeDef *hspi,
            GPIO_TypeDef *csPort, uint16_t csPin,
            GPIO_TypeDef *intPort, uint16_t intPin);
#endif
