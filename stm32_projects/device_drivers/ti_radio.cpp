/*
 * ti_radio.c
 *
 *  Created on: May 24, 2020
 *      Author: ben helfrich
 */

#include "ti_radio.h"

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
#include "ao_fec.h"
#endif

#if HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915)

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cc1120_cc1200_defs.h"
#include "ti_fec.h"

#define TI_RADIO_STATUS_MASK 0xF0

#pragma GCC optimize("-O2")

// Static fec encode/decode helpers
// This assumes no multi-threading weirdness
#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
FecEncoder encoder;
FecDecoder decoder;
#endif

// Forward declarations for all internal register read/writes
static uint8_t tiRadio_spiReadReg(TiRadioCtrl_s *radio, uint16_t addr,
                                  uint8_t *data, uint8_t len);
static uint8_t tiRadio_spiWriteReg(TiRadioCtrl_s *radio, uint16_t addr,
                                   uint8_t *data, uint8_t len);
static uint8_t tiRadio_spiWriteTxFifo(TiRadioCtrl_s *radio, uint8_t *pWriteData,
                                      uint8_t len);
static uint8_t tiRadio_spiReadRxFifo(TiRadioCtrl_s *radio, uint8_t *pReadData,
                                     uint8_t len);
static uint8_t tiRadio_txRxSpiCmdStrobe(TiRadioCtrl_s *radio, uint8_t cmd);
static void tiRadio_txRxReadWriteBurstSingle(TiRadioCtrl_s *radio, uint8_t addr,
                                             uint8_t *pData, uint16_t len);
//! Try to transmit a packet. Returns true if we were able to add the packet to
//! TX FIFO
static bool tiRadio_TransmitPacket(TiRadioCtrl_s *radio, uint8_t *payload,
                                   uint8_t payloadLength);

// For now, only need to do software FEC on CC1120s
#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
static bool manualCalibration(TiRadioCtrl_s *radio);
#endif

#define TIRADIO_MAX_DELAY 10  // ms

// Initially configure radio, apply settings provided
bool tiRadio_init(TiRadioCtrl_s *radio) {
  // Set SPI pins into a known good state. Lots of waiting to make sure it
  // actually happens
  HAL_GPIO_WritePin(radio->RST_port, radio->RST_pin, GPIO_PIN_SET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_SET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_RESET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_SET);
  HAL_Delay(50);

  // Reset everything
  tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SRES);

  // Write provided default register settings
  uint8_t writeByte;
  for (uint16_t i = 0; i < (radio->settingsSize / sizeof(RegisterSetting_s));
       i++) {
    writeByte = radio->settingsPtr[i].data;
    tiRadio_spiWriteReg(radio, radio->settingsPtr[i].addr, &writeByte, 1);
  }

  // Put radio back into RX after receiving a good packet
  uint8_t rfend1 = 0b00110000 | (0x07 << 1) | 0x01;
  tiRadio_spiWriteReg(radio, TIRADIO_RFEND_CFG1, &rfend1, 1);

  // Put radio back into RX after sending a packet (TXOFF_MODE)
  uint8_t rfend0 = 0b00110000;
  tiRadio_spiWriteReg(radio, TIRADIO_RFEND_CFG0, &rfend0, 1);

  // Figure out what packet length/config we need to send to the radio
  uint8_t pkt_len = 0xFF;
  uint8_t pkt_cfg0 = 0x20;
  if (radio->payloadSize == 0xFF) {
    pkt_len = 0xFF;
    pkt_cfg0 = 0x20;
  } else {
    // If we're doing software FEC, packets grow to (len + 4) * 2
    if (radio->doSoftwareFEC) {
      pkt_len = 4 * ((radio->payloadSize + TI_FEC_CRC_LEN_BYTES) / 2 + 1);
    } else {
      pkt_len = radio->payloadSize;
    }

    // Tell radio to be in fixed-len mode
    pkt_cfg0 = 0x00;
  }

  // The size for fixed length should be known on init
  tiRadio_spiWriteReg(radio, TIRADIO_PKT_LEN, &pkt_len, 0x01);
  tiRadio_spiWriteReg(radio, TIRADIO_PKT_CFG0, &pkt_cfg0, 0x01);

  tiRadio_spiWriteReg(radio, TIRADIO_FIFO_CFG, &radio->payloadSize, 0x01);

  // SetFrequency is broken, for now the prefered settings do this for us
  // tiRadio_setFrequency(radio);

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
  // calibrate the radio per errata
  if (!manualCalibration(radio)) return false;
