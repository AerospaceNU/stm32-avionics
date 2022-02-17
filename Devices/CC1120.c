/*
 * cc1120_spi_drv.c
 *
 *  Created on: May 24, 2020
 *      Author: ben helfrich
 */

#include "board_config.h"

#if defined(HAS_CC1120) || defined(HAS_CC1200)

#include "CC1120.h"
#include "radioconfig/cc1120_cc1200_defs.h"

#include "stdint.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

const float twoToThe16 = 65536.0;
//const float twoToThe20 = 1048576.0;
//const float twoToThe21 = 2097152.0;
//const float twoToThe22 = 4194304.0;
//const float twoToThe38 = 274877906944.0;
//const float twoToThe39 = 549755813888.0;


//const size_t maxValue3Bits = 8 - 1;
//const size_t maxValue4Bits = 16 - 1;
//const size_t maxValue8Bits = 256 - 1;
//const size_t maxValue20Bits = 1048576 - 1;
const size_t maxValue24Bits = 16777216 - 1;

// Initially configure radio, apply settings provided
bool cc1120_init(CC1120Ctrl_t* radio) {

	// Set SPI pins into a known good state. Lots of waiting to make sure it actually happens
	HAL_GPIO_WritePin(radio->RST_port, radio->RST_pin, SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);
	HAL_Delay(50);

	// Reset everything
	trxSpiCmdStrobe(radio, CC112X_SRES);

	// Write provided default register settings
	uint8_t writeByte;
	for(uint16_t i = 0; i < (radio->settingsSize/sizeof(registerSetting_t)); i++) {
	        writeByte = radio->settingsPtr[i].data;
	        cc1120SpiWriteReg(radio, radio->settingsPtr[i].addr, &writeByte, 1);
	}

	// Figure out what packet length/config we need to send to the radio
	uint8_t pkt_len = 0xFF;
	uint8_t pkt_cfg0 = 0x20;
	if (radio->payloadSize == 0xFF){
		pkt_len = 0xFF;
		pkt_cfg0 = 0x20;
	}else{
		pkt_len = radio->payloadSize;
		pkt_cfg0 = 0x00;
	}

	// The size for fixed length should be known on init
	cc1120SpiWriteReg(radio, CC112X_PKT_LEN, &pkt_len, 0x01);
	cc1120SpiWriteReg(radio, CC112X_PKT_CFG0, &pkt_cfg0, 0x01);

	cc1120SpiWriteReg(radio, CC112X_FIFO_CFG, &radio->payloadSize, 0x01);

	//cc1120_setFrequency(radio);

#ifdef HAS_CC1120
		//calibrate the radio per errata
	if (!manualCalibration(radio))
		return false;
#else
	trxSpiCmdStrobe(radio, CC112X_SCAL);
#endif

	cc1120ForceIdle(radio);

	radio->initialized = true;

	return true;
}

// The main tick function. Checks for new packets and transmits the waiting one, if non-zero
void cc1120State(CC1120Ctrl_t* radio){
	// Don't let anything happen if radio isn't initialized
	if (!radio->initialized)
		return;

	uint8_t status;
	static uint8_t blank[128] = {0};

	// Check for a new packet; if we've got one, dequeue it
	cc1120_checkNewPacket(radio);

	if(memcmp(radio->packetToTX, blank, radio->payloadSize) != 0){
		cc1120_transmitPacket(radio, radio->packetToTX, radio->payloadSize);
	}

	status = trxSpiCmdStrobe(radio, CC112X_SNOP);
	status = (status & CC1120_STATUS_MASK);


	switch(status){
	case CC112X_STATE_IDLE:
		cc1120_startRX(radio);
		break;
	case CC112X_STATE_RX:

		break;
	case CC112X_STATE_TX:

		break;
	case CC112X_STATE_FSTXON:

		break;
	case CC112X_STATE_CALIBRATE:
		break;
	case CC112X_STATE_SETTLING:
		break;
	case CC112X_STATE_RXFIFO_ERROR:
		trxSpiCmdStrobe(radio, CC112X_SFRX);
		break;
	case CC112X_STATE_TXFIFO_ERROR:
		trxSpiCmdStrobe(radio, CC112X_SFTX);
		break;
	default:
		trxSpiCmdStrobe(radio, CC112X_SIDLE);
	}

}

