#include "math_utils.h"

#include <cmath>

// From
// https://github.com/PhotonVision/photonvision/blob/715ef62c85b631b68f446c4bc37c28bfa57c5ec6/photon-core/src/main/java/org/photonvision/common/util/math/MathUtils.java#L75
// Licensed under GNU/GPL Version 3
double map(double value, double in_min, double in_max, double out_min,
           double out_max) {
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double clamp(double value, double low, double high) {
  return fmax(low, fmin(value, high));
}

float decimalminutes_to_decimaldegrees(float ddmm) {
  int degrees = ddmm / 100;
  float minutes = ddmm - (degrees * 100);
  return degrees + (minutes / 60.);
}
