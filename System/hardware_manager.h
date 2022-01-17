#ifndef HARDWARE_MANAGER_H_
#define HARDWARE_MANAGER_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include <stdint.h>

#include "circular_buffer.h"

/* Useful defines for files that need to know some info about hardware it works with */
#define FLASH_SECTOR_BYTES 		0x40000
#define FLIGHT_METADATA_PAGES 	2
#define FLASH_SIZE_BYTES 		0x4000000
#define FLASH_PAGE_SIZE_BYTES	0x200
#define FLASH_TIMEOUT_MS 		500
#define PYRO_CONTINUITY_THRESHOLD 3

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

typedef enum hardware_t{
	CC1120 = 0,
	IMU1,
	IMU2,
	BAROMETER1,
	BAROMETER2,
	HIGH_G_ACCELEROMETER,
	NUM_HARDWARE
} hardware_t;

void HM_HardwareInit();

/* Microcontroller timer functions */
uint32_t HM_Millis();

/* Flash functions */
bool HM_FlashReadStart(uint32_t startLoc, uint32_t numBytes, uint8_t *pData);
bool HM_FlashWriteStart(uint32_t startLoc, uint32_t numBytes, uint8_t *data);
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
bool HM_RadioSend(const uint8_t *data, uint16_t numBytes);

/* USB functions */
bool HM_UsbIsConnected();
bool HM_UsbTransmit(uint8_t* data, uint16_t numBytes);
CircularBuffer_t* HM_UsbGetRxBuffer();

/* Bluetooth functions */
bool HM_BluetoothSend(const uint8_t* data, uint16_t numBytes);

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

#endif /* HARDWARE_MANAGER_H_ */