bool cc1120_transmitPacket(CC1120Ctrl_t* radio, uint8_t * payload, uint8_t payloadLength){

	uint8_t packetLength;

	packetLength = payloadLength+1;

	if (packetLength > MAX_PACKET_SIZE){
		return false;
	}

	if (radio->payloadSize==0xFF){
		cc1120SpiWriteTxFifo(radio, &payloadLength, 1);
	}


	cc1120SpiWriteTxFifo(radio, payload, payloadLength);

	uint8_t status;
	status = trxSpiCmdStrobe(radio, CC112X_SNOP);
	status = (status & CC1120_STATUS_MASK);

	if(status == CC112X_STATE_TXFIFO_ERROR) {
	  // Flush TX FIFO
	  trxSpiCmdStrobe(radio, CC112X_SFTX);

	  return false;
	}

	//this strobe will only TX the pkt length defined by the register, fifo advances
	trxSpiCmdStrobe(radio, CC112X_STX);
	return true;
}

bool cc1120_checkNewPacket(CC1120Ctrl_t* radio){
	uint8_t rxbytes = 0x00;
	uint8_t rxBuffer[128] = {0};

	// Read number of bytes in RX FIFO
	cc1120SpiReadReg(radio, CC112X_NUM_RXBYTES, &rxbytes, 1);

	if(rxbytes < 1){
		return false;
	}
	//printf("RX FIFO has %u\n", rxbytes);
	uint8_t rxfirst = 0x00;
	uint8_t rxlast = 0x00;
	cc1120SpiReadReg(radio, CC112X_RXFIRST, &rxfirst, 1);
	cc1120SpiReadReg(radio, CC112X_RXLAST, &rxlast, 1);
	//printf("RXFIRST %u TXFIRST %u\n", rxfirst, rxlast);

	//Read GPIO
	if(HAL_GPIO_ReadPin(radio->GP3_port, radio->GP3_pin) == GPIO_PIN_RESET){ // PKT_SYNC_RXTX, maybe, should be low when packet RX is done
		//printf("GP3 is low, so not currently receiving pkt\n");

		// This should do the same thing as the below check
		uint8_t state;
		cc1120SpiReadReg(radio, CC112X_MARCSTATE, &state, 1);
		if(state == 0x11) { // TODO don't hardcode the marcstate status
			trxSpiCmdStrobe(radio, CC112X_SFRX);
			return false;
		}

		uint8_t status;
		status = trxSpiCmdStrobe(radio, CC112X_SNOP);
		status = (status & CC1120_STATUS_MASK);
		if(status == CC112X_STATE_RXFIFO_ERROR) {
			// Flush RX FIFO
		    trxSpiCmdStrobe(radio, CC112X_SFRX);
		    return false;
		}

		// Read n bytes from RX FIFO
		//cc1120SpiReadReg(raido, CC112X_RXFIRST)

		if (radio->packetCfg == 0x20){
            cc1120SpiReadRxFifo(radio, rxBuffer, rxbytes);
			memcpy(radio->packetRX, rxBuffer, rxbytes-2);
			radio->RSSI = rxBuffer[rxbytes-2];
			radio->CRC_LQI = rxBuffer[rxbytes-1];
		} else {
		    cc1120SpiReadRxFifo(radio, radio->packetRX, radio->payloadSize);
			cc1120SpiReadRxFifo(radio, &radio->RSSI, 1);
			cc1120SpiReadRxFifo(radio, &radio->CRC_LQI, 1);
		}



		return true;

		  // Check CRC ok (CRC_OK: bit7 in second status byte)
		  // This assumes status bytes are appended in RX_FIFO
		  // (PKT_CFG1.APPEND_STATUS = 1)
		  // If CRC is disabled the CRC_OK field will read 1
		  /*if(rxBuffer[rxbytes - 1] & 0x80) {
			  //RingBuffer_Write(&radio->buf.RXbuf, rxBuffer, pktLen);
			  return true;
		  }*/

	}

	return false;
}

