#ifndef HARDWARE_MANAGER_H_
#define HARDWARE_MANAGER_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include <stdint.h>

/* Useful defines for files that need to know some info about hardware it works with */
#define FLASH_SECTOR_BYTES 		0x40000
#define FLIGHT_METADATA_PAGES 	2
#define FLASH_SIZE_BYTES 		0x40000000
#define FLASH_PAGE_SIZE_BYTES	0x200
#define FLASH_TIMEOUT_MS 		500

typedef struct {
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
	double baro1_alt;
	double baro2_pres;
	double baro2_temp;
	double baro2_alt;
	double gps_lat;
	double gps_long;
	double gps_alt;
	double battery_voltage;
	bool pyro1_continuity;
	bool pyro2_continuity;
	bool pyro3_continuity;
	bool pyro4_continuity;
	bool pyro5_continuity;
	bool pyro6_continuity;
	double heading;
	double vtg;
	double pos_x;
	double pos_y;
	double pos_z;
	double vel_x;
	double vel_y;
	double vel_z;
	double qx;
	double qy;
	double qz;
	double qw;
} SensorData_t;

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
bool HM_RadioSend(const uint8_t *data, uint32_t numBytes);

/* USB functions */
bool HM_UsbIsConnected();

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

#ifdef __cplusplus
}
#endif

#endif /* HARDWARE_MANAGER_H_ */
