#ifndef SYSTEM_HARDWARE_MANAGER_H_
#define SYSTEM_HARDWARE_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "circular_buffer.h"
#include "data_structures.h"

/* Useful defines for files that need to know some info about hardware it works
 * with */
#define FLASH_SECTOR_BYTES 0x40000
#define FLIGHT_METADATA_PAGES 2
#define FLASH_SIZE_BYTES 0x4000000
#define FLASH_PAGE_SIZE_BYTES 0x200
#define FLASH_TIMEOUT_MS 500
#define PYRO_CONTINUITY_THRESHOLD 3

#define KHZ_TO_HZ 1000
#define MHZ_TO_HZ 1000000
#define RADIO_CHANNEL_BANDWIDTH 150 * KHZ_TO_HZ
#define RAD433_CHAN0 433 * MHZ_TO_HZ
#define RAD915_CHAN0 914 * MHZ_TO_HZ

typedef struct __attribute__((__packed__)) {
  uint32_t timestamp_s;
  uint32_t timestamp_us;
  int16_t imu1_accel_x_raw;
  int16_t imu1_accel_y_raw;
  int16_t imu1_accel_z_raw;
  double imu1_accel_x;
  double imu1_accel_y;
  double imu1_accel_z;
  int16_t imu1_gyro_x_raw;
  int16_t imu1_gyro_y_raw;
  int16_t imu1_gyro_z_raw;
  double imu1_gyro_x;
  double imu1_gyro_y;
  double imu1_gyro_z;
  int16_t imu1_mag_x_raw;
  int16_t imu1_mag_y_raw;
  int16_t imu1_mag_z_raw;
  double imu1_mag_x;
  double imu1_mag_y;
  double imu1_mag_z;
  int16_t imu2_accel_x_raw;
  int16_t imu2_accel_y_raw;
  int16_t imu2_accel_z_raw;
  double imu2_accel_x;
  double imu2_accel_y;
  double imu2_accel_z;
  int16_t imu2_gyro_x_raw;
  int16_t imu2_gyro_y_raw;
  int16_t imu2_gyro_z_raw;
  double imu2_gyro_x;
  double imu2_gyro_y;
  double imu2_gyro_z;
  int16_t imu2_mag_x_raw;
  int16_t imu2_mag_y_raw;
  int16_t imu2_mag_z_raw;
  double imu2_mag_x;
  double imu2_mag_y;
  double imu2_mag_z;
  int16_t high_g_accel_x_raw;
  int16_t high_g_accel_y_raw;
  int16_t high_g_accel_z_raw;
  double high_g_accel_x;
  double high_g_accel_y;
  double high_g_accel_z;
  double baro1_pres;
  double baro1_temp;
  double baro2_pres;
  double baro2_temp;
  float gps_lat;
  float gps_long;
  float gps_alt;
  float gps_speed;
  float gps_course;
  float gps_latitude_deviation;
  float gps_longitude_deviation;
  float gps_altitude_deviation;
  float gps_speed_kph;
  float gps_speed_knots;
  uint64_t gps_timestamp;
  int gps_seconds;
  int gps_minutes;
  int gps_hours;
  int gps_day;
  int gps_month;
  int gps_year;
  int gps_num_sats;
  char gps_status;
  double battery_voltage;
  bool pyro_continuity[6];
} SensorData_t;

// Sensor fullscales, in m/s/s
typedef struct {
  double imu1_accel_fs;
  double imu2_accel_fs;
} SensorProperties_t;

typedef enum hardware_t {
  RADIO_433 = 0,
  RADIO_915,
  IMU1,
  IMU2,
  BAROMETER1,
  BAROMETER2,
  HIGH_G_ACCELEROMETER,
  NUM_HARDWARE
} Hardware_t;

// Addresses
typedef enum {
  ADDR_NRF_FCB = 0,  // the NRF on the FCB
  ADDR_PHONE = 1,    // A phone
  ADDR_CUTTER1 = 2,  // LC 1
  ADDR_CUTTER2 = 3,  // LC 2

  MAX_ADDRESS = ADDR_CUTTER2,
  NUM_ADDRESSES = ADDR_CUTTER2 + 1
} BluetoothAddresses_te;

