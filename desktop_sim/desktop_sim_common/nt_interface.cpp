#include "board_config.h"
#include "board_config_common.h"

#if HAS_DEV(NT_INTERFACE)

#include "nt_interface.h"

#include <networktables/NetworkTableInstance.h>
#include <networktables/RawTopic.h>

#include "circular_buffer.h"
#include "data_structures.h"

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

#endif
