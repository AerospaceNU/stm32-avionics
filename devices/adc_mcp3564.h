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
} AdcMcp3564Ctrl_s;

void adcMCP3564_init(AdcMcp3564Ctrl_s *dev, SPI_HandleTypeDef *hspi,
            GPIO_TypeDef *csPort, uint16_t csPin, 
            GPIO_TypeDef *intPort, uint16_t intPin);

void adcMCP3564_readChan(AdcMcp3564Ctrl_s *dev, int chan);

#endif
