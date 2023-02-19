//
// Created by matth on 3/26/2022.
//

#include "flight_replay.h"

#include <algorithm>  // std::min
#include <string>

#include "hardware_manager.h"

CsvReplay::CsvReplay(std::string path) : doc{path} {}

void CsvReplay::getNext(SensorData_s* data) {
  try {
    char rowName[30];

    data->timestampMs = doc.GetCell<double>("timestamp_s", m_row);

#if HAS_DEV(IMU_DESKTOP_FILE)
    for (int i = 0; i < NUM_IMU_DESKTOP_FILE; i++) {
      snprintf(rowName, sizeof(rowName), "imu%i_accel_x_real", i + 1);
      data->imuData[i].accelRealMps2.x = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_accel_y_real", i + 1);
      data->imuData[i].accelRealMps2.y = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_accel_z_real", i + 1);
      data->imuData[i].accelRealMps2.z = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_gyro_x_real", i + 1);
      data->imuData[i].angVelRealRadps.x = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_gyro_y_real", i + 1);
      data->imuData[i].angVelRealRadps.y = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_gyro_z_real", i + 1);
      data->imuData[i].angVelRealRadps.z = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_mag_x_real", i + 1);
      data->imuData[i].magRealG.x = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_mag_y_real", i + 1);
      data->imuData[i].magRealG.y = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "imu%i_mag_z_real", i + 1);
      data->imuData[i].magRealG.z = doc.GetCell<double>(rowName, m_row);
    }
#endif  // HAS_DEV(IMU_DESKTOP_FILE)

#if HAS_DEV(ACCEL_DESKTOP_FILE)
    for (int i = 0; i < NUM_ACCEL_DESKTOP_FILE; i++) {
      snprintf(rowName, sizeof(rowName), "high_g_accel_x_real");
      data->accelData[i].realMps2.x = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "high_g_accel_y_real");
      data->accelData[i].realMps2.y = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "high_g_accel_z_real");
      data->accelData[i].realMps2.z = doc.GetCell<double>(rowName, m_row);
    }
#endif  // HAS_DEV(ACCEL_DESKTOP_FILE)

#if HAS_DEV(BAROMETER_DESKTOP_FILE)
    for (int i = 0; i < NUM_BAROMETER_DESKTOP_FILE; i++) {
      snprintf(rowName, sizeof(rowName), "baro%i_pres", i + 1);
      data->barometerData[i].pressureAtm = doc.GetCell<double>(rowName, m_row);
      snprintf(rowName, sizeof(rowName), "baro%i_temp", i + 1);
      data->barometerData[i].temperatureC = doc.GetCell<double>(rowName, m_row);
    }
#endif  // HAS_DEV(BAROMETER_DESKTOP_FILE)

#if HAS_DEV(VBAT_DESKTOP_FILE)
    for (int i = 0; i < NUM_VBAT_DESKTOP_FILE; i++) {
      data->vbatData[i] = doc.GetCell<double>("battery_voltage", m_row);
    }
#endif  // HAS_DEV(VBAT_DESKTOP_FILE)

#if HAS_DEV(PYRO_CONT_DESKTOP_FILE)
    uint8_t pyroCont = doc.GetCell<unsigned int>("pyro_cont", m_row);
    for (int i = 0; i < NUM_PYRO_CONT_DESKTOP_FILE; i++) {
      data->pyroContData[i] = (pyroCont >> i) & 0b1;
    }
#endif  // HAS_DEV(PYRO_CONT_DESKTOP_FILE)

#if HAS_DEV(GPS_DESKTOP_FILE)
    for (int i = 0; i < NUM_GPS_DESKTOP_FILE; i++) {
      data->gpsData[i].generalData.latitude =
          doc.GetCell<double>("gps_lat", m_row);
      data->gpsData[i].generalData.longitude =
          doc.GetCell<double>("gps_long", m_row);
      data->gpsData[i].generalData.altitude =
          doc.GetCell<double>("gps_alt", m_row);
    }
#endif  // HAS_DEV(GPS_DESKTOP_FILE)

    m_row = std::min(m_row + 1, doc.GetRowCount() - 1);
  } catch (const std::exception& e) {
    std::cout << e.what();
  }
}