void HM_HardwareInit();

bool* HM_GetHardwareStatus();

/* Microcontroller timer functions */
uint32_t HM_Millis();

/* Flash functions */
bool HM_FlashReadStart(uint32_t startLoc, uint32_t numBytes, uint8_t* pData);
bool HM_FlashWriteStart(uint32_t startLoc, uint32_t numBytes, uint8_t* data);
bool HM_FlashEraseSectorStart(uint32_t sectorNum);
bool HM_FlashEraseChipStart();
bool HM_FlashIsReadComplete();
bool HM_FlashIsWriteComplete();
bool HM_FlashIsEraseComplete();

/* Buzzer functions for hardware manager */
void HM_BuzzerSetFrequency(float fHz);
void HM_BuzzerStart();
void HM_BuzzerStop();

/* Servo functions */
void HM_ServoSetAngle(int servoNum, float degrees);

/* LED functions */
void HM_LedSet(int ledNum, bool on);
void HM_LedToggle(int ledNum);

/* Radio functions */
bool HM_RadioSend(Hardware_t radio, uint8_t* data, uint16_t numBytes);
void HM_RadioRegisterConsumer(Hardware_t radio, CircularBuffer_t* rxBuffer);
void HM_RadioUpdate();
void HM_RadioSetChannel(Hardware_t radio, int channel);

/* USB functions */
bool HM_UsbIsConnected();
bool HM_UsbTransmit(uint8_t* data, uint16_t numBytes);
CircularBuffer_t* HM_UsbGetRxBuffer();

/* Bluetooth functions */
//! Return if a phone is connected
bool HM_BluetoothCliConnected();
bool HM_BluetoothSend(uint8_t address, const uint8_t* data, uint16_t numBytes);
CircularBuffer_t* Hm_BleConsoleGetRxBuffer();

void HM_BluetoothTick();

LineCutterData_t* HM_GetLineCutterData(BluetoothAddresses_te address);
LineCutterFlightVars_t* HM_GetLineCutterFlightVariables(
    BluetoothAddresses_te address);

bool HM_LineCutterSendString(int id, char* string);
bool HM_LineCuttersSendCut(int chan);

/* Outputs */
void HM_PyroFire(int channel, uint32_t duration);
void HM_PyroUpdate();
void HM_CameraPyroSet(bool on);

/* Sensor functions */

/* Sensor sampling enable/disable functions. All true initially */
void HM_SetImu1Sampling(bool enable);
void HM_SetImu2Sampling(bool enable);
void HM_SetHighGSampling(bool enable);
void HM_SetBaro1Sampling(bool enable);
void HM_SetBaro2Sampling(bool enable);
void HM_SetGpsSampling(bool enable);
void HM_SetBatteryVoltageSampling(bool enable);
void HM_SetPyroContinuitySampling(bool enable);
void HM_IWDG_Refresh();

/**
 * @brief Updates sensor data from enabled sensors
 */
void HM_ReadSensorData();

/**
 * @brief Returns a pointer to the current sensor data (statically allocated)
 * @return Pointer to statically-allocated struct where sensor data is stored
 */
SensorData_t* HM_GetSensorData();

/**
 * @brief Returns a pointer to the current sensor properties
 * @return Pointer to statically-allocated struct where sensor properties are
 * stored
 */
SensorProperties_t* HM_GetSensorProperties();

/**
 * @brief Turn on or off sim mode in the hardware manager
 * @param[in] rxBuffer: Buffer to read from in during sim
 */
void HM_EnableSimMode(CircularBuffer_t* rxBuffer);

/**
 * @brief Turn off sim mode in the hardware manager
 */
void HM_DisableSimMode();

/**
 * @brief Return whether hardware manager is currently in sim mode
 * @return True if in sim mode, false if not
 */
bool HM_InSimMode();

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_HARDWARE_MANAGER_H_
