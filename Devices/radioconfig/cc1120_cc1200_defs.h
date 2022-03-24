/*
 * cc1120_cc1200_defs.h
 *
 *  Created on: Feb 10, 2022
 *      Author: matth
 */

#ifndef RADIOCONFIG_CC1120_CC1200_DEFS_H_
#define RADIOCONFIG_CC1120_CC1200_DEFS_H_


/******************************************************************************
 * CONSTANTS
 */

#define CC1120_OSC_FREQ 32000000//32 MHz
#define CC1200_OSC_FREQ 40000000

#define BAND_820_960MHz 	0x2
#define BAND_410_480MHz		 0x4
#define BAND_273_320MHz 		0x6
#define BAND_205_240MHz		 0x8
#define BAND_164_192MHz		 0xA
#define BAND_136_160MHz 		0xB

/* user config values */

#define FS_CFG_FS_LOCK_EN 4
#define FS_CFG_FSD_BANDSELECT 0
#define FIX_PACKET_SIZE		8
#define MAX_PACKET_SIZE		128


/* configuration registers */
#define EXTENDED_REGISTER_TOP_BYTE 0x2f
#define NORMAL_REGISTER_TOP_BYTE 0x0

// Common config values

// CCA mode, bits [4:2] of PKT_CFG2
typedef enum {
	CCA_MODE_NONE = 0,
	CCA_MODE_RSSI,
	CCA_MODE_INRX,// Not currently recieving packet
	CCA_MODE_RRSIandRX,
	CCA_MODE_RSSI_LBT // Listen-before-talk, adds 5-10ms per transmit
} CCAMode_e;
#define CCA_MODE_SHIFT 2

#ifdef HAS_CC1120

#define TIRADIO_IOCFG3                   0x0000
#define TIRADIO_IOCFG2                   0x0001
#define TIRADIO_IOCFG1                   0x0002
#define TIRADIO_IOCFG0                   0x0003
#define TIRADIO_SYNC3                    0x0004
#define TIRADIO_SYNC2                    0x0005
#define TIRADIO_SYNC1                    0x0006
#define TIRADIO_SYNC0                    0x0007
#define TIRADIO_SYNC_CFG1                0x0008
#define TIRADIO_SYNC_CFG0                0x0009
#define TIRADIO_DEVIATION_M              0x000A
#define TIRADIO_MODCFG_DEV_E             0x000B
#define TIRADIO_DCFILT_CFG               0x000C
#define TIRADIO_PREAMBLE_CFG1            0x000D
#define TIRADIO_PREAMBLE_CFG0            0x000E
#define TIRADIO_FREQ_IF_CFG              0x000F
#define TIRADIO_IQIC                     0x0010
#define TIRADIO_CHAN_BW                  0x0011
#define TIRADIO_MDMCFG1                  0x0012
#define TIRADIO_MDMCFG0                  0x0013
#define TIRADIO_SYMBOL_RATE2             0x0014
#define TIRADIO_SYMBOL_RATE1             0x0015
#define TIRADIO_SYMBOL_RATE0             0x0016
#define TIRADIO_AGC_REF                  0x0017
#define TIRADIO_AGC_CS_THR               0x0018
#define TIRADIO_AGC_GAIN_ADJUST          0x0019
#define TIRADIO_AGC_CFG3                 0x001A
#define TIRADIO_AGC_CFG2                 0x001B
#define TIRADIO_AGC_CFG1                 0x001C
#define TIRADIO_AGC_CFG0                 0x001D
#define TIRADIO_FIFO_CFG                 0x001E
#define TIRADIO_DEV_ADDR                 0x001F
#define TIRADIO_SETTLING_CFG             0x0020
#define TIRADIO_FS_CFG                   0x0021
#define TIRADIO_WOR_CFG1                 0x0022
#define TIRADIO_WOR_CFG0                 0x0023
#define TIRADIO_WOR_EVENT0_MSB           0x0024
#define TIRADIO_WOR_EVENT0_LSB           0x0025
#define TIRADIO_PKT_CFG2                 0x0026
#define TIRADIO_PKT_CFG1                 0x0027
#define TIRADIO_PKT_CFG0                 0x0028
#define TIRADIO_RFEND_CFG1               0x0029
#define TIRADIO_RFEND_CFG0               0x002A
#define TIRADIO_PA_CFG2                  0x002B
#define TIRADIO_PA_CFG1                  0x002C
#define TIRADIO_PA_CFG0                  0x002D
#define TIRADIO_PKT_LEN                  0x002E