#else
  tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SCAL);
#endif

  // We need GPIO3 to find when packets come in
  // We look for when full above thresh or packet end reached
  tiRadio_configGpio(radio, TIRADIO_IOCFG3, TIRADIO_RXFIFO_THR_PKT, false);

  tiRadio_forceIdle(radio);

  radio->initialized = true;

  cb_init(&radio->txBuffer, (unknownPtr_t)radio->txArray, PACKET_TX_SIZE, 1);
  radio->currentConsumerCount = 0;

  // TODO we should be smart about not killing our LNA by accident, that's why I
  // wrote tiRadio_SetOutputPower

  return true;
}

// The main tick function. Checks for new packets and transmits the waiting one,
// if non-zero
void tiRadio_update(TiRadioCtrl_s *radio) {
  // Don't let anything happen if radio isn't initialized
  if (!radio->initialized) return;

  // The MARC_STATUS1 register "tells us what caused MCU_WAKEUP to be asserted"
  // We don't actually check if it's been asserted here, though -- that's
  // probably not ideal instead, we just use it to figure out of the TX or RX
  // FIFO queues inside the radio chip have overflowed.

  // Due (probably) to another bug with how we enqueue packets to send to the
  // radio, spamming CLI commands from ground station up to the rocket can make
  // the TX FIFO overflow, causing the radio to become stuck in the TX state
  // forever. This makes everything hang. When this happens, MARC_STATUS1 will
  // read 7 (TX FIFO overflow) and the radio state will be stuck in TX.

  // To get out of this state, we can't use SFTX or SFRX directly (states that
  // flush the FIFOs), as we're still in the TX state, not the TX FIFO error
  // state. So first, we have to force the radio chip to IDLE (See the state
  // machine on page 6 of the CC1200 programming manual)

  // But this alone isn't sufficient, since there's there can still be some
  // residual data left in the TX FIFOs. Essentially, this means all future
  // packets will be offset by some number of bytes lmao. Page 13 says that
  // sending SFTX is allowed in IDLE, so we should be good

  // TODO 1) we should be checking that the TX FIFO is _empty_ before adding a
  // packet to it in the TX function, and figuring out if we need to flush there
  // to ensure the FIFO is actually empty before we stuff our packet in
  // TODO 2) we should probably attach the MCU_WAKEUP interrupt to something,
  // and replace the FIFO_THRESH GPIO I think? It's unclear if it's undefined
  // behavior to try to read MARC_STATUS1 at some random time, not just when
  // MCU_WAKEUP happens

  uint8_t marcstate;
  tiRadio_spiReadReg(radio, TIRADIO_MARC_STATUS1, &marcstate, 1);
  // TX over-/underflow 7-8, RX over-/underflow 9-10 -- want to flush in any of
  // these cases
  if (marcstate >= 0x07 &&
      marcstate <= 0x0A) {  // TX overflow 7-8, RX overflow 9-10
    tiRadio_forceIdle(radio);
    tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFTX);
    tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFRX);
  }

  // Check for new packets until the GPIO goes low
  // realistically, we'll only ever get like 2, but
  // it doesn't hurt since the cost for each check
  // is a GPIO read
  int i = 0;
  while (tiRadio_checkNewPacket(radio) && (i++ < 10)) {
  }

  // Attempt to transmit one packet, since we don't want to
  // overflow the TX FIFO. Alternately, we could write bytes
  // until the TX FIFO is full. But right now at least, a packet
  // takes 10ms or so to transmit anyways
  if (cb_count(&radio->txBuffer) >= radio->payloadSize) {
    // Try to dequeue radio->payloadSize many bytes
    static uint8_t tempBuffer[128] = {0};
    size_t size = radio->payloadSize;
    cb_peek(&radio->txBuffer, (unknownPtr_t)tempBuffer, &size);

    // If we managed to dequeue bytes we're good
    if (size == radio->payloadSize) {
      bool added = tiRadio_TransmitPacket(radio, tempBuffer, size);
      if (added) cb_dequeue(&radio->txBuffer, size);
    }
  }

  uint8_t status = tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SNOP);
  status = (status & TI_RADIO_STATUS_MASK);

  switch (status) {
    case TIRADIO_STATE_IDLE:
      tiRadio_startRx(radio);
      break;
    case TIRADIO_STATE_RX:

      break;
    case TIRADIO_STATE_TX:

      break;
    case TIRADIO_STATE_FSTXON:

      break;
    case TIRADIO_STATE_CALIBRATE:
      break;
    case TIRADIO_STATE_SETTLING:
      break;
    case TIRADIO_STATE_RXFIFO_ERROR:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFRX);
      break;
    case TIRADIO_STATE_TXFIFO_ERROR:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFTX);
      break;
    default:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SIDLE);
      break;
  }
}

