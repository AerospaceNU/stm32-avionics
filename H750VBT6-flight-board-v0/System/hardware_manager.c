
#include "hardware_manager.h"
#include "main.h"
#include "LSM9DS1.h"
#include "ms5607.h"
#include "H3LIS331DL.h"
#include "servo.h"
#include "buzzer.h"
#include "S25FLx.h"
#include "usbd_cdc_if.h"

#include "tim.h"

/* IMUs */
static LSM9DS1Ctrl_t lsm9ds1_1;
static LSM9DS1Ctrl_t lsm9ds1_2;

/* Barometers */
static MS5607Ctrl_t ms5607_1;
static MS5607Ctrl_t ms5607_2;

/* High G Accelerometers */
static H3LIS331DLCtrl_t h3lis_1;

/* Servos */
static ServoCtrl_t servo1;
static ServoCtrl_t servo2;
static ServoCtrl_t servo3;
static ServoCtrl_t servo4;

/* Buzzers */
static BuzzerCtrl_t buzzer;

/* Flash memory */
static S25FLXCtrl_t s25flx1;

/* USB device */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* Sensor data */
static SensorData_t sensorData;

/* Sensor states */
static bool bImu1Sampling = true;
static bool bImu2Sampling = true;
static bool bHighGSampling = true;
static bool bBaro1Sampling = true;
static bool bBaro2Sampling = true;
static bool bGpsSampling = true;
static bool bBatteryVoltageSampling = true;
static bool bPyroContinuitySampling = true;

