#include <gtest/gtest.h>

#include "math_utils.h"

TEST(GpsFormatConversion, SimplePositive) {
  ASSERT_FLOAT_EQ(decimalminutes_to_decimaldegrees(4315.0), 43.25);
}

TEST(GpsFormatConversion, LongNegative) {
  ASSERT_FLOAT_EQ(decimalminutes_to_decimaldegrees(-12345.33), -123.7555);
}

TEST(GpsFormatConversion, NegativeZero) {
  ASSERT_FLOAT_EQ(decimalminutes_to_decimaldegrees(-012.0), -0.2);
}

TEST(GpsFormatConversion, Precise) {
  ASSERT_FLOAT_EQ(decimalminutes_to_decimaldegrees(11037.48265), 110.624711);
}
