/*
 * board_config.c
 *
 *  Created on: Apr 21, 2022
 *      Author: sam
 */

#include "board_config.h"

Orientation_s IMU1_ACCEL_BOARD_TO_LOCAL[3] = {
    {AXIS_X, -1}, {AXIS_Z, -1}, {AXIS_Y, 1}};

Orientation_s IMU2_ACCEL_BOARD_TO_LOCAL[3] = {
    {AXIS_X, -1}, {AXIS_Z, -1}, {AXIS_Y, 1}};

Orientation_s HIGH_G_ACCEL_BOARD_TO_LOCAL[3] = {
    {AXIS_Y, 1}, {AXIS_Z, -1}, {AXIS_X, -1}};
