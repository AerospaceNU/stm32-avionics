//
// Created by matth on 3/26/2022.
//

#include "flight_replay.h"

#include <algorithm>  // std::min

CsvReplay::CsvReplay(std::string path) : doc{path} {}

void CsvReplay::GetNext(SensorData_t* data) {
  try {
    data->timestamp_s = doc.GetCell<double>("timestamp_s", m_row);

    data->imu1_accel_x = doc.GetCell<double>("imu1_accel_x_real", m_row);
    data->imu2_accel_x = doc.GetCell<double>("imu2_accel_x_real", m_row);
    data->imu1_accel_y = doc.GetCell<double>("imu1_accel_y_real", m_row);
    data->imu2_accel_y = doc.GetCell<double>("imu2_accel_y_real", m_row);
    data->imu1_accel_z = doc.GetCell<double>("imu1_accel_z_real", m_row);
    data->imu2_accel_z = doc.GetCell<double>("imu2_accel_z_real", m_row);
    data->imu1_gyro_x = doc.GetCell<double>("imu1_gyro_x_real", m_row);
    data->imu2_gyro_x = doc.GetCell<double>("imu2_gyro_x_real", m_row);
    data->imu1_gyro_y = doc.GetCell<double>("imu1_gyro_y_real", m_row);
    data->imu2_gyro_y = doc.GetCell<double>("imu2_gyro_y_real", m_row);
    data->imu1_gyro_z = doc.GetCell<double>("imu1_gyro_z_real", m_row);
    data->imu2_gyro_z = doc.GetCell<double>("imu2_gyro_z_real", m_row);
    data->imu1_mag_x = doc.GetCell<double>("imu1_mag_x_real", m_row);
    data->imu2_mag_x = doc.GetCell<double>("imu2_mag_x_real", m_row);
    data->imu1_mag_y = doc.GetCell<double>("imu1_mag_y_real", m_row);
    data->imu2_mag_y = doc.GetCell<double>("imu2_mag_y_real", m_row);
    data->imu1_mag_z = doc.GetCell<double>("imu1_mag_z_real", m_row);
    data->imu2_mag_z = doc.GetCell<double>("imu2_mag_z_real", m_row);

    data->high_g_accel_x = doc.GetCell<double>("high_g_accel_x_real", m_row);
    data->baro1_pres = doc.GetCell<double>("baro1_pres", m_row);
    data->baro2_pres = doc.GetCell<double>("baro2_pres", m_row);

    data->battery_voltage = doc.GetCell<double>("battery_voltage", m_row);

    uint8_t pyroCont = doc.GetCell<unsigned int>("pyro_cont", m_row);
    for (int i = 0; i < sizeof(data->pyro_continuity); i++) {
      data->pyro_continuity[i] = (pyroCont >> i) & 0b1;
    }

    data->gps_lat = doc.GetCell<double>("gps_lat", m_row);
    data->gps_long = doc.GetCell<double>("gps_long", m_row);
    data->gps_alt = doc.GetCell<double>("gps_alt", m_row);

    m_row = std::min(m_row + 1, doc.GetRowCount() - 1);
  } catch (const std::exception& e) {
    std::cout << e.what();
  }
}
