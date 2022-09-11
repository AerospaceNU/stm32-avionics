#ifndef SYSTEM_HARDWARE_MANAGER_H_
#define SYSTEM_HARDWARE_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"
#include "circular_buffer.h"
#include "data_structures.h"

/* Useful defines for files that need to know about hardware it works with */
#define FLASH_MAX_SECTOR_BYTES 0x40000
#define FLASH_MIN_PAGE_SIZE_BYTES 0x200
#define PYRO_CONTINUITY_THRESHOLD 3

#define KHZ_TO_HZ 1000
#define MHZ_TO_HZ 1000000
#define RADIO_CHANNEL_BANDWIDTH 150 * KHZ_TO_HZ
#define RAD433_CHAN0 433 * MHZ_TO_HZ
#define RAD915_CHAN0 914 * MHZ_TO_HZ

typedef struct __attribute__((__packed__)) {
  uint32_t timestampS;
  uint32_t timestampUs;
#if HAS_DEV(IMU)
  ImuData_s imuData[NUM_IMU];
#endif  // HAS_DEV(IMU)
#if HAS_DEV(ACCEL)
  AccelData_s accelData[NUM_ACCEL];
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
  BarometerData_s barometerData[NUM_BAROMETER];
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(GPS)
  GpsData_s gpsData[NUM_GPS];
#endif  // HAS_DEV(GPS)
#if HAS_DEV(VBAT)
  double vbatData[NUM_VBAT];
#endif  // HAS_DEV(VBAT)
#if HAS_DEV(PYRO_CONT)
  bool pyroContData[NUM_PYRO_CONT];
#endif  // HAS_DEV(PYRO_CONT)
} SensorData_s;

// Sensor fullscales, in m/s/s
typedef struct {
#if HAS_DEV(IMU)
  double imuAccelFs[NUM_IMU];
#endif  // HAS_DEV(IMU)
#if HAS_DEV(ACCEL)
  double accelFs[NUM_ACCEL];
#endif  // HAS_DEV(ACCEL)
} SensorProperties_s;

#if HAS_DEV(ACCEL)
extern bool hardwareStatusAccel[NUM_ACCEL];
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
extern bool hardwareStatusBarometer[NUM_BAROMETER];
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(BLE_CHIP)
extern bool hardwareStatusBleChip[NUM_BLE_CHIP];
#endif  // HAS_DEV(BLE)
#if HAS_DEV(BLE_CLIENT)
extern bool hardwareStatusBleClient[NUM_BLE_CLIENT];
#endif  // HAS_DEV(BLE_CLIENT)
#if HAS_DEV(BUZZER)
extern bool hardwareStatusBuzzer[NUM_BUZZER];
#endif  // HAS_DEV(BUZZER)
#if HAS_DEV(DC_MOTOR)
extern bool hardwareStatusDcMotor[NUM_DC_MOTOR];
#endif  // HAS_DEV(DC_MOTOR)
#if HAS_DEV(FLASH)
extern bool hardwareStatusFlash[NUM_FLASH];
#endif  // HAS_DEV(FLASH)
#if HAS_DEV(GPS)
extern bool hardwareStatusGps[NUM_GPS];
#endif  // HAS_DEV(GPS)
#if HAS_DEV(IMU)
extern bool hardwareStatusImu[NUM_IMU];
#endif  // HAS_DEV(IMU)
#if HAS_DEV(LED)
extern bool hardwareStatusLed[NUM_LED];
#endif  // HAS_DEV(LED)
#if HAS_DEV(LINE_CUTTER)
extern bool hardwareStatusLineCutter[NUM_LINE_CUTTER];
#endif  // HAS_DEV(LINE_CUTTER)
#if HAS_DEV(PYRO)
extern bool hardwareStatusPyro[NUM_PYRO];
#endif  // HAS_DEV(PYRO)
#if HAS_DEV(RADIO)
extern bool hardwareStatusRadio[NUM_RADIO];
#endif  // HAS_DEV(RADIO)
#if HAS_DEV(SERVO)
extern bool hardwareStatusServo[NUM_SERVO];
#endif  // HAS_DEV(SERVO)
#if HAS_DEV(USB)
extern bool hardwareStatusUsb[NUM_USB];
#endif  // HAS_DEV(USB)
#if HAS_DEV(VBAT)
extern bool hardwareStatusVbat[NUM_VBAT];
#endif  // // HAS_DEV(USB)

void HM_HardwareInit();

/* Microcontroller timer functions */
uint32_t HM_Millis();

/* Flash functions */
bool HM_FlashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                       uint8_t* pData);
bool HM_FlashWriteStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                        uint8_t* data);
bool HM_FlashEraseSectorStart(int flashId, uint32_t sectorNum);
bool HM_FlashEraseChipStart(int flashId);
bool HM_FlashIsReadComplete(int flashId);
bool HM_FlashIsWriteComplete(int flashId);
bool HM_FlashIsEraseComplete(int flashId);

/* Buzzer functions */
void HM_BuzzerSetFrequency(int buzzerId, float fHz);
void HM_BuzzerStart(int buzzerId);
void HM_BuzzerStop(int buzzerId);

/* LED functions */
void HM_LedSet(int ledId, bool on);
void HM_LedToggle(int ledId);

/* Radio functions */
bool HM_RadioSend(int radioNum, uint8_t* data, uint16_t numBytes);
void HM_RadioRegisterConsumer(int radioNum, CircularBuffer_s* rxBuffer);
void HM_RadioUpdate();
void HM_RadioSetChannel(int radioNum, int channel);

/* USB functions */
bool HM_UsbIsConnected(int usbId);
bool HM_UsbTransmit(int usbId, uint8_t* data, uint16_t numBytes);
CircularBuffer_s* HM_UsbGetRxBuffer(int usbId);

/* Bluetooth functions */
bool HM_BleClientConnected(int bleClientId);
bool HM_BleClientSend(int bleClientId, const uint8_t* data, uint16_t numBytes);
CircularBuffer_s* HM_BleClientGetRxBuffer(int bleClientId);

void HM_BleTick();

LineCutterData_s* HM_GetLineCutterData(int lineCutterId);
LineCutterFlightVars_s* HM_GetLineCutterFlightVariables(int lineCutterId);

bool HM_LineCutterSendString(int lineCutterNumber, char* string);
bool HM_LineCuttersSendCut(int chan);

/* Outputs */
void HM_ServoSetAngle(int servoId, float degrees);
void HM_PyroFire(int pyroId, uint32_t duration);
void HM_PyroUpdate();
void HM_DCMotorSetPercent(int dcMotorId, double percent);

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
void HM_WatchdogRefresh();

/**
 * @brief Updates sensor data from enabled sensors
 */
void HM_ReadSensorData();

/**
 * @brief Returns a pointer to the current sensor data (statically allocated)
 * @return Pointer to statically-allocated struct where sensor data is stored
 */
SensorData_s* HM_GetSensorData();

/**
 * @brief Returns a pointer to the current sensor properties
 * @return Pointer to statically-allocated struct where sensor properties are
 * stored
 */
SensorProperties_s* HM_GetSensorProperties();

/**
 * @brief Turn on or off sim mode in the hardware manager
 * @param[in] rxBuffer: Buffer to read from in during sim
 */
void HM_EnableSimMode(CircularBuffer_s* rxBuffer);

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