bool cc1120ForceIdle(CC1120Ctrl_t* radio){

	//return the radio to idle somehow
	uint8_t status;
	status = trxSpiCmdStrobe(radio, CC112X_SNOP);
	status = (status & CC1120_STATUS_MASK);

	switch(status){
	case CC112X_STATE_IDLE:
		return true;
		break;
	case CC112X_STATE_RX:
		trxSpiCmdStrobe(radio, CC112X_SIDLE);
		return true;
		break;
	case CC112X_STATE_TX:
		trxSpiCmdStrobe(radio, CC112X_SIDLE);
		return true;
		break;
	case CC112X_STATE_FSTXON:
		trxSpiCmdStrobe(radio, CC112X_SIDLE);
		return true;
				break;
	case CC112X_STATE_CALIBRATE:
		return false;
		break;
	case CC112X_STATE_SETTLING:
		return false;
		break;
	case CC112X_STATE_RXFIFO_ERROR:
		trxSpiCmdStrobe(radio, CC112X_SFRX);
		return true;
		break;
	case CC112X_STATE_TXFIFO_ERROR:
		trxSpiCmdStrobe(radio, CC112X_SFTX);
		return true;
		break;

	default:
		trxSpiCmdStrobe(radio, CC112X_SIDLE);
		return true;
	}

	return false;
}

bool cc1120_startRX(CC1120Ctrl_t* radio){
	uint8_t readByte;
	 trxSpiCmdStrobe(radio, CC112X_SRX);
	 //make sure we actually switch into rx
	uint32_t startMS = HAL_GetTick();
	do {
		cc1120SpiReadReg(radio, CC112X_MARCSTATE, &readByte, 1);
	} while (readByte != 0x6D && HAL_GetTick() - startMS < 1000);

	return true;
}