/* Extended Configuration Registers */
#define TIRADIO_IF_MIX_CFG               0x2F00
#define TIRADIO_FREQOFF_CFG              0x2F01
#define TIRADIO_TOC_CFG                  0x2F02
#define TIRADIO_MARC_SPARE               0x2F03
#define TIRADIO_ECG_CFG                  0x2F04
#define TIRADIO_CFM_DATA_CFG             0x2F05
#define TIRADIO_EXT_CTRL                 0x2F06
#define TIRADIO_RCCAL_FINE               0x2F07
#define TIRADIO_RCCAL_COARSE             0x2F08
#define TIRADIO_RCCAL_OFFSET             0x2F09
#define TIRADIO_FREQOFF1                 0x2F0A
#define TIRADIO_FREQOFF0                 0x2F0B
#define TIRADIO_FREQ2                    0x2F0C
#define TIRADIO_FREQ1                    0x2F0D
#define TIRADIO_FREQ0                    0x2F0E
#define TIRADIO_IF_ADC2                  0x2F0F
#define TIRADIO_IF_ADC1                  0x2F10
#define TIRADIO_IF_ADC0                  0x2F11
#define TIRADIO_FS_DIG1                  0x2F12
#define TIRADIO_FS_DIG0                  0x2F13
#define TIRADIO_FS_CAL3                  0x2F14
#define TIRADIO_FS_CAL2                  0x2F15
#define TIRADIO_FS_CAL1                  0x2F16
#define TIRADIO_FS_CAL0                  0x2F17
#define TIRADIO_FS_CHP                   0x2F18
#define TIRADIO_FS_DIVTWO                0x2F19
#define TIRADIO_FS_DSM1                  0x2F1A
#define TIRADIO_FS_DSM0                  0x2F1B
#define TIRADIO_FS_DVC1                  0x2F1C
#define TIRADIO_FS_DVC0                  0x2F1D
#define TIRADIO_FS_LBI                   0x2F1E
#define TIRADIO_FS_PFD                   0x2F1F
#define TIRADIO_FS_PRE                   0x2F20
#define TIRADIO_FS_REG_DIV_CML           0x2F21
#define TIRADIO_FS_SPARE                 0x2F22
#define TIRADIO_FS_VCO4                  0x2F23
#define TIRADIO_FS_VCO3                  0x2F24
#define TIRADIO_FS_VCO2                  0x2F25
#define TIRADIO_FS_VCO1                  0x2F26
#define TIRADIO_FS_VCO0                  0x2F27
#define TIRADIO_GBIAS6                   0x2F28
#define TIRADIO_GBIAS5                   0x2F29
#define TIRADIO_GBIAS4                   0x2F2A
#define TIRADIO_GBIAS3                   0x2F2B
#define TIRADIO_GBIAS2                   0x2F2C
#define TIRADIO_GBIAS1                   0x2F2D
#define TIRADIO_GBIAS0                   0x2F2E
#define TIRADIO_IFAMP                    0x2F2F
#define TIRADIO_LNA                      0x2F30
#define TIRADIO_RXMIX                    0x2F31
#define TIRADIO_XOSC5                    0x2F32
#define TIRADIO_XOSC4                    0x2F33
#define TIRADIO_XOSC3                    0x2F34
#define TIRADIO_XOSC2                    0x2F35
#define TIRADIO_XOSC1                    0x2F36
#define TIRADIO_XOSC0                    0x2F37
#define TIRADIO_ANALOG_SPARE             0x2F38
#define TIRADIO_PA_CFG3                  0x2F39
#define TIRADIO_IRQ0M                    0x2F3F
#define TIRADIO_IRQ0F                    0x2F40

