#ifndef COMMON_SYSTEM_HARDWARE_MANAGER_H_
#define COMMON_SYSTEM_HARDWARE_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"
#include "circular_buffer.h"
#include "data_structures.h"

/* Useful defines for files that need to know about hardware it works with */
#define PYRO_CONTINUITY_THRESHOLD 3

#define KHZ_TO_HZ 1000.0
#define MHZ_TO_HZ 1000000.0
#define RADIO_CHANNEL_BANDWIDTH 150 * KHZ_TO_HZ
#define RAD433_CHAN0 433 * MHZ_TO_HZ
#define RAD915_CHAN0 914 * MHZ_TO_HZ

typedef struct __attribute__((__packed__)) {
  uint32_t timestampMs;
#if HAS_DEV(IMU)
  ImuData_s imuData[NUM_IMU];
#endif  // HAS_DEV(IMU)
#if HAS_DEV(MAG)
  MagData_s magData[NUM_MAG];
#endif  // HAS_DEV(MAG)
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
#if HAS_DEV(MAG)
extern bool hardwareStatusMag[NUM_MAG];
#endif  // HAS_DEV(MAG)
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
#if HAS_DEV(ADC)
extern bool hardwareStatusAdcs[NUM_ADC];
#endif  // // HAS_DEV(USB)

void hm_hardwareInit();

/* Microcontroller timer functions */
uint32_t hm_millis();

/* Flash functions */
bool hm_flashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                       uint8_t* pData);
bool hm_flashWriteStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                        uint8_t* data);
bool hm_flashEraseSectorStart(int flashId, uint32_t sectorNum);
bool hm_flashEraseChipStart(int flashId);
bool hm_flashIsReadComplete(int flashId);
bool hm_flashIsWriteComplete(int flashId);
bool hm_flashIsEraseComplete(int flashId);

/* Buzzer functions */
void hm_buzzerSetFrequency(int buzzerId, float fHz);
void hm_buzzerStart(int buzzerId);
void hm_buzzerStop(int buzzerId);

/* LED functions */
void hm_ledSet(int ledId, bool on);
void hm_ledToggle(int ledId);

/* Radio functions */
bool hm_radioSend(int radioNum, uint8_t* data, uint16_t numBytes);
/**
 * Register a  callback to be called whenever a new packet is recieved by a
 * particular radio
 * @param radioNum the radio index to add a callback to
 * @param rxBuffer CircularBuffer of RadioRecievedOTAPacket's that will be
 * filled as packets come in
 */
void hm_radioRegisterConsumer(int radioNum, CircularBuffer_s* rxBuffer);
void hm_radioUpdate();
void hm_radioSetChannel(int radioNum, int channel);

/* USB functions */
bool hm_usbIsConnected(int usbId);
bool hm_usbTransmit(int usbId, uint8_t* data, uint16_t numBytes);
CircularBuffer_s* hm_usbGetRxBuffer(int usbId);

/* Bluetooth functions */
bool hm_bleClientConnected(int bleClientId);
bool hm_bleClientSend(int bleClientId, const uint8_t* data, uint16_t numBytes);
CircularBuffer_s* hm_bleClientGetRxBuffer(int bleClientId);

void hm_bleTick();

LineCutterData_s* hm_getLineCutterData(int lineCutterId);
LineCutterFlightVars_s* hm_getLineCutterFlightVariables(int lineCutterId);

bool hm_lineCutterSendString(int lineCutterNumber, char* string);
bool hm_lineCuttersSendCut(int device);

/* Outputs */
void hm_servoSetAngle(int servoId, float degrees);
void hm_pyroFire(int pyroId, uint32_t duration);
void hm_pyroSet(int pyroId, bool enabled);
void hm_pyroSetPwm(int pyroId, uint32_t frequency, uint32_t pulseWidth,
                   uint32_t duration);
void hm_pyroUpdate(void* pUserData);
void hm_dcMotorSetPercent(int dcMotorId, double percent);

/* Watchdogs */
void hm_watchdogRefresh();

/**
 * @brief Updates sensor data from enabled sensors
 */
void hm_readSensorData();

/**
 * @brief Returns a pointer to the current sensor data (statically allocated)
 * @return Pointer to statically-allocated struct where sensor data is stored
 */
SensorData_s* hm_getSensorData();

/**
 * @brief Returns a pointer to the current sensor properties
 * @return Pointer to statically-allocated struct where sensor properties are
 * stored
 */
SensorProperties_s* hm_getSensorProperties();

/**
 * @brief Turn on or off sim mode in the hardware manager
 * @param[in] rxBuffer: Buffer to read from in during sim
 */
void hm_enableSimMode(CircularBuffer_s* rxBuffer);

/**
 * @brief Turn off sim mode in the hardware manager
 */
void hm_disableSimMode();

/**
 * @brief Return whether hardware manager is currently in sim mode
 * @return True if in sim mode, false if not
 */
bool hm_inSimMode();

#ifdef __cplusplus
}
#endif

#endif  // COMMON_SYSTEM_HARDWARE_MANAGER_H_