#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2
bool manualCalibration(CC1120Ctrl_t* radio) {

	uint8_t original_fs_cal2;
	uint8_t calResults_for_vcdac_start_high[3];
	uint8_t calResults_for_vcdac_start_mid[3];
	uint8_t marcstate;
	uint8_t writeByte;

	cc1120SpiReadReg(radio, CC112X_MARCSTATE, &marcstate, 1);

	// 1) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
	writeByte = 0x00;
	cc1120SpiWriteReg(radio, CC112X_FS_VCO2, &writeByte, 1);

	// 2) Start with high VCDAC (original VCDAC_START + 2):
	cc1120SpiReadReg(radio, CC112X_FS_CAL2, &original_fs_cal2, 1);
	writeByte = original_fs_cal2 + VCDAC_START_OFFSET;
	cc1120SpiWriteReg(radio, CC112X_FS_CAL2, &writeByte, 1);

	// 3) Calibrate and wait for calibration to be done
	//   (radio back in IDLE state)
	trxSpiCmdStrobe(radio, CC112X_SCAL);

	uint32_t startMS = HAL_GetTick();
	do {
		cc1120SpiReadReg(radio, CC112X_MARCSTATE, &marcstate, 1);
	} while (marcstate != 0x41 && HAL_GetTick() - startMS < 5000);
	if (marcstate != 0x41)
		return false;

	// 4) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with
	//    high VCDAC_START value
	cc1120SpiReadReg(radio, CC112X_FS_VCO2,
			&calResults_for_vcdac_start_high[FS_VCO2_INDEX], 1);
	cc1120SpiReadReg(radio, CC112X_FS_VCO4,
			&calResults_for_vcdac_start_high[FS_VCO4_INDEX], 1);
	cc1120SpiReadReg(radio, CC112X_FS_CHP,
			&calResults_for_vcdac_start_high[FS_CHP_INDEX], 1);

	// 5) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
	writeByte = 0x00;
	cc1120SpiWriteReg(radio, CC112X_FS_VCO2, &writeByte, 1);

	// 6) Continue with mid VCDAC (original VCDAC_START):
	writeByte = original_fs_cal2;
	cc1120SpiWriteReg(radio, CC112X_FS_CAL2, &writeByte, 1);

	// 7) Calibrate and wait for calibration to be done
	//   (radio back in IDLE state)
	trxSpiCmdStrobe(radio, CC112X_SCAL);

	startMS = HAL_GetTick();
	do {
		cc1120SpiReadReg(radio, CC112X_MARCSTATE, &marcstate, 1);
	} while (marcstate != 0x41 && HAL_GetTick() - startMS < 5000);
	if (marcstate != 0x41)
		return false;

	// 8) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained
	//    with mid VCDAC_START value
	cc1120SpiReadReg(radio, CC112X_FS_VCO2,
			&calResults_for_vcdac_start_mid[FS_VCO2_INDEX], 1);
	cc1120SpiReadReg(radio, CC112X_FS_VCO4,
			&calResults_for_vcdac_start_mid[FS_VCO4_INDEX], 1);
	cc1120SpiReadReg(radio, CC112X_FS_CHP,
			&calResults_for_vcdac_start_mid[FS_CHP_INDEX], 1);

	// 9) Write back highest FS_VCO2 and corresponding FS_VCO
	//    and FS_CHP result
	if (calResults_for_vcdac_start_high[FS_VCO2_INDEX]
			> calResults_for_vcdac_start_mid[FS_VCO2_INDEX]) {
		writeByte = calResults_for_vcdac_start_high[FS_VCO2_INDEX];
		cc1120SpiWriteReg(radio, CC112X_FS_VCO2, &writeByte, 1);
		writeByte = calResults_for_vcdac_start_high[FS_VCO4_INDEX];
		cc1120SpiWriteReg(radio, CC112X_FS_VCO4, &writeByte, 1);
		writeByte = calResults_for_vcdac_start_high[FS_CHP_INDEX];
		cc1120SpiWriteReg(radio, CC112X_FS_CHP, &writeByte, 1);
	} else {
		writeByte = calResults_for_vcdac_start_mid[FS_VCO2_INDEX];
		cc1120SpiWriteReg(radio, CC112X_FS_VCO2, &writeByte, 1);
		writeByte = calResults_for_vcdac_start_mid[FS_VCO4_INDEX];
		cc1120SpiWriteReg(radio, CC112X_FS_VCO4, &writeByte, 1);
		writeByte = calResults_for_vcdac_start_mid[FS_CHP_INDEX];
		cc1120SpiWriteReg(radio, CC112X_FS_CHP, &writeByte, 1);
	}

	return true;
}

////! Set power in db. Make sure to respect your PA maximum!
//void cc1120_SetOutputPower(CC1120Ctrl_t *radio, uint8_t powerDbM) {
//#ifdef HAS_CC1190
//	if(powerDbM > 10) powerDbM = 10;
//#endif
//
//	// output = (ramp + 1)/2-18
//	// (output+18)*2 - 1 = ramp
//	uint8_t power = (powerDmB + 18) * 2 - 1;
//
//	// Register must be between 3 and 64
//	if(power < 3) power = 3;
//	if(power > 64) power = 64;
//	cc1120SpiReadReg(radio, CC112X_MARCSTATE, &state, 1);
//	uint8_t reg = 0b01000000; // bit 7 set to enable ramping
//	reg |= power; // Lower 6 bits are power
//	cc1120SpiWriteReg(radio, PA_CFG2, &reg, 1);
//}

enum CC1120_GPIO_CFG {
	RXFIFO_THR_PKT = 1, // Associated to the RX FIFO. Asserted when the RX FIFO is filled above
	// FIFO_CFG.FIFO_THR or the end of packet is reached. De-asserted
	// when the RX FIFO is empty

	// RX: Asserted when sync word has been received and de-asserted at the
	// end of the packet. Will de-assert when the optional address and/or
	// length check fails or the RX FIFO overflows/underflows.
	// TX: Asserted when sync word has been sent, and de-asserted at the end
	// of the packet. Will de-assert if the TX FIFO underflows/overflows
	PKT_SYNC_RXTX = 6,