/* Status Registers */
#define TIRADIO_WOR_TIME1                0x2F64
#define TIRADIO_WOR_TIME0                0x2F65
#define TIRADIO_WOR_CAPTURE1             0x2F66
#define TIRADIO_WOR_CAPTURE0             0x2F67
#define TIRADIO_BIST                     0x2F68
#define TIRADIO_DCFILTOFFSET_I1          0x2F69
#define TIRADIO_DCFILTOFFSET_I0          0x2F6A
#define TIRADIO_DCFILTOFFSET_Q1          0x2F6B
#define TIRADIO_DCFILTOFFSET_Q0          0x2F6C
#define TIRADIO_IQIE_I1                  0x2F6D
#define TIRADIO_IQIE_I0                  0x2F6E
#define TIRADIO_IQIE_Q1                  0x2F6F
#define TIRADIO_IQIE_Q0                  0x2F70
#define TIRADIO_RSSI1                    0x2F71
#define TIRADIO_RSSI0                    0x2F72
#define TIRADIO_MARCSTATE                0x2F73
#define TIRADIO_LQI_VAL                  0x2F74
#define TIRADIO_PQT_SYNC_ERR             0x2F75
#define TIRADIO_DEM_STATUS               0x2F76
#define TIRADIO_FREQOFF_EST1             0x2F77
#define TIRADIO_FREQOFF_EST0             0x2F78
#define TIRADIO_AGC_GAIN3                0x2F79
#define TIRADIO_AGC_GAIN2                0x2F7A
#define TIRADIO_AGC_GAIN1                0x2F7B
#define TIRADIO_AGC_GAIN0                0x2F7C
#define TIRADIO_CFM_RX_DATA_OUT          0x2F7D
#define TIRADIO_CFM_TX_DATA_IN           0x2F7E
#define TIRADIO_ASK_SOFT_RX_DATA         0x2F7F
#define TIRADIO_RNDGEN                   0x2F80
#define TIRADIO_MAGN2                    0x2F81
#define TIRADIO_MAGN1                    0x2F82
#define TIRADIO_MAGN0                    0x2F83
#define TIRADIO_ANG1                     0x2F84
#define TIRADIO_ANG0                     0x2F85
#define TIRADIO_CHFILT_I2                0x2F86
#define TIRADIO_CHFILT_I1                0x2F87
#define TIRADIO_CHFILT_I0                0x2F88
#define TIRADIO_CHFILT_Q2                0x2F89
#define TIRADIO_CHFILT_Q1                0x2F8A
#define TIRADIO_CHFILT_Q0                0x2F8B
#define TIRADIO_GPIO_STATUS              0x2F8C
#define TIRADIO_FSCAL_CTRL               0x2F8D
#define TIRADIO_PHASE_ADJUST             0x2F8E
#define TIRADIO_PARTNUMBER               0x2F8F
#define TIRADIO_PARTVERSION              0x2F90
#define TIRADIO_SERIAL_STATUS            0x2F91
#define TIRADIO_MODEM_STATUS1            0x2F92
#define TIRADIO_MODEM_STATUS0            0x2F93
#define TIRADIO_MARC_STATUS1             0x2F94
#define TIRADIO_MARC_STATUS0             0x2F95
#define TIRADIO_PA_IFAMP_TEST            0x2F96
#define TIRADIO_FSRF_TEST                0x2F97
#define TIRADIO_PRE_TEST                 0x2F98
#define TIRADIO_PRE_OVR                  0x2F99
#define TIRADIO_ADC_TEST                 0x2F9A
#define TIRADIO_DVC_TEST                 0x2F9B
#define TIRADIO_ATEST                    0x2F9C
#define TIRADIO_ATEST_LVDS               0x2F9D
#define TIRADIO_ATEST_MODE               0x2F9E
#define TIRADIO_XOSC_TEST1               0x2F9F
#define TIRADIO_XOSC_TEST0               0x2FA0

#define TIRADIO_RXFIRST                  0x2FD2
#define TIRADIO_TXFIRST                  0x2FD3
#define TIRADIO_RXLAST                   0x2FD4
#define TIRADIO_TXLAST                   0x2FD5
#define TIRADIO_NUM_TXBYTES              0x2FD6  /* Number of bytes in TXFIFO */
#define TIRADIO_NUM_RXBYTES              0x2FD7  /* Number of bytes in RXFIFO */
#define TIRADIO_FIFO_NUM_TXBYTES         0x2FD8
#define TIRADIO_FIFO_NUM_RXBYTES         0x2FD9


