#include <cinttypes>
#include <cstdio>

#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "data_structures.h"
#include "dtoa.h"
#include "hardware_manager.h"

static void flashUsageBar(char* barStr, uint8_t usage, uint8_t totalBarWidth) {
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

void CliTasks::sense() {
  // Send ack of command
  cli_sendAck(true, nullptr);

  // Read sensor data and send in human-readable format
  hm_readSensorData();
  SensorData_s* data = hm_getSensorData();
  char cliStr[60];
  char float1[10];
  char float2[10];
  char float3[10];

  cli_send("\r\n");
  snprintf(cliStr, sizeof(cliStr), "Timestamp (ms): %" PRIu32 "\r\n",
           data->timestampMs);
  cli_send(cliStr);
#if HAS_DEV(IMU)
  for (int i = 0; i < NUM_IMU; i++) {
    dtoa(float1, sizeof(float1), data->imuData[i].accelRealMps2.x, 3);
    dtoa(float2, sizeof(float2), data->imuData[i].accelRealMps2.y, 3);
    dtoa(float3, sizeof(float3), data->imuData[i].accelRealMps2.z, 3);
    snprintf(cliStr, sizeof(cliStr), "IMU %d Accel XYZ (m/s^2): %s %s %s\r\n",
             i, float1, float2, float3);
    cli_send(cliStr);
    dtoa(float1, sizeof(float1), data->imuData[i].angVelRealRadps.x, 3);
    dtoa(float2, sizeof(float2), data->imuData[i].angVelRealRadps.y, 3);
    dtoa(float3, sizeof(float3), data->imuData[i].angVelRealRadps.z, 3);
    snprintf(cliStr, sizeof(cliStr), "IMU %d Gyro XYZ (rad/s): %s %s %s\r\n", i,
             float1, float2, float3);
    cli_send(cliStr);
    dtoa(float1, sizeof(float1), data->imuData[i].magRealG.x, 3);
    dtoa(float2, sizeof(float2), data->imuData[i].magRealG.y, 3);
    dtoa(float3, sizeof(float3), data->imuData[i].magRealG.z, 3);
    snprintf(cliStr, sizeof(cliStr), "IMU %d Mag XYZ (gauss): %s %s %s\r\n", i,
             float1, float2, float3);
    cli_send(cliStr);
  }
#endif  // HAS_DEV(IMU)
#if HAS_DEV(ACCEL)
  for (int i = 0; i < NUM_ACCEL; i++) {
    dtoa(float1, sizeof(float1), data->accelData[i].realMps2.x, 3);
    dtoa(float2, sizeof(float2), data->accelData[i].realMps2.y, 3);
    dtoa(float3, sizeof(float3), data->accelData[i].realMps2.z, 3);
    snprintf(cliStr, sizeof(cliStr), "Accel %d XYZ (m/s^2): %s %s %s\r\n", i,
             float1, float2, float3);
    cli_send(cliStr);
  }
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
  for (int i = 0; i < NUM_BAROMETER; i++) {
    dtoa(float1, sizeof(float1), data->barometerData[i].pressureAtm, 5);
    snprintf(cliStr, sizeof(cliStr), "Baro %d Pressure (atm): %s\r\n", i,
             float1);
    cli_send(cliStr);
    dtoa(float1, sizeof(float1), data->barometerData[i].temperatureC, 1);
    snprintf(cliStr, sizeof(cliStr), "Baro %d Temp (C): %s\r\n", i, float1);
    cli_send(cliStr);
  }
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(GPS)
  for (int i = 0; i < NUM_GPS; i++) {
    dtoa(float1, sizeof(float1), data->gpsData[i].generalData.latitude / 100.0,
         5);
    dtoa(float2, sizeof(float2), data->gpsData[i].generalData.longitude / 100.0,
         5);
    snprintf(cliStr, sizeof(cliStr), "GPS %d Lat/Lon: %s %s\r\n", i, float1,
             float2);
    cli_send(cliStr);
    dtoa(float1, sizeof(float1), data->gpsData[i].generalData.altitude, 3);
    snprintf(cliStr, sizeof(cliStr), "GPS %d Alt (m): %s\r\n", i, float1);
    cli_send(cliStr);
    snprintf(cliStr, sizeof(cliStr),
             "GPS %d Time: %02i:%02i:%02i %02i/%02i/%04i\r\n", i,
             data->gpsData[i].timeData.hours, data->gpsData[i].timeData.minutes,
             data->gpsData[i].timeData.seconds, data->gpsData[i].timeData.month,
             data->gpsData[i].timeData.day, data->gpsData[i].timeData.year);
    cli_send(cliStr);
  }
#endif  // HAS_DEV(GPS)
#if HAS_DEV(VBAT)
  for (int i = 0; i < NUM_VBAT; i++) {
    dtoa(float1, sizeof(float1), data->vbatData[i], 3);
    snprintf(cliStr, sizeof(cliStr), "Battery Voltage %d (V): %s\r\n", i,
             float1);
    cli_send(cliStr);
  }
#endif  // HAS_DEV(VBAT)
#if HAS_DEV(PYRO_CONT)
  snprintf(cliStr, sizeof(cliStr), "Pyro Continuity 1-%" PRIu32 ":",
           (uint32_t)(NUM_PYRO_CONT));
  cli_send(cliStr);
  for (int i = 0; i < NUM_PYRO_CONT; i++) {
    snprintf(cliStr, sizeof(cliStr), " %i", data->pyroContData[i]);
    cli_send(cliStr);
  }
#endif  // HAS_DEV(PYRO_CONT)
#if HAS_DEV(FLASH)
  char flashStr[43];
  uint8_t flash_usage_percent = dataLog_getFlashUsage();
  flashUsageBar(flashStr, flash_usage_percent, 40);
  snprintf(cliStr, sizeof(cliStr), "\r\nCurrent Flash Usage: %u%%\r\n",
           flash_usage_percent);
  cli_send(cliStr);
  cli_send(flashStr);
#endif  // HAS_DEV(FLASH)
  snprintf(cliStr, sizeof(cliStr), "\r\n");
  cli_send(cliStr);

  cli_sendComplete(true, nullptr);
}