	LNA_PD = 24, // control external LNA. Active low.
	PA_PD = 25, // External PA, active low
	RX0TX1_CFG = 26, // 0 in idle or rx, 1 in transmit

	HIGHZ = 48 // High-impedance, if we need this to not do anything
};

//! Configure a GPIO pin. register should be CC112X_IOCFG3 or CC112X_IOCFG2 or CC112X_IOCFG1 or CC112X_IOCFG0
//! gpio_config from CC1120_GPIO_CFG, and outputInverted flips the logic of the pin
void cc1120_configGPIO(CC1120Ctrl_t *radio, uint8_t gpio_register, uint8_t gpio_config, bool outputInverted) {
	uint8_t reg = 0;
	reg |= (outputInverted ? 1 : 0) << 6; // invert is bit 6, 7th from bottom
	reg |= gpio_config; // lower 6 are output selection
	cc1120SpiWriteReg(radio, CC112X_IOCFG3, &reg, 1);
}

uint8_t trx8BitRegAccess(CC1120Ctrl_t* radio, uint8_t accessType, uint8_t addrByte, uint8_t *pData,
		uint16_t len) {
	uint8_t readValue;
	uint8_t txBuf = (accessType | addrByte);

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, RESET); //Pull CS LOW
	uint32_t startMS = HAL_GetTick();
	while (HAL_GPIO_ReadPin(radio->MISO_port, radio->MISO_pin) == 1 && HAL_GetTick() - startMS < 1000)
		; //Wait for SO to go low

	HAL_SPI_TransmitReceive(radio->radhspi, &txBuf, &readValue, 0x01, HAL_MAX_DELAY);

	trxReadWriteBurstSingle(radio, (accessType | addrByte), pData, len); //write the data

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET); //Pull CS High

	return (readValue);

}

uint8_t trx16BitRegAccess(CC1120Ctrl_t* radio, uint8_t accessType, uint8_t extAddr, uint8_t regAddr,
		uint8_t *pData, uint16_t len) {

	uint8_t readValue;
	uint8_t txBuf = (accessType | extAddr);

	//Pull CS LOW
	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, RESET);
	uint32_t startMS = HAL_GetTick();
	while (HAL_GPIO_ReadPin(radio->MISO_port, radio->MISO_pin) == 1 && HAL_GetTick() - startMS < 1000)
		;
	//Wait for SO to go low
	//while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);

	//Write (accessType|addrByte)
	//store the chip status
	HAL_SPI_TransmitReceive(radio->radhspi, &txBuf, &readValue, 1, HAL_MAX_DELAY);

	HAL_SPI_Transmit(radio->radhspi, &regAddr, 1, HAL_MAX_DELAY);

	//write the data
	trxReadWriteBurstSingle(radio, accessType | extAddr, pData, len);

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);

	return (readValue);

}

uint8_t trxSpiCmdStrobe(CC1120Ctrl_t* radio, uint8_t cmd) {

	uint8_t rc;

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, RESET);
	uint32_t startMS = HAL_GetTick();
	while (HAL_GPIO_ReadPin(radio->MISO_port, radio->MISO_pin) == 1 && HAL_GetTick() - startMS < 1000)
		;
	//Wait for SO to go low

	HAL_SPI_TransmitReceive(radio->radhspi, &cmd, &rc, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);

	return (rc);
}

void trxReadWriteBurstSingle(CC1120Ctrl_t* radio, uint8_t addr, uint8_t *pData, uint16_t len) {

	uint16_t i;
	uint8_t pushByte = 0x00;
	uint8_t rxdump;

	if (addr & RADIO_READ_ACCESS) {  //if reading from radio registers
		if (addr & RADIO_BURST_ACCESS) {
			for (i = 0; i < len; i++) //push zeros to push out data
					{

				HAL_SPI_TransmitReceive(radio->radhspi, &pushByte, pData, 1,
						HAL_MAX_DELAY);
				pData++;
			}

		} else {
			HAL_SPI_TransmitReceive(radio->radhspi, &pushByte, pData, 1, HAL_MAX_DELAY);
		}
	} else { // if writing to radio registers
		if (addr & RADIO_BURST_ACCESS) {
			for (i = 0; i < len; i++) { //push in data
				HAL_SPI_TransmitReceive(radio->radhspi, pData, &rxdump, 1,
						HAL_MAX_DELAY);
				pData++;
			}
		} else {
			HAL_SPI_TransmitReceive(radio->radhspi, pData, &rxdump, 1, HAL_MAX_DELAY);
		}
	}

	return;
}

