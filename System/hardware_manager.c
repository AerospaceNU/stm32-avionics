#include "hardware_manager.h"
#include "main.h"
#include "LSM9DS1.h"
#include "ms5607.h"
#include "H3LIS331DL.h"
#include "servo.h"
#include "buzzer.h"
#include "adc_device.h"
#include "S25FLx.h"
#include "CC1120.h"
#include "ICM20948.h"
#include "GPS.h"
#include "usb.h"
#include "iwdg.h"

#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "dma.h"

#include "data_transmission.h"

/* IMUs */
static LSM9DS1Ctrl_t lsm9ds1_1;
static LSM9DS1Ctrl_t lsm9ds1_2;
static ICM20948Ctrl_t icm20948;

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
#if (FCB_VERSION <= 0)
static AdcCtrl_t adcBatteryVoltage;
static AdcCtrl_t adcCurrent;
#endif /* FCB_VERSION */
static AdcCtrl_t adcPyro[6];

/* Flash memory */
static S25FLXCtrl_t s25flx1;

/* GPS */
static GPSCtrl_t gps;

/* Radio */
static CC1120Ctrl_t cc1120;
static const uint8_t payloadSize = sizeof(TransmitData_t);

/* Sensor info */
static SensorProperties_t sensorProperties;
static SensorData_t sensorData;
static size_t SENSOR_DATA_SIZE = sizeof(SensorData_t);

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

/* Hardware manager sim mode trackers */
static bool inSim = false;
static CircularBuffer_t *simRxBuffer = NULL;

