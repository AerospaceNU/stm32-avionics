/*
 * cc1120_spi_drv.h
 *
 *  Created on: May 24, 2020
 *      Author: bph71
 */

#ifndef INC_CC1120_H_
#define INC_CC1120_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config.h"

#include "stdint.h"
#include <stdbool.h>
#include "radioconfig/smartrf_registersettings.h"

#define CC1120_STATUS_MASK 0xF0

typedef struct CC1120Ctrl_s{

    SPI_HandleTypeDef* radhspi;
    GPIO_TypeDef* RST_port;
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

    //0x00 is fixed packet size, payload determined by payloadSize
    //0x20 is variable packet size, payload determined by payloadSize, packetSize is payloadSize +1
    uint8_t packetCfg;

    uint8_t payloadSize;

    uint8_t packetRX[128];
	uint8_t packetToTX[128];
	uint8_t RSSI;
	uint8_t CRC_LQI;

	// Set from smartRF config
	const registerSetting_t* settingsPtr;
	size_t settingsSize;

	bool initialized;
}CC1120Ctrl_t;

enum CC1120_GPIO_CFG {
	CC1120_RXFIFO_THR_PKT = 1, // Associated to the RX FIFO. Asserted when the RX FIFO is filled above
	// FIFO_CFG.FIFO_THR or the end of packet is reached. De-asserted
	// when the RX FIFO is empty

	// RX: Asserted when sync word has been received and de-asserted at the
	// end of the packet. Will de-assert when the optional address and/or
	// length check fails or the RX FIFO overflows/underflows.
	// TX: Asserted when sync word has been sent, and de-asserted at the end
	// of the packet. Will de-assert if the TX FIFO underflows/overflows
	CC1120_PKT_SYNC_RXTX = 6,

	CC1120_CRC_OK = 7,
	CC1120_PKT_CRC_OK = 19,

	CC1120_LNA_PD = 24, // control external LNA. Active low.
	CC1120_PA_PD = 25, // External PA, active low
	CC1120_RX0TX1_CFG = 26, // 0 in idle or rx, 1 in transmit

	CC1120_HIGHZ = 48 // High-impedance, if we need this to not do anything
};

/* basic set of access functions */
uint8_t cc1120SpiReadReg(CC1120Ctrl_t* radio,uint16_t addr, uint8_t *data, uint8_t len);
uint8_t cc1120GetTxStatus(CC1120Ctrl_t* radio);
uint8_t cc1120GetRxStatus(CC1120Ctrl_t* radio);
uint8_t cc1120SpiWriteReg(CC1120Ctrl_t* radio, uint16_t addr, uint8_t *data, uint8_t len);
uint8_t cc1120SpiWriteTxFifo(CC1120Ctrl_t* radio, uint8_t *pWriteData, uint8_t len);
uint8_t cc1120SpiReadRxFifo(CC1120Ctrl_t* radio, uint8_t *pReadData, uint8_t len);
uint8_t trxSpiCmdStrobe(CC1120Ctrl_t* radio, uint8_t cmd);
uint8_t cc1120GetTxStatus(CC1120Ctrl_t* radio);
uint8_t cc1120GetRxStatus(CC1120Ctrl_t* radio);
void trxReadWriteBurstSingle(CC1120Ctrl_t* radio, uint8_t addr, uint8_t *pData, uint16_t len);
bool cc1120_init(CC1120Ctrl_t* radio);
bool manualCalibration(CC1120Ctrl_t* radio);
bool cc1120ForceIdle(CC1120Ctrl_t* radio);
void cc1120State(CC1120Ctrl_t* radio);
bool cc1120_transmitPacket(CC1120Ctrl_t* radio, uint8_t * payload, uint8_t payloadLength);
bool cc1120_startRX(CC1120Ctrl_t* radio);
bool cc1120_checkNewPacket(CC1120Ctrl_t* radio);
bool cc1120_setFrequency(CC1120Ctrl_t* radio);

void cc1120_configGPIO(CC1120Ctrl_t *radio, uint8_t gpio_register, uint8_t gpio_config, bool outputInverted);

#ifdef  __cplusplus
}
#endif


#endif /* INC_CC1120_H_ */
