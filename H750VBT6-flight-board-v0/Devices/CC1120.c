/*
 * cc1120_spi_drv.c
 *
 *  Created on: May 24, 2020
 *      Author: ben helfrich
 */

#include <CC1120.h>
#include "stdint.h"
#include "stm32h7xx_hal.h"
#include "smartrf_CC1120_cfg.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "hardware_manager.h"

static void trxReadWriteBurstSingle(CC1120Ctrl_t* radio, uint8_t addr, uint8_t *pData, uint16_t len);


extern SPI_HandleTypeDef hspi4;
//#define FIX_PACKET_SIZE	8
#define MAX_PACKET_SIZE		128




bool cc1120_init(CC1120Ctrl_t* radio) {

	HAL_GPIO_WritePin(radio->RST_port, radio->RST_pin, SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);
	HAL_Delay(50);

	//IO CONFIGURATIONS
	uint8_t iocfg3 = SMARTRF_SETTING_IOCFG3;	cc1120SpiWriteReg(radio, CC112X_IOCFG3, &iocfg3, 0x01);
	uint8_t iocfg2 = SMARTRF_SETTING_IOCFG2;	cc1120SpiWriteReg(radio, CC112X_IOCFG2, &iocfg2, 0x01);
	uint8_t iocfg1 = SMARTRF_SETTING_IOCFG1;	cc1120SpiWriteReg(radio, CC112X_IOCFG1, &iocfg1, 0x01);
	uint8_t iocfg0 = SMARTRF_SETTING_IOCFG0;	cc1120SpiWriteReg(radio, CC112X_IOCFG0, &iocfg0, 0x01);

	//uint8_t sync3 = SMARTRF_SETTING_SYNC3;		cc1120SpiWriteReg(CC112X_SYNC3, sync3, 0x01);
	//uint8_t sync2 = SMARTRF_SETTING_SYNC2;		cc1120SpiWriteReg(CC112X_SYNC2, sync2, 0x01);
	//uint8_t sync1 = SMARTRF_SETTING_SYNC1;		cc1120SpiWriteReg(CC112X_SYNC1, sync1, 0x01);
	//uint8_t sync0 = SMARTRF_SETTING_SYNC0;		cc1120SpiWriteReg(CC112X_SYNC0, sync0, 0x01);

	uint8_t sync_cfg1 = SMARTRF_SETTING_SYNC_CFG1;	cc1120SpiWriteReg(radio,  CC112X_SYNC_CFG1, &sync_cfg1, 0x01);
	//uint8_t sync_cfg0 = SMARTRF_SETTING_SYNC_CFG0;	c1120SpiWriteReg(radio,  CC112X_SYNC_CFG0, &sync_cfg1, 0x01);

	uint8_t deviation_m = SMARTRF_SETTING_DEVIATION_M;		cc1120SpiWriteReg(radio, CC112X_DEVIATION_M, &deviation_m, 0x01);
	uint8_t modcfg_dev_e = SMARTRF_SETTING_MODCFG_DEV_E;	cc1120SpiWriteReg(radio, CC112X_MODCFG_DEV_E, &modcfg_dev_e, 0x01);
	uint8_t dcfilt_cfg = SMARTRF_SETTING_DCFILT_CFG;		cc1120SpiWriteReg(radio, CC112X_DCFILT_CFG, &dcfilt_cfg, 0x01);

	uint8_t preamble_cfg1 = SMARTRF_SETTING_PREAMBLE_CFG1;	cc1120SpiWriteReg(radio, CC112X_PREAMBLE_CFG1, &preamble_cfg1, 0x01);
	//uint8_t preamble_cfg0 = SMARTRF_SETTING_PREAMBLE_CFG0;	cc1120SpiWriteReg(radio, CC112X_PREAMBLE_CFG0, &preamble_cfg0, 0x01);

	uint8_t freq_if_cfg = SMARTRF_SETTING_FREQ_IF_CFG;		cc1120SpiWriteReg(radio, CC112X_FREQ_IF_CFG, &freq_if_cfg, 0x01);
	uint8_t iqic = SMARTRF_SETTING_IQIC;					cc1120SpiWriteReg(radio, CC112X_IQIC, &iqic, 0x01);
	uint8_t chan_bw = SMARTRF_SETTING_CHAN_BW;				cc1120SpiWriteReg(radio, CC112X_CHAN_BW, &chan_bw, 0x01);

	//uint8_t mdmcfg1 = SMARTRF_SETTING_MDMCFG1;				cc1120SpiWriteReg(radio, CC112X_MDMCFG1, &mdmcfg1, 0x01);
	uint8_t mdmcfg0 = SMARTRF_SETTING_MDMCFG0;				cc1120SpiWriteReg(radio, CC112X_MDMCFG0, &mdmcfg0, 0x01);

	uint8_t symbol_rate_2 = SMARTRF_SETTING_SYMBOL_RATE2;	cc1120SpiWriteReg(radio, CC112X_SYMBOL_RATE2, &symbol_rate_2, 0x01);
	uint8_t symbol_rate_1 = SMARTRF_SETTING_SYMBOL_RATE1;	cc1120SpiWriteReg(radio, CC112X_SYMBOL_RATE1, &symbol_rate_1, 0x01);
	uint8_t symbol_rate_0 = SMARTRF_SETTING_SYMBOL_RATE0;	cc1120SpiWriteReg(radio, CC112X_SYMBOL_RATE0, &symbol_rate_0, 0x01);

	uint8_t agc_ref = SMARTRF_SETTING_AGC_REF;				cc1120SpiWriteReg(radio, CC112X_AGC_REF, &agc_ref, 0x01);
	uint8_t agc_cs_thr = SMARTRF_SETTING_AGC_CS_THR;		cc1120SpiWriteReg(radio, CC112X_AGC_CS_THR, &agc_cs_thr, 0x01);

	//cc1120SpiWriteReg(CC112X_AGC_GAIN_ADJUST, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_AGC_CFG3, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_AGC_CFG2, 0xxx, 0x01);

	uint8_t agc_cfg1 = SMARTRF_SETTING_AGC_CFG1;			cc1120SpiWriteReg(radio, CC112X_AGC_CFG1, &agc_cfg1, 0x01);
	uint8_t agc_cfg0 = SMARTRF_SETTING_AGC_CFG0;			cc1120SpiWriteReg(radio, CC112X_AGC_CFG0, &agc_cfg0, 0x01);
	uint8_t fifo_cfg = SMARTRF_SETTING_FIFO_CFG & (FIX_PACKET_SIZE-1);			cc1120SpiWriteReg(radio, CC112X_FIFO_CFG, &fifo_cfg, 0x01);

	//cc1120SpiWriteReg(CC112X_DEV_ADDR, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_SETTLING_CFG, 0xxx, 0x01);

	uint8_t fs_cfg = SMARTRF_SETTING_FS_CFG;				cc1120SpiWriteReg(radio, CC112X_FS_CFG, &fs_cfg, 0x01);

	//cc1120SpiWriteReg(CC112X_WOR_CFG1, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_WOR_CFG0, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_WOR_EVENT0_MSB, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_WOR_EVENT0_LSB, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_PKT_CFG2, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_PKT_CFG1, 0xxx, 0x01);

	uint8_t pkt_cfg0 = SMARTRF_SETTING_PKT_CFG0;			cc1120SpiWriteReg(radio, CC112X_PKT_CFG0, &pkt_cfg0, 0x01);

	//cc1120SpiWriteReg(CC112X_RFEND_CFG1 , 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_RFEND_CFG0 , 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_PA_CFG2 , 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_PA_CFG1 , 0xxx, 0x01);

	uint8_t pa_cfg0 = SMARTRF_SETTING_PA_CFG0;				cc1120SpiWriteReg(radio, CC112X_PA_CFG0, &pa_cfg0, 0x01);
	//uint8_t pkt_len = SMARTRF_SETTING_PKT_LEN;				cc1120SpiWriteReg(radio, CC112X_PKT_LEN, &pkt_len, 0x01);
	uint8_t pkt_len = FIX_PACKET_SIZE;				cc1120SpiWriteReg(radio, CC112X_PKT_LEN, &pkt_len, 0x01);


	/* Extended Configuration Registers */

	uint8_t if_mix_cfg = SMARTRF_SETTING_IF_MIX_CFG;		cc1120SpiWriteReg(radio, CC112X_IF_MIX_CFG, &if_mix_cfg, 0x01);

	//uint8_t freqoff_cfg = 0x22;		cc1120SpiWriteReg(CC112X_FREQOFF_CFG, &freqoff_cfg, 0x01);

	uint8_t toc_cfg = SMARTRF_SETTING_TOC_CFG;				cc1120SpiWriteReg(radio, CC112X_TOC_CFG, &toc_cfg, 0x01);

	//cc1120SpiWriteReg(CC112X_MARC_SPARE, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_ECG_CFG, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_CFM_DATA_CFG, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_EXT_CTRL, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_RCCAL_FINE, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_RCCAL_COARSE, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_RCCAL_OFFSET, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_FREQOFF1, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_FREQOFF0, 0xxx, 0x01);

	uint8_t freq2 = SMARTRF_SETTING_FREQ2;					cc1120SpiWriteReg(radio, CC112X_FREQ2, &freq2, 0x01);
	uint8_t freq1 = SMARTRF_SETTING_FREQ1;					cc1120SpiWriteReg(radio, CC112X_FREQ1, &freq1, 0x01);

	//cc1120SpiWriteReg(CC112X_FREQ0, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_IF_ADC2, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_IF_ADC1, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_IF_ADC0, 0xxx, 0x01);

	uint8_t fs_dig1 = SMARTRF_SETTING_FS_DIG1;				cc1120SpiWriteReg(radio, CC112X_FS_DIG1, &fs_dig1, 0x01);
	uint8_t fs_dig0 = SMARTRF_SETTING_FS_DIG0;				cc1120SpiWriteReg(radio, CC112X_FS_DIG0, &fs_dig0, 0x01);

	//cc1120SpiWriteReg(CC112X_FS_CAL3, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_FS_CAL2, 0xxx, 0x01);

	uint8_t fs_cal1 = SMARTRF_SETTING_FS_CAL1;				cc1120SpiWriteReg(radio, CC112X_FS_CAL1, &fs_cal1, 0x01);
	uint8_t fs_cal0 = SMARTRF_SETTING_FS_CAL0;				cc1120SpiWriteReg(radio, CC112X_FS_CAL0, &fs_cal0, 0x01);

	//cc1120SpiWriteReg(CC112X_FS_CHP, 0xxx, 0x01);

	uint8_t fs_divtwo = SMARTRF_SETTING_FS_DIVTWO;			cc1120SpiWriteReg(radio, CC112X_FS_DIVTWO, &fs_divtwo, 0x01);

	//cc1120SpiWriteReg(CC112X_FS_DSM1, 0xxx, 0x01);

	uint8_t fs_dsm0 = SMARTRF_SETTING_FS_DSM0;				cc1120SpiWriteReg(radio, CC112X_FS_DSM0, &fs_dsm0, 0x01);

	//cc1120SpiWriteReg(CC112X_FS_DVC1, 0xxx, 0x01);

	uint8_t fs_dvc0 = SMARTRF_SETTING_FS_DVC0;				cc1120SpiWriteReg(radio, CC112X_FS_DVC0, &fs_dvc0, 0x01);

	//cc1120SpiWriteReg(CC112X_FS_LBI, 0xxx, 0x01);

	uint8_t fs_pfd = SMARTRF_SETTING_FS_PFD;					cc1120SpiWriteReg(radio, CC112X_FS_PFD, &fs_pfd, 0x01);
	uint8_t fs_pre = SMARTRF_SETTING_FS_PRE;					cc1120SpiWriteReg(radio, CC112X_FS_PRE, &fs_pre, 0x01);
	uint8_t fs_reg_div_cml = SMARTRF_SETTING_FS_REG_DIV_CML;	cc1120SpiWriteReg(radio, CC112X_FS_REG_DIV_CML, &fs_reg_div_cml, 0x01);
	uint8_t fs_spare = SMARTRF_SETTING_FS_SPARE;				cc1120SpiWriteReg(radio, CC112X_FS_SPARE, &fs_spare, 0x01);

	//cc1120SpiWriteReg(CC112X_FS_VCO4, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_FS_VCO3, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_FS_VCO2, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_FS_VCO1, 0xxx, 0x01);

	uint8_t fs_vco0 = SMARTRF_SETTING_FS_VCO0;				cc1120SpiWriteReg(radio, CC112X_FS_VCO0, &fs_vco0, 0x01);

	//cc1120SpiWriteReg(CC112X_GBIAS6, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_GBIAS5, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_GBIAS4, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_GBIAS3, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_GBIAS2, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_GBIAS1, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_GBIAS0, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_IFAMP, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_LNA, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_RXMIX, 0xxx, 0x01);

	uint8_t xosc5 = SMARTRF_SETTING_XOSC5;					cc1120SpiWriteReg(radio, CC112X_XOSC5, &xosc5, 0x01);

	//cc1120SpiWriteReg(CC112X_XOSC4, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_XOSC3, 0xxx, 0x01);
	//cc1120SpiWriteReg(CC112X_XOSC2, 0xxx, 0x01);

	uint8_t xosc1 = SMARTRF_SETTING_XOSC1;					cc1120SpiWriteReg(radio, CC112X_XOSC1, &xosc1, 0x01);


		//calibrate the radio
	manualCalibration(radio);

	cc1120ForceIdle(radio);

	return true;

}

