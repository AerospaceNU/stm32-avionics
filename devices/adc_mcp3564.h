/*
 * adc_mcp3564.h
 *
 *  Created on: Sep 22, 2022
 *      Author: mcmorley
 */

#ifndef DEVICES_ADC_MCP3564_H_
#define DEVICES_ADC_MCP3564_H_

#include "board_config.h"
#include HAL_HEADER

typedef struct {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *csPort;
  uint16_t csPin;
  GPIO_TypeDef *intPort;
  uint16_t intPin;

  int32_t result[8];
  uint8_t rx_read_buf[5];
  uint8_t tx_read_buf[5];
} AdcMcp3564Ctrl_s;

// Channel IDs, see table 5-14
// These are for SCAN MODE ONLY!
typedef enum {
  SINGLE_CH0 = 0,
  SINGLE_CH1,
  SINGLE_CH2,
  SINGLE_CH3,
  SINGLE_CH4,
  SINGLE_CH5,
  SINGLE_CH6,
  SINGLE_CH7,
  DIFF_CHA,
  DIFF_CHB,
  DIFF_CHC,
  DIFF_CHD,
  TEMP,  // 1x gain
  AVDD,  // 0.33x gain
  VCM,   // 1x gain
  OFFSET
} AdcMcp3564ScanModeChannels_e;

// Page 88
typedef enum {
  MUX_CH0 = 0,
  MUX_CH1,
  MUX_CH2,
  MUX_CH3,
  MUX_CH4,
  MUX_CH5,
  MUX_CH6,
  MUX_CH7,
  MUX_AGND,
  MUX_AVDD,
  MUX_DNU,  // Do not use!
  MUX_REFIN_P,
  MUX_REFIN_N,
  MUX_TEMP_P,
  MUX_TEMP_N,
  MUX_VCM
} AdcMcp3564MuxChannels_e;

//! Struct for data_format = 00
typedef struct {
  int data : 24;
} AdcMcp3564_DataFormat_00;

//! Struct for data_format = 01
typedef struct {
  int unused : 8;
  int data : 24;
} AdcMcp3564_DataFormat_01;

//! Struct for data_format = 10
// The top 8 bits just repeats the sign bit (page 42)
typedef struct {
  int data : 25;
  int unused : 7;
} AdcMcp3564_DataFormat_10;

//! Struct for data_format = 11
typedef struct {
  int data : 25;
  int unused : 3;
  AdcMcp3564ScanModeChannels_e channel_id : 4;  // (See table 5-14, page 54)
} AdcMcp3564_DataFormat_11;

void mcp356x_read(void *pdev);
int mcp356x_process(AdcMcp3564Ctrl_s *dev);
int mcp356x_channel_setup(AdcMcp3564Ctrl_s *dev,
                          const AdcMcp3564MuxChannels_e in_p,
                          const AdcMcp3564MuxChannels_e in_n);
int mcp3564_init(AdcMcp3564Ctrl_s *dev, SPI_HandleTypeDef *hspi,
                 GPIO_TypeDef *csPort, uint16_t csPin, GPIO_TypeDef *intPort,
                 uint16_t intPin);
#endif  // DEVICES_ADC_MCP3564_H_
