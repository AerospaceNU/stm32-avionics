#ifndef SMARTRF_CC1200_H
#define SMARTRF_CC1200_H

#include "smartrf_registersettings.h"

// Address Config = No address check
// Bit Rate = 1.2
// Carrier Frequency = 433.999939
// Deviation = 20.523071
// Device Address = 0
// Manchester Enable = false
// Modulation Format = 2-GFSK
// Packet Bit Length = 0
// Packet Length = 88
// Packet Length Mode = Fixed
// RX Filter BW = 98.039216
// Symbol rate = 1.2
// Whitening = true

static const registerSetting_t cc1200_433_1_2kbps_cfg[]=
{
  {CC112X_IOCFG3,            0x01}, // GPIO3 IO Pin Configuration
  {CC112X_IOCFG2,            0x01}, // GPIO2 IO Pin Configuration
  {CC112X_IOCFG1,            0xB0}, // GPIO1 IO Pin Configuration
  {CC112X_IOCFG0,            0x01}, // GPIO0 IO Pin Configuration
  {CC112X_SYNC_CFG1,         0xA8}, // Sync Word Detection Configuration Reg. 1
  {CC112X_DEVIATION_M,       0x0D}, // Frequency Deviation Configuration
  {CC112X_MODCFG_DEV_E,      0x0B}, // 0x0b | Modulation Format and Frequency Deviation Configur..
  {CC112X_DCFILT_CFG,        0x5D}, // 0x5d | Digital DC Removal Configuration
  {CC112X_PREAMBLE_CFG0,     0x8A}, // 0x8a | Preamble Detection Configuration Reg. 0
  {CC112X_IQIC,              0xCB}, // 0xcb | Digital Image Channel Compensation Configuration
  {CC112X_CHAN_BW,           0x11}, // 0x11 | Channel Filter Configuration
  {CC112X_MDMCFG1,           0x40}, // 0x40 | General Modem Parameter Configuration Reg. 1
  {CC112X_MDMCFG0,           0x05}, // 0x05 | General Modem Parameter Configuration Reg. 0
  {CC112X_SYMBOL_RATE2,      0x3F}, // 0x3f | Symbol Rate Configuration Exponent and Mantissa [1..
  {CC112X_SYMBOL_RATE1,      0x75}, // 0x75 | Symbol Rate Configuration Mantissa [15:8]
  {CC112X_SYMBOL_RATE0,      0x10}, // 0x10 | Symbol Rate Configuration Mantissa [7:0]
  {CC112X_AGC_REF,           0x38}, // 0x38 | AGC Reference Level Configuration
  {CC112X_AGC_CS_THR,        0xEC}, // 0xec | Carrier Sense Threshold Configuration
  {CC112X_AGC_CFG1,          0x51}, // 0x51 | Automatic Gain Control Configuration Reg. 1
  {CC112X_AGC_CFG0,          0x87}, // 0x87 | Automatic Gain Control Configuration Reg. 0
  {CC112X_FIFO_CFG,          0x00}, // 0x00 | FIFO Configuration
  {CC112X_FS_CFG,            0x14}, // 0x14 | Frequency Synthesizer Configuration
  {CC112X_PKT_CFG2,          0x00}, // 0x00 | Packet Configuration Reg. 2
  {CC112X_PKT_CFG1,          0x43}, // 0x43 | Packet Configuration Reg. 1
  {CC112X_PKT_LEN,           0x58}, // 0x58 | Packet Length Configuration
  {CC112X_IF_MIX_CFG,        0x1C}, // 0x1c | IF Mix Configuration
  {CC112X_FREQOFF_CFG,       0x22}, // 0x22 | Frequency Offset Correction Configuration
  {CC112X_MDMCFG2,           0x0C}, // 0x0c | General Modem Parameter Configuration Reg. 2
  {CC112X_FREQ2,             0x56}, // 0x56 | Frequency Configuration [23:16]
  {CC112X_FREQ1,             0xCC}, // 0xcc | Frequency Configuration [15:8]
  {CC112X_FREQ0,             0xCC}, // 0xcc | Frequency Configuration [7:0]
  {CC112X_IF_ADC1,           0xEE}, // 0xee | Analog to Digital Converter Configuration Reg. 1
  {CC112X_IF_ADC0,           0x10}, // 0x10 | Analog to Digital Converter Configuration Reg. 0
  {CC112X_FS_DIG1,           0x07}, // 0x07 | Frequency Synthesizer Digital Reg. 1
  {CC112X_FS_DIG0,           0xAF}, // 0xaf | Frequency Synthesizer Digital Reg. 0
  {CC112X_FS_CAL1,           0x40}, // 0x40 | Frequency Synthesizer Calibration Reg. 1
  {CC112X_FS_CAL0,           0x0E}, // 0x0e | Frequency Synthesizer Calibration Reg. 0
  {CC112X_FS_DIVTWO,         0x03}, // 0x03 | Frequency Synthesizer Divide by 2
  {CC112X_FS_DSM0,           0x33}, // 0x33 | FS Digital Synthesizer Module Configuration Reg. 0
  {CC112X_FS_DVC0,           0x17}, // 0x17 | Frequency Synthesizer Divider Chain Configuration ..
  {CC112X_FS_PFD,            0x00}, // 0x00 | Frequency Synthesizer Phase Frequency Detector Con..
  {CC112X_FS_PRE,            0x6E}, // 0x6e | Frequency Synthesizer Prescaler Configuration
  {CC112X_FS_REG_DIV_CML,    0x1C}, // 0x1c | Frequency Synthesizer Divider Regulator Configurat..
  {CC112X_FS_SPARE,          0xAC}, // 0xac | Frequency Synthesizer Spare
  {CC112X_FS_VCO0,           0xB5}, // 0xb5 | FS Voltage Controlled Oscillator Configuration Reg..
  {CC112X_XOSC5,             0x0E}, // 0x0e | Crystal Oscillator Configuration Reg. 5
  {CC112X_XOSC1,             0x03}, // 0x03 | Crystal Oscillator Configuration Reg. 1
};

#define cc1200_433_1_2kbps_size sizeof(cc1200_433_1_2kbps_cfg)


#endif
