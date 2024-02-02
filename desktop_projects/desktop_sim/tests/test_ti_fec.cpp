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
  uint8_t in[] = {3, 1, 2, 3};

  FecEncoder encoder(true);
  FecDecoder decoder;

  encoder.Encode(reinterpret_cast<uint8_t*>(in), sizeof(in));
  uint8_t out[encoder.OutputSize(sizeof(in)) + 2];
  memcpy(out, encoder.OutputArray(), sizeof(out));

  uint8_t expectedEncoding[] = {0xC8, 0x3C, 0x00, 0x20, 0x84, 0xCF, 0x33, 0x31,
                                0xA2, 0xFC, 0x40, 0x4A, 0x44, 0x30, 0x47, 0xEF};
  // out includes the CRC, we don't need that
  PRINT_ARRAY(expectedEncoding);
  PRINT_ARRAY(out);
  ASSERT_EQ(sizeof(expectedEncoding) + 2, sizeof(out));
  ArraysEqual(out, expectedEncoding, sizeof(expectedEncoding));

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

  ASSERT_TRUE(ende.Encode(raw, ota1) == 0);
  ASSERT_TRUE(ende.Decode(ota1, decoded) == 0);

  ArraysEqual<uint8_t*, uint8_t*>((uint8_t*)&raw, (uint8_t*)&decoded,
                                  sizeof(raw));
}
