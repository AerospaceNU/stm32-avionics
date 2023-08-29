/*
 * ti_radio.h
 *
 *  Created on: May 24, 2020
 *      Author: bph71
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_TI_RADIO_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_TI_RADIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"

#if (HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915))

#include "cc1120_cc1200_defs.h"
#include "circular_buffer.h"
#include "data_structures.h"
#include "smartrf_registersettings.h"

#include HAL_HEADER

#define MAX_COMSUMER 5

#define PACKET_TX_SIZE 512  // 10ish packets seems reasonable

typedef enum {
  TIRADIO_RXFIFO_THR_PKT = 1,  // Associated to the RX FIFO. Asserted when the
                               // RX FIFO is filled above
  // FIFO_CFG.FIFO_THR or the end of packet is reached. De-asserted
  // when the RX FIFO is empty

  // RX: Asserted when sync word has been received and de-asserted at the
  // end of the packet. Will de-assert when the optional address and/or
  // length check fails or the RX FIFO overflows/underflows.
  // TX: Asserted when sync word has been sent, and de-asserted at the end
  // of the packet. Will de-assert if the TX FIFO underflows/overflows
  TIRADIO_PKT_SYNC_RXTX = 6,

  TIRADIO_CRC_OK = 7,
  TIRADIO_PKT_CRC_OK = 19,

  // MCU Wakeup, read MARC_STATUS1 to determine the cause. Signal is (allegedly)
  // a "pulse", so best to attach an interrupt if used? See page 20 of CC1200
  // programming reference
  TIRADIO_MCU_WAKEUP = 20,

  TIRADIO_nLNA_PD = 24,     // control external LNA. Active low. Low in sleep!
  TIRADIO_nPA_PD = 25,      // External PA, active low. Low in sleep!
  TIRADIO_RX0TX1_CFG = 26,  // 0 in idle or rx, 1 in transmit

  TIRADIO_HIGHZ = 48  // High-impedance, if we need this to not do anything
} TiRadioGpioCfg_e;

typedef enum {
  TIRADIO_PKTLEN_FIXED = 0,
  TIRADIO_PKTLEN_VARIABLE = 0x20
} TiRadioPacketCfg_e;

// Radio band for the chip to operate on.
// See user guide description for FS_CFG register.
typedef enum {
  TIRADIO_BAND_820_960MHz = 0x2,
  TIRADIO_BAND_410_480MHz = 0x4,
  TIRADIO_BAND_273_320MHz = 0x6,
  TIRADIO_BAND_205_240MHz = 0x8,
  TIRADIO_BAND_164_192MHz = 0xA,
  TIRADIO_BAND_136_160MHz = 0xB
} TiRadioBand_e;

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
  const RegisterSetting_s *settingsPtr;
  size_t settingsSize;
  bool doSoftwareFEC;

  bool initialized;
} TiRadioCtrl_s;

bool tiRadio_init(TiRadioCtrl_s *radio);

void tiRadio_update(TiRadioCtrl_s *radio);
//! Add one packet to the transmit queue, if there is space in it
bool tiRadio_addTxPacket(TiRadioCtrl_s *radio, uint8_t *packet, uint8_t len);
void tiRadio_registerConsumer(TiRadioCtrl_s *radio, CircularBuffer_s *rxBuffer);

void tiRadio_setRadioFrequency(TiRadioCtrl_s *radio, TiRadioBand_e band,
                               float frequencyHz);

bool tiRadio_forceIdle(TiRadioCtrl_s *radio);
bool tiRadio_startRx(TiRadioCtrl_s *radio);
bool tiRadio_checkNewPacket(TiRadioCtrl_s *radio);
void tiRadio_setOutputPower(TiRadioCtrl_s *radio, uint8_t powerDbM);
void tiRadio_configGpio(TiRadioCtrl_s *radio, uint16_t gpio_register,
                        uint8_t gpio_config, bool outputInverted);
bool tiRadio_calibrate(TiRadioCtrl_s *radio);

#endif // (HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915))

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_TI_RADIO_H_
