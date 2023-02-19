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

  CircularBuffer_s* radioRxBuffer;

 public:
  inline void setRadioRXBuffer(CircularBuffer_s* rx_buffer) {
    radioRxBuffer = rx_buffer;
  }

  void update();
  void radioSend(uint8_t*, size_t);
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

      if (radioRxBuffer)
        cb_enqueue(radioRxBuffer, &packet);
    }
  }
}
