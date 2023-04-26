/***************************************************************
 *  SmartRF Studio(tm) Export
 *
 *  Radio register settings specifed with address, value
 *
 *  RF device: CC1200
 *
 ***************************************************************/

/*
 * Values affecting receive sensitivity:
 *
 *
 *	PQT		- sets how good the preamble needs to look before
 *			  we start looking for a sync word
 *	SYNC_THR	- sets how good the sync needs to be before we
 *			  start decoding a packet
 */

/* Values depending on data rate
 *
 *	DCFILT_BW_SETTLE
 *	DCFILT_BW
 */

#ifndef AO_CC1200_AGC_GAIN_ADJUST
#define AO_CC1200_AGC_GAIN_ADJUST -94
#endif

	CC1200_IOCFG2, 0x06,	/* GPIO2 IO Pin Configuration */
	CC1200_SYNC3, 0xD3, /* Sync Word Configuration [23:16] */
	CC1200_SYNC2, 0x91, /* Sync Word Configuration [23:16] */
	CC1200_SYNC1, 0xD3, /* Sync Word Configuration [15:8] */
	CC1200_SYNC0, 0x91, /* Sync Word Configuration [7:0] */
	CC1200_SYNC_CFG1,	/* Sync Word Detection Configuration Reg. 1 */
	((CC1200_SYNC_CFG1_SYNC_MODE_16_BITS << CC1200_SYNC_CFG1_SYNC_MODE) |
	 (11 << CC1200_SYNC_CFG1_SYNC_THR)),
	CC1200_SYNC_CFG0, /* Sync Word Detection Configuration Reg. 0 */
	((1 << CC1200_SYNC_CFG0_AUTO_CLEAR) |
	 (0 << CC1200_SYNC_CFG0_RX_CONFIG_LIMITATION) |
	 (1 << CC1200_SYNC_CFG0_PQT_GATING_EN) |
	 (0 << CC1200_SYNC_CFG0_EXT_SYNC_DETECT) |
	 (CC1200_SYNC_CFG0_SYNC_STRICT_SYNC_CHECK_DISABLED << CC1200_SYNC_CFG0_SYNC_STRICT_SYNC_CHECK)),
	CC1200_DEVIATION_M, 0x50, /* Frequency Deviation Configuration */
	CC1200_DCFILT_CFG, 0x5d,  /* Digital DC Removal Configuration */
	CC1200_PREAMBLE_CFG0,	  /* Preamble Detection Configuration Reg. 0 */
	((1 << CC1200_PREAMBLE_CFG0_PQT_EN) |
	 (CC1200_PREAMBLE_CFG0_PQT_VALID_TIMEOUT_11 << CC1200_PREAMBLE_CFG0_PQT_VALID_TIMEOUT) |
	 (15 << CC1200_PREAMBLE_CFG0_PQT)),
	CC1200_IQIC, 0xcb,		   /* Digital Image Channel Compensation Configuration */
	CC1200_CHAN_BW, 0x11,	   /* Channel Filter Configuration */
	CC1200_MDMCFG1, 0x40,	   /* General Modem Parameter Configuration Reg. 1 */
	CC1200_MDMCFG0, 0x05,	   /* General Modem Parameter Configuration Reg. 0 */
	CC1200_SYMBOL_RATE2, 0x93, /* Symbol Rate Configuration Exponent and Mantissa [1.. */
	CC1200_AGC_REF, 0x27,	   /* AGC Reference Level Configuration */
	CC1200_AGC_CS_THR, 0xec,   /* Carrier Sense Threshold Configuration */
	CC1200_AGC_GAIN_ADJUST,	   /* RSSI adjustment */
	(uint16_t)AO_CC1200_AGC_GAIN_ADJUST,
	CC1200_AGC_CFG1, 0x51, /* Automatic Gain Control Configuration Reg. 1 */
	CC1200_AGC_CFG0, 0x87, /* Automatic Gain Control Configuration Reg. 0 */
	CC1200_FIFO_CFG, 0x40, /* FIFO Configuration */
	CC1200_SETTLING_CFG,   /* Frequency Synthesizer Calibration and Settling Configuration */
	((CC1200_SETTLING_CFG_FS_AUTOCAL_EVERY_4TH_TIME << CC1200_SETTLING_CFG_FS_AUTOCAL) |
	 (CC1200_SETTLING_CFG_LOCK_TIME_75_30 << CC1200_SETTLING_CFG_LOCK_TIME) |
	 (CC1200_SETTLING_CFG_FSREG_TIME_60 << CC1200_SETTLING_CFG_FSREG_TIME)),
	CC1200_FS_CFG, /* Frequency Synthesizer Configuration */
	((1 << CC1200_FS_CFG_LOCK_EN) |
	 (CC1200_FS_CFG_FSD_BANDSELECT_410_480 << CC1200_FS_CFG_FSD_BANDSELECT)),
	CC1200_PKT_CFG2, /* Packet Configuration Reg. 2 */
	((0 << CC1200_PKT_CFG2_FG_MODE_EN) |
	 (CC1200_PKT_CFG2_CCA_MODE_ALWAYS_CLEAR << CC1200_PKT_CFG2_CCA_MODE) |
	 (CC1200_PKT_CFG2_PKT_FORMAT_NORMAL << CC1200_PKT_CFG2_PKT_FORMAT)),
	CC1200_PKT_CFG1, /* Packet Configuration Reg. 1 */
	((1 << CC1200_PKT_CFG1_FEC_EN) |
	 (1 << CC1200_PKT_CFG1_WHITE_DATA) |
	 (0 << CC1200_PKT_CFG1_PN9_SWAP_EN) |
	 (CC1200_PKT_CFG1_ADDR_CHECK_CFG_NONE << CC1200_PKT_CFG1_ADDR_CHECK_CFG) |
	 (CC1200_PKT_CFG1_CRC_CFG_CRC16_INIT_ONES << CC1200_PKT_CFG1_CRC_CFG) |
	 (1 << CC1200_PKT_CFG1_APPEND_STATUS)),
	CC1200_PKT_CFG0, /* Packet Configuration Reg. 0 */
	((CC1200_PKT_CFG0_LENGTH_CONFIG_FIXED << CC1200_PKT_CFG0_LENGTH_CONFIG) |
	 (0 << CC1200_PKT_CFG0_PKG_BIT_LEN) |
	 (0 << CC1200_PKT_CFG0_UART_MODE_EN) |
	 (0 << CC1200_PKT_CFG0_UART_SWAP_EN)),
	CC1200_RFEND_CFG1, /* RFEND Configuration Reg. 1 */
	((CC1200_RFEND_CFG1_RXOFF_MODE_IDLE << CC1200_RFEND_CFG1_RXOFF_MODE) |
	 (CC1200_RFEND_CFG1_RX_TIME_INFINITE << CC1200_RFEND_CFG1_RX_TIME) |
	 (0 << CC1200_RFEND_CFG1_RX_TIME_QUAL)),
	CC1200_RFEND_CFG0, /* RFEND Configuration Reg. 0 */
	((0 << CC1200_RFEND_CFG0_CAL_END_WAKE_UP_EN) |
	 (CC1200_RFEND_CFG0_TXOFF_MODE_IDLE << CC1200_RFEND_CFG0_TXOFF_MODE) |
	 (1 << CC1200_RFEND_CFG0_TERM_ON_BAD_PACKET_EN) |
	 (0 << CC1200_RFEND_CFG0_ANT_DIV_RX_TERM_CFG)),
	CC1200_PA_CFG1, 0x3f,	  /* Power Amplifier Configuration Reg. 1 */
	CC1200_PA_CFG0, 0x53,	  /* Power Amplifier Configuration Reg. 0 */
	CC1200_PKT_LEN, 0xff,	  /* Packet Length Configuration */
	CC1200_IF_MIX_CFG, 0x1c,  /* IF Mix Configuration */
	CC1200_FREQOFF_CFG, 0x22, /* Frequency Offset Correction Configuration */
	CC1200_MDMCFG2,			  /* General Modem Parameter Configuration Reg. 2 */
	((CC1200_MDMCFG2_ASK_SHAPE_8 << CC1200_MDMCFG2_ASK_SHAPE) |
	 (CC1200_MDMCFG2_SYMBOL_MAP_CFG_MODE_0 << CC1200_MDMCFG2_SYMBOL_MAP_CFG) |
	 (CC1200_MDMCFG2_UPSAMPLER_P_8 << CC1200_MDMCFG2_UPSAMPLER_P) |
	 (0 << CC1200_MDMCFG2_CFM_DATA_EN)),
	CC1200_MDMCFG0, /* General Modem Parameter Configuration Reg. 0 */
	((0 << CC1200_MDMCFG0_TRANSPARENT_MODE_EN) |
	 (0 << CC1200_MDMCFG0_TRANSPARENT_INTFACT) |
	 (0 << CC1200_MDMCFG0_DATA_FILTER_EN) |
	 (1 << CC1200_MDMCFG0_VITERBI_EN)),
	CC1200_TOC_CFG, /* Timing Offset Correction Configuration */
	((CC1200_TOC_CFG_TOC_LIMIT_2 << CC1200_TOC_CFG_TOC_LIMIT) |
	 (CC1200_TOC_CFG_TOC_PRE_SYNC_BLOCKLEN_6_16 << CC1200_TOC_CFG_TOC_PRE_SYNC_BLOCKLEN) |
	 (CC1200_TOC_CFG_TOC_POST_SYNC_BLOCKLEN_2_32 << CC1200_TOC_CFG_TOC_POST_SYNC_BLOCKLEN)),
	CC1200_FREQ2, 0x6c,			 /* Frequency Configuration [23:16] */
	CC1200_FREQ1, 0xa3,			 /* Frequency Configuration [15:8] */
	CC1200_FREQ0, 0x33,			 /* Frequency Configuration [7:0] */
	CC1200_IF_ADC1, 0xee,		 /* Analog to Digital Converter Configuration Reg. 1 */
	CC1200_IF_ADC0, 0x10,		 /* Analog to Digital Converter Configuration Reg. 0 */
	CC1200_FS_DIG1, 0x07,		 /* Frequency Synthesizer Digital Reg. 1 */
	CC1200_FS_DIG0, 0xaf,		 /* Frequency Synthesizer Digital Reg. 0 */
	CC1200_FS_CAL1, 0x40,		 /* Frequency Synthesizer Calibration Reg. 1 */
	CC1200_FS_CAL0, 0x0e,		 /* Frequency Synthesizer Calibration Reg. 0 */
	CC1200_FS_DIVTWO, 0x03,		 /* Frequency Synthesizer Divide by 2 */
	CC1200_FS_DSM0, 0x33,		 /* FS Digital Synthesizer Module Configuration Reg. 0 */
	CC1200_FS_DVC0, 0x17,		 /* Frequency Synthesizer Divider Chain Configuration .. */
	CC1200_FS_PFD, 0x00,		 /* Frequency Synthesizer Phase Frequency Detector Con.. */
	CC1200_FS_PRE, 0x6e,		 /* Frequency Synthesizer Prescaler Configuration */
	CC1200_FS_REG_DIV_CML, 0x1c, /* Frequency Synthesizer Divider Regulator Configurat.. */
	CC1200_FS_SPARE, 0xac,		 /* Frequency Synthesizer Spare */
	CC1200_FS_VCO0, 0xb5,		 /* FS Voltage Controlled Oscillator Configuration Reg.. */
	CC1200_XOSC5, 0x0e,			 /* Crystal Oscillator Configuration Reg. 5 */
	CC1200_XOSC1, 0x03,			 /* Crystal Oscillator Configuration Reg. 1 */
