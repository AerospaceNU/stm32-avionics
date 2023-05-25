/**
 * This file SHOULD NOT be edited manually.
 * You should edit `filter_data_gen.py`
 * and run it with Python to generate this file automatically.
 */
#ifndef SYSTEM_FILTERING_FILTERDATA_TYPES_H_
#define SYSTEM_FILTERING_FILTERDATA_TYPES_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <string.h>
typedef struct {
  double heading;
  double vtg;
  double pos_x;
  double pos_y;
  double pos_z_agl;
  double rocket_vel_x;
  double rocket_vel_y;
  double rocket_vel_z;
  double rocket_acc_x;
  double rocket_acc_y;
  double rocket_acc_z;
  double world_vel_x;
  double world_vel_y;
  double world_vel_z;
  double world_acc_x;
  double world_acc_y;
  double world_acc_z;
  float rocket_ang_vel_x;
  float rocket_ang_vel_y;
  float rocket_ang_vel_z;
  float rocket_magnetic_flux_x;
  float rocket_magnetic_flux_y;
  float rocket_magnetic_flux_z;
  double qx;
  double qy;
  double qz;
  double qw;
  double roll;
  double pitch;
  double yaw;
  double angle_vertical;
} FilterData_s;

typedef enum {
  invalid,
  heading,
  vtg,
  pos_x,
  pos_y,
  pos_z_agl,
  rocket_vel_x,
  rocket_vel_y,
  rocket_vel_z,
  rocket_acc_x,
  rocket_acc_y,
  rocket_acc_z,
  world_vel_x,
  world_vel_y,
  world_vel_z,
  world_acc_x,
  world_acc_y,
  world_acc_z,
  rocket_ang_vel_x,
  rocket_ang_vel_y,
  rocket_ang_vel_z,
  qx,
  qy,
  qz,
  qw,
  roll,
  pitch,
  yaw,
  angle_vertical,
} FilterData_e;

FilterData_e getVariableEnumFromString(char *input);
float getVariableValue(FilterData_s *filterData, FilterData_e valueType);
void fillVariableName(char *buffer, int n, FilterData_e valueType);
#ifdef __cplusplus
}
#endif
#endif  // SYSTEM_FILTERING_FILTERDATA_TYPES_H_
