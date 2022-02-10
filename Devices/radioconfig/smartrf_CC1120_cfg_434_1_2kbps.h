/***************************************************************
 *  SmartRF Studio(tm) Export
 *
 *  Radio register settings specifed with C-code
 *  compatible #define statements.
 *
 *  RF device: CC1120
 *
 ***************************************************************/

#ifndef SMARTRF_CC1120_H
#define SMARTRF_CC1120_H

#include "smartrf_registersettings.h"

// Address Config = No address check
// Bit Rate = 38.4
// Carrier Frequency = 434.000000
// Deviation = 20.019531
// Device Address = 0
// Manchester Enable = false
// Modulation Format = 2-GFSK
// PA Ramping = true
// Packet Bit Length = 0
// Packet Length = 255
// Packet Length Mode = Variable
// Performance Mode = High Performance
// RX Filter BW = 100.000000
// Symbol rate = 38.4
// TX Power = 15
// Whitening = false

static const registerSetting_t cc1120_433_1_2kbps_cfg[]=
{
  {CC112X_IOCFG3,            0xB0}, // GPIO3 IO Pin Configuration
  {CC112X_IOCFG2,            0x06}, // GPIO2 IO Pin Configuration
  {CC112X_IOCFG1,            0xB0}, // GPIO1 IO Pin Configuration
  {CC112X_IOCFG0,            0x40}, // GPIO0 IO Pin Configuration
  {CC112X_SYNC_CFG1,         0x08}, // Sync Word Detection Configuration Reg. 1
  {CC112X_DEVIATION_M,       0x48}, // Frequency Deviation Configuration
  {CC112X_MODCFG_DEV_E,      0x0D}, // Modulation Format and Frequency Deviation Configur..
  {CC112X_DCFILT_CFG,        0x15}, // Digital DC Removal Configuration
  {CC112X_PREAMBLE_CFG1,     0x18}, // Preamble Length Configuration Reg. 1
  {CC112X_FREQ_IF_CFG,       0x3A}, // RX Mixer Frequency Configuration
  {CC112X_IQIC,              0x00}, // Digital Image Channel Compensation Configuration
  {CC112X_CHAN_BW,           0x02}, // Channel Filter Configuration
  {CC112X_MDMCFG0,           0x05}, // General Modem Parameter Configuration Reg. 0
  {CC112X_SYMBOL_RATE2,      0x93}, // Symbol Rate Configuration Exponent and Mantissa [1..
  {CC112X_AGC_REF,           0x3C}, // AGC Reference Level Configuration
  {CC112X_AGC_CS_THR,        0xEF}, // Carrier Sense Threshold Configuration
  {CC112X_AGC_CFG1,          0xA9}, // Automatic Gain Control Configuration Reg. 1
  {CC112X_AGC_CFG0,          0xC0}, // Automatic Gain Control Configuration Reg. 0
  {CC112X_FIFO_CFG,          0x00}, // FIFO Configuration
  {CC112X_FS_CFG,            0x14}, // Frequency Synthesizer Configuration
  {CC112X_PKT_CFG0,          0x20}, // Packet Configuration Reg. 0
  {CC112X_PA_CFG0,           0x7B}, // Power Amplifier Configuration Reg. 0
  {CC112X_PKT_LEN,           0xFF}, // Packet Length Configuration
  {CC112X_IF_MIX_CFG,        0x00}, // IF Mix Configuration
  {CC112X_TOC_CFG,           0x0A}, // Timing Offset Correction Configuration
  {CC112X_FREQ2,             0x6C}, // Frequency Configuration [23:16]
  {CC112X_FREQ1,             0x80}, // Frequency Configuration [15:8]
  {CC112X_FS_DIG1,           0x00}, // Frequency Synthesizer Digital Reg. 1
  {CC112X_FS_DIG0,           0x5F}, // Frequency Synthesizer Digital Reg. 0
  {CC112X_FS_CAL1,           0x40}, // Frequency Synthesizer Calibration Reg. 1
  {CC112X_FS_CAL0,           0x0E}, // Frequency Synthesizer Calibration Reg. 0
  {CC112X_FS_DIVTWO,         0x03}, // Frequency Synthesizer Divide by 2
  {CC112X_FS_DSM0,           0x33}, // FS Digital Synthesizer Module Configuration Reg. 0
  {CC112X_FS_DVC0,           0x17}, // Frequency Synthesizer Divider Chain Configuration ..
  {CC112X_FS_PFD,            0x50}, // Frequency Synthesizer Phase Frequency Detector Con..
  {CC112X_FS_PRE,            0x6E}, // Frequency Synthesizer Prescaler Configuration
  {CC112X_FS_REG_DIV_CML,    0x14}, // Frequency Synthesizer Divider Regulator Configurat..
  {CC112X_FS_SPARE,          0xAC}, // Frequency Synthesizer Spare
  {CC112X_FS_VCO0,           0xB4}, // FS Voltage Controlled Oscillator Configuration Reg..
  {CC112X_XOSC5,             0x0E}, // Crystal Oscillator Configuration Reg. 5
  {CC112X_XOSC1,             0x03}, // Crystal Oscillator Configuration Reg. 1
};



#endif