static bool cc1120InTx(uint8_t status) {
  return status == TIRADIO_STATE_TX || status == TIRADIO_STATE_FSTXON ||
         status == TIRADIO_STATE_SETTLING;
}

static bool tiRadio_TransmitPacket(TiRadioCtrl_s *radio, uint8_t *payload,
                                   uint8_t payloadLength) {
  static bool added = false;

  uint8_t status;
  status = tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SNOP);
  status = (status & TI_RADIO_STATUS_MASK);
  if (cc1120InTx(status)) {
    // If we're currently in TX, we should wait a tick to add our packet,
    // it won't get sent till we strobe TX anyhow
    return false;
  }

  // If there's still a packet in the TX FIFO at this point, and we aren't
  // in TX state, the channel probably wasn't clear. I think it's reasonable to
  // just try again

  // At the same time, page 42 says that in LBT mode we'll keep trying to enter
  // TX mode

  uint8_t marcstat;
  tiRadio_spiReadReg(radio, TIRADIO_MARC_STATUS1, &marcstat, 1);
  if (marcstat == TIRADIO_CCA_FAILED) {
    // We shouldn't add another packet, but should try sending it again
    added = false;
  } else {
    // FIFO empty; let's add our packet

    if (radio->payloadSize == 0xFF &&
        radio->packetCfg == TIRADIO_PKTLEN_VARIABLE) {
      // I think this is only for variable length?
      // Since we need to add a length byte to the message
      uint8_t packetLength = payloadLength + 1;
      if (packetLength > MAX_PACKET_SIZE) {
        return false;
      }

      // If in variable length mode, length needs to be the first bit in fifo
      tiRadio_spiWriteTxFifo(radio, &payloadLength, 1);
    }

    // Write packet contents to TX FIFO
    tiRadio_spiWriteTxFifo(radio, payload, payloadLength);

    added = true;
  }

  // Check state again
  status = tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SNOP);
  status = (status & TI_RADIO_STATUS_MASK);

  if (status == TIRADIO_STATE_TXFIFO_ERROR) {
    // FIFO overflow. Flush TX FIFO and try
    // to send the packet next tick
    tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFTX);

    added = false;
  }
#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
  if (status == TIRADIO_STATE_RX) {
    // Sometimes, the FCB V0 seems to get stuck in RX mode
    // The only way I've found to prevent this is to force
    // it idle before sending STX. This will interrupt any TX's in
    // progress though!!
    tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SIDLE);
  }
#endif

  // Actually put radio into TX mode
  // If CCA is enabled, FSTXON is only entered if CCA is met
  // Otherwise we will stay in TX mode
  tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_STX);

  return added;
}

static void cc1120EnqueuePacket(TiRadioCtrl_s *radio, uint8_t *buff,
                                uint8_t size, bool crc) {
  // Fill our packet
  static RadioRecievedPacket_s packet;
  packet.radioId = radio->id;
  packet.rssi = radio->RSSI;
  packet.crc = crc;
  packet.lqi = radio->LQI;
  memset(packet.data, 0, sizeof(packet.data));
  memcpy(packet.data, buff, size);
  // uint8_t *pPacket = (uint8_t *) &packet;

  for (int i = 0; i < MAX_COMSUMER; i++) {
    CircularBuffer_s *consumer = radio->messageConsumers[i];
    if (consumer == NULL) continue;

    cb_enqueue(consumer, (unknownPtr_t)&packet);
  }
}