uint8_t trx8BitRegAccess(CC1120Ctrl_t* radio, uint8_t accessType, uint8_t addrByte, uint8_t *pData,
		uint16_t len) {
	uint8_t readValue;
	uint8_t txBuf = (accessType | addrByte);

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, RESET); //Pull CS LOW
	while (HAL_GPIO_ReadPin(radio->RDY_port, radio->RDY_pin) == 1)
		; //Wait for SO to go low

	HAL_SPI_TransmitReceive(radio->radhspi, &txBuf, &readValue, 0x01, HAL_MAX_DELAY);

	while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
		;//Wait until done


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
	while (HAL_GPIO_ReadPin(radio->RDY_port, radio->RDY_pin) == 1)
		;
	//Wait for SO to go low
	//while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);

	//Write (accessType|addrByte)
	//store the chip status
	HAL_SPI_TransmitReceive(radio->radhspi, &txBuf, &readValue, 1, HAL_MAX_DELAY);
	//Wait until done
	while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
		;

	HAL_SPI_Transmit(radio->radhspi, &regAddr, 1, HAL_MAX_DELAY);
	while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
		;

	//write the data
	trxReadWriteBurstSingle(radio, accessType | extAddr, pData, len);

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);

	return (readValue);

}

uint8_t trxSpiCmdStrobe(CC1120Ctrl_t* radio, uint8_t cmd) {

	uint8_t rc;

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, RESET);
	while (HAL_GPIO_ReadPin(radio->RDY_port, radio->RDY_pin) == 1)
		;
	//Wait for SO to go low

	HAL_SPI_TransmitReceive(radio->radhspi, &cmd, &rc, 1, HAL_MAX_DELAY);

	while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
		;

	HAL_GPIO_WritePin(radio->CS_port, radio->CS_pin, SET);

	return (rc);
}

