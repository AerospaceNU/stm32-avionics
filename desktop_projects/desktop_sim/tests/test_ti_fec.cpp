#include <gtest/gtest.h>

#include "packet_encoder.h"
#include "ti_fec.h"

template <typename A, typename B>
void ArraysEqual(A a, B b, size_t count) {
  for (int i = 0; i < count; i++) {
    EXPECT_EQ(a[i], b[i]);
  }
}

template <typename A>
void PrintArray(A a, size_t count, const char* line) {
  std::cout << std::setw(15) << std::hex;
  std::cout << line;
  std::cout << ":\t[";
  for (int i = 0; i < count; i++) {
    std::cout << (int)a[i] << ", ";
  }
  std::cout << "]\n";
}
#define PRINT_ARRAY(a) PrintArray(a, sizeof(a), #a)

TEST(TiFEC, EncodeDecode) {
  uint8_t in[] = {3, 1, 2, 3, 57};

  FecEncoder encoder;
  FecDecoder decoder;

  encoder.Encode(reinterpret_cast<uint8_t*>(in), sizeof(in));
  uint8_t out[encoder.OutputSize(sizeof(in)) + 2];
  memcpy(out, encoder.OutputArray(), sizeof(out));

  // for debugging
  PRINT_ARRAY(out);

  // see if it decodes to the same thing
  uint8_t decoded[sizeof(in)];
  decoder.FecDecode(out, decoded, sizeof(decoded));
  ArraysEqual(in, decoded, sizeof(in));
}

TEST(PacketEncoder, FSKEncodeDecode) {
  FSKPacketRadioEncoder ende;

  RadioDecodedPacket_s raw = {0};
  raw.board_serial_num = 45;
  RadioDecodedPacket_s decoded;
  RadioOTAPayload_s ota1;

  ASSERT_EQ(0, ende.encode(raw, ota1));
  ASSERT_EQ(0, ende.decode(ota1, decoded));

  ArraysEqual<uint8_t*, uint8_t*>((uint8_t*)&raw, (uint8_t*)&decoded,
                                  sizeof(raw));
}