void HM_HardwareInit() {

	/* LSM9DS1 IMU 1 */
	HAL_GPIO_WritePin(IMU1_AG_CS_PORT, IMU1_AG_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(IMU1_M_CS_PORT, IMU1_M_CS_PIN, GPIO_PIN_SET);
	lsm9ds1_1.ag.LSM9DS1SPI.hspi = IMU1_AG_HSPI;
	lsm9ds1_1.ag.LSM9DS1SPI.port = IMU1_AG_CS_PORT;
	lsm9ds1_1.ag.LSM9DS1SPI.pin = IMU1_AG_CS_PIN;
	lsm9ds1_1.m.LSM9DS1SPI.hspi = IMU1_M_HSPI;
	lsm9ds1_1.m.LSM9DS1SPI.port = IMU1_M_CS_PORT;
	lsm9ds1_1.m.LSM9DS1SPI.pin = IMU1_M_CS_PIN;
	LSM9DS1_init(&lsm9ds1_1);

	/* LSM9DS1 IMU 2 */
	HAL_GPIO_WritePin(IMU2_AG_CS_PORT, IMU2_AG_CS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(IMU2_M_CS_PORT, IMU2_M_CS_PIN, GPIO_PIN_SET);
	lsm9ds1_2.ag.LSM9DS1SPI.hspi = IMU2_AG_HSPI;
	lsm9ds1_2.ag.LSM9DS1SPI.port = IMU2_AG_CS_PORT;
	lsm9ds1_2.ag.LSM9DS1SPI.pin = IMU2_AG_CS_PIN;
	lsm9ds1_2.m.LSM9DS1SPI.hspi = IMU2_M_HSPI;
	lsm9ds1_2.m.LSM9DS1SPI.port = IMU2_M_CS_PORT;
	lsm9ds1_2.m.LSM9DS1SPI.pin = IMU2_M_CS_PIN;
	LSM9DS1_init(&lsm9ds1_2);

	/* MS5607 Barometer 1 */
	HAL_GPIO_WritePin(BARO1_CS_PORT, BARO1_CS_PIN, GPIO_PIN_SET);
	ms5607_1.spiconfig.hspi = BARO1_HSPI;
	ms5607_1.spiconfig.port = BARO1_CS_PORT;
	ms5607_1.spiconfig.pin = BARO1_CS_PIN;
	MS5607_init(&ms5607_1);

	/* MS5607 Barometer 2 */
	HAL_GPIO_WritePin(BARO2_CS_PORT, BARO2_CS_PIN, GPIO_PIN_SET);
	ms5607_2.spiconfig.hspi = BARO2_HSPI;
	ms5607_2.spiconfig.port = BARO2_CS_PORT;
	ms5607_2.spiconfig.pin = BARO2_CS_PIN;
	MS5607_init(&ms5607_2);

	/* H3LIS331DL High G Accelerometer */
	HAL_GPIO_WritePin(HIGH_G_CS_PORT, HIGH_G_CS_PIN, GPIO_PIN_SET);
	h3lis_1.H3LIS331DLSPI.hspi = HIGH_G_HSPI;
	h3lis_1.H3LIS331DLSPI.port = HIGH_G_CS_PORT;
	h3lis_1.H3LIS331DLSPI.pin = HIGH_G_CS_PIN;
	H3LIS331DL_init(&h3lis_1);

	/* Servos 1-4
	 *
	 * TODO: minPulseMS and maxPulseMS will need a method of tuning based on servo (currently 0.75-2.25)
	 * TODO: Initial angles need to be determined
	 */
	servoInit(&servo1, SERVO1_HTIM, SERVO1_CHANNEL, 20, 0.75, 2.25, -90, 90, 0);
	servoInit(&servo2, SERVO2_HTIM, SERVO2_CHANNEL, 20, 0.75, 2.25, -90, 90, 0);
	servoInit(&servo3, SERVO3_HTIM, SERVO3_CHANNEL, 20, 0.75, 2.25, -90, 90, 0);
	servoInit(&servo4, SERVO4_HTIM, SERVO4_CHANNEL, 20, 0.75, 2.25, -90, 90, 0);

	/* Buzzer */
	buzzerInit(&buzzer, BUZZER_HTIM, BUZZER_CHANNEL, 500);

	/* Flash */
	S25FLX_init(&s25flx1, FLASH_HSPI, FLASH_CS_PORT, FLASH_CS_PIN, FLASH_SIZE_BYTES);

	/* LED 1 */
	HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
}

uint32_t HM_Millis() {
	return HAL_GetTick();
}

bool HM_FlashReadStart(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
	return S25FLX_read_start(&s25flx1, startLoc, numBytes, pData);
}

bool HM_FlashWriteStart(uint32_t startLoc, uint32_t numBytes, uint8_t *data) {
	return S25FLX_write_start(&s25flx1, startLoc, numBytes, data);
}

bool HM_FlashEraseSectorStart(uint32_t sectorNum) {
	return S25FLX_erase_sector_start(&s25flx1, sectorNum);
}

bool HM_FlashEraseChipStart() {
	return S25FLX_erase_chip_start(&s25flx1);
}


bool HM_FlashIsReadComplete() {
#ifdef USE_S25FLx_DMA
	return S25FLX_is_read_complete(&s25flx1);
#else
	return true;
#endif
}

bool HM_FlashIsWriteComplete() {
	return S25FLX_is_write_complete(&s25flx1);
}

bool HM_FlashIsEraseComplete() {
	return S25FLX_is_erase_complete(&s25flx1);
}

void HM_BuzzerSetFrequency(float fHz) {
	buzzerSetFrequency(&buzzer, fHz);
}

void HM_BuzzerStart() {
	buzzerStart(&buzzer);
}

void HM_BuzzerStop() {
	buzzerStop(&buzzer);
}

void HM_ServoSetAngle(int servoNum, float degrees) {
	switch(servoNum) {
	case 1:
		servoSetAngle(&servo1, degrees);
		break;
	case 2:
		servoSetAngle(&servo2, degrees);
		break;
	case 3:
		servoSetAngle(&servo3, degrees);
		break;
	case 4:
		servoSetAngle(&servo4, degrees);
		break;
	default:
		break;
	}
}

void HM_LedSet(int ledNum, bool set) {
	switch(ledNum) {
	case 1:
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, set);
		break;
	default:
		break;
	}
}

void HM_LedToggle(int ledNum) {
	switch(ledNum) {
	case 1:
		HAL_GPIO_TogglePin(LED1_PORT, LED1_PIN);
		break;
	default:
		break;
	}
}

bool HM_RadioSend(const uint8_t *data, uint32_t numBytes) {
	return false;
}

bool HM_UsbIsConnected() {
	return hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED;
}

void HM_SetImu1Sampling(bool enable) {
	bImu1Sampling = enable;
}

void HM_SetImu2Sampling(bool enable) {
	bImu2Sampling = enable;
}

void HM_SetHighGSampling(bool enable) {
	bBatteryVoltageSampling = enable;
}

void HM_SetBaro1Sampling(bool enable) {
	bBaro1Sampling = enable;
}

void HM_SetBaro2Sampling(_Bool enable) {
	bBaro2Sampling = enable;
}

void HM_SetGpsSampling(bool enable) {
	bGpsSampling = enable;
}

void HM_SetBatteryVoltageSampling(bool enable) {
	bBatteryVoltageSampling = enable;
}

void HM_SetPyroContinuitySampling(bool enable) {
	bPyroContinuitySampling = enable;
}

void HM_ReadSensorData() {

	// IMU 1 data
	if (bImu1Sampling) {
		LSM9DS1_get_data(&lsm9ds1_1);
		sensorData.imu1_accel_x_raw = lsm9ds1_1.ag.aRawVal.x;
		sensorData.imu1_accel_x_raw = lsm9ds1_1.ag.aRawVal.y;
		sensorData.imu1_accel_x_raw = lsm9ds1_1.ag.aRawVal.z;
		sensorData.imu1_accel_x = lsm9ds1_1.ag.aVal.x;
		sensorData.imu1_accel_x = lsm9ds1_1.ag.aVal.y;
		sensorData.imu1_accel_x = lsm9ds1_1.ag.aVal.z;
		sensorData.imu1_gyro_x_raw = lsm9ds1_1.ag.gRawVal.x;
		sensorData.imu1_gyro_x_raw = lsm9ds1_1.ag.gRawVal.y;
		sensorData.imu1_gyro_x_raw = lsm9ds1_1.ag.gRawVal.z;
		sensorData.imu1_gyro_x = lsm9ds1_1.ag.gVal.x;
		sensorData.imu1_gyro_x = lsm9ds1_1.ag.gVal.y;
		sensorData.imu1_gyro_x = lsm9ds1_1.ag.gVal.z;
		sensorData.imu1_mag_x_raw = lsm9ds1_1.m.mRawVal.x;
		sensorData.imu1_mag_x_raw = lsm9ds1_1.m.mRawVal.y;
		sensorData.imu1_mag_x_raw = lsm9ds1_1.m.mRawVal.z;
		sensorData.imu1_mag_x = lsm9ds1_1.m.mVal.x;
		sensorData.imu1_mag_x = lsm9ds1_1.m.mVal.y;
		sensorData.imu1_mag_x = lsm9ds1_1.m.mVal.z;
	}

	// IMU 2 data
	if (bImu2Sampling) {
		LSM9DS1_get_data(&lsm9ds1_2);
		sensorData.imu1_accel_x_raw = lsm9ds1_2.ag.aRawVal.x;
		sensorData.imu1_accel_x_raw = lsm9ds1_2.ag.aRawVal.y;
		sensorData.imu1_accel_x_raw = lsm9ds1_2.ag.aRawVal.z;
		sensorData.imu1_accel_x = lsm9ds1_2.ag.aVal.x;
		sensorData.imu1_accel_x = lsm9ds1_2.ag.aVal.y;
		sensorData.imu1_accel_x = lsm9ds1_2.ag.aVal.z;
		sensorData.imu1_gyro_x_raw = lsm9ds1_2.ag.gRawVal.x;
		sensorData.imu1_gyro_x_raw = lsm9ds1_2.ag.gRawVal.y;
		sensorData.imu1_gyro_x_raw = lsm9ds1_2.ag.gRawVal.z;
		sensorData.imu1_gyro_x = lsm9ds1_2.ag.gVal.x;
		sensorData.imu1_gyro_x = lsm9ds1_2.ag.gVal.y;
		sensorData.imu1_gyro_x = lsm9ds1_2.ag.gVal.z;
		sensorData.imu1_mag_x_raw = lsm9ds1_2.m.mRawVal.x;
		sensorData.imu1_mag_x_raw = lsm9ds1_2.m.mRawVal.y;
		sensorData.imu1_mag_x_raw = lsm9ds1_2.m.mRawVal.z;
		sensorData.imu1_mag_x = lsm9ds1_2.m.mVal.x;
		sensorData.imu1_mag_x = lsm9ds1_2.m.mVal.y;
		sensorData.imu1_mag_x = lsm9ds1_2.m.mVal.z;
	}

	// High G Accelerometer data
	if (bHighGSampling) {
		H3LIS331DL_get_data(&h3lis_1);
		sensorData.high_g_accel_x_raw = h3lis_1.rawVal.x;
		sensorData.high_g_accel_x = h3lis_1.val.x;
		sensorData.high_g_accel_y_raw = h3lis_1.rawVal.y;
		sensorData.high_g_accel_y = h3lis_1.val.y;
		sensorData.high_g_accel_z_raw = h3lis_1.rawVal.z;
		sensorData.high_g_accel_z = h3lis_1.val.z;
	}

	// Baro 1 data
	if (bBaro1Sampling) {
		MS5607_get_data(&ms5607_1);
		sensorData.baro1_pres = ms5607_1.altData.baro;
		sensorData.baro1_temp = ms5607_1.altData.temp;
		sensorData.baro1_alt = ms5607_1.altData.alt;
	}

	// Baro 2 data
	if (bBaro2Sampling) {
		MS5607_get_data(&ms5607_2);
		sensorData.baro2_pres = ms5607_2.altData.baro;
		sensorData.baro2_temp = ms5607_2.altData.temp;
		sensorData.baro2_alt = ms5607_2.altData.alt;
	}

	// GPS data
	// TODO: Implement GPS data
	if (bGpsSampling) {

	}

	// ADC data
	// TODO: Implement ADC data
	if (bBatteryVoltageSampling) {

	}
	if (bPyroContinuitySampling) {

	}

	// Timestamp data
	// TODO: Implement timer data

	// Filtered data
	// TODO: Get results of filtering from other files and implement
}

SensorData_t* HM_GetSensorData() {
	return &sensorData;
}
