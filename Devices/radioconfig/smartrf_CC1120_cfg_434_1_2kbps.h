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


static const registerSetting_t cc1120_433_1_2kbps_cfg[]=
{
  {CC112X_IOCFG3,            0x01},
  {CC112X_IOCFG2,            0x01},
  {CC112X_IOCFG1,            0xB0},
  {CC112X_IOCFG0,            0x01},
  {CC112X_SYNC_CFG1,         0x0B},
  {CC112X_DCFILT_CFG,        0x1C},
  {CC112X_PREAMBLE_CFG1,     0x18},
  {CC112X_IQIC,              0xC6},
  {CC112X_CHAN_BW,           0x08},
  {CC112X_MDMCFG0,           0x05},
  {CC112X_AGC_REF,           0x20},
  {CC112X_AGC_CS_THR,        0x19},
  {CC112X_AGC_CFG1,          0xA9},
  {CC112X_AGC_CFG0,          0xCF},
  {CC112X_FIFO_CFG,          0x00},
  {CC112X_FS_CFG,            0x14},
  {CC112X_PKT_CFG0,          0x20},
  {CC112X_PKT_LEN,           0xFF},
  {CC112X_IF_MIX_CFG,        0x00},
  {CC112X_FREQOFF_CFG,       0x22},
  {CC112X_FREQ2,             0x6C},
  {CC112X_FREQ1,             0x80},
  {CC112X_FS_DIG1,           0x00},
  {CC112X_FS_DIG0,           0x5F},
  {CC112X_FS_CAL1,           0x40},
  {CC112X_FS_CAL0,           0x0E},
  {CC112X_FS_DIVTWO,         0x03},
  {CC112X_FS_DSM0,           0x33},
  {CC112X_FS_DVC0,           0x17},
  {CC112X_FS_PFD,            0x50},
  {CC112X_FS_PRE,            0x6E},
  {CC112X_FS_REG_DIV_CML,    0x14},
  {CC112X_FS_SPARE,          0xAC},
  {CC112X_FS_VCO0,           0xB4},
  {CC112X_XOSC5,             0x0E},
  {CC112X_XOSC1,             0x03},
};

#define cc1120_433_1_2kbps_size sizeof(cc1120_433_1_2kbps_cfg)


#endif
