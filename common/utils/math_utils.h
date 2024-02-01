/*
 * math_utils.h
 *
 *  Created on: Oct 1, 2022
 *      Author: matth
 */

#ifndef COMMON_UTILS_MATH_UTILS_H_
#define COMMON_UTILS_MATH_UTILS_H_

// From
// https://github.com/PhotonVision/photonvision/blob/715ef62c85b631b68f446c4bc37c28bfa57c5ec6/photon-core/src/main/java/org/photonvision/common/util/math/MathUtils.java#L75
// Licensed under GNU/GPL Version 3
double map(double value, double in_min, double in_max, double out_min,
           double out_max);

double clamp(double value, double low, double high);

/**
 * Convert a latitude or longitude in DDDMM.MMMM format to DDD.DDDD format.
 */
float decimalminutes_to_decimaldegrees(float ddmm);

#endif  //  COMMON_UTILS_MATH_UTILS_H_
