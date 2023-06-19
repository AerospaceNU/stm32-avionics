/**
 * This file SHOULD NOT be edited manually.
 * You should edit `filter_data_gen.py`
 * and run it with Python to generate this file automatically.
 */
#include "filterdata_types.h"
FilterData_e getVariableEnumFromString(char *input) {
  if (strncmp(input, "heading", 7) == 0) {
    return heading;
  }
  if (strncmp(input, "vtg", 3) == 0) {
    return vtg;
  }
  if (strncmp(input, "pos_x", 5) == 0) {
    return pos_x;
  }
  if (strncmp(input, "pos_y", 5) == 0) {
    return pos_y;
  }
  if (strncmp(input, "pos_z_agl", 9) == 0) {
    return pos_z_agl;
  }
  if (strncmp(input, "rocket_vel_x", 12) == 0) {
    return rocket_vel_x;
  }
  if (strncmp(input, "rocket_vel_y", 12) == 0) {
    return rocket_vel_y;
  }
  if (strncmp(input, "rocket_vel_z", 12) == 0) {
    return rocket_vel_z;
  }
  if (strncmp(input, "rocket_acc_x", 12) == 0) {
    return rocket_acc_x;
  }
  if (strncmp(input, "rocket_acc_y", 12) == 0) {
    return rocket_acc_y;
  }
  if (strncmp(input, "rocket_acc_z", 12) == 0) {
    return rocket_acc_z;
  }
  if (strncmp(input, "world_vel_x", 11) == 0) {
    return world_vel_x;
  }
  if (strncmp(input, "world_vel_y", 11) == 0) {
    return world_vel_y;
  }
  if (strncmp(input, "world_vel_z", 11) == 0) {
    return world_vel_z;
  }
  if (strncmp(input, "world_acc_x", 11) == 0) {
    return world_acc_x;
  }
  if (strncmp(input, "world_acc_y", 11) == 0) {
    return world_acc_y;
  }
  if (strncmp(input, "world_acc_z", 11) == 0) {
    return world_acc_z;
  }
  if (strncmp(input, "rocket_ang_vel_x", 16) == 0) {
    return rocket_ang_vel_x;
  }
  if (strncmp(input, "rocket_ang_vel_y", 16) == 0) {
    return rocket_ang_vel_y;
  }
  if (strncmp(input, "rocket_ang_vel_z", 16) == 0) {
    return rocket_ang_vel_z;
  }
  if (strncmp(input, "qx", 2) == 0) {
    return qx;
  }
  if (strncmp(input, "qy", 2) == 0) {
    return qy;
  }
  if (strncmp(input, "qz", 2) == 0) {
    return qz;
  }
  if (strncmp(input, "qw", 2) == 0) {
    return qw;
  }
  if (strncmp(input, "roll", 4) == 0) {
    return roll;
  }
  if (strncmp(input, "pitch", 5) == 0) {
    return pitch;
  }
  if (strncmp(input, "yaw", 3) == 0) {
    return yaw;
  }
  if (strncmp(input, "angle_vertical", 14) == 0) {
    return angle_vertical;
  }
  // Matched no other valid value
  return invalid;
}
double getVariableValue(FilterData_s *filterData, FilterData_e valueType) {
  switch (valueType) {
    case invalid:
      return 0;
    case heading:
      return filterData->heading;
    case vtg:
      return filterData->vtg;
    case pos_x:
      return filterData->pos_x;
    case pos_y:
      return filterData->pos_y;
    case pos_z_agl:
      return filterData->pos_z_agl;
    case rocket_vel_x:
      return filterData->rocket_vel_x;
    case rocket_vel_y:
      return filterData->rocket_vel_y;
    case rocket_vel_z:
      return filterData->rocket_vel_z;
    case rocket_acc_x:
      return filterData->rocket_acc_x;
    case rocket_acc_y:
      return filterData->rocket_acc_y;
    case rocket_acc_z:
      return filterData->rocket_acc_z;
    case world_vel_x:
      return filterData->world_vel_x;
    case world_vel_y:
      return filterData->world_vel_y;
    case world_vel_z:
      return filterData->world_vel_z;
    case world_acc_x:
      return filterData->world_acc_x;
    case world_acc_y:
      return filterData->world_acc_y;
    case world_acc_z:
      return filterData->world_acc_z;
    case rocket_ang_vel_x:
      return filterData->rocket_ang_vel_x;
    case rocket_ang_vel_y:
      return filterData->rocket_ang_vel_y;
    case rocket_ang_vel_z:
      return filterData->rocket_ang_vel_z;
    case qx:
      return filterData->qx;
    case qy:
      return filterData->qy;
    case qz:
      return filterData->qz;
    case qw:
      return filterData->qw;
    case roll:
      return filterData->roll;
    case pitch:
      return filterData->pitch;
    case yaw:
      return filterData->yaw;
    case angle_vertical:
      return filterData->angle_vertical;
    default:
      return 0;
  }
}
void fillVariableName(char *buffer, size_t n, FilterData_e valueType) {
  switch (valueType) {
    case invalid:
      strncpy(buffer, "invalid", n);
      return;
    case heading:
      strncpy(buffer, "heading", n);
      return;
    case vtg:
      strncpy(buffer, "vtg", n);
      return;
    case pos_x:
      strncpy(buffer, "pos_x", n);
      return;
    case pos_y:
      strncpy(buffer, "pos_y", n);
      return;
    case pos_z_agl:
      strncpy(buffer, "pos_z_agl", n);
      return;
    case rocket_vel_x:
      strncpy(buffer, "rocket_vel_x", n);
      return;
    case rocket_vel_y:
      strncpy(buffer, "rocket_vel_y", n);
      return;
    case rocket_vel_z:
      strncpy(buffer, "rocket_vel_z", n);
      return;
    case rocket_acc_x:
      strncpy(buffer, "rocket_acc_x", n);
      return;
    case rocket_acc_y:
      strncpy(buffer, "rocket_acc_y", n);
      return;
    case rocket_acc_z:
      strncpy(buffer, "rocket_acc_z", n);
      return;
    case world_vel_x:
      strncpy(buffer, "world_vel_x", n);
      return;
    case world_vel_y:
      strncpy(buffer, "world_vel_y", n);
      return;
    case world_vel_z:
      strncpy(buffer, "world_vel_z", n);
      return;
    case world_acc_x:
      strncpy(buffer, "world_acc_x", n);
      return;
    case world_acc_y:
      strncpy(buffer, "world_acc_y", n);
      return;
    case world_acc_z:
      strncpy(buffer, "world_acc_z", n);
      return;
    case rocket_ang_vel_x:
      strncpy(buffer, "rocket_ang_vel_x", n);
      return;
    case rocket_ang_vel_y:
      strncpy(buffer, "rocket_ang_vel_y", n);
      return;
    case rocket_ang_vel_z:
      strncpy(buffer, "rocket_ang_vel_z", n);
      return;
    case qx:
      strncpy(buffer, "qx", n);
      return;
    case qy:
      strncpy(buffer, "qy", n);
      return;
    case qz:
      strncpy(buffer, "qz", n);
      return;
    case qw:
      strncpy(buffer, "qw", n);
      return;
    case roll:
      strncpy(buffer, "roll", n);
      return;
    case pitch:
      strncpy(buffer, "pitch", n);
      return;
    case yaw:
      strncpy(buffer, "yaw", n);
      return;
    case angle_vertical:
      strncpy(buffer, "angle_vertical", n);
      return;
  }
}