bool tiRadio_addTxPacket(TiRadioCtrl_s *radio, uint8_t *packet, uint8_t len) {
  // Need to transmit in distinct packets, and we assume in fixed that there's
  // no delimination between packets This will require some refactoring to
  // support variable packet length
  if (len != radio->payloadSize &&
      radio->packetCfg == TIRADIO_PKTLEN_VARIABLE) {
    return false;
  }

  uint8_t *txPtr;

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
  // If we need to encode with FEC, do so now
  if (radio->doSoftwareFEC) {
    encoder.Encode(packet, len);
    txPtr = encoder.OutputArray();
  } else {
    // No software FEC, directly enqueue packet
    txPtr = packet;
  }
#else
  txPtr = packet;
#endif

  // Check if adding payloadSinze many bytes won't overflow
  // this is probably wrong for FEC -- should be 4 * (MessageLen/2+1)
  if (radio->payloadSize + cb_count(&radio->txBuffer) <=
      cb_capacity(&radio->txBuffer)) {
    // Add our whole packet to the queue
    for (int j = 0; j < radio->payloadSize; j++) {
      cb_enqueue(&radio->txBuffer, (unknownPtr_t)txPtr + j);
    }
  } else {
    // Buffer full, all we can do is return
    return false;
  }

  return true;
}

void tiRadio_registerConsumer(TiRadioCtrl_s *radio,
                              CircularBuffer_s *rxBuffer) {
  radio->messageConsumers[radio->currentConsumerCount] = rxBuffer;
  radio->currentConsumerCount++;
}

bool tiRadio_checkNewPacket(TiRadioCtrl_s *radio) {
  static uint8_t rxbytes = 0x00;
  static uint8_t rxBuffer[MAX_PACKET_SIZE] = {0};

  // Read GPIO, should be high until the RX FIFO is empty
  if (HAL_GPIO_ReadPin(radio->GP3_port, radio->GP3_pin) == GPIO_PIN_SET) {
    // Read number of bytes in RX FIFO
    tiRadio_spiReadReg(radio, TIRADIO_NUM_RXBYTES, &rxbytes, 1);
    if (rxbytes < 1) {
      return false;
    }

    // Double check RX FIFO has not yet overflowed
    uint8_t status = tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SNOP);
    status = (status & TI_RADIO_STATUS_MASK);
    if (status == TIRADIO_STATE_RXFIFO_ERROR) {
      // Flush RX FIFO
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFRX);
      return false;
    }

    // TODO this variable-length packet implementation seems wrong
    if ((radio->packetCfg & CC1120_LENGTH_CONFIG_MASK) ==
        TIRADIO_PKTLEN_VARIABLE) {
      /*
      tiRadio_spiReadRxFifo(radio, rxBuffer, rxbytes);
      memcpy(radio->packetRX, rxBuffer, rxbytes - 2);
      radio->RSSI = rxBuffer[rxbytes - 2];
      radio->CRC_LQI = rxBuffer[rxbytes - 1];
      */
    } else {
      // Fixed means we have payloadSize many data bytes, then RSSI and CRC_LQI
      // appended
      size_t payloadLenWithoutCRC = radio->payloadSize;

      uint8_t pkt_len;
      if (radio->doSoftwareFEC) {
        pkt_len = 4 * ((payloadLenWithoutCRC + TI_FEC_CRC_LEN_BYTES) / 2 + 1);
      } else {
        pkt_len = payloadLenWithoutCRC;
      }

      tiRadio_spiReadRxFifo(radio, rxBuffer, pkt_len);
      tiRadio_spiReadRxFifo(radio, (uint8_t *)&radio->RSSI, 1);
      uint8_t crc_lqi;
      tiRadio_spiReadRxFifo(radio, &crc_lqi, 1);

      radio->LQI = crc_lqi & TIRADIO_LQI_EST_BM;

      uint8_t *pOutputBuffer;
      bool crc_good;
      // Decoded payload + 2 bytes CRC
      static uint8_t rxBuffer_decoded[MAX_PACKET_SIZE] = {0};
      if (radio->doSoftwareFEC) {
        decoder.FecDecode(rxBuffer, rxBuffer_decoded, payloadLenWithoutCRC + 2);

        auto packetArrayCRC =
            ti_fec::calculateCRC_array(rxBuffer_decoded, payloadLenWithoutCRC);
        auto decodedPacketCRC =
            (rxBuffer_decoded[payloadLenWithoutCRC + 0] << 8) |
            (rxBuffer_decoded[payloadLenWithoutCRC + 1] << 8);

        pOutputBuffer = rxBuffer_decoded;
        crc_good = (packetArrayCRC == decodedPacketCRC);
      } else {
        pOutputBuffer = rxBuffer;
        crc_good = crc_lqi & TIRADIO_LQI_CRC_OK_BM;
      }

      cc1120EnqueuePacket(radio, pOutputBuffer, radio->payloadSize, crc_good);
    }

    return true;
  }

  return false;
}

