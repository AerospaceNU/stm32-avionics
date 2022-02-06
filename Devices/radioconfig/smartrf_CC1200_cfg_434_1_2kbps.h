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


#define SMARTRF_SETTING_IOCFG3           0x01 //GPIO3 IO Pin Configuration
#define SMARTRF_SETTING_IOCFG2           0x01 //GPIO2 IO Pin Configuration
#define SMARTRF_SETTING_IOCFG1           0x30 //GPIO1 IO Pin Configuration
#define SMARTRF_SETTING_IOCFG0           0x01 //GPIO0 IO Pin Configuration
#define SMARTRF_SETTING_DEVIATION_M      0xD1 // Frequency Deviation Configuration
#define SMARTRF_SETTING_MODCFG_DEV_E     0x00 // Modulation Format and Frequency Deviation Configur..
#define SMARTRF_SETTING_DCFILT_CFG       0x5D // Digital DC Removal Configuration
#define SMARTRF_SETTING_PREAMBLE_CFG0    0x8A // Preamble Detection Configuration Reg. 0
#define SMARTRF_SETTING_IQIC             0xCB // Digital Image Channel Compensation Configuration
#define SMARTRF_SETTING_CHAN_BW          0xA6 // Channel Filter Configuration
#define SMARTRF_SETTING_MDMCFG1          0x40 // General Modem Parameter Configuration Reg. 1
#define SMARTRF_SETTING_MDMCFG0          0x05 // General Modem Parameter Configuration Reg. 0
#define SMARTRF_SETTING_SYMBOL_RATE2     0x3F // Symbol Rate Configuration Exponent and Mantissa [1..
#define SMARTRF_SETTING_SYMBOL_RATE1     0x75 // Symbol Rate Configuration Mantissa [15:8]
#define SMARTRF_SETTING_SYMBOL_RATE0     0x10 // Symbol Rate Configuration Mantissa [7:0]
#define SMARTRF_SETTING_AGC_REF          0x20 // AGC Reference Level Configuration
#define SMARTRF_SETTING_AGC_CS_THR       0xEC // Carrier Sense Threshold Configuration
#define SMARTRF_SETTING_AGC_CFG1         0x51 // Automatic Gain Control Configuration Reg. 1
#define SMARTRF_SETTING_AGC_CFG0         0x87 // Automatic Gain Control Configuration Reg. 0
#define SMARTRF_SETTING_FIFO_CFG         0x00 // FIFO Configuration
#define SMARTRF_SETTING_FS_CFG           0x14 // Frequency Synthesizer Configuration
#define SMARTRF_SETTING_PKT_CFG2         0x00 // Packet Configuration Reg. 2
#define SMARTRF_SETTING_PKT_CFG0         0x00 // Packet Configuration Reg. 0
#define SMARTRF_SETTING_PKT_LEN          0xFF // Packet Length Configuration
#define SMARTRF_SETTING_IF_MIX_CFG       0x1C // IF Mix Configuration
#define SMARTRF_SETTING_FREQOFF_CFG      0x22 // Frequency Offset Correction Configuration
#define SMARTRF_SETTING_MDMCFG2          0x0C // General Modem Parameter Configuration Reg. 2
#define SMARTRF_SETTING_FREQ2            0x56 // Frequency Configuration [23:16]
#define SMARTRF_SETTING_FREQ1            0xCC // Frequency Configuration [15:8]
#define SMARTRF_SETTING_FREQ0            0xCC // Frequency Configuration [7:0]
#define SMARTRF_SETTING_IF_ADC1          0xEE // Analog to Digital Converter Configuration Reg. 1
#define SMARTRF_SETTING_IF_ADC0          0x10 // Analog to Digital Converter Configuration Reg. 0
#define SMARTRF_SETTING_FS_DIG1          0x07 // Frequency Synthesizer Digital Reg. 1
#define SMARTRF_SETTING_FS_DIG0          0xAF // Frequency Synthesizer Digital Reg. 0
#define SMARTRF_SETTING_FS_CAL1          0x40 // Frequency Synthesizer Calibration Reg. 1
#define SMARTRF_SETTING_FS_CAL0          0x0E // Frequency Synthesizer Calibration Reg. 0
#define SMARTRF_SETTING_FS_DIVTWO        0x03 // Frequency Synthesizer Divide by 2
#define SMARTRF_SETTING_FS_DSM0          0x33 // FS Digital Synthesizer Module Configuration Reg. 0
#define SMARTRF_SETTING_FS_DVC0          0x17 // Frequency Synthesizer Divider Chain Configuration ..
#define SMARTRF_SETTING_FS_PFD           0x00 // Frequency Synthesizer Phase Frequency Detector Con..
#define SMARTRF_SETTING_FS_PRE           0x6E // Frequency Synthesizer Prescaler Configuration
#define SMARTRF_SETTING_FS_REG_DIV_CML   0x1C // Frequency Synthesizer Divider Regulator Configurat..
#define SMARTRF_SETTING_FS_SPARE         0xAC // Frequency Synthesizer Spare
#define SMARTRF_SETTING_FS_VCO0          0xB5 // FS Voltage Controlled Oscillator Configuration Reg..
#define SMARTRF_SETTING_XOSC5            0x0E // Crystal Oscillator Configuration Reg. 5
#define SMARTRF_SETTING_XOSC1            0x03 // Crystal Oscillator Configuration Reg. 1

