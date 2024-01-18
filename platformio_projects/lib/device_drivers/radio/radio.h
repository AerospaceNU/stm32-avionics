//
// Created by nathan on 5/14/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RADIO_H_
#define PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RADIO_H_

#include <cstddef>
#include <cstdint>

#include "base_device.h"

class Radio : public virtual BaseDevice {
 public:
  explicit Radio(int max_message_length);

  virtual bool sendData(uint8_t *data, size_t len);

  virtual bool isDataAvailable();

  virtual bool readData(uint8_t *buffer, size_t buffer_length);

  int getMaxMessageLength() const;

  int getLastRssi() const;

 protected:
  int maxMessageLength;
  int lastRssi = 1;
};

#endif  // PLATFORMIO_PROJECTS_LIB_DEVICE_DRIVERS_RADIO_RADIO_H_
