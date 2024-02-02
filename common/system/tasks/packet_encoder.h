#pragma once

#include "board_config.h"
#include "radio_packet_types.h"
#include "ti_fec.h"

class FSKPacketRadioEncoder {
 public:
  int Encode(RadioDecodedPacket_s& input, RadioOTAPayload_s& output);
  int Decode(RadioOTAPayload_s& input, RadioDecodedPacket_s& output);
  inline const size_t EncodedLength() { return encoder.OutputSize(sizeof(RadioDecodedPacket_s)); }

 private:
  FecEncoder encoder;
  FecDecoder decoder;
};

class PassthroughRadioEncoder {
 public:
  int Encode(RadioDecodedPacket_s& input, RadioOTAPayload_s& output);
  int Decode(RadioOTAPayload_s& input, RadioDecodedPacket_s& output);
  inline const size_t EncodedLength() { return sizeof(RadioDecodedPacket_s); }
};