static const registerSetting_t preferredSettings[]=
{
  {CC112X_IOCFG3,            0x01},
  {CC112X_IOCFG2,            0x01},
  {CC112X_IOCFG1,            0x30},
  {CC112X_IOCFG0,            0x01},
  {CC112X_DEVIATION_M,       0xD1},
  {CC112X_MODCFG_DEV_E,      0x00},
  {CC112X_DCFILT_CFG,        0x5D},
  {CC112X_PREAMBLE_CFG0,     0x8A},
  {CC112X_IQIC,              0xCB},
  {CC112X_CHAN_BW,           0xA6},
  {CC112X_MDMCFG1,           0x40},
  {CC112X_MDMCFG0,           0x05},
  {CC112X_SYMBOL_RATE2,      0x3F},
  {CC112X_SYMBOL_RATE1,      0x75},
  {CC112X_SYMBOL_RATE0,      0x10},
  {CC112X_AGC_REF,           0x20},
  {CC112X_AGC_CS_THR,        0xEC},
  {CC112X_AGC_CFG1,          0x51},
  {CC112X_AGC_CFG0,          0x87},
  {CC112X_FIFO_CFG,          0x00},
  {CC112X_FS_CFG,            0x14},
  {CC112X_PKT_CFG2,          0x00},
  {CC112X_PKT_CFG0,          0x20},
  {CC112X_PKT_LEN,           0xFF},
  {CC112X_IF_MIX_CFG,        0x1C},
  {CC112X_FREQOFF_CFG,       0x22},
  {CC112X_MDMCFG2,           0x0C},
  {CC112X_FREQ2,             0x56},
  {CC112X_FREQ1,             0xCC},
  {CC112X_FREQ0,             0xCC},
  {CC112X_IF_ADC1,           0xEE},
  {CC112X_IF_ADC0,           0x10},
  {CC112X_FS_DIG1,           0x07},
  {CC112X_FS_DIG0,           0xAF},
  {CC112X_FS_CAL1,           0x40},
  {CC112X_FS_CAL0,           0x0E},
  {CC112X_FS_DIVTWO,         0x03},
  {CC112X_FS_DSM0,           0x33},
  {CC112X_FS_DVC0,           0x17},
  {CC112X_FS_PFD,            0x00},
  {CC112X_FS_PRE,            0x6E},
  {CC112X_FS_REG_DIV_CML,    0x1C},
  {CC112X_FS_SPARE,          0xAC},
  {CC112X_FS_VCO0,           0xB5},
  {CC112X_XOSC5,             0x0E},
  {CC112X_XOSC1,             0x03},
};


#endif