uint8_t cc1120SpiReadReg(CC1120Ctrl_t* radio, uint16_t addr, uint8_t *pData, uint8_t len) {

	uint8_t tempExt = (uint8_t) (addr >> 8);
	uint8_t tempAddr = (uint8_t) (addr & 0x00FF);
	uint8_t rc = 0;

	//If this is a TX FIFO access, return that the chip is not ready
	if ((CC112X_SINGLE_TXFIFO <= tempAddr) && (tempExt == 0))
		return STATUS_CHIP_RDYn_BM;

	//if its a regular register
	if (!tempExt) {
		rc = trx8BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_READ_ACCESS),
				tempAddr, pData, len);

		//if its an extended access register
	} else if (tempExt == 0x2F) {
		rc = trx16BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_READ_ACCESS),
				tempExt, tempAddr, pData, len);
	}
	return (rc);
}

uint8_t cc1120SpiWriteReg(CC1120Ctrl_t* radio, uint16_t addr, uint8_t *pData, uint8_t len) {
	uint8_t tempExt = (uint8_t) (addr >> 8);
	uint8_t tempAddr = (uint8_t) (addr & 0x00FF);
	uint8_t rc = 0;

	//DEBUG



	if (!tempExt) {
		rc = trx8BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_WRITE_ACCESS),
				tempAddr, pData, len);

		//DEBUG

		/*uint8_t readByte = 0;
	uint8_t writeByte = *pData;
	cc1120SpiReadReg(radio, addr, &readByte, 0x01);
		testread = readByte;
		if (readByte != writeByte) {
			readByte = 0;
		}*/

	} else if (tempExt == 0x2F) {
		rc = trx16BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_WRITE_ACCESS),
				tempExt, tempAddr, pData, len);
	}
	return (rc);
}

uint8_t cc1120SpiWriteTxFifo(CC1120Ctrl_t* radio, uint8_t *pData, uint8_t len) {
	uint8_t rc;
	rc = trx8BitRegAccess(radio, (RADIO_BURST_ACCESS | RADIO_WRITE_ACCESS),
			CC112X_BURST_TXFIFO, pData, len);
	return (rc);
}
uint8_t cc1120SpiReadRxFifo(CC1120Ctrl_t* radio, uint8_t *pData, uint8_t len) {
	uint8_t rc;
	rc = trx8BitRegAccess(radio, 0x00, CC112X_BURST_RXFIFO, pData, len);
	return (rc);
}

uint8_t cc1120GetTxStatus(CC1120Ctrl_t* radio) {
	return (trxSpiCmdStrobe(radio, CC112X_SNOP));
}

uint8_t cc1120GetRxStatus(CC1120Ctrl_t* radio) {
	return (trxSpiCmdStrobe(radio, CC112X_SNOP | RADIO_READ_ACCESS));
}

