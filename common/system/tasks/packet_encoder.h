#pragma once

#include "board_config.h"
#include "radio_packet_types.h"
#include "ti_fec.h"

class FSKPacketRadioEncoder {
 public:
  int encode(const RadioDecodedPacket_s& input, RadioOTAPayload_s& output);
  int decode(const RadioOTAPayload_s& input, RadioDecodedPacket_s& output);

 private:
  FecEncoder encoder;
  FecDecoder decoder;
};

class PassthroughRadioEncoder {
 public:
  int encode(const RadioDecodedPacket_s& input, RadioOTAPayload_s& output);
  int decode(const RadioOTAPayload_s& input, RadioDecodedPacket_s& output);
};