bool tiRadio_forceIdle(TiRadioCtrl_s *radio) {
  // return the radio to idle somehow
  uint8_t status;
  status = tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SNOP);
  status = (status & TI_RADIO_STATUS_MASK);

  switch (status) {
    case TIRADIO_STATE_IDLE:
      return true;
      break;
    case TIRADIO_STATE_RX:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SIDLE);
      return true;
      break;
    case TIRADIO_STATE_TX:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SIDLE);
      return true;
      break;
    case TIRADIO_STATE_FSTXON:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SIDLE);
      return true;
      break;
    case TIRADIO_STATE_CALIBRATE:
      return false;
      break;
    case TIRADIO_STATE_SETTLING:
      return false;
      break;
    case TIRADIO_STATE_RXFIFO_ERROR:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFRX);
      return true;
      break;
    case TIRADIO_STATE_TXFIFO_ERROR:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SFTX);
      return true;
      break;

    default:
      tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SIDLE);
      return true;
  }

  return false;
}

bool tiRadio_startRx(TiRadioCtrl_s *radio) {
  uint8_t readByte;
  tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SRX);

  // make sure we actually switch into rx by waiting till marcstate is right
  uint32_t startMS = HAL_GetTick();
  do {
    tiRadio_spiReadReg(radio, TIRADIO_MARCSTATE, &readByte, 1);
  } while (readByte != TIRADIO_MARCSTATE_RX &&
           HAL_GetTick() - startMS < TIRADIO_MAX_DELAY);

  return readByte == TIRADIO_MARCSTATE_RX;
}

static const float twoToThe16 = 65536;
static const size_t maxValue24Bits = 16777216 - 1;

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
#define RADIO_OSC_FREQ CC1120_OSC_FREQ
#endif
#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
#define RADIO_OSC_FREQ CC1200_OSC_FREQ
#endif

void tiRadio_setRadioFrequency(TiRadioCtrl_s *radio, TiRadioBand_e band,
                               float frequencyHz) {
  // LO divider and stuff should already be set for us, as we assume we must
  // be on the correct band already

  // convert band to LO Divider value.
  // Most of the bands just multiply the register value by 2, but nooo, not
  // BAND_136_160MHz.
  uint8_t loDividerValue;
  if (band == TIRADIO_BAND_136_160MHz) {
    loDividerValue = 24;
  } else {
    loDividerValue = (uint8_t)(band)*2;
  }

  // program band (also enable FS out of lock detector, which is useful for
  // testing)
  uint8_t fsCfg =
      (1 << FS_CFG_FS_LOCK_EN) | ((uint8_t)(band) << FS_CFG_FSD_BANDSELECT);
  tiRadio_spiWriteReg(radio, TIRADIO_FS_CFG, &fsCfg, 1);

  // equation derived from user guide section 9.12
  // f_RF = FREQ / 2^16 * f_XOSC / LO
  // so f_RF * 2^16 * LO / f_XOSC = FREQ
  float exactFreqRegValue =
      (twoToThe16 * frequencyHz * (float)(loDividerValue)) / RADIO_OSC_FREQ;
  uint32_t actualFreqRegValue =
      (uint32_t)(fmin((float)(maxValue24Bits), exactFreqRegValue));

  // program frequency registers
  uint8_t freqRegisters[] = {(uint8_t)((actualFreqRegValue >> 16) & 0xFF),
                             (uint8_t)((actualFreqRegValue >> 8) & 0xFF),
                             (uint8_t)((actualFreqRegValue & 0xFF))};

  tiRadio_spiWriteReg(radio, TIRADIO_FREQ2, freqRegisters, 3);

#ifdef CC1200_DEBUG
  // sanity check: calculate actual frequency
  float radioFreqHz = ((float)(actualFreqRegValue)*CC1200_OSC_FREQ) /
                      (twoToThe16 * (float)(loDividerValue));

  printf("Setting radio frequency, requested %.00f Hz, setting FREQ = 0x%X\n",
         frequencyHz, (unsigned int)actualFreqRegValue);
  HAL_Delay(1);
  printf("This yields an actual frequency of %.00f Hz\n", radioFreqHz);
#endif

  // We need to recalibrate our radio after changing the frequency
  tiRadio_calibrate(radio);
}

bool tiRadio_calibrate(TiRadioCtrl_s *radio) {
#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
  // calibrate the radio per errata
  return manualCalibration(radio);
#else
  tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SCAL);
  return true;  // TODO how can we tell we've calibrated?
#endif
}

