#pragma once

#include <networktables/NetworkTableInstance.h>
#include <networktables/RawTopic.h>

#include "circular_buffer.h"
#include "data_structures.h"

class RocketNTInterface {
 public:
  RocketNTInterface();

 private:
  nt::NetworkTableInstance m_inst;
  nt::RawSubscriber m_radioUplink;
  nt::RawPublisher m_radioDownlink;
  nt::RawSubscriber m_cliPhoneUplink;
  nt::RawPublisher m_cliPhoneDownlink;
  nt::RawSubscriber m_usbUplink;
  nt::RawPublisher m_usbDownlink;

  CircularBuffer_s* m_radioRxBuffer;
  CircularBuffer_s* m_cliPhoneRxBuffer;
  CircularBuffer_s* m_usbDownlinkBuffer;

 public:
  inline void setRadioRXBuffer(CircularBuffer_s* rx_buffer) {
    m_radioRxBuffer = rx_buffer;
  }
  inline void setCliPhoneRxBuffer(CircularBuffer_s* rx_buffer) {
    m_cliPhoneRxBuffer = rx_buffer;
  }
  inline void setUsbDownlinkBuffer(CircularBuffer_s* rx_buffer) {
    m_usbDownlinkBuffer = rx_buffer;
  }

  void update();

  void radioSend(uint8_t*, size_t);
  void usbSend(uint8_t*, size_t);
  void cliPhoneTransmit(uint8_t*, size_t);
};
