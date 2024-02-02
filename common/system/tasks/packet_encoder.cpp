#include "packet_encoder.h"

#include <errno.h>
#include <string.h>

int FSKPacketRadioEncoder::Encode(RadioDecodedPacket_s& input,
                                  RadioOTAPayload_s& output) {
  encoder.Encode(reinterpret_cast<uint8_t*>(&input), sizeof(input));
  size_t outlen = encoder.OutputSize(sizeof(input));
  memcpy(output.payload, encoder.OutputArray(), outlen);
  output.payloadLen = outlen;

  return 0;
}

int FSKPacketRadioEncoder::Decode(RadioOTAPayload_s& input,
                                  RadioDecodedPacket_s& output) {
  if (input.payloadLen != encoder.OutputSize(sizeof(output))) {
    return -EMSGSIZE;
  }

  decoder.FecDecode(reinterpret_cast<uint8_t*>(&input.payload),
                    reinterpret_cast<uint8_t*>(&output), sizeof(output));

  return 0;
}

int LoRaRadioEncoder::Encode(RadioDecodedPacket_s& input,
                             RadioOTAPayload_s& output) {
  size_t outlen = sizeof(input);
  memcpy(output.payload, &input, outlen);
  output.payloadLen = outlen;

  return 0;
}

int LoRaRadioEncoder::Decode(RadioOTAPayload_s& input,
                             RadioDecodedPacket_s& output) {
  if (input.payloadLen != sizeof(output)) {
    return -EMSGSIZE;
  }
  memcpy(&output.payload, &input, sizeof(output));
  return 0;
}
