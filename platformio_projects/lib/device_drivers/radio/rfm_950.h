//
// Created by nathan on 5/14/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RFM_950_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RFM_950_H_

#if __has_include(<RH_RF95.h>)

#include <RH_RF95.h>
#include <SPI.h>

#include <new>

#include "radio.h"

class Rfm950 : public Radio {
 public:
  Rfm950();

  void init(int reset_pin, int cs_pin, int interrupt_pin);

  bool sendData(uint8_t *data, size_t len) override;

  bool isDataAvailable() override;

  bool readData(uint8_t *buffer, size_t buffer_length) override;

 protected:
  uint8_t driver_memory[sizeof(RH_RF95)]{};
  RH_RF95 *rf95{};
};

#endif  // RH_RF95_h

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RFM_950_H_