#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
static bool manualCalibration(TiRadioCtrl_s *radio) {
  uint8_t original_fs_cal2;
  uint8_t calResults_for_vcdac_start_high[3];
  uint8_t calResults_for_vcdac_start_mid[3];
  uint8_t marcstate;
  uint8_t writeByte;

  tiRadio_spiReadReg(radio, TIRADIO_MARCSTATE, &marcstate, 1);

  // 1) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
  writeByte = 0x00;
  tiRadio_spiWriteReg(radio, TIRADIO_FS_VCO2, &writeByte, 1);

  // 2) Start with high VCDAC (original VCDAC_START + 2):
  tiRadio_spiReadReg(radio, TIRADIO_FS_CAL2, &original_fs_cal2, 1);
  writeByte = original_fs_cal2 + VCDAC_START_OFFSET;
  tiRadio_spiWriteReg(radio, TIRADIO_FS_CAL2, &writeByte, 1);

  // 3) Calibrate and wait for calibration to be done
  //   (radio back in IDLE state)
  tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SCAL);

  uint32_t startMS = HAL_GetTick();
  do {
    tiRadio_spiReadReg(radio, TIRADIO_MARCSTATE, &marcstate, 1);
  } while (marcstate != 0x41 && HAL_GetTick() - startMS < 5000);
  if (marcstate != 0x41) return false;

  // 4) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with
  //    high VCDAC_START value
  tiRadio_spiReadReg(radio, TIRADIO_FS_VCO2,
                     &calResults_for_vcdac_start_high[FS_VCO2_INDEX], 1);
  tiRadio_spiReadReg(radio, TIRADIO_FS_VCO4,
                     &calResults_for_vcdac_start_high[FS_VCO4_INDEX], 1);
  tiRadio_spiReadReg(radio, TIRADIO_FS_CHP,
                     &calResults_for_vcdac_start_high[FS_CHP_INDEX], 1);

  // 5) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
  writeByte = 0x00;
  tiRadio_spiWriteReg(radio, TIRADIO_FS_VCO2, &writeByte, 1);

  // 6) Continue with mid VCDAC (original VCDAC_START):
  writeByte = original_fs_cal2;
  tiRadio_spiWriteReg(radio, TIRADIO_FS_CAL2, &writeByte, 1);

  // 7) Calibrate and wait for calibration to be done
  //   (radio back in IDLE state)
  tiRadio_txRxSpiCmdStrobe(radio, TIRADIO_SCAL);

  startMS = HAL_GetTick();
  do {
    tiRadio_spiReadReg(radio, TIRADIO_MARCSTATE, &marcstate, 1);
  } while (marcstate != 0x41 && HAL_GetTick() - startMS < 5000);
  if (marcstate != 0x41) return false;

  // 8) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained
  //    with mid VCDAC_START value
  tiRadio_spiReadReg(radio, TIRADIO_FS_VCO2,
                     &calResults_for_vcdac_start_mid[FS_VCO2_INDEX], 1);
  tiRadio_spiReadReg(radio, TIRADIO_FS_VCO4,
                     &calResults_for_vcdac_start_mid[FS_VCO4_INDEX], 1);
  tiRadio_spiReadReg(radio, TIRADIO_FS_CHP,
                     &calResults_for_vcdac_start_mid[FS_CHP_INDEX], 1);

  // 9) Write back highest FS_VCO2 and corresponding FS_VCO
  //    and FS_CHP result
  if (calResults_for_vcdac_start_high[FS_VCO2_INDEX] >
      calResults_for_vcdac_start_mid[FS_VCO2_INDEX]) {
    writeByte = calResults_for_vcdac_start_high[FS_VCO2_INDEX];
    tiRadio_spiWriteReg(radio, TIRADIO_FS_VCO2, &writeByte, 1);
    writeByte = calResults_for_vcdac_start_high[FS_VCO4_INDEX];
    tiRadio_spiWriteReg(radio, TIRADIO_FS_VCO4, &writeByte, 1);
    writeByte = calResults_for_vcdac_start_high[FS_CHP_INDEX];
    tiRadio_spiWriteReg(radio, TIRADIO_FS_CHP, &writeByte, 1);
  } else {
    writeByte = calResults_for_vcdac_start_mid[FS_VCO2_INDEX];
    tiRadio_spiWriteReg(radio, TIRADIO_FS_VCO2, &writeByte, 1);
    writeByte = calResults_for_vcdac_start_mid[FS_VCO4_INDEX];
    tiRadio_spiWriteReg(radio, TIRADIO_FS_VCO4, &writeByte, 1);
    writeByte = calResults_for_vcdac_start_mid[FS_CHP_INDEX];
    tiRadio_spiWriteReg(radio, TIRADIO_FS_CHP, &writeByte, 1);
  }

  return true;
}
#endif