void HM_HardwareInit() {

	/* LSM9DS1 IMU 1 */
	lsm9ds1_1.ag.LSM9DS1SPI.hspi = IMU1_AG_HSPI;
	lsm9ds1_1.ag.LSM9DS1SPI.port = IMU1_AG_CS_GPIO_Port;
	lsm9ds1_1.ag.LSM9DS1SPI.pin = IMU1_AG_CS_Pin;
	lsm9ds1_1.m.LSM9DS1SPI.hspi = IMU1_M_HSPI;
	lsm9ds1_1.m.LSM9DS1SPI.port = IMU1_M_CS_GPIO_Port;
	lsm9ds1_1.m.LSM9DS1SPI.pin = IMU1_M_CS_Pin;
	lsm9ds1_1.ag.aFs = FS_XL_16;
	lsm9ds1_1.ag.gFs = FS_G_500;
	lsm9ds1_1.m.mFs = FS_M_8;
	LSM9DS1_init(&lsm9ds1_1);
	sensorProperties.imu1_accel_fs = 156.96; // 16 G * 9.81 mpsps/G

#if (FCB_VERSION <= 0)
	/* LSM9DS1 IMU 2 */
	lsm9ds1_2.ag.LSM9DS1SPI.hspi = IMU2_AG_HSPI;
	lsm9ds1_2.ag.LSM9DS1SPI.port = IMU2_AG_CS_GPIO_Port;
	lsm9ds1_2.ag.LSM9DS1SPI.pin = IMU2_AG_CS_Pin;
	lsm9ds1_2.m.LSM9DS1SPI.hspi = IMU2_M_HSPI;
	lsm9ds1_2.m.LSM9DS1SPI.port = IMU2_M_CS_GPIO_Port;
	lsm9ds1_2.m.LSM9DS1SPI.pin = IMU2_M_CS_Pin;
	lsm9ds1_2.ag.aFs = FS_XL_16;
	lsm9ds1_2.ag.gFs = FS_G_500;
	lsm9ds1_2.m.mFs = FS_M_8;
	LSM9DS1_init(&lsm9ds1_2);
	sensorProperties.imu2_accel_fs = 156.96; // 16 G * 9.81 mpsps/G
#else
	/* ICM20948 IMU 2 */
	sensorProperties.imu2_accel_fs = 16 * 9.81; // 16 gees FS

	/* ICM20948 IMU 2 */
	icm20948.spiconfig.hspi = IMU2_HSPI;
	icm20948.spiconfig.port = IMU2_CS_GPIO_Port;
	icm20948.spiconfig.pin = IMU2_CS_Pin;
	ICM_20948_ACCEL_CONFIG_t icm20948AccelConfig = { .ACCEL_FS_SEL = 3,
			.ACCEL_FCHOICE = 1, .ACCEL_DLPFCFG = 0, .reserved_0 = 0 };
	ICM_20948_GYRO_CONFIG_t icm20948GyroConfig = { .GYRO_FS_SEL = 3,
			.GYRO_FCHOICE = 1, .GYRO_DLPFCFG = 0, .reserved_0 = 0 };
	hardwareStatus[IMU2] = ICM20948_init(&icm20948, icm20948AccelConfig,
			icm20948GyroConfig);
#endif /* FCB_VERSION */

	/* MS5607 Barometer 1 */
	ms5607_1.spiconfig.hspi = BARO1_HSPI;
	ms5607_1.spiconfig.port = BARO1_CS_GPIO_Port;
	ms5607_1.spiconfig.pin = BARO1_CS_Pin;
	MS5607_init(&ms5607_1);

	/* MS5607 Barometer 2 */
	ms5607_2.spiconfig.hspi = BARO2_HSPI;
	ms5607_2.spiconfig.port = BARO2_CS_GPIO_Port;
	ms5607_2.spiconfig.pin = BARO2_CS_Pin;
	MS5607_init(&ms5607_2);

	/* H3LIS331DL High G Accelerometer */
	h3lis_1.H3LIS331DLSPI.hspi = HIGH_G_HSPI;
	h3lis_1.H3LIS331DLSPI.port = HIGH_G_CS_GPIO_Port;
	h3lis_1.H3LIS331DLSPI.pin = HIGH_G_CS_Pin;
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
	// Battery voltage - 0 min, 36.3 max (110k/10k*3.3V)
	// Pyros - 0 min, 36.3 max (110k/10k*3.3V)
	static const float voltageDividerMax = 3.3 * (110.0 / 10.0);
#if (FCB_VERSION <= 0)
    adcInit(&adcBatteryVoltage, VBAT_ADC, VBAT_ADC_RANK, 0, voltageDividerMax, true);
    adcInit(&adcCurrent, CURRENT_ADC, CURRENT_ADC_RANK, -12.5, 17.5, true);
#endif /* FCB_VERSION */
	adcInit(&adcPyro[0], PYRO1_ADC, PYRO1_ADC_RANK, 0, voltageDividerMax, true);
	adcInit(&adcPyro[1], PYRO2_ADC, PYRO2_ADC_RANK, 0, voltageDividerMax, true);
	adcInit(&adcPyro[2], PYRO3_ADC, PYRO3_ADC_RANK, 0, voltageDividerMax, true);
	adcInit(&adcPyro[3], PYRO4_ADC, PYRO4_ADC_RANK, 0, voltageDividerMax, true);
	adcInit(&adcPyro[4], PYRO5_ADC, PYRO5_ADC_RANK, 0, voltageDividerMax, true);
	adcInit(&adcPyro[5], PYRO6_ADC, PYRO6_ADC_RANK, 0, voltageDividerMax, true);

	/* Flash */
	S25FLX_init(&s25flx1, FLASH_HSPI, FLASH_CS_GPIO_Port, FLASH_CS_Pin,
			FLASH_SIZE_BYTES);

	/* GPS */
	gps.gps_uart = GPS_HUART;
	gps_init(&gps);

	/* Radio */
	cc1120.radhspi = RADIO_HSPI;
	cc1120.CS_port = RADIO_CS_GPIO_Port;
	cc1120.CS_pin = RADIO_CS_Pin;
	cc1120.RST_port = RADIO_RST_GPIO_Port;
	cc1120.RST_pin = RADIO_RST_Pin;
	cc1120.RDY_port = RADIO_RDY_GPIO_Port;
	cc1120.RDY_pin = RADIO_RDY_Pin;
	cc1120.GP0_port = RADIO_GP0_GPIO_Port;
	cc1120.GP0_pin = RADIO_GP0_Pin;
	cc1120.GP2_port = RADIO_GP2_GPIO_Port;
	cc1120.GP2_pin = RADIO_GP2_Pin;
	cc1120.GP3_port = RADIO_GP3_GPIO_Port;
	cc1120.GP3_pin = RADIO_GP3_Pin;
	cc1120.payloadSize = payloadSize;
	cc1120.initialized = false;

	/* LED 1 */
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

	/* USB */
	usbInit();

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
	switch (servoNum) {
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
	switch (ledNum) {
	case 1:
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, set);
		break;
	default:
		break;
	}
}