/* DATA FIFO Access */
#define TIRADIO_SINGLE_TXFIFO            0x003F      /*  TXFIFO  - Single accecss to Transmit FIFO */
#define TIRADIO_BURST_TXFIFO             0x007F      /*  TXFIFO  - Burst accecss to Transmit FIFO  */
#define TIRADIO_SINGLE_RXFIFO            0x00BF      /*  RXFIFO  - Single accecss to Receive FIFO  */
#define TIRADIO_BURST_RXFIFO             0x00FF      /*  RXFIFO  - Busrrst ccecss to Receive FIFO  */

#define TIRADIO_LQI_CRC_OK_BM            0x80
#define TIRADIO_LQI_EST_BM               0x7F

/* Command strobe registers */
#define TIRADIO_SRES                     0x30      /*  SRES    - Reset chip. */
#define TIRADIO_SFSTXON                  0x31      /*  SFSTXON - Enable and calibrate frequency synthesizer. */
#define TIRADIO_SXOFF                    0x32      /*  SXOFF   - Turn off crystal oscillator. */
#define TIRADIO_SCAL                     0x33      /*  SCAL    - Calibrate frequency synthesizer and turn it off. */
#define TIRADIO_SRX                      0x34      /*  SRX     - Enable RX. Perform calibration if enabled. */
#define TIRADIO_STX                      0x35      /*  STX     - Enable TX. If in RX state, only enable TX if CCA passes. */
#define TIRADIO_SIDLE                    0x36      /*  SIDLE   - Exit RX / TX, turn off frequency synthesizer. */
#define TIRADIO_SWOR                     0x38      /*  SWOR    - Start automatic RX polling sequence (Wake-on-Radio) */
#define TIRADIO_SPWD                     0x39      /*  SPWD    - Enter power down mode when CSn goes high. */
#define TIRADIO_SFRX                     0x3A      /*  SFRX    - Flush the RX FIFO buffer. */
#define TIRADIO_SFTX                     0x3B      /*  SFTX    - Flush the TX FIFO buffer. */
#define TIRADIO_SWORRST                  0x3C      /*  SWORRST - Reset real time clock. */
#define TIRADIO_SNOP                     0x3D      /*  SNOP    - No operation. Returns status byte. */
#define TIRADIO_AFC                      0x37      /*  AFC     - Automatic Frequency Correction */

/* Chip states returned in status byte */
#define TIRADIO_STATE_IDLE               0x00
#define TIRADIO_STATE_RX                 0x10
#define TIRADIO_STATE_TX                 0x20
#define TIRADIO_STATE_FSTXON             0x30
#define TIRADIO_STATE_CALIBRATE          0x40
#define TIRADIO_STATE_SETTLING           0x50
#define TIRADIO_STATE_RXFIFO_ERROR       0x60
#define TIRADIO_STATE_TXFIFO_ERROR       0x70

#endif
#ifdef HAS_CC1200