//! Set power in db. Make sure to respect your PA maximum!
void tiRadio_setOutputPower(TiRadioCtrl_s *radio, uint8_t powerDbM) {
  if (radio->has_cc1190 && powerDbM > 10) powerDbM = 10;

  // output = (ramp + 1)/2-18
  // (output+18)*2 - 1 = ramp
  uint8_t power = (powerDbM + 18) * 2 - 1;

  // Register must be between 3 and 64
  if (power < 3) power = 3;
  if (power > 63) power = 63;

  uint8_t pa_cfg_val;
  uint8_t pa_reg_addr;

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
  pa_reg_addr = TIRADIO_PA_CFG2;
#elif RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
  pa_reg_addr = TIRADIO_PA_CFG1;
#else
  return;
#endif

  tiRadio_spiReadReg(radio, pa_reg_addr, &pa_cfg_val, 1);
  uint8_t reg = 0b01000000;  // bit 7 set to enable ramping
  reg |= power;              // Lower 6 bits are power
  tiRadio_spiWriteReg(radio, pa_reg_addr, &reg, 1);
}

//! Configure a GPIO pin. register should be TIRADIO_IOCFG3 or TIRADIO_IOCFG2 or
//! TIRADIO_IOCFG1 or TIRADIO_IOCFG0 gpio_config from tiRadio_GPIO_CFG, and
//! outputInverted flips the logic of the pin
void tiRadio_configGpio(TiRadioCtrl_s *radio, uint16_t gpio_register,
                        uint8_t gpio_config, bool outputInverted) {
  uint8_t invert = (outputInverted ? 1 : 0);
  // invert is bit 6, 7th from bottom
  // lower 6 are output selection
  uint8_t reg = (invert << 6) | gpio_config;
  tiRadio_spiWriteReg(radio, gpio_register, &reg, 1);
}

uint8_t trx8BitRegAccess(TiRadioCtrl_s *radio, uint8_t accessType,
                         uint8_t addrByte, uint8_t *pData, uint16_t len) {
  uint8_t readValue;
  uint8_t txBuf = (accessType | addrByte);

  // Pull CS low
  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_RESET);
  uint32_t startMS = HAL_GetTick();

  // Wait for SO to go low (radio ready)
  while (HAL_GPIO_ReadPin(radio->MISO_port, radio->MISO_pin) == 1 &&
         HAL_GetTick() - startMS < TIRADIO_MAX_DELAY) {
  }

  HAL_SPI_TransmitReceive(radio->radhspi, &txBuf, &readValue, 0x01,
                          TIRADIO_MAX_DELAY);
  tiRadio_txRxReadWriteBurstSingle(radio, (accessType | addrByte), pData,
                                   len);  // write the data
  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin,
                    GPIO_PIN_SET);  // Pull CS High

  return (readValue);
}

uint8_t trx16BitRegAccess(TiRadioCtrl_s *radio, uint8_t accessType,
                          uint8_t extAddr, uint8_t regAddr, uint8_t *pData,
                          uint16_t len) {
  uint8_t readValue;
  uint8_t txBuf = (accessType | extAddr);

  // Pull CS LOW
  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_RESET);
  uint32_t startMS = HAL_GetTick();
  while (HAL_GPIO_ReadPin(radio->MISO_port, radio->MISO_pin) == 1 &&
         HAL_GetTick() - startMS < TIRADIO_MAX_DELAY) {
  }
  // Wait for SO to go low
  // while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);

  // Write (accessType|addrByte)
  // store the chip status
  HAL_SPI_TransmitReceive(radio->radhspi, &txBuf, &readValue, 1,
                          TIRADIO_MAX_DELAY);

  HAL_SPI_Transmit(radio->radhspi, &regAddr, 1, TIRADIO_MAX_DELAY);

  // write the data
  tiRadio_txRxReadWriteBurstSingle(radio, accessType | extAddr, pData, len);

  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_SET);

  return (readValue);
}

