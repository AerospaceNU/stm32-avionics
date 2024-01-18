//
// Created by nathan on 5/14/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RFM_950_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RFM_950_H_

#if __has_include(<RadioLib.h>)

#include <RadioLib.h>
#include <SPI.h>

#include <new>

#include "radio.h"

class Sx1262 : public Radio {
 public:
  Sx1262();

  void init(int reset_pin, int cs_pin, int interrupt_pin);

  bool sendData(uint8_t *data, size_t len) override;

  bool isDataAvailable() override;

  bool readData(uint8_t *buffer, size_t buffer_length) override;

 protected:
  SX1262 *rf95{};
};

#endif

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RFM_950_H_