static void trxReadWriteBurstSingle(CC1120Ctrl_t* radio, uint8_t addr, uint8_t *pData, uint16_t len) {

	uint16_t i;
	uint8_t pushByte = 0x00;
	uint8_t rxdump;

	if (addr & RADIO_READ_ACCESS) {  //if reading from radio registers
		if (addr & RADIO_BURST_ACCESS) {
			for (i = 0; i < len; i++) //push zeros to push out data
					{

				HAL_SPI_TransmitReceive(radio->radhspi, &pushByte, pData, 1,
						HAL_MAX_DELAY);
				while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
					;
				pData++;
			}

		} else {
			HAL_SPI_TransmitReceive(radio->radhspi, &pushByte, pData, 1, HAL_MAX_DELAY);
			while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
				;
		}
	} else { // if writing to radio registers
		if (addr & RADIO_BURST_ACCESS) {
			for (i = 0; i < len; i++) { //push in data
				HAL_SPI_TransmitReceive(radio->radhspi, pData, &rxdump, 1,
						HAL_MAX_DELAY);
				while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
					;
				pData++;
			}
		} else {
			HAL_SPI_TransmitReceive(radio->radhspi, pData, &rxdump, 1, HAL_MAX_DELAY);
			while (HAL_SPI_GetState(radio->radhspi) != HAL_SPI_STATE_READY)
				;
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

uint8_t mask = 0xF0;

bool cc1120ForceIdle(CC1120Ctrl_t* radio){

	//return the radio to idle somehow
	uint8_t status;
	status = trxSpiCmdStrobe(radio, CC112X_SNOP);
	status = (status & mask);

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

#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2
void manualCalibration(CC1120Ctrl_t* radio) {

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

	do {
		cc1120SpiReadReg(radio, CC112X_MARCSTATE, &marcstate, 1);
	} while (marcstate != 0x41);

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

	do {
		cc1120SpiReadReg(radio, CC112X_MARCSTATE, &marcstate, 1);
	} while (marcstate != 0x41);

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
}


uint8_t waiting_watchdog = 0;



void cc1120State(CC1120Ctrl_t* radio, uint8_t *packetRX, uint8_t *packetToTX, uint8_t * RSSI, uint8_t * CRC_LQI){
	uint8_t status;



	uint8_t blank[FIX_PACKET_SIZE] = {0};


	cc1120_hasReceivedPacket(radio, packetRX, RSSI, CRC_LQI);

	if(memcmp( packetToTX, blank, sizeof(blank)) != 0){
		cc1120_transmitPacket(radio, packetToTX, FIX_PACKET_SIZE);
	}

	status = trxSpiCmdStrobe(radio, CC112X_SNOP);
	status = (status & mask);


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

	//uint8_t dataLength;

	/*dataLength = payloadLength+1;

	if (dataLength > MAX_PACKET_SIZE){
		return false;
	}


	cc1120SpiWriteTxFifo(radio, &payloadLength, 1);*/
	cc1120SpiWriteTxFifo(radio, payload, payloadLength);

	uint8_t status;
	status = trxSpiCmdStrobe(radio, CC112X_SNOP);
	status = (status & mask);

	if(status == CC112X_STATE_TXFIFO_ERROR) {
	  // Flush TX FIFO
	  trxSpiCmdStrobe(radio, CC112X_SFTX);

	  return false;
	}

	//this strobe will only TX the pkt length defined by the register, fifo advances
	trxSpiCmdStrobe(radio, CC112X_STX);
	return true;
}

bool cc1120_hasReceivedPacket(CC1120Ctrl_t* radio, uint8_t * rxBuf, uint8_t * RSSI, uint8_t * CRC_LQI){
	uint8_t rxbytes = 0x00;

	// Read number of bytes in RX FIFO
	cc1120SpiReadReg(radio, CC112X_NUM_RXBYTES, &rxbytes, 1);

	if(rxbytes < 1){
		return false;
	}

	//Read GPIO
	if(HAL_GPIO_ReadPin(radio->GP3_port, radio->GP3_pin)){
		  uint8_t status;
		  status = trxSpiCmdStrobe(radio, CC112X_SNOP);
		  status = (status & mask);
		  if(status == CC112X_STATE_RXFIFO_ERROR) {

			  // Flush RX FIFO
			  trxSpiCmdStrobe(radio, CC112X_SFRX);
			  return false;
		  } else {

			  // Read n bytes from RX FIFO
			  cc1120SpiReadRxFifo(radio, rxBuf, FIX_PACKET_SIZE);
			  cc1120SpiReadRxFifo(radio, RSSI, 1);
			  cc1120SpiReadRxFifo(radio, CRC_LQI, 1);

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
	}

	return false;
}

bool cc1120_startRX(CC1120Ctrl_t* radio){
	uint8_t readByte;
	 trxSpiCmdStrobe(radio, CC112X_SRX);
	 //make sure we actually switch into rx
	do {
		cc1120SpiReadReg(radio, CC112X_MARCSTATE, &readByte, 1);
	} while (readByte != 0x6D);

	return true;
}
