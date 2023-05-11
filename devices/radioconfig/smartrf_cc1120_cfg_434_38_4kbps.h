/**
 * smartrf_cc1120_cfg_434_38_4kbps.h
 */

#ifndef DEVICES_RADIOCONFIG_SMARTRF_CC1120_CFG_434_38_4KBPS_H_
#define DEVICES_RADIOCONFIG_SMARTRF_CC1120_CFG_434_38_4KBPS_H_

#include "board_config_common.h"
#include "cc1120_cc1200_defs.h"
#include "smartrf_registersettings.h"

// Address Config = No address check
// Bit Rate = 1.2
// Carrier Frequency = 433.000000
// Deviation = 3.997803
// Device Address = 0
// Manchester Enable = false
// Modulation Format = 2-FSK
// PA Ramping = true
// Packet Bit Length = 0
// Packet Length = 255
// Packet Length Mode = Variable
// Performance Mode = High Performance
// RX Filter BW = 125.000000
// Symbol rate = 1.2
// TX Power = 0
// Whitening = false

const RegisterSetting_s cc1120_433_1_2kbps_cfg[] = {
    {TIRADIO_IOCFG3, 0xB0},         // GPIO3 IO Pin Configuration
    {TIRADIO_IOCFG2, 0x06},         // GPIO2 IO Pin Configuration
    {TIRADIO_IOCFG1, 0xB0},         // GPIO1 IO Pin Configuration
    {TIRADIO_IOCFG0, 0x40},         // GPIO0 IO Pin Configuration
    {TIRADIO_SYNC_CFG1, 0x0B},      // Sync Word Detection Configuration Reg. 1
    {TIRADIO_DCFILT_CFG, 0x1C},     // Digital DC Removal Configuration
    {TIRADIO_PREAMBLE_CFG1, 0x18},  // Preamble Length Configuration Reg. 1
    {TIRADIO_IQIC, 0x46},  // Digital Image Channel Compensation Configuration
    {TIRADIO_CHAN_BW, 0x41},     // Channel Filter Configuration
    {TIRADIO_MDMCFG0, 0x05},     // General Modem Parameter Configuration Reg. 0
    {TIRADIO_AGC_REF, 0x20},     // AGC Reference Level Configuration
    {TIRADIO_AGC_CS_THR, 0x19},  // Carrier Sense Threshold Configuration
    {TIRADIO_AGC_CFG1, 0xA9},    // Automatic Gain Control Configuration Reg. 1
    {TIRADIO_AGC_CFG0, 0xCF},    // Automatic Gain Control Configuration Reg. 0
    {TIRADIO_FIFO_CFG, 0x00},    // FIFO Configuration
    {TIRADIO_FS_CFG, 0x14},      // Frequency Synthesizer Configuration
    {TIRADIO_PKT_CFG0, 0x20},    // Packet Configuration Reg. 0
    {TIRADIO_PA_CFG2, 0x5D},     // Power Amplifier Configuration Reg. 2
    {TIRADIO_PA_CFG0, 0x7E},     // Power Amplifier Configuration Reg. 0
    {TIRADIO_PKT_LEN, 0xFF},     // Packet Length Configuration
    {TIRADIO_IF_MIX_CFG, 0x00},  // IF Mix Configuration
    {TIRADIO_FREQOFF_CFG, 0x22},  // Frequency Offset Correction Configuration
    {TIRADIO_FREQ2, 0x6C},        // Frequency Configuration [23:16]
    {TIRADIO_FREQ1, 0x40},        // Frequency Configuration [15:8]
    {TIRADIO_FS_DIG1, 0x00},      // Frequency Synthesizer Digital Reg. 1
    {TIRADIO_FS_DIG0, 0x5F},      // Frequency Synthesizer Digital Reg. 0
    {TIRADIO_FS_CAL1, 0x40},      // Frequency Synthesizer Calibration Reg. 1
    {TIRADIO_FS_CAL0, 0x0E},      // Frequency Synthesizer Calibration Reg. 0
    {TIRADIO_FS_DIVTWO, 0x03},    // Frequency Synthesizer Divide by 2
    {TIRADIO_FS_DSM0,
     0x33},  // FS Digital Synthesizer Module Configuration Reg. 0
    {TIRADIO_FS_DVC0,
     0x17},  // Frequency Synthesizer Divider Chain Configuration ..
    {TIRADIO_FS_PFD,
     0x50},  // Frequency Synthesizer Phase Frequency Detector Con..
    {TIRADIO_FS_PRE, 0x6E},  // Frequency Synthesizer Prescaler Configuration
    {TIRADIO_FS_REG_DIV_CML,
     0x14},  // Frequency Synthesizer Divider Regulator Configurat..
    {TIRADIO_FS_SPARE, 0xAC},  // Frequency Synthesizer Spare
    {TIRADIO_FS_VCO0,
     0xB4},  // FS Voltage Controlled Oscillator Configuration Reg..
    {TIRADIO_XOSC5, 0x0E},  // Crystal Oscillator Configuration Reg. 5
    {TIRADIO_XOSC1, 0x03},  // Crystal Oscillator Configuration Reg. 1
};

//#endif
#endif  // DEVICES_RADIOCONFIG_SMARTRF_CC1120_CFG_434_38_4KBPS_H_
