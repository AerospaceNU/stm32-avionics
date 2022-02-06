/* Address Config = No address check */
/* Bit Rate = 1.2 */
/* Carrier Frequency = 434.000000 */
/* Deviation = 20.019531 */
/* Device Address = 0 */
/* Manchester Enable = false */
/* Modulation Format = 2-FSK */
/* PA Ramping = true */
/* Packet Bit Length = 0 */
/* Packet Length = 255 */
/* Packet Length Mode = Variable */
/* Performance Mode = High Performance */
/* RX Filter BW = 50.000000 */
/* Symbol rate = 1.2 */
/* TX Power = 15 */
/* Whitening = false */
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
// Bit Rate = 1.2
// Carrier Frequency = 433.999939
// Deviation = 20.507813
// Device Address = 0
// Manchester Enable = false
// Modulation Format = 2-GFSK
// PA Ramping = true
// Packet Bit Length = 0
// Packet Length = 86
// Packet Length Mode = Fixed
// Performance Mode = High Performance
// RX Filter BW = 50.000000
// Symbol rate = 1.2
// TX Power = 15
// Whitening = true

static const registerSetting_t cc1120_433_1_2kbps_cfg[]=
{
  {CC112X_IOCFG3,            0x01}, // GPIO3 IO Pin Configuration
  {CC112X_IOCFG2,            0x01}, // GPIO2 IO Pin Configuration
  {CC112X_IOCFG1,            0xB0}, // GPIO1 IO Pin Configuration
  {CC112X_IOCFG0,            0x01}, // GPIO0 IO Pin Configuration
  {CC112X_SYNC_CFG1,         0x08}, // 0x08 | Sync Word Detection Configuration Reg. 1
  {CC112X_DEVIATION_M,       0x50}, // 0x50 | Frequency Deviation Configuration
  {CC112X_MODCFG_DEV_E,      0x0D}, // 0x0d | Modulation Format and Frequency Deviation Configur..
  {CC112X_DCFILT_CFG,        0x1C}, // 0x1c | Digital DC Removal Configuration
  {CC112X_IQIC,              0x00}, // 0x00 | Digital Image Channel Compensation Configuration
  {CC112X_CHAN_BW,           0x04}, // 0x04 | Channel Filter Configuration
  {CC112X_MDMCFG0,           0x05}, // 0x05 | General Modem Parameter Configuration Reg. 0
  {CC112X_AGC_CFG1,          0xA9}, // 0xa9 | Automatic Gain Control Configuration Reg. 1
  {CC112X_AGC_CFG0,          0xCF}, // 0xcf | Automatic Gain Control Configuration Reg. 0
  {CC112X_FIFO_CFG,          0x00}, // 0x00 | FIFO Configuration
  {CC112X_FS_CFG,            0x14}, // 0x14 | Frequency Synthesizer Configuration
  {CC112X_PKT_CFG1,          0x45}, // 0x45 | Packet Configuration Reg. 1
  {CC112X_PA_CFG0,           0x7E}, // 0x7e | Power Amplifier Configuration Reg. 0
  {CC112X_PKT_LEN,           0x56}, // 0x56 | Packet Length Configuration
  {CC112X_IF_MIX_CFG,        0x00}, // 0x00 | IF Mix Configuration
  {CC112X_FREQOFF_CFG,       0x22}, // 0x22 | Frequency Offset Correction Configuration
  {CC112X_FREQ2,             0x6C}, // 0x6c | Frequency Configuration [23:16]
  {CC112X_FREQ1,             0x7F}, // 0x7f | Frequency Configuration [15:8]
  {CC112X_FREQ0,             0xFF}, // 0xff | Frequency Configuration [7:0]
  {CC112X_FS_DIG1,           0x00}, // 0x00 | Frequency Synthesizer Digital Reg. 1
  {CC112X_FS_DIG0,           0x5F}, // 0x5f | Frequency Synthesizer Digital Reg. 0
  {CC112X_FS_CAL1,           0x40}, // 0x40 | Frequency Synthesizer Calibration Reg. 1
  {CC112X_FS_CAL0,           0x0E}, // 0x0e | Frequency Synthesizer Calibration Reg. 0
  {CC112X_FS_DIVTWO,         0x03}, // 0x03 | Frequency Synthesizer Divide by 2
  {CC112X_FS_DSM0,           0x33}, // 0x33 | FS Digital Synthesizer Module Configuration Reg. 0
  {CC112X_FS_DVC0,           0x17}, // 0x17 | Frequency Synthesizer Divider Chain Configuration ..
  {CC112X_FS_PFD,            0x50}, // 0x50 | Frequency Synthesizer Phase Frequency Detector Con..
  {CC112X_FS_PRE,            0x6E}, // 0x6e | Frequency Synthesizer Prescaler Configuration
  {CC112X_FS_REG_DIV_CML,    0x14}, // 0x14 | Frequency Synthesizer Divider Regulator Configurat..
  {CC112X_FS_SPARE,          0xAC}, // 0xac | Frequency Synthesizer Spare
  {CC112X_FS_VCO0,           0xB4}, // 0xb4 | FS Voltage Controlled Oscillator Configuration Reg..
  {CC112X_XOSC5,             0x0E}, // 0x0e | Crystal Oscillator Configuration Reg. 5
  {CC112X_XOSC1,             0x03}, // 0x03 | Crystal Oscillator Configuration Reg. 1
};

#define cc1120_433_1_2kbps_size sizeof(cc1120_433_1_2kbps_cfg)


#endif
