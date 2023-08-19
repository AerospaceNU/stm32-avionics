#include <gtest/gtest.h>
#include "ti_fec.h"
TEST(TiFEC, EncodeDecode) {
    uint8_t in[] = {3, 1, 2, 3};

    FecEncoder encoder;
    FecDecoder decoder;

    encoder.Encode(in, sizeof(in));

    uint8_t out[encoder.OutputSize()];
    memcpy(out, encoder.GetOutputArray(), sizeof(out));

    ASSERT_THAT(out, ElementsAre(0x00, 0x0E, 0x8C, 0x03, 0x7C, 0x0D, 0xF0, 0x0E, 0x82, 0x8C, 0x0E, 0x5E, 0xF0, 0xD1, 0x8C, 0xD1));
}