/* configuration registers */
#define TIRADIO_IOCFG3                   0x0000
#define TIRADIO_IOCFG2                   0x0001
#define TIRADIO_IOCFG1                   0x0002
#define TIRADIO_IOCFG0                   0x0003
#define TIRADIO_SYNC3                    0x0004
#define TIRADIO_SYNC2                    0x0005
#define TIRADIO_SYNC1                    0x0006
#define TIRADIO_SYNC0                    0x0007
#define TIRADIO_SYNC_CFG1                0x0008
#define TIRADIO_SYNC_CFG0                0x0009
#define TIRADIO_DEVIATION_M              0x000A
#define TIRADIO_MODCFG_DEV_E             0x000B
#define TIRADIO_DCFILT_CFG               0x000C
#define TIRADIO_PREAMBLE_CFG1            0x000D
#define TIRADIO_PREAMBLE_CFG0            0x000E
#define TIRADIO_IQIC                     0x000F
#define TIRADIO_CHAN_BW                  0x0010
#define TIRADIO_MDMCFG1                  0x0011
#define TIRADIO_MDMCFG0                  0x0012
#define TIRADIO_SYMBOL_RATE2             0x0013
#define TIRADIO_SYMBOL_RATE1             0x0014
#define TIRADIO_SYMBOL_RATE0             0x0015
#define TIRADIO_AGC_REF                  0x0016
#define TIRADIO_AGC_CS_THR               0x0017
#define TIRADIO_AGC_GAIN_ADJUST          0x0018
#define TIRADIO_AGC_CFG3                 0x0019
#define TIRADIO_AGC_CFG2                 0x001A
#define TIRADIO_AGC_CFG1                 0x001B
#define TIRADIO_AGC_CFG0                 0x001C
#define TIRADIO_FIFO_CFG                 0x001D
#define TIRADIO_DEV_ADDR                 0x001E
#define TIRADIO_SETTLING_CFG             0x001F
#define TIRADIO_FS_CFG                   0x0020
#define TIRADIO_WOR_CFG1                 0x0021
#define TIRADIO_WOR_CFG0                 0x0022
#define TIRADIO_WOR_EVENT0_MSB           0x0023
#define TIRADIO_WOR_EVENT0_LSB           0x0024
#define TIRADIO_RXDCM_TIME               0x0025
#define TIRADIO_PKT_CFG2                 0x0026
#define TIRADIO_PKT_CFG1                 0x0027
#define TIRADIO_PKT_CFG0                 0x0028
#define TIRADIO_RFEND_CFG1               0x0029
#define TIRADIO_RFEND_CFG0               0x002A
#define TIRADIO_PA_CFG1                  0x002B
#define TIRADIO_PA_CFG0                  0x002C
#define TIRADIO_ASK_CFG                  0x002D
#define TIRADIO_PKT_LEN                  0x002E

/* Extended Configuration Registers */
#define TIRADIO_IF_MIX_CFG               0x2F00
#define TIRADIO_FREQOFF_CFG              0x2F01
#define TIRADIO_TOC_CFG                  0x2F02
#define TIRADIO_MARC_SPARE               0x2F03
#define TIRADIO_ECG_CFG                  0x2F04
#define TIRADIO_MDMCFG2                  0x2F05
#define TIRADIO_EXT_CTRL                 0x2F06
#define TIRADIO_RCCAL_FINE               0x2F07
#define TIRADIO_RCCAL_COARSE             0x2F08
#define TIRADIO_RCCAL_OFFSET             0x2F09
#define TIRADIO_FREQOFF1                 0x2F0A
#define TIRADIO_FREQOFF0                 0x2F0B
#define TIRADIO_FREQ2                    0x2F0C
#define TIRADIO_FREQ1                    0x2F0D
#define TIRADIO_FREQ0                    0x2F0E
#define TIRADIO_IF_ADC2                  0x2F0F
#define TIRADIO_IF_ADC1                  0x2F10
#define TIRADIO_IF_ADC0                  0x2F11
#define TIRADIO_FS_DIG1                  0x2F12
#define TIRADIO_FS_DIG0                  0x2F13
#define TIRADIO_FS_CAL3                  0x2F14
#define TIRADIO_FS_CAL2                  0x2F15
#define TIRADIO_FS_CAL1                  0x2F16
#define TIRADIO_FS_CAL0                  0x2F17
#define TIRADIO_FS_CHP                   0x2F18
#define TIRADIO_FS_DIVTWO                0x2F19
#define TIRADIO_FS_DSM1                  0x2F1A
#define TIRADIO_FS_DSM0                  0x2F1B
#define TIRADIO_FS_DVC1                  0x2F1C
#define TIRADIO_FS_DVC0                  0x2F1D
#define TIRADIO_FS_LBI                   0x2F1E
#define TIRADIO_FS_PFD                   0x2F1F
#define TIRADIO_FS_PRE                   0x2F20
#define TIRADIO_FS_REG_DIV_CML           0x2F21
#define TIRADIO_FS_SPARE                 0x2F22
#define TIRADIO_FS_VCO4                  0x2F23
#define TIRADIO_FS_VCO3                  0x2F24
#define TIRADIO_FS_VCO2                  0x2F25
#define TIRADIO_FS_VCO1                  0x2F26
#define TIRADIO_FS_VCO0                  0x2F27
#define TIRADIO_GBIAS6                   0x2F28
#define TIRADIO_GBIAS5                   0x2F29
#define TIRADIO_GBIAS4                   0x2F2A
#define TIRADIO_GBIAS3                   0x2F2B
#define TIRADIO_GBIAS2                   0x2F2C
#define TIRADIO_GBIAS1                   0x2F2D
#define TIRADIO_GBIAS0                   0x2F2E
#define TIRADIO_IFAMP                    0x2F2F
#define TIRADIO_LNA                      0x2F30
#define TIRADIO_RXMIX                    0x2F31
#define TIRADIO_XOSC5                    0x2F32
#define TIRADIO_XOSC4                    0x2F33
#define TIRADIO_XOSC3                    0x2F34
#define TIRADIO_XOSC2                    0x2F35
#define TIRADIO_XOSC1                    0x2F36
#define TIRADIO_XOSC0                    0x2F37
#define TIRADIO_ANALOG_SPARE             0x2F38
#define TIRADIO_PA_CFG3                  0x2F39
#define TIRADIO_IRQ0M                    0x2F3F
#define TIRADIO_IRQ0F                    0x2F40