/*
//function courtesy of USC
//run before self-cal!
bool cc1120_setFrequency(CC1120Ctrl_t* radio)
{

    // these are only generated by SmartRF studio.


	//uint8_t adc1 = 0xEE; cc1120SpiWriteReg(radio, CC112X_IF_ADC1,0xEE,1);
	//uint8_t adc0 = 0x10; cc1120SpiWriteReg(radio, CC112X_IF_ADC0,0x10,1);

	uint8_t dig1 = SMARTRF_SETTING_FS_DIG1; cc1120SpiWriteReg(radio, CC112X_FS_DIG1,&dig1,1);
	uint8_t dig0 = SMARTRF_SETTING_FS_DIG0; cc1120SpiWriteReg(radio, CC112X_FS_DIG0,&dig0,1);

	uint8_t cal1 = SMARTRF_SETTING_FS_CAL1; cc1120SpiWriteReg(radio, CC112X_FS_CAL1,&cal1,1);
	uint8_t cal0 = SMARTRF_SETTING_FS_CAL0; cc1120SpiWriteReg(radio, CC112X_FS_CAL0,&cal0,1);

	uint8_t divtwo = SMARTRF_SETTING_FS_DIVTWO; cc1120SpiWriteReg(radio, CC112X_FS_DIVTWO,&divtwo,1);

	uint8_t dsm0 = SMARTRF_SETTING_FS_DSM0; cc1120SpiWriteReg(radio, CC112X_FS_DSM0,&dsm0,1);

	//uint8_t dvc1 = 0xFF; cc1120SpiWriteReg(radio, CC112X_FS_DVC1,&dvc1,1);
	uint8_t dvc0 = SMARTRF_SETTING_FS_DVC0; cc1120SpiWriteReg(radio, CC112X_FS_DVC0,&dvc0,1);

	uint8_t pfd = SMARTRF_SETTING_FS_PFD; cc1120SpiWriteReg(radio, CC112X_FS_PFD,&pfd,1);
	uint8_t pre = SMARTRF_SETTING_FS_PRE; cc1120SpiWriteReg(radio, CC112X_FS_PRE,&pre,1);

	uint8_t reg_div_cml = SMARTRF_SETTING_FS_REG_DIV_CML; cc1120SpiWriteReg(radio, CC112X_FS_REG_DIV_CML,&reg_div_cml,1);

	uint8_t spare = SMARTRF_SETTING_FS_SPARE; cc1120SpiWriteReg(radio, CC112X_FS_SPARE,&spare,1);

	uint8_t vco0 = SMARTRF_SETTING_FS_VCO0; cc1120SpiWriteReg(radio, CC112X_FS_VCO0,&vco0,1);

	uint8_t xosc5 = SMARTRF_SETTING_XOSC5; cc1120SpiWriteReg(radio, CC112X_XOSC5,&xosc5,1);
	uint8_t xosc1 = SMARTRF_SETTING_XOSC1; cc1120SpiWriteReg(radio, CC112X_XOSC1,&xosc1,1);

	//uint8_t ifamp = 0x03; cc1120SpiWriteReg(radio, CC112X_IFAMP,&ifamp,1);

	//cc1120 errata eliminates need for this operation
#if (!HAS_CC1120)
    if(radio->band == BAND_820_960MHz)
    {
    	cc1120SpiWriteReg(radio, CC112X_FS_DIG0,0x55,1);
    	cc1120SpiWriteReg(radio, CC112X_FS_DVC0,0x17,1);
    	cc1120SpiWriteReg(radio, CC112X_IFAMP,0x09,1);
    }
    else if(radio->band == BAND_410_480MHz || radio->band == BAND_164_192MHz)
    {
    	cc1120SpiWriteReg(radio, CC112X_FS_DIG0,0x5F,1);
    	cc1120SpiWriteReg(radio, CC112X_FS_DVC0,0x17,1);
    	cc1120SpiWriteReg(radio, CC112X_IFAMP,0x0D,1);
    }
    else
    {
    	//not public info for other bands
    	cc1120SpiWriteReg(radio, CC112X_FS_DIG0,0x50,1);
    	cc1120SpiWriteReg(radio, CC112X_FS_DVC0,0x0F,1);
    	cc1120SpiWriteReg(radio, CC112X_IFAMP,0x0D,1);
    }
#endif

    // convert band to LO Divider value.
    // Most of the bands just multiply the register value by 2, but nooo, not BAND_136_160MHz.
    uint8_t loDividerValue;
    if(radio->band == BAND_136_160MHz)
    {
        loDividerValue = 24;
    }
    else
    {
        loDividerValue = (radio->band) * 2;
    }

    // program band (also enable FS out of lock detector, which is useful for testing)
    uint8_t fs_cfg = (1 << FS_CFG_FS_LOCK_EN) | ((radio->band) << FS_CFG_FSD_BANDSELECT);
    cc1120SpiWriteReg(radio, CC112X_FS_CFG, &fs_cfg, 1);

    // equation derived from user guide section 9.12
    float exactFreqValue = (twoToThe16 * radio->frequencyHz * (float)loDividerValue) / CC1120_OSC_FREQ;

    if (maxValue24Bits < exactFreqValue){
    	exactFreqValue = maxValue24Bits;
    }

    uint32_t actualFreqValue = (uint32_t)(exactFreqValue);

    // program frequency registers

    uint8_t freq2 = (uint8_t)((actualFreqValue >> 16) & 0xFF);
    uint8_t freq1 = (uint8_t)((actualFreqValue >> 8) & 0xFF);
    uint8_t freq0 = (uint8_t)((actualFreqValue & 0xFF));

    cc1120SpiWriteReg(radio, CC112X_FREQ2, &freq2, 1);
    cc1120SpiWriteReg(radio, CC112X_FREQ1, &freq1, 1);
    cc1120SpiWriteReg(radio, CC112X_FREQ0, &freq0, 1);

#if CC1200_DEBUG
    debugStream->printf("Setting radio frequency, requested %.00f Hz, setting FREQ = 0x%" PRIx32 "\n",
                        frequencyHz, actualFreqValue);

    // sanity check: calculate actual frequency
    float actualFrequency = (static_cast<float>(actualFreqValue) * CC1200_OSC_FREQ) / (twoToThe16 * static_cast<float>(loDividerValue));

    debugStream->printf("This yields an actual frequency of %.00f Hz\n", actualFrequency);
#endif
    return true;
}
*/


