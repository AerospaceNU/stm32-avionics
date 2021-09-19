
#include "hardware_manager.h"
#include "main.h"
#include "LSM9DS1.h"
#include "ms5607.h"
#include "H3LIS331DL.h"
#include "servo.h"
#include "buzzer.h"
#include "adc_device.h"
#include "S25FLx.h"
#include "usbd_cdc_if.h"
#include "cc1120.h"
#include "GPS.h"

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "dma.h"

#include "data_transmission.h"
#include <stdbool.h>

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

/* ADCs */
static AdcCtrl_t adcBatteryVoltage;
static AdcCtrl_t adcCurrent;
static AdcCtrl_t adcPyro[6];

/* Flash memory */
static S25FLXCtrl_t s25flx1;

/* GPS */
static GPSCtrl_t gps;

/* Radio */
static CC1120Ctrl_t cc1120;
static const uint8_t payloadSize = sizeof(TransmitData_t);

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

/* Array of hardware status states */
bool hardwareStatus[NUM_HARDWARE];

void HM_HardwareInit() {

	/* LSM9DS1 IMU 1 */
	lsm9ds1_1.ag.LSM9DS1SPI.hspi = IMU1_AG_HSPI;
	lsm9ds1_1.ag.LSM9DS1SPI.port = IMU1_AG_CS_PORT;
	lsm9ds1_1.ag.LSM9DS1SPI.pin = IMU1_AG_CS_PIN;
	lsm9ds1_1.m.LSM9DS1SPI.hspi = IMU1_M_HSPI;
	lsm9ds1_1.m.LSM9DS1SPI.port = IMU1_M_CS_PORT;
	lsm9ds1_1.m.LSM9DS1SPI.pin = IMU1_M_CS_PIN;
	lsm9ds1_1.ag.aFs = FS_XL_16;
	lsm9ds1_1.ag.gFs = FS_G_500;
	lsm9ds1_1.m.mFs = FS_M_8;

	LSM9DS1_init(&lsm9ds1_1);

	/* LSM9DS1 IMU 2 */
	lsm9ds1_2.ag.LSM9DS1SPI.hspi = IMU2_AG_HSPI;
	lsm9ds1_2.ag.LSM9DS1SPI.port = IMU2_AG_CS_PORT;
	lsm9ds1_2.ag.LSM9DS1SPI.pin = IMU2_AG_CS_PIN;
	lsm9ds1_2.m.LSM9DS1SPI.hspi = IMU2_M_HSPI;
	lsm9ds1_2.m.LSM9DS1SPI.port = IMU2_M_CS_PORT;
	lsm9ds1_2.m.LSM9DS1SPI.pin = IMU2_M_CS_PIN;
	lsm9ds1_2.ag.aFs = FS_XL_16;
	lsm9ds1_2.ag.gFs = FS_G_500;
	lsm9ds1_2.m.mFs = FS_M_8;
	LSM9DS1_init(&lsm9ds1_2);

	/* MS5607 Barometer 1 */
	ms5607_1.spiconfig.hspi = BARO1_HSPI;
	ms5607_1.spiconfig.port = BARO1_CS_PORT;
	ms5607_1.spiconfig.pin = BARO1_CS_PIN;
	MS5607_init(&ms5607_1);

	/* MS5607 Barometer 2 */
	ms5607_2.spiconfig.hspi = BARO2_HSPI;
	ms5607_2.spiconfig.port = BARO2_CS_PORT;
	ms5607_2.spiconfig.pin = BARO2_CS_PIN;
	MS5607_init(&ms5607_2);

	/* H3LIS331DL High G Accelerometer */
	h3lis_1.H3LIS331DLSPI.hspi = HIGH_G_HSPI;
	h3lis_1.H3LIS331DLSPI.port = HIGH_G_CS_PORT;
	h3lis_1.H3LIS331DLSPI.pin = HIGH_G_CS_PIN;
	H3LIS331DL_init(&h3lis_1);

	/* Servos 1-4
	 *
	 * TODO: minPulseMS and maxPulseMS will need a method of tuning based on servo (currently 0.75-2.25)
	 */
	servoInit(&servo1, SERVO1_HTIM, SERVO1_CHANNEL, 20, 0.75, 2.25, -90, 90);
	servoInit(&servo2, SERVO2_HTIM, SERVO2_CHANNEL, 20, 0.75, 2.25, -90, 90);
	servoInit(&servo3, SERVO3_HTIM, SERVO3_CHANNEL, 20, 0.75, 2.25, -90, 90);
	servoInit(&servo4, SERVO4_HTIM, SERVO4_CHANNEL, 20, 0.75, 2.25, -90, 90);

	/* Buzzer */
	buzzerInit(&buzzer, BUZZER_HTIM, BUZZER_CHANNEL, 500);

	/* ADCs */
    // Battery voltage - 0 min, 15.52 max (127k/27k*3.3V)
    // Pyros - 0 min, 15.52 max (127k/27k*3.3V)
    static const float voltageDividerMax = 3.3 * (110.0 / 10.0); // 3.3 * 110.0 / 10.0 for FCB 1
    adcInit(&adcBatteryVoltage, &hadc3, 1, 0, voltageDividerMax, true);
    adcInit(&adcCurrent, &hadc1, 1, -12.53, 19.50, true); // -12.5 to 17.5 for FCB 1
    adcInit(&adcPyro[0], &hadc2, 1, 0, voltageDividerMax, true);
    adcInit(&adcPyro[1], &hadc2, 4, 0, voltageDividerMax, true);
    adcInit(&adcPyro[2], &hadc2, 3, 0, voltageDividerMax, true);
    adcInit(&adcPyro[3], &hadc2, 2, 0, voltageDividerMax, true);
    adcInit(&adcPyro[4], &hadc1, 3, 0, voltageDividerMax, true);
    adcInit(&adcPyro[5], &hadc1, 2, 0, voltageDividerMax, true);

	/* Flash */
	S25FLX_init(&s25flx1, FLASH_HSPI, FLASH_CS_PORT, FLASH_CS_PIN, FLASH_SIZE_BYTES);

	/* GPS */
	gps.gps_uart = GPS_HUART;
	gps_init(&gps);

	/* Radio */
	cc1120.radhspi = RADIO_HSPI;
	cc1120.CS_port = RADIO_CS_PORT;
	cc1120.CS_pin = RADIO_CS_PIN;
	cc1120.RST_port = RADIO_RST_PORT;
	cc1120.RST_pin = RADIO_RST_PIN;
	cc1120.RDY_port = RADIO_RDY_PORT;
	cc1120.RDY_pin = RADIO_RDY_PIN;
	cc1120.GP0_port = RADIO_GP0_PORT;
	cc1120.GP0_pin = RADIO_GP0_PIN;
	cc1120.GP2_port = RADIO_GP2_PORT;
	cc1120.GP2_pin = RADIO_GP2_PIN;
	cc1120.GP3_port = RADIO_GP3_PORT;
	cc1120.GP3_pin = RADIO_GP3_PIN;
	cc1120.payloadSize = payloadSize;
	cc1120.initialized = false;

	/* LED 1 */
	HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);

	/* Checking if inits are successful (inits that don't return a boolean are assumed true) */
	hardwareStatus[CC1120] = cc1120_init(&cc1120);
	hardwareStatus[IMU1] = true;
	hardwareStatus[IMU2] = true;
	hardwareStatus[BAROMETER1] = true;
	hardwareStatus[BAROMETER2] = true;
	hardwareStatus[HIGH_G_ACCELEROMETER] = true;
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
	//for (uint32_t i = 0; i < numBytes; i += payloadSize) {
	//	memcpy(cc1120.packetToTX, data, payloadSize);
	//	cc1120State(&cc1120);
	//}
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
		sensorData.imu1_accel_y_raw = lsm9ds1_1.ag.aRawVal.y;
		sensorData.imu1_accel_z_raw = lsm9ds1_1.ag.aRawVal.z;
		sensorData.imu1_accel_x = lsm9ds1_1.ag.aVal.x;
		sensorData.imu1_accel_y = lsm9ds1_1.ag.aVal.y;
		sensorData.imu1_accel_z = lsm9ds1_1.ag.aVal.z;
		sensorData.imu1_gyro_x_raw = lsm9ds1_1.ag.gRawVal.x;
		sensorData.imu1_gyro_y_raw = lsm9ds1_1.ag.gRawVal.y;
		sensorData.imu1_gyro_z_raw = lsm9ds1_1.ag.gRawVal.z;
		sensorData.imu1_gyro_x = lsm9ds1_1.ag.gVal.x;
		sensorData.imu1_gyro_y = lsm9ds1_1.ag.gVal.y;
		sensorData.imu1_gyro_z = lsm9ds1_1.ag.gVal.z;
		sensorData.imu1_mag_x_raw = lsm9ds1_1.m.mRawVal.x;
		sensorData.imu1_mag_y_raw = lsm9ds1_1.m.mRawVal.y;
		sensorData.imu1_mag_z_raw = lsm9ds1_1.m.mRawVal.z;
		sensorData.imu1_mag_x = lsm9ds1_1.m.mVal.x;
		sensorData.imu1_mag_y = lsm9ds1_1.m.mVal.y;
		sensorData.imu1_mag_z = lsm9ds1_1.m.mVal.z;
	}

	// IMU 2 data
	if (bImu2Sampling) {
		LSM9DS1_get_data(&lsm9ds1_2);
		sensorData.imu2_accel_x_raw = lsm9ds1_2.ag.aRawVal.x;
		sensorData.imu2_accel_y_raw = lsm9ds1_2.ag.aRawVal.y;
		sensorData.imu2_accel_z_raw = lsm9ds1_2.ag.aRawVal.z;
		sensorData.imu2_accel_x = lsm9ds1_2.ag.aVal.x;
		sensorData.imu2_accel_y = lsm9ds1_2.ag.aVal.y;
		sensorData.imu2_accel_z = lsm9ds1_2.ag.aVal.z;
		sensorData.imu2_gyro_x_raw = lsm9ds1_2.ag.gRawVal.x;
		sensorData.imu2_gyro_y_raw = lsm9ds1_2.ag.gRawVal.y;
		sensorData.imu2_gyro_z_raw = lsm9ds1_2.ag.gRawVal.z;
		sensorData.imu2_gyro_x = lsm9ds1_2.ag.gVal.x;
		sensorData.imu2_gyro_y = lsm9ds1_2.ag.gVal.y;
		sensorData.imu2_gyro_z = lsm9ds1_2.ag.gVal.z;
		sensorData.imu2_mag_x_raw = lsm9ds1_2.m.mRawVal.x;
		sensorData.imu2_mag_y_raw = lsm9ds1_2.m.mRawVal.y;
		sensorData.imu2_mag_z_raw = lsm9ds1_2.m.mRawVal.z;
		sensorData.imu2_mag_x = lsm9ds1_2.m.mVal.x;
		sensorData.imu2_mag_y = lsm9ds1_2.m.mVal.y;
		sensorData.imu2_mag_z = lsm9ds1_2.m.mVal.z;
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
	}

	// Baro 2 data
	if (bBaro2Sampling) {
		MS5607_get_data(&ms5607_2);
		sensorData.baro2_pres = ms5607_2.altData.baro;
		sensorData.baro2_temp = ms5607_2.altData.temp;
	}

	// GPS data
	// TODO: Poll GPS status to determine if data is good
	if (bGpsSampling) {
		gps_new_data(&gps);
		sensorData.gps_lat = gps.latitude;
		sensorData.gps_long = gps.longitude;
		sensorData.gps_alt = gps.altitude;
		sensorData.gps_speed = gps.speed;
		sensorData.gps_course = gps.course;
		sensorData.gps_latitude_deviation = gps.latitude_deviation;
		sensorData.gps_longitude_deviation = gps.longitude_deviation;
		sensorData.gps_altitude_deviation = gps.altitude_deviation;
		sensorData.gps_speed_kph = gps.speed_kph;
		sensorData.gps_speed_knots = gps.speed_knots;
		sensorData.gps_seconds = gps.seconds;
		sensorData.gps_minutes = gps.minutes;
		sensorData.gps_hours = gps.hours;
		sensorData.gps_day = gps.day;
		sensorData.gps_month = gps.month;
		sensorData.gps_year = gps.year;
		sensorData.gps_num_sats = gps.num_sats;
		sensorData.gps_status = gps.status;
	}

	// ADC data
	float adcVal = 0;
	if (bBatteryVoltageSampling) {
		adcStartSingleRead(&adcBatteryVoltage);
		if (adcGetValue(&adcBatteryVoltage, &adcVal, 5))
			sensorData.battery_voltage = (double) adcVal;
		else
			sensorData.battery_voltage = 0;
	}
	if (bPyroContinuitySampling) {
		for (int i = 0; i < sizeof(sensorData.pyro_continuity) / sizeof(bool); i++) {
			adcStartSingleRead(&adcPyro[i]);
			if (adcGetValue(&adcPyro[i], &adcVal, 5))
				sensorData.pyro_continuity[i] = adcVal > PYRO_CONTINUITY_THRESHOLD;
			else
				sensorData.pyro_continuity[i] = false;
		}
	}



	// Timestamp data
	// TODO: Make sensor data timestamp get time from PPS-updated timer
	sensorData.timestamp_s = HM_Millis();
}

SensorData_t* HM_GetSensorData() {
	return &sensorData;
}
