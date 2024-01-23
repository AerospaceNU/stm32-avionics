#include <gtest/gtest.h>

#include "math_utils.h"

/**
* See http://web.archive.org/web/20240122230227/https://www.maptools.com/tutorials/lat_lon/formats
* for information about Lat/Long formats. To convert from degrees and decimal minutes to decimal degrees,
* separate out the hundreds place and up (degrees). The rest is the minutes and can be divided by 60
* to get the digits that should go after the decimal place.
*
* Example: 1030.0 -> 10 degrees, 30.0 minutes -> 30.0/60 = 0.5 -> 10.5
*/

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