/* Status Registers */
#define TIRADIO_WOR_TIME1                0x2F64
#define TIRADIO_WOR_TIME0                0x2F65
#define TIRADIO_WOR_CAPTURE1             0x2F66
#define TIRADIO_WOR_CAPTURE0             0x2F67
#define TIRADIO_BIST                     0x2F68
#define TIRADIO_DCFILTOFFSET_I1          0x2F69
#define TIRADIO_DCFILTOFFSET_I0          0x2F6A
#define TIRADIO_DCFILTOFFSET_Q1          0x2F6B
#define TIRADIO_DCFILTOFFSET_Q0          0x2F6C
#define TIRADIO_IQIE_I1                  0x2F6D
#define TIRADIO_IQIE_I0                  0x2F6E
#define TIRADIO_IQIE_Q1                  0x2F6F
#define TIRADIO_IQIE_Q0                  0x2F70
#define TIRADIO_RSSI1                    0x2F71
#define TIRADIO_RSSI0                    0x2F72
#define TIRADIO_MARCSTATE                0x2F73
#define TIRADIO_LQI_VAL                  0x2F74
#define TIRADIO_PQT_SYNC_ERR             0x2F75
#define TIRADIO_DEM_STATUS               0x2F76
#define TIRADIO_FREQOFF_EST1             0x2F77
#define TIRADIO_FREQOFF_EST0             0x2F78
#define TIRADIO_AGC_GAIN3                0x2F79
#define TIRADIO_AGC_GAIN2                0x2F7A
#define TIRADIO_AGC_GAIN1                0x2F7B
#define TIRADIO_AGC_GAIN0                0x2F7C
#define TIRADIO_CFM_RX_DATA_OUT         0x2F7D
#define TIRADIO_CFM_TX_DATA_IN          0x2F7E
#define TIRADIO_ASK_SOFT_RX_DATA         0x2F7F
#define TIRADIO_RNDGEN                   0x2F80
#define TIRADIO_MAGN2                    0x2F81
#define TIRADIO_MAGN1                    0x2F82
#define TIRADIO_MAGN0                    0x2F83
#define TIRADIO_ANG1                     0x2F84
#define TIRADIO_ANG0                     0x2F85
#define TIRADIO_CHFILT_I2                0x2F86
#define TIRADIO_CHFILT_I1                0x2F87
#define TIRADIO_CHFILT_I0                0x2F88
#define TIRADIO_CHFILT_Q2                0x2F89
#define TIRADIO_CHFILT_Q1                0x2F8A
#define TIRADIO_CHFILT_Q0                0x2F8B
#define TIRADIO_GPIO_STATUS              0x2F8C
#define TIRADIO_FSCAL_CTRL               0x2F8D
#define TIRADIO_PHASE_ADJUST             0x2F8E
#define TIRADIO_PARTNUMBER               0x2F8F
#define TIRADIO_PARTVERSION              0x2F90
#define TIRADIO_SERIAL_STATUS            0x2F91
#define TIRADIO_MODEM_STATUS1            0x2F92
#define TIRADIO_MODEM_STATUS0            0x2F93
#define TIRADIO_MARC_STATUS1             0x2F94
#define TIRADIO_MARC_STATUS0             0x2F95
#define TIRADIO_PA_IFAMP_TEST            0x2F96
#define TIRADIO_FSRF_TEST                0x2F97
#define TIRADIO_PRE_TEST                 0x2F98
#define TIRADIO_PRE_OVR                  0x2F99
#define TIRADIO_ADC_TEST                 0x2F9A
#define TIRADIO_DVC_TEST                 0x2F9B
#define TIRADIO_ATEST                    0x2F9C
#define TIRADIO_ATEST_LVDS               0x2F9D
#define TIRADIO_ATEST_MODE               0x2F9E
#define TIRADIO_XOSC_TEST1               0x2F9F
#define TIRADIO_XOSC_TEST0               0x2FA0
#define TIRADIO_AES                      0x2FA1
#define TIRADIO_MDM_TEST                 0x2FA2

