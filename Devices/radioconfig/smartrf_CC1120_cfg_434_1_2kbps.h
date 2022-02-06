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

typedef struct
{
  uint16_t  addr;
  uint8_t   data;
}registerSetting_t;

#ifndef SMARTRF_CC1120_H
#define SMARTRF_CC1120_H

#define SMARTRF_RADIO_CC1120
#define SMARTRF_SETTING_IOCFG3           0x01 //GPIO3 IO Pin Configuration
#define SMARTRF_SETTING_IOCFG2           0x01 //GPIO2 IO Pin Configuration
#define SMARTRF_SETTING_IOCFG1           0xB0 //GPIO1 IO Pin Configuration
#define SMARTRF_SETTING_IOCFG0           0x01 //GPIO0 IO Pin Configuration
#define SMARTRF_SETTING_SYNC_CFG1        0x0B //Sync Word Detection Configuration Reg. 1
#define SMARTRF_SETTING_DCFILT_CFG       0x1C //Digital DC Removal Configuration
#define SMARTRF_SETTING_PREAMBLE_CFG1    0x18 //Preamble Length Configuration Reg. 1
#define SMARTRF_SETTING_IQIC             0xC6 //Digital Image Channel Compensation Configuration
#define SMARTRF_SETTING_CHAN_BW          0x08 //Channel Filter Configuration
#define SMARTRF_SETTING_MDMCFG0          0x05 //General Modem Parameter Configuration Reg. 0
#define SMARTRF_SETTING_AGC_REF          0x20 //AGC Reference Level Configuration
#define SMARTRF_SETTING_AGC_CS_THR       0x19 //Carrier Sense Threshold Configuration
#define SMARTRF_SETTING_AGC_CFG1         0xA9 //Automatic Gain Control Configuration Reg. 1
#define SMARTRF_SETTING_AGC_CFG0         0xCF //Automatic Gain Control Configuration Reg. 0
#define SMARTRF_SETTING_FIFO_CFG         0x00 //FIFO Configuration
#define SMARTRF_SETTING_FS_CFG           0x14 //Frequency Synthesizer Configuration
#define SMARTRF_SETTING_PKT_CFG0         0x20 //Packet Configuration Reg. 0
#define SMARTRF_SETTING_PKT_LEN          0xFF //Packet Length Configuration
#define SMARTRF_SETTING_IF_MIX_CFG       0x00 //IF Mix Configuration
#define SMARTRF_SETTING_FREQOFF_CFG      0x22 //Frequency Offset Correction Configuration
#define SMARTRF_SETTING_FREQ2            0x6C //Frequency Configuration [23:16]
#define SMARTRF_SETTING_FREQ1            0x80 //Frequency Configuration [15:8]
#define SMARTRF_SETTING_FS_DIG1          0x00 //Frequency Synthesizer Digital Reg. 1
#define SMARTRF_SETTING_FS_DIG0          0x5F //Frequency Synthesizer Digital Reg. 0
#define SMARTRF_SETTING_FS_CAL1          0x40 //Frequency Synthesizer Calibration Reg. 1
#define SMARTRF_SETTING_FS_CAL0          0x0E //Frequency Synthesizer Calibration Reg. 0
#define SMARTRF_SETTING_FS_DIVTWO        0x03 //Frequency Synthesizer Divide by 2
#define SMARTRF_SETTING_FS_DSM0          0x33 //FS Digital Synthesizer Module Configuration Reg. 0
#define SMARTRF_SETTING_FS_DVC0          0x17 //Frequency Synthesizer Divider Chain Configuration ..
#define SMARTRF_SETTING_FS_PFD           0x50 //Frequency Synthesizer Phase Frequency Detector Con..
#define SMARTRF_SETTING_FS_PRE           0x6E //Frequency Synthesizer Prescaler Configuration
#define SMARTRF_SETTING_FS_REG_DIV_CML   0x14 //Frequency Synthesizer Divider Regulator Configurat..
#define SMARTRF_SETTING_FS_SPARE         0xAC //Frequency Synthesizer Spare
#define SMARTRF_SETTING_FS_VCO0          0xB4 //FS Voltage Controlled Oscillator Configuration Reg..
#define SMARTRF_SETTING_XOSC5            0x0E //Crystal Oscillator Configuration Reg. 5
#define SMARTRF_SETTING_XOSC1            0x03 //Crystal Oscillator Configuration Reg. 1


static const registerSetting_t preferredSettings[]=
{
  {CC1120_IOCFG3,            0x01},
  {CC1120_IOCFG2,            0x01},
  {CC1120_IOCFG1,            0xB0},
  {CC1120_IOCFG0,            0x01},
  {CC1120_SYNC_CFG1,         0x0B},
  {CC1120_DCFILT_CFG,        0x1C},
  {CC1120_PREAMBLE_CFG1,     0x18},
  {CC1120_IQIC,              0xC6},
  {CC1120_CHAN_BW,           0x08},
  {CC1120_MDMCFG0,           0x05},
  {CC1120_AGC_REF,           0x20},
  {CC1120_AGC_CS_THR,        0x19},
  {CC1120_AGC_CFG1,          0xA9},
  {CC1120_AGC_CFG0,          0xCF},
  {CC1120_FIFO_CFG,          0x00},
  {CC1120_FS_CFG,            0x14},
  {CC1120_PKT_CFG0,          0x20},
  {CC1120_PKT_LEN,           0xFF},
  {CC1120_IF_MIX_CFG,        0x00},
  {CC1120_FREQOFF_CFG,       0x22},
  {CC1120_FREQ2,             0x6C},
  {CC1120_FREQ1,             0x80},
  {CC1120_FS_DIG1,           0x00},
  {CC1120_FS_DIG0,           0x5F},
  {CC1120_FS_CAL1,           0x40},
  {CC1120_FS_CAL0,           0x0E},
  {CC1120_FS_DIVTWO,         0x03},
  {CC1120_FS_DSM0,           0x33},
  {CC1120_FS_DVC0,           0x17},
  {CC1120_FS_PFD,            0x50},
  {CC1120_FS_PRE,            0x6E},
  {CC1120_FS_REG_DIV_CML,    0x14},
  {CC1120_FS_SPARE,          0xAC},
  {CC1120_FS_VCO0,           0xB4},
  {CC1120_XOSC5,             0x0E},
  {CC1120_XOSC1,             0x03},
};


#endif
