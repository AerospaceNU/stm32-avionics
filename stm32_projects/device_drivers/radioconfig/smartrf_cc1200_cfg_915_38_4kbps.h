/**
 * smartrf_cc1200_cfg_915_38_4kbps.h
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_RADIOCONFIG_SMARTRF_CC1200_CFG_915_38_4KBPS_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_RADIOCONFIG_SMARTRF_CC1200_CFG_915_38_4KBPS_H_

#include "board_config_common.h"
#include "cc1120_cc1200_defs.h"
#include "smartrf_registersettings.h"

// Address Config = No address check
// Bit Rate = 38.4
// Carrier Frequency = 915.999756
// Deviation = 19.989014
// Device Address = 0
// Manchester Enable = false
// Modulation Format = 2-GFSK
// Packet Bit Length = 0
// Packet Length = 255
// Packet Length Mode = Variable
// RX Filter BW = 104.166667
// Symbol rate = 38.4
// Whitening = true, FEC = true

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
const RegisterSetting_s cc1200_916_1_2kbps_cfg[] = {
    {TIRADIO_IOCFG2, 0x06},     // GPIO2 IO Pin Configuration
    {TIRADIO_SYNC_CFG1, 0xA9},  // Sync Word Detection Configuration Reg. 1
    {TIRADIO_MODCFG_DEV_E,
     0x0B},  // Modulation Format and Frequency Deviation Configur..
    {TIRADIO_PREAMBLE_CFG0, 0x8A},  // Preamble Detection Configuration Reg. 0
    {TIRADIO_IQIC, 0xC8},  // Digital Image Channel Compensation Configuration
    {TIRADIO_CHAN_BW, 0x10},  // Channel Filter Configuration
    {TIRADIO_MDMCFG1, 0x42},  // General Modem Parameter Configuration Reg. 1
    {TIRADIO_MDMCFG0, 0x05},  // General Modem Parameter Configuration Reg. 0
    {TIRADIO_SYMBOL_RATE2,
     0x8F},  // Symbol Rate Configuration Exponent and Mantissa [1..
    {TIRADIO_SYMBOL_RATE1, 0x75},  // Symbol Rate Configuration Mantissa [15:8]
    {TIRADIO_SYMBOL_RATE0, 0x10},  // Symbol Rate Configuration Mantissa [7:0]
    {TIRADIO_AGC_REF, 0x27},       // AGC Reference Level Configuration
    {TIRADIO_AGC_CS_THR, 0x01},    // Carrier Sense Threshold Configuration
    {TIRADIO_AGC_CFG1, 0x11},  // Automatic Gain Control Configuration Reg. 1
    {TIRADIO_AGC_CFG0, 0x94},  // Automatic Gain Control Configuration Reg. 0
    {TIRADIO_FIFO_CFG, 0x00},  // FIFO Configuration
    {TIRADIO_FS_CFG, 0x12},    // Frequency Synthesizer Configuration
    {TIRADIO_PKT_CFG2,
     CCA_MODE_RSSI_LBT << CCA_MODE_SHIFT},  // Packet Configuration Reg. 2
    {TIRADIO_PKT_CFG1, 0b00000011},  // No hardware FEC, no hardware whiten, otherwise default
    {TIRADIO_PKT_CFG0, 0x20},        // Packet Configuration Reg. 0
    {TIRADIO_PA_CFG1, 0x55},         // Power Amplifier Configuration Reg. 1
    {TIRADIO_PKT_LEN, 0xFF},         // Packet Length Configuration
    {TIRADIO_IF_MIX_CFG, 0x1C},      // IF Mix Configuration
    {TIRADIO_TOC_CFG, 0x03},         // Timing Offset Correction Configuration
    {TIRADIO_MDMCFG2, 0x02},  // General Modem Parameter Configuration Reg. 2
    {TIRADIO_FREQ2, 0x5B},    // Frequency Configuration [23:16]
    {TIRADIO_FREQ1, 0x99},    // Frequency Configuration [15:8]
    {TIRADIO_FREQ0, 0x98},    // Frequency Configuration [7:0]
    {TIRADIO_IF_ADC1,
     0xEE},  // Analog to Digital Converter Configuration Reg. 1
    {TIRADIO_IF_ADC0,
     0x10},  // Analog to Digital Converter Configuration Reg. 0
    {TIRADIO_FS_DIG1, 0x04},    // Frequency Synthesizer Digital Reg. 1
    {TIRADIO_FS_DIG0, 0x55},    // Frequency Synthesizer Digital Reg. 0
    {TIRADIO_FS_CAL1, 0x40},    // Frequency Synthesizer Calibration Reg. 1
    {TIRADIO_FS_CAL0, 0x0E},    // Frequency Synthesizer Calibration Reg. 0
    {TIRADIO_FS_DIVTWO, 0x03},  // Frequency Synthesizer Divide by 2
    {TIRADIO_FS_DSM0,
     0x33},  // FS Digital Synthesizer Module Configuration Reg. 0
    {TIRADIO_FS_DVC0,
     0x17},  // Frequency Synthesizer Divider Chain Configuration ..
    {TIRADIO_FS_PFD,
     0x00},  // Frequency Synthesizer Phase Frequency Detector Con..
    {TIRADIO_FS_PRE, 0x6E},  // Frequency Synthesizer Prescaler Configuration
    {TIRADIO_FS_REG_DIV_CML,
     0x1C},  // Frequency Synthesizer Divider Regulator Configurat..
    {TIRADIO_FS_SPARE, 0xAC},  // Frequency Synthesizer Spare
    {TIRADIO_FS_VCO0,
     0xB5},  // FS Voltage Controlled Oscillator Configuration Reg..
    {TIRADIO_IFAMP, 0x09},  // Intermediate Frequency Amplifier Configuration
    {TIRADIO_XOSC5, 0x0E},  // Crystal Oscillator Configuration Reg. 5
    {TIRADIO_XOSC1, 0x03},  // Crystal Oscillator Configuration Reg. 1
};

#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_RADIOCONFIG_SMARTRF_CC1200_CFG_915_38_4KBPS_H_