#define TIRADIO_RXFIRST                  0x2FD2
#define TIRADIO_TXFIRST                  0x2FD3
#define TIRADIO_RXLAST                   0x2FD4
#define TIRADIO_TXLAST                   0x2FD5
#define TIRADIO_NUM_TXBYTES              0x2FD6  /* Number of bytes in TXFIFO */
#define TIRADIO_NUM_RXBYTES              0x2FD7  /* Number of bytes in RXFIFO */
#define TIRADIO_FIFO_NUM_TXBYTES         0x2FD8
#define TIRADIO_FIFO_NUM_RXBYTES         0x2FD9
#define TIRADIO_RXFIFO_PRE_BUF           0x2FDA

/* DATA FIFO Access */
#define TIRADIO_SINGLE_TXFIFO            0x003F     /*  TXFIFO  - Single accecss to Transmit FIFO */
#define TIRADIO_BURST_TXFIFO             0x007F     /*  TXFIFO  - Burst accecss to Transmit FIFO  */
#define TIRADIO_SINGLE_RXFIFO            0x00BF     /*  RXFIFO  - Single accecss to Receive FIFO  */
#define TIRADIO_BURST_RXFIFO             0x00FF     /*  RXFIFO  - Busrrst ccecss to Receive FIFO  */

/* AES Workspace */
/* AES Key */
#define TIRADIO_AES_KEY                  0x2FE0     /*  AES_KEY    - Address for AES key input  */
#define TIRADIO_AES_KEY15	        0x2FE0
#define TIRADIO_AES_KEY14	        0x2FE1
#define TIRADIO_AES_KEY13	        0x2FE2
#define TIRADIO_AES_KEY12	        0x2FE3
#define TIRADIO_AES_KEY11	        0x2FE4
#define TIRADIO_AES_KEY10	        0x2FE5
#define TIRADIO_AES_KEY9	                0x2FE6
#define TIRADIO_AES_KEY8	                0x2FE7
#define TIRADIO_AES_KEY7	                0x2FE8
#define TIRADIO_AES_KEY6	                0x2FE9
#define TIRADIO_AES_KEY5	                0x2FE10
#define TIRADIO_AES_KEY4	                0x2FE11
#define TIRADIO_AES_KEY3	                0x2FE12
#define TIRADIO_AES_KEY2	                0x2FE13
#define TIRADIO_AES_KEY1	                0x2FE14
#define TIRADIO_AES_KEY0	                0x2FE15

/* AES Buffer */
#define TIRADIO_AES_BUFFER               0x2FF0     /*  AES_BUFFER - Address for AES Buffer     */
#define TIRADIO_AES_BUFFER15		0x2FF0
#define TIRADIO_AES_BUFFER14		0x2FF1
#define TIRADIO_AES_BUFFER13		0x2FF2
#define TIRADIO_AES_BUFFER12		0x2FF3
#define TIRADIO_AES_BUFFER11		0x2FF4
#define TIRADIO_AES_BUFFER10		0x2FF5
#define TIRADIO_AES_BUFFER9		0x2FF6
#define TIRADIO_AES_BUFFER8		0x2FF7
#define TIRADIO_AES_BUFFER7		0x2FF8
#define TIRADIO_AES_BUFFER6		0x2FF9
#define TIRADIO_AES_BUFFER5		0x2FF10
#define TIRADIO_AES_BUFFER4		0x2FF11
#define TIRADIO_AES_BUFFER3		0x2FF12
#define TIRADIO_AES_BUFFER2		0x2FF13
#define TIRADIO_AES_BUFFER1		0x2FF14
#define TIRADIO_AES_BUFFER0		0x2FF15