void HM_LedToggle(int ledNum) {
	switch (ledNum) {
	case 1:
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		break;
	default:
		break;
	}
}

bool HM_RadioSend(const uint8_t *data, uint16_t numBytes) {
#if (FCB_VERSION <= 0)
	for (uint32_t i = 0; i < numBytes; i += payloadSize) {
		memcpy(cc1120.packetToTX, data, payloadSize);
		cc1120State(&cc1120);
	}
#endif /* FCB_VERSION */
	return false;
}

bool HM_UsbIsConnected() {
	return usbIsConnected();
}

bool HM_UsbTransmit(uint8_t *data, uint16_t numBytes) {
	return usbTransmit(data, numBytes);
}

CircularBuffer_t* HM_UsbGetRxBuffer() {
	return usbGetRxBuffer();
}

bool HM_BluetoothSend(const uint8_t *data, uint16_t numBytes) {
	// TODO: Implement HM_BluetoothSend
	return false;
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

void HM_IWDG_Refresh() {
	HAL_IWDG_Refresh(&hiwdg1);
}

static void HM_SimReadSensorData() {
	size_t buffCount = cbCount(simRxBuffer); // This might change if in middle of reception, so use consistently across function

	// If buffer full, throw everything away since it's probably bad
	if (cbFull(simRxBuffer)) {
		cbFlush(simRxBuffer);
	}

	// Buffer isn't full, continue as normal
	else {
		// First throw away any old data that wasn't processed in time
		if (buffCount > SENSOR_DATA_SIZE) {
			cbDequeue(simRxBuffer, buffCount - SENSOR_DATA_SIZE);
			buffCount = SENSOR_DATA_SIZE;
		}
		// If right amount of bytes are seen, grab them as sensor data
		if (buffCount == SENSOR_DATA_SIZE) {
			cbPeek(simRxBuffer, &sensorData, &SENSOR_DATA_SIZE);
			cbFlush(simRxBuffer);
		}
	}
}

void HM_ReadSensorData() {

	// Sim mode is off, collect data from sensors
	if (!inSim) {

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
			sensorData.gps_timestamp = gps.timestamp;
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
#if (FCB_VERSION <= 0)
			adcStartSingleRead(&adcBatteryVoltage);
			if (adcGetValue(&adcBatteryVoltage, &adcVal, 5))
				sensorData.battery_voltage = (double) adcVal;
			else
				sensorData.battery_voltage = 0;
#else
			// TODO: Battery voltage from external ADC
#endif /* FCB_VERSION */
		}
		if (bPyroContinuitySampling) {
			for (int i = 0;
					i < sizeof(sensorData.pyro_continuity) / sizeof(bool);
					i++) {
				adcStartSingleRead(&adcPyro[i]);
				if (adcGetValue(&adcPyro[i], &adcVal, 5))
					sensorData.pyro_continuity[i] = adcVal
							> PYRO_CONTINUITY_THRESHOLD;
				else
					sensorData.pyro_continuity[i] = false;
			}
		}

		// Timestamp data
		// TODO: Make sensor data timestamp get time from PPS-updated timer
		sensorData.timestamp_s = HM_Millis();
	}

	// Simming
	else {
		HM_SimReadSensorData();
	}
}

SensorData_t* HM_GetSensorData() {
	return &sensorData;
}

SensorProperties_t* HM_GetSensorProperties() {
	return &sensorProperties;
}

void HM_EnableSimMode(CircularBuffer_t *rxBuffer) {
	inSim = true;
	simRxBuffer = rxBuffer;
}

void HM_DisableSimMode(CircularBuffer_t *rxBuffer) {
	inSim = false;
}

bool HM_InSimMode() {
	return inSim;
}
