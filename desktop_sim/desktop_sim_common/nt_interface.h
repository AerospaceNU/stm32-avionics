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

RocketNTInterface::RocketNTInterface()
    : m_inst{nt::NetworkTableInstance::GetDefault()},
      m_radioUplink(
          m_inst.GetTable("rocket_sim")
              ->GetRawTopic("radio_uplink")
              .Subscribe("raw", {}, {.periodic = 1.0 / 65.0, .sendAll = true, .keepDuplicates = true})),
      m_radioDownlink(
          m_inst.GetTable("rocket_sim")
              ->GetRawTopic("radio_downlink")
              .Publish("raw", {.periodic = 1.0 / 65.0, .sendAll = true, .keepDuplicates = true})) {
  m_inst.StopClient();
  m_inst.StartServer();
}

void RocketNTInterface::usbSend(uint8_t *data, size_t len) {
  m_usbDownlink.Set(std::span<uint8_t>(data, data + len));
}

void RocketNTInterface::cliPhoneTransmit(uint8_t *data, size_t len) {
  m_cliPhoneDownlink.Set(std::span<uint8_t>(data, data + len));
}

void RocketNTInterface::radioSend(uint8_t *data, size_t len) {
  m_radioDownlink.Set(std::span<uint8_t>(data, data + len));
}

void RocketNTInterface::update() {
  for (const auto& uplinked : m_radioUplink.ReadQueue()) {
    auto bytes_received = uplinked.value.size();
    if (bytes_received > 0) {

      printf("Received Radio Message [");
      fwrite(uplinked.value.data() + 15, 1, bytes_received - 15, stdout);
      printf("]\n");

      static RadioRecievedPacket_s packet;
      packet.radioId = 0;
      packet.rssi = 0;
      packet.crc = true;
      packet.lqi = 0;
      memset(packet.data, 0, sizeof(packet.data));
      memcpy(packet.data, uplinked.value.data(), bytes_received);

      if (m_radioRxBuffer)
        cb_enqueue(m_radioRxBuffer, &packet);
    }
  }

  for (const auto& uplinked : m_cliPhoneUplink.ReadQueue()) {
    auto bytes_received = uplinked.value.size();
    if (bytes_received > 0) {

      printf("Received CLI Phone Message [");
      fwrite(uplinked.value.data(), 1, bytes_received, stdout);
      printf("]\n");

      if (m_cliPhoneRxBuffer) {
        for (const auto& byte : uplinked.value) {
          cb_enqueue(m_cliPhoneRxBuffer, &byte);
        }
      }
    }
  }

  for (const auto& uplinked : m_usbUplink.ReadQueue()) {
    auto bytes_received = uplinked.value.size();
    if (bytes_received > 0) {

      printf("Received USB Message [");
      fwrite(uplinked.value.data(), 1, bytes_received, stdout);
      printf("]\n");


      if (m_usbDownlinkBuffer) {
        for (const auto& byte : uplinked.value) {
          cb_enqueue(m_usbDownlinkBuffer, &byte);
        }
      }
    }
  }
}