#define TIRADIO_LQI_CRC_OK_BM            0x80
#define TIRADIO_LQI_EST_BM               0x7F

/* Command strobe registers */
#define TIRADIO_SRES                     0x30      /*  SRES    - Reset chip. */
#define TIRADIO_SFSTXON                  0x31      /*  SFSTXON - Enable and calibrate frequency synthesizer. */
#define TIRADIO_SXOFF                    0x32      /*  SXOFF   - Turn off crystal oscillator. */
#define TIRADIO_SCAL                     0x33      /*  SCAL    - Calibrate frequency synthesizer and turn it off. */
#define TIRADIO_SRX                      0x34      /*  SRX     - Enable RX. Perform calibration if enabled. */
#define TIRADIO_STX                      0x35      /*  STX     - Enable TX. If in RX state, only enable TX if CCA passes. */
#define TIRADIO_SIDLE                    0x36      /*  SIDLE   - Exit RX / TX, turn off frequency synthesizer. */
#define TIRADIO_SAFC                     0x37      /*  AFC     - Automatic Frequency Correction */
#define TIRADIO_SWOR                     0x38      /*  SWOR    - Start automatic RX polling sequence (Wake-on-Radio) */
#define TIRADIO_SPWD                     0x39      /*  SPWD    - Enter power down mode when CSn goes high. */
#define TIRADIO_SFRX                     0x3A      /*  SFRX    - Flush the RX FIFO buffer. */
#define TIRADIO_SFTX                     0x3B      /*  SFTX    - Flush the TX FIFO buffer. */
#define TIRADIO_SWORRST                  0x3C      /*  SWORRST - Reset real time clock. */
#define TIRADIO_SNOP                     0x3D      /*  SNOP    - No operation. Returns status byte. */

/* Chip states returned in status byte */
#define TIRADIO_STATE_IDLE               0x00
#define TIRADIO_STATE_RX                 0x10
#define TIRADIO_STATE_TX                 0x20
#define TIRADIO_STATE_FSTXON             0x30
#define TIRADIO_STATE_CALIBRATE          0x40
#define TIRADIO_STATE_SETTLING           0x50
#define TIRADIO_STATE_RXFIFO_ERROR       0x60
#define TIRADIO_STATE_TXFIFO_ERROR       0x70

#endif


#define TRXEM_PORT_SEL       P3SEL
#define TRXEM_PORT_OUT       P3OUT
#define TRXEM_PORT_DIR       P3DIR
#define TRXEM_PORT_IN        P3IN

#define TRXEM_SPI_MOSI_PIN   BIT1
#define TRXEM_SPI_MISO_PIN   BIT2
#define TRXEM_SPI_SCLK_PIN   BIT3
#define TRXEM_SPI_SC_N_PIN   BIT0

#define RF_RESET_N_PORT_OUT  P8OUT
#define RF_RESET_N_PORT_SEL  P8SEL
#define RF_RESET_N_PORT_DIR  P8DIR
#define RF_RESET_N_PIN       BIT0


#define RADIO_BURST_ACCESS   0x40
#define RADIO_SINGLE_ACCESS  0x00
#define RADIO_READ_ACCESS    0x80
#define RADIO_WRITE_ACCESS   0x00


/* Bit fields in the chip status byte */
#define STATUS_CHIP_RDYn_BM             0x80
#define STATUS_STATE_BM                 0x70
#define STATUS_FIFO_BYTES_AVAILABLE_BM  0x0F

#define CC1120_LENGTH_CONFIG_MASK 0b01100000

/* Expected values */
#define TIRADIO_MARCSTATE_RX 0b01101101
#define TIRADIO_CCA_FAILED 0b00001011

/******************************************************************************
 * PROTPTYPES
 */





#endif /* RADIOCONFIG_CC1120_CC1200_DEFS_H_ */
