/*
 * smartrf_registersettings.h
 *
 *  Created on: Feb 6, 2022
 *      Author: matth
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_RADIOCONFIG_SMARTRF_REGISTERSETTINGS_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_RADIOCONFIG_SMARTRF_REGISTERSETTINGS_H_

typedef struct {
  uint16_t addr;
  uint8_t data;
} RegisterSetting_s;

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_RADIOCONFIG_SMARTRF_REGISTERSETTINGS_H_
