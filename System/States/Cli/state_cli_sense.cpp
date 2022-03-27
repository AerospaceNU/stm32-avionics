
#include "state_cli_sense.h"

#include <stdio.h>

#include "cli.h"
#include "data_log.h"
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

void CliSenseState::init() {
  // Send ack of command
  cliSendAck(true, nullptr);
}

EndCondition_t CliSenseState::run() {
  // Read sensor data and send in human-readable format
  HM_ReadSensorData();
  SensorData_t* data = HM_GetSensorData();
  char cliStr[50];

  cliSend("\r\n");
  sprintf(cliStr, "Timestamp (ms): %lu\r\n", data->timestamp_s);
  cliSend(cliStr);
  sprintf(cliStr, "IMU 1 Accel XYZ (m/s^2): %.3f %.3f %.3f\r\n",
          data->imu1_accel_x, data->imu1_accel_y, data->imu1_accel_z);
  cliSend(cliStr);
  sprintf(cliStr, "IMU 1 Gyro XYZ (rad/s): %.3f %.3f %.3f\r\n",
          data->imu1_gyro_x, data->imu1_gyro_y, data->imu1_gyro_z);
  cliSend(cliStr);
  sprintf(cliStr, "IMU 1 Mag XYZ (gauss): %.3f %.3f %.3f\r\n", data->imu1_mag_x,
          data->imu1_mag_y, data->imu1_mag_z);
  cliSend(cliStr);
  sprintf(cliStr, "IMU 2 Accel XYZ (m/s^2): %.3f %.3f %.3f\r\n",
          data->imu2_accel_x, data->imu2_accel_y, data->imu2_accel_z);
  cliSend(cliStr);
  sprintf(cliStr, "IMU 2 Gyro XYZ (rad/s): %.3f %.3f %.3f\r\n",
          data->imu2_gyro_x, data->imu2_gyro_y, data->imu2_gyro_z);
  cliSend(cliStr);
  sprintf(cliStr, "IMU 2 Mag XYZ (gauss): %.3f %.3f %.3f\r\n", data->imu2_mag_x,
          data->imu2_mag_y, data->imu2_mag_z);
  cliSend(cliStr);
  sprintf(cliStr, "High G Accel XYZ (m/s^2): %.3f %.3f %.3f\r\n",
          data->high_g_accel_x, data->high_g_accel_y, data->high_g_accel_z);
  cliSend(cliStr);
  sprintf(cliStr, "Baro 1 Pressure (atm): %.5f\r\n", data->baro1_pres);
  cliSend(cliStr);
  sprintf(cliStr, "Baro 1 Temp (C): %.1f\r\n", data->baro1_temp);
  cliSend(cliStr);
  sprintf(cliStr, "Baro 2 Pressure (atm): %.5f\r\n", data->baro2_pres);
  cliSend(cliStr);
  sprintf(cliStr, "Baro 2 Temp (C): %.1f\r\n", data->baro2_temp);
  cliSend(cliStr);
  sprintf(cliStr, "GPS Lat/Lon: %.5f %.5f\r\n", data->gps_lat, data->gps_long);
  cliSend(cliStr);
  sprintf(cliStr, "GPS Alt (m): %f\r\n", data->gps_alt);
  cliSend(cliStr);
  sprintf(cliStr, "GPS Time: %02i:%02i:%02i %02i/%02i/%04i\r\n",
          data->gps_hours, data->gps_minutes, data->gps_seconds,
          data->gps_month, data->gps_day, data->gps_year);
  cliSend(cliStr);
  sprintf(cliStr, "Battery Voltage (V): %3f\r\n", data->battery_voltage);
  cliSend(cliStr);
  ssize_t numPyros =
      sizeof(data->pyro_continuity) / sizeof(data->pyro_continuity[0]);
  sprintf(cliStr, "Pyro Continuity 1-%i:", numPyros);
  cliSend(cliStr);
  for (ssize_t i = 0; i < numPyros; i++) {
    sprintf(cliStr, " %i", data->pyro_continuity[i]);
    cliSend(cliStr);
  }
  char flashStr[43];
  uint8_t flash_usage_percent = data_log_get_flash_usage();
  flash_usage_bar(flashStr, flash_usage_percent, 40);
  sprintf(cliStr, "\r\nCurrent Flash Usage: %u%%\r\n", flash_usage_percent);
  cliSend(cliStr);
  cliSend(flashStr);
  sprintf(cliStr, "\r\n");
  cliSend(cliStr);

  return EndCondition_t::CliCommandComplete;
}

void CliSenseState::cleanup() { cliSendComplete(true, nullptr); }