//TODO: Resolve error here between cc1200 and cc1120
/*
bool cc1120_startContinuousTX(CC1120Ctrl_t *radio){
	uint8_t preamble_cfg1 = 0x00; 	cc1120SpiWriteReg(radio, CC112X_PREAMBLE_CFG1, &preamble_cfg1, 1);
	uint8_t mdmcfg1 = 0x06;			cc1120SpiWriteReg(radio, CC112X_MDMCFG1, &mdmcfg1, 1);
	uint8_t pkt_cfg2 = 0x05;		cc1120SpiWriteReg(radio, CC112X_PKT_CFG2, &pkt_cfg2, 1);
	uint8_t pkt_cfg1 = 0x00;		cc1120SpiWriteReg(radio, CC112X_PKT_CFG1, &pkt_cfg1, 1);
	uint8_t pkt_len = 0x03;			cc1120SpiWriteReg(radio, CC112X_PKT_LEN, &pkt_len, 1);
	uint8_t cfm_data_cfg = 0x01;	cc1120SpiWriteReg(radio, CC112X_CFM_DATA_CFG, &cfm_data_cfg, 1);

	trxSpiCmdStrobe(radio, CC112X_STX);

	return true;
}

bool cc1120_stopContinuousTX(CC1120Ctrl_t *radio){
	uint8_t preamble_cfg1_def = 0x14;	cc1120SpiWriteReg(radio, CC112X_PREAMBLE_CFG1, &preamble_cfg1_def, 1);
	uint8_t mdmcfg1_def = 0x46;			cc1120SpiWriteReg(radio, CC112X_MDMCFG1, &mdmcfg1_def, 1);
	uint8_t pkt_cfg2_def = 0x05;		cc1120SpiWriteReg(radio, CC112X_PKT_CFG2, &pkt_cfg2_def, 1);
	uint8_t pkt_cfg1_def = 0x00;		cc1120SpiWriteReg(radio, CC112X_PKT_CFG1, &pkt_cfg1_def, 1);
	uint8_t pkt_len_def = 0xFF;			cc1120SpiWriteReg(radio, CC112X_PKT_LEN, &pkt_len_def, 1);
	uint8_t cfm_data_cfg_def = 0x00;	cc1120SpiWriteReg(radio, CC112X_CFM_DATA_CFG, &cfm_data_cfg_def, 1);

	trxSpiCmdStrobe(radio, CC112X_SIDLE);
	return true;
}
*/

#endif