uint8_t tiRadio_txRxSpiCmdStrobe(TiRadioCtrl_s *radio, uint8_t cmd) {
  uint8_t rc;

  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_RESET);
  uint32_t startMS = HAL_GetTick();
  while (HAL_GPIO_ReadPin(radio->MISO_port, radio->MISO_pin) == 1 &&
         HAL_GetTick() - startMS < TIRADIO_MAX_DELAY) {
  }
  // Wait for SO to go low

  HAL_SPI_TransmitReceive(radio->radhspi, &cmd, &rc, 1, TIRADIO_MAX_DELAY);

  HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, GPIO_PIN_SET);

  return (rc);
}

void tiRadio_txRxReadWriteBurstSingle(TiRadioCtrl_s *radio, uint8_t addr,
                                      uint8_t *pData, uint16_t len) {
  uint16_t i;
  uint8_t pushByte = 0x00;
  uint8_t rxdump;

  if (addr & RADIO_READ_ACCESS) {  // if reading from radio registers
    if (addr & RADIO_BURST_ACCESS) {
      for (i = 0; i < len; i++) {  // push zeros to push out data
        HAL_SPI_TransmitReceive(radio->radhspi, &pushByte, pData, 1,
                                TIRADIO_MAX_DELAY);
        pData++;
      }

    } else {
      HAL_SPI_TransmitReceive(radio->radhspi, &pushByte, pData, 1,
                              TIRADIO_MAX_DELAY);
    }
  } else {  // if writing to radio registers
    if (addr & RADIO_BURST_ACCESS) {
      for (i = 0; i < len; i++) {  // push in data
        HAL_SPI_TransmitReceive(radio->radhspi, pData, &rxdump, 1,
                                TIRADIO_MAX_DELAY);
        pData++;
      }
    } else {
      HAL_SPI_TransmitReceive(radio->radhspi, pData, &rxdump, 1,
                              TIRADIO_MAX_DELAY);
    }
  }

  return;
}

uint8_t tiRadio_spiReadReg(TiRadioCtrl_s *radio, uint16_t addr, uint8_t *pData,
                           uint8_t len) {
  uint8_t tempExt = (uint8_t)(addr >> 8);
  uint8_t tempAddr = (uint8_t)(addr & 0x00FF);
  uint8_t rc = 0;

  // If this is a TX FIFO access, return that the chip is not ready
  if ((TIRADIO_SINGLE_TXFIFO <= tempAddr) && (tempExt == 0))
    return STATUS_CHIP_RDYn_BM;

  // if its a regular register, it'll be zero
  if (tempExt == NORMAL_REGISTER_TOP_BYTE) {
    rc = trx8BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_READ_ACCESS),
                          tempAddr, pData, len);

    // if its an extended access register
  } else if (tempExt == EXTENDED_REGISTER_TOP_BYTE) {
    rc = trx16BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_READ_ACCESS),
                           tempExt, tempAddr, pData, len);
  }
  return (rc);
}

uint8_t tiRadio_spiWriteReg(TiRadioCtrl_s *radio, uint16_t addr, uint8_t *pData,
                            uint8_t len) {
  uint8_t tempExt = (uint8_t)(addr >> 8);
  uint8_t tempAddr = (uint8_t)(addr & 0x00FF);
  uint8_t rc = 0;

  if (!tempExt) {
    rc = trx8BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_WRITE_ACCESS),
                          tempAddr, pData, len);

    // DEBUG todo what does this do?
    /*
    uint8_t readByte = 0;
    uint8_t writeByte = *pData;
    tiRadio_spiReadReg(radio, addr, &readByte, 0x01);
    testread = readByte;
    if (readByte != writeByte) {
      readByte = 0;
    }
    */

  } else if (tempExt == EXTENDED_REGISTER_TOP_BYTE) {
    rc = trx16BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_WRITE_ACCESS),
                           tempExt, tempAddr, pData, len);
  }
  return (rc);
}

uint8_t tiRadio_spiWriteTxFifo(TiRadioCtrl_s *radio, uint8_t *pData,
                               uint8_t len) {
  uint8_t rc;
  rc = trx8BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_WRITE_ACCESS),
                        TIRADIO_BURST_TXFIFO, pData, len);
  return (rc);
}
uint8_t tiRadio_spiReadRxFifo(TiRadioCtrl_s *radio, uint8_t *pData,
                              uint8_t len) {
  uint8_t rc;
  rc = trx8BitRegAccess(radio, 0x00, TIRADIO_BURST_RXFIFO, pData, len);
  return (rc);
}

#endif  // HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915)
