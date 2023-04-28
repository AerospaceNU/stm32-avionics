#pragma once

#include "board_config.h"
#include HAL_HEADER
#include <stdbool.h>
#include "circular_buffer.h"
#include "data_structures.h"

#define MAX_COMSUMER 5

#define PACKET_TX_SIZE 512  // 10ish packets seems reasonable

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  // GPIO pins
  SPI_HandleTypeDef *radhspi;
  GPIO_TypeDef *RST_port;
  uint16_t RST_pin;
  GPIO_TypeDef *CS_port;
  uint16_t CS_pin;
  GPIO_TypeDef *MISO_port;
  uint16_t MISO_pin;
  GPIO_TypeDef *GP0_port;
  uint16_t GP0_pin;
  GPIO_TypeDef *GP2_port;
  uint16_t GP2_pin;
  GPIO_TypeDef *GP3_port;
  uint16_t GP3_pin;

  // 0x00 is fixed packet size, payload determined by payloadSize
  // 0x20 is variable packet size, payload determined by payloadSize, packetSize
  // is payloadSize +1
  //  packetCfg is of type tiRadio_Packet_Cfg
  uint8_t packetCfg;

  // If we have a CC1190 range-extender
  bool has_cc1190;

  // up to 128 in fixed, or 127 in variable
  uint8_t payloadSize;

  // Buffers for incoming/outgoing packets
  CircularBuffer_s *messageConsumers[MAX_COMSUMER];
  uint8_t currentConsumerCount;

  uint8_t txArray[PACKET_TX_SIZE];
  CircularBuffer_s txBuffer;

  // ID assigned to this transciver
  uint8_t id;

  // Last RSSI/LQI
  int8_t RSSI;
  uint8_t LQI;

  // Set from smartRF config
//  const RegisterSetting_s *settingsPtr;
//  size_t settingsSize;

  bool initialized;
} TiRadioCtrl_s;

bool tiRadio_init(TiRadioCtrl_s *radio);

void tiRadio_update(TiRadioCtrl_s *radio);
//! Add one packet to the transmit queue, if there is space in it
bool tiRadio_addTxPacket(TiRadioCtrl_s *radio, uint8_t *packet, uint8_t len);
void tiRadio_registerConsumer(TiRadioCtrl_s *radio, CircularBuffer_s *rxBuffer);

//void tiRadio_setRadioFrequency(TiRadioCtrl_s *radio, TiRadioBand_e band,
//                               float frequencyHz);

bool tiRadio_forceIdle(TiRadioCtrl_s *radio);
bool tiRadio_startRx(TiRadioCtrl_s *radio);
bool tiRadio_checkNewPacket(TiRadioCtrl_s *radio);
void tiRadio_setOutputPower(TiRadioCtrl_s *radio, uint8_t powerDbM);
void tiRadio_configGpio(TiRadioCtrl_s *radio, uint16_t gpio_register,
                        uint8_t gpio_config, bool outputInverted);
bool tiRadio_calibrate(TiRadioCtrl_s *radio);


int cc1200_main();

#ifdef __cplusplus
}
#endif
