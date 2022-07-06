#include <stdio.h>

#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "dtoa.h"
#include "hardware_manager.h"

static void flash_usage_bar(char* barStr, uint8_t usage,
                            uint8_t totalBarWidth) {
  barStr[0] = '[';
  int i;
  for (i = 1; i <= (usage / 100.0) * totalBarWidth; ++i) {
    barStr[i] = '#';
  }
  int k;
  for (k = i; k <= totalBarWidth; ++k) {
    barStr[k] = ' ';
  }
  barStr[k++] = ']';
  barStr[k] = '\0';
}

void cli_tasks::cliSense() {
  // Send ack of command
  cliSendAck(true, nullptr);

  // Read sensor data and send in human-readable format
  HM_ReadSensorData();
  SensorData_t* data = HM_GetSensorData();
  char cliStr[60];
  char float1[10];
  char float2[10];
  char float3[10];

  cliSend("\r\n");
  snprintf(cliStr, sizeof(cliStr), "Timestamp (ms): %lu\r\n",
           data->timestamp_s);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->imu1_accel_x, 3);
  dtoa(float2, sizeof(float2), data->imu1_accel_y, 3);
  dtoa(float3, sizeof(float3), data->imu1_accel_z, 3);
  snprintf(cliStr, sizeof(cliStr), "IMU 1 Accel XYZ (m/s^2): %s %s %s\r\n",
           float1, float2, float3);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->imu1_gyro_x, 3);
  dtoa(float2, sizeof(float2), data->imu1_gyro_y, 3);
  dtoa(float3, sizeof(float3), data->imu1_gyro_z, 3);
  snprintf(cliStr, sizeof(cliStr), "IMU 1 Gyro XYZ (rad/s): %s %s %s\r\n",
           float1, float2, float3);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->imu1_mag_x, 3);
  dtoa(float2, sizeof(float2), data->imu1_mag_y, 3);
  dtoa(float3, sizeof(float3), data->imu1_mag_z, 3);
  snprintf(cliStr, sizeof(cliStr), "IMU 1 Mag XYZ (gauss): %s %s %s\r\n",
           float1, float2, float3);
  cliSend(cliStr);

  dtoa(float1, sizeof(float1), data->imu2_accel_x, 3);
  dtoa(float2, sizeof(float2), data->imu2_accel_y, 3);
  dtoa(float3, sizeof(float3), data->imu2_accel_z, 3);
  snprintf(cliStr, sizeof(cliStr), "IMU 2 Accel XYZ (m/s^2): %s %s %s\r\n",
           float1, float2, float3);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->imu2_gyro_x, 3);
  dtoa(float2, sizeof(float2), data->imu2_gyro_y, 3);
  dtoa(float3, sizeof(float3), data->imu2_gyro_z, 3);
  snprintf(cliStr, sizeof(cliStr), "IMU 2 Gyro XYZ (rad/s): %s %s %s\r\n",
           float1, float2, float3);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->imu2_mag_x, 3);
  dtoa(float2, sizeof(float2), data->imu2_mag_y, 3);
  dtoa(float3, sizeof(float3), data->imu2_mag_z, 3);
  snprintf(cliStr, sizeof(cliStr), "IMU 2 Mag XYZ (gauss): %s %s %s\r\n",
           float1, float2, float3);
  cliSend(cliStr);

  dtoa(float1, sizeof(float1), data->high_g_accel_x, 3);
  dtoa(float2, sizeof(float2), data->high_g_accel_y, 3);
  dtoa(float3, sizeof(float3), data->high_g_accel_z, 3);
  snprintf(cliStr, sizeof(cliStr), "High G Accel XYZ (m/s^2): %s %s %s\r\n",
           float1, float2, float3);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->baro1_pres, 5);
  snprintf(cliStr, sizeof(cliStr), "Baro 1 Pressure (atm): %s\r\n", float1);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->baro1_temp, 1);
  snprintf(cliStr, sizeof(cliStr), "Baro 1 Temp (C): %s\r\n", float1);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->baro2_pres, 5);
  snprintf(cliStr, sizeof(cliStr), "Baro 2 Pressure (atm): %s\r\n", float1);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->baro2_temp, 1);
  snprintf(cliStr, sizeof(cliStr), "Baro 2 Temp (C): %s\r\n", float1);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->gps_lat, 5);
  dtoa(float2, sizeof(float2), data->gps_long, 5);
  snprintf(cliStr, sizeof(cliStr), "GPS Lat/Lon: %s %s\r\n", float1, float2);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->gps_alt, 3);
  snprintf(cliStr, sizeof(cliStr), "GPS Alt (m): %s\r\n", float1);
  cliSend(cliStr);
  snprintf(cliStr, sizeof(cliStr),
           "GPS Time: %02i:%02i:%02i %02i/%02i/%04i\r\n", data->gps_hours,
           data->gps_minutes, data->gps_seconds, data->gps_month, data->gps_day,
           data->gps_year);
  cliSend(cliStr);
  dtoa(float1, sizeof(float1), data->battery_voltage, 3);
  snprintf(cliStr, sizeof(cliStr), "Battery Voltage (V): %s\r\n", float1);
  cliSend(cliStr);
  ssize_t numPyros =
      sizeof(data->pyro_continuity) / sizeof(data->pyro_continuity[0]);
  snprintf(cliStr, sizeof(cliStr), "Pyro Continuity 1-%i:", numPyros);
  cliSend(cliStr);
  for (ssize_t i = 0; i < numPyros; i++) {
    snprintf(cliStr, sizeof(cliStr), " %i", data->pyro_continuity[i]);
    cliSend(cliStr);
  }
  char flashStr[43];
  uint8_t flash_usage_percent = data_log_get_flash_usage();
  flash_usage_bar(flashStr, flash_usage_percent, 40);
  snprintf(cliStr, sizeof(cliStr), "\r\nCurrent Flash Usage: %u%%\r\n",
           flash_usage_percent);
  cliSend(cliStr);
  cliSend(flashStr);
  snprintf(cliStr, sizeof(cliStr), "\r\n");
  cliSend(cliStr);

  cliSendComplete(true, nullptr);
}
