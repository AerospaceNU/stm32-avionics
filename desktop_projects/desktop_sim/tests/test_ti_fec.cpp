#include <gtest/gtest.h>
#include "ti_fec.h"

template<typename A, typename B>
void ArraysEqual(A a, B b, size_t count) {
    for (int i = 0; i < count; i++) {
        EXPECT_EQ(a[i], b[i]);
    }
}

typedef struct __attribute__((packed)) {
  uint8_t data[4] = {0};
  uint8_t crcHi;
  uint8_t crcLo;
} TestPacket_s;

TEST(TiFEC, EncodeDecode) {
    uint8_t in[] = {3, 1, 2, 3};

    uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
    for (int i = 0; i < sizeof(in); i++) {
        checksum = ti_fec::calculateCRC(in[i], checksum);
    }

    TestPacket_s packet;
    memcpy(packet.data, in, sizeof(in));
    packet.crcHi = checksum >> 8;
    packet.crcLo = checksum & 0xff;

    FecEncoder encoder;
    FecDecoder decoder;

    encoder.Encode(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));

    uint8_t out[encoder.OutputSize(sizeof(packet))];
    memcpy(out, encoder.OutputArray(), sizeof(out));

    uint8_t expectedEncoding[] = {0xC8, 0x3C, 0x00, 0x20, 0x84, 0xCF, 0x33, 0x31, 0xA2, 0xFC, 0x40, 0x4A, 0x44, 0x30, 0x47, 0xEF};
    ASSERT_EQ(sizeof(expectedEncoding), sizeof(out));
    ArraysEqual(out, expectedEncoding, sizeof(expectedEncoding));

    uint8_t decoded[sizeof(in)];
    decoder.FecDecode(out, decoded, sizeof(decoded));
    ArraysEqual(in, decoded, sizeof(in));
}