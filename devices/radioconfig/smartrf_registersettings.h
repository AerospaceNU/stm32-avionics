/*
 * smartrf_registersettings.h
 *
 *  Created on: Feb 6, 2022
 *      Author: matth
 */

#ifndef DEVICES_RADIOCONFIG_SMARTRF_REGISTERSETTINGS_H_
#define DEVICES_RADIOCONFIG_SMARTRF_REGISTERSETTINGS_H_

typedef struct {
  uint16_t addr;
  uint8_t data;
} RegisterSetting_t;

#endif  // DEVICES_RADIOCONFIG_SMARTRF_REGISTERSETTINGS_H_
