#include "hardware_manager.h"

#include "board_config.h"
#include "main.h"
#include "sensor_types.h"

#ifdef HAL_ADC_MODULE_ENABLED
#include "adc.h"
#endif

#ifdef HAL_TIM_MODULE_ENABLED
#include "tim.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
#include "usart.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
#include "spi.h"
#endif

#ifdef HAS_LSM9DS1
#include "LSM9DS1.h"
#endif

#if (BARO_1 == BARO_MS5607_SPI || BARO_2 == BARO_MS5607_SPI)
#include "ms5607.h"
#endif

#ifdef HAS_H3LIS331DL
#include "H3LIS331DL.h"
#endif

#ifdef HAS_SERVO
#include "servo.h"
#endif

#ifdef HAS_BUZZER
#include "buzzer.h"
#endif

#ifdef HAS_ADC_DEVICE
#include "adc_device.h"
#endif

#ifdef HAS_S25FLX
#include "S25FLx.h"
#endif

#if defined(HAS_CC1120) || defined(HAS_CC1200)
#include "ti_radio.h"
#endif
#ifdef HAS_CC1200
#include <radioconfig/smartrf_CC1200_cfg_434_38_4kbps.h>
#include <radioconfig/smartrf_CC1200_cfg_915_38_4kbps.h>
#endif
#ifdef HAS_CC1120
#include <radioconfig/smartrf_CC1120_cfg_434_38_4kbps.h>
#endif

#ifdef HAS_GPS
#include "GPS.h"
#endif

#ifdef HAS_USB
#include "usb.h"
#endif

#ifdef HAS_PYRO
#include "pyro.h"
#endif

#include "iwdg.h"
#include "radio_manager.h"

/* IMUs */
#if (IMU_1 == IMU_LSM9DS1)
static LSM9DS1Ctrl_t lsm9ds1_1;
#endif

#if (IMU_2 == IMU_LSM9DS1)
static LSM9DS1Ctrl_t lsm9ds1_2;
#endif

/* Barometers */
#if (BARO_1 == BARO_MS5607_SPI)
static MS5607Ctrl_t ms5607_1;
#endif
#if (BARO_2 == BARO_MS5607_SPI)
static MS5607Ctrl_t ms5607_2;
#endif

/* High G Accelerometers */
#ifdef HAS_H3LIS331DL
static H3LIS331DLCtrl_t h3lis_1;
#endif

/* Servos */
#ifdef HAS_SERVO
static ServoCtrl_t servo1;
static ServoCtrl_t servo2;
static ServoCtrl_t servo3;
static ServoCtrl_t servo4;
#endif

/* Buzzers */
#ifdef HAS_BUZZER
static BuzzerCtrl_t buzzer;
#endif

/* ADCs */
#ifdef HAS_ADC_DEVICE
#if (FCB_VERSION <= 0)
static AdcCtrl_t adcBatteryVoltage;
static AdcCtrl_t adcCurrent;
#endif /* FCB_VERSION */
static AdcCtrl_t adcPyro[6];
#endif

/* Flash memory */
#ifdef HAS_S25FLX
static S25FLXCtrl_t s25flx1;
#endif

/* GPS */
#ifdef HAS_GPS
static GPSCtrl_t gps;
#endif

#ifdef HAS_PYRO
/* Pyro */
PyroCtrl_t pyroCtrl[MAX_PYRO];
#endif

/* Radio */
static const uint8_t payloadSize = RADIO_PACKET_SIZE;
#ifdef HAS_RADIO_433
static TiRadioCtrl_t radio433;
#endif
#ifdef HAS_RADIO_915
static TiRadioCtrl_t radio915;
#endif

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

bool *HM_GetHardwareStatus() { return hardwareStatus; }

void HM_HardwareInit() {
#if (IMU_1 == IMU_LSM9DS1)
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
  sensorProperties.imu1_accel_fs = 156.96;  // 16 G * 9.81 m/s/s/G
#else
  sensorProperties.imu2_accel_fs = 0;
#endif

#if (IMU_2 == IMU_LSM9DS1)
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
  sensorProperties.imu2_accel_fs = 156.96;  // 16 G * 9.81 mpsps/G
#else
  sensorProperties.imu2_accel_fs = 0;
#endif

#ifdef BARO_1
#if (BARO_1 == BARO_MS5607_SPI)
  /* MS5607 Barometer 1 */
  ms5607_1.spiconfig.hspi = BARO1_HSPI;
  ms5607_1.spiconfig.port = BARO1_CS_GPIO_Port;
  ms5607_1.spiconfig.pin = BARO1_CS_Pin;
  MS5607_init(&ms5607_1);
#endif
#endif

#ifdef BARO_2
#if (BARO_2 == BARO_MS5607_SPI)
  /* MS5607 Barometer 2 */
  ms5607_2.spiconfig.hspi = BARO2_HSPI;
  ms5607_2.spiconfig.port = BARO2_CS_GPIO_Port;
  ms5607_2.spiconfig.pin = BARO2_CS_Pin;
  MS5607_init(&ms5607_2);
#endif
#endif

#ifdef HAS_H3LIS331DL
  /* H3LIS331DL High G Accelerometer */
  h3lis_1.H3LIS331DLSPI.hspi = HIGH_G_HSPI;
  h3lis_1.H3LIS331DLSPI.port = HIGH_G_CS_GPIO_Port;
  h3lis_1.H3LIS331DLSPI.pin = HIGH_G_CS_Pin;
  H3LIS331DL_init(&h3lis_1);
#endif

#ifdef HAS_SERVO
  /* Servos 1-4
   *
   * TODO: minPulseMS and maxPulseMS will need a method of tuning based on servo
   * (currently 0.75-2.25)
   */
  servoInit(&servo1, SERVO1_HTIM, SERVO1_CHANNEL, 20, 0.75, 2.25, -90, 90);
  servoInit(&servo2, SERVO2_HTIM, SERVO2_CHANNEL, 20, 0.75, 2.25, -90, 90);
  servoInit(&servo3, SERVO3_HTIM, SERVO3_CHANNEL, 20, 0.75, 2.25, -90, 90);
  servoInit(&servo4, SERVO4_HTIM, SERVO4_CHANNEL, 20, 0.75, 2.25, -90, 90);
#endif

#ifdef HAS_BUZZER
  /* Buzzer */
  buzzerInit(&buzzer, BUZZER_HTIM, BUZZER_CHANNEL, 500);
#endif

#ifdef HAS_ADC_DEVICE
  /* ADCs */
  // Battery voltage - 0 min, seems like 10v in = 2.4v on the voltage divider?
  // Pyros - 0 min, (127k/27k*3.3V) max

  // I had to multiply by 10/3 on my V0 to get these to make sense. TODO Why???
  static const float voltageDividerMax = 3.3 * (127.0 / 27.0) * 10 / 3;
#if (FCB_VERSION <= 0)
  static const float vbattMax = 67;  // 4.72 Volts/volt, 67v fullscale??
  // Also the scale seems non linear: at 10vin this is right, but at 13vin it
  // reads 0.25 above the true number. Apparently this is coz our ADCs aren't
  // buffered
  // TODO why does 67 make things work?
  adcInit(&adcBatteryVoltage, VBAT_ADC, VBAT_ADC_RANK, 0, vbattMax, true);
  adcInit(&adcCurrent, CURRENT_ADC, CURRENT_ADC_RANK, -12.5, 17.5, true);
#endif /* FCB_VERSION */
  adcInit(&adcPyro[0], PYRO1_ADC, PYRO1_ADC_RANK, 0, voltageDividerMax, true);
  adcInit(&adcPyro[1], PYRO2_ADC, PYRO2_ADC_RANK, 0, voltageDividerMax, true);
  adcInit(&adcPyro[2], PYRO3_ADC, PYRO3_ADC_RANK, 0, voltageDividerMax, true);
  adcInit(&adcPyro[3], PYRO4_ADC, PYRO4_ADC_RANK, 0, voltageDividerMax, true);
  adcInit(&adcPyro[4], PYRO5_ADC, PYRO5_ADC_RANK, 0, voltageDividerMax, true);
  adcInit(&adcPyro[5], PYRO6_ADC, PYRO6_ADC_RANK, 0, voltageDividerMax, true);
#endif

#ifdef HAS_S25FLX
  /* Flash */
  S25FLX_init(&s25flx1, FLASH_HSPI, FLASH_CS_GPIO_Port, FLASH_CS_Pin,
              FLASH_SIZE_BYTES);
#endif

#ifdef HAS_GPS
  /* GPS */
  gps.gps_uart = GPS_HUART;
  gps.type = GPS_TYPE_UBLOX;
  gps_init(&gps);
#endif

#ifdef HAS_RADIO_433
  /* Radio */
  radio433.radhspi = RAD433_HSPI;
  radio433.CS_port = RAD433_CS_GPIO_Port;
  radio433.CS_pin = RAD433_CS_Pin;
  radio433.RST_port = RAD433_RST_GPIO_Port;
  radio433.RST_pin = RAD433_RST_Pin;
  radio433.MISO_port = RAD433_MISO_GPIO_Port;
  radio433.MISO_pin = RAD433_MISO_Pin;
  radio433.GP0_port = RAD433_GP0_GPIO_Port;
  radio433.GP0_pin = RAD433_GP0_Pin;
  radio433.GP2_port = RAD433_GP2_GPIO_Port;
  radio433.GP2_pin = RAD433_GP2_Pin;
  radio433.GP3_port = RAD433_GP3_GPIO_Port;
  radio433.GP3_pin = RAD433_GP3_Pin;
  radio433.payloadSize = payloadSize;
  radio433.id = RADIO_433;
  radio433.packetCfg = TIRADIO_PKTLEN_FIXED;
  radio433.initialized = false;

  // All 433 chips do NOT have a 1190 -- how can they?
  radio433.has_cc1190 = false;

#ifdef HAS_CC1200
  radio433.settingsPtr = cc1200_433_1_2kbps_cfg;
  radio433.settingsSize = cc1200_433_1_2kbps_size;
#endif  // HAS_CC1200

#ifdef HAS_CC1120
  radio433.settingsPtr = cc1120_433_1_2kbps_cfg;
  radio433.settingsSize = sizeof(cc1120_433_1_2kbps_cfg);
#endif  // HAS_CC1120

  // Enable our radio and configure pins
  hardwareStatus[RADIO_433] = tiRadio_Init(&radio433);

#endif  // HAS_RADIO_433

#ifdef HAS_RADIO_915
  /* Radio */
  radio915.radhspi = RAD915_HSPI;
  radio915.CS_port = RAD915_CS_GPIO_Port;
  radio915.CS_pin = RAD915_CS_Pin;
  radio915.RST_port = RAD915_RST_GPIO_Port;
  radio915.RST_pin = RAD915_RST_Pin;
  radio915.MISO_port = RAD915_MISO_GPIO_Port;
  radio915.MISO_pin = RAD915_MISO_Pin;
  radio915.GP0_port = RAD915_GP0_GPIO_Port;
  radio915.GP0_pin = RAD915_GP0_Pin;
  radio915.GP2_port = RAD915_GP2_GPIO_Port;
  radio915.GP2_pin = RAD915_GP2_Pin;
  radio915.GP3_port = RAD915_GP3_GPIO_Port;
  radio915.GP3_pin = RAD915_GP3_Pin;
  radio915.payloadSize = payloadSize;
  radio915.initialized = false;
  radio915.packetCfg = TIRADIO_PKTLEN_FIXED;
  radio915.id = RADIO_915;

#ifdef HAS_CC1200
  radio915.has_cc1190 = true;

  radio915.settingsPtr = cc1200_916_1_2kbps_cfg;
  radio915.settingsSize = sizeof(cc1200_916_1_2kbps_cfg);
#endif

  // Enable our radio and configure pins
  hardwareStatus[RADIO_915] = tiRadio_Init(&radio915);

#if defined(HAS_CC1200) && defined(HAS_RADIO_915)
  // PA/LNA control pins. TODO this seems somewhat code-smell-y since we have
  // CC1200 registers in hardware manager?

  // Both registers need to be inverted since the signal is active low. This is
  // fine since we never go into sleep state anyways

  // IO0 is PA_EN, so should be high in TX
  tiRadio_ConfigGPIO(&radio915, TIRADIO_IOCFG0, TIRADIO_nPA_PD, true);
  // IO2 is LNA_EN, so high in RX
  tiRadio_ConfigGPIO(&radio915, TIRADIO_IOCFG2, TIRADIO_nLNA_PD, true);

#if FCB_VERSION == 1
  // Enable HGM on the V1
  HAL_GPIO_WritePin(HGM_GPIO_Port, HGM_Pin, GPIO_PIN_SET);
#endif  // FCB_VERSION

#endif  // HAS 915 and has cc1200

#endif  // has_radio_915

#ifdef HAS_PYRO
  /* Pyros */
  pyroCtrl[0].port = FIRE1_GPIO_Port;
  pyroCtrl[0].pin = FIRE1_Pin;
  pyroCtrl[1].port = FIRE2_GPIO_Port;
  pyroCtrl[1].pin = FIRE2_Pin;

  Pyro_init(&pyroCtrl[0]);
  Pyro_init(&pyroCtrl[1]);
#endif

#ifdef HAS_LED_1
  /* LED 1 */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
#endif

#ifdef HAS_USB
  /* USB */
  usbInit();
#endif

  hardwareStatus[IMU1] = true;
  hardwareStatus[IMU2] = true;
  hardwareStatus[BAROMETER1] = true;
  hardwareStatus[BAROMETER2] = true;
  hardwareStatus[HIGH_G_ACCELEROMETER] = true;
}

uint32_t HM_Millis() { return HAL_GetTick(); }

bool HM_FlashReadStart(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
#ifdef HAS_S25FLX
  return S25FLX_read_start(&s25flx1, startLoc, numBytes, pData);
#else
  return false;
#endif
}

bool HM_FlashWriteStart(uint32_t startLoc, uint32_t numBytes, uint8_t *data) {
#ifdef HAS_S25FLX
  return S25FLX_write_start(&s25flx1, startLoc, numBytes, data);
#else
  return false;
#endif
}

bool HM_FlashEraseSectorStart(uint32_t sectorNum) {
#ifdef HAS_S25FLX
  return S25FLX_erase_sector_start(&s25flx1, sectorNum);
#else
  return false;
#endif
}

bool HM_FlashEraseChipStart() {
#ifdef HAS_S25FLX
  return S25FLX_erase_chip_start(&s25flx1);
#else
  return false;
#endif
}

bool HM_FlashIsReadComplete() {
#ifdef HAS_S25FLX

#ifdef USE_S25FLx_DMA
  return S25FLX_is_read_complete(&s25flx1);
#else
  return true;
#endif

#else
  return false;
#endif  // HAS_S25FLX
}

bool HM_FlashIsWriteComplete() {
#ifdef HAS_S25FLX
  return S25FLX_is_write_complete(&s25flx1);
#else
  return false;
#endif
}

bool HM_FlashIsEraseComplete() {
#ifdef HAS_S25FLX
  return S25FLX_is_erase_complete(&s25flx1);
#else
  return false;
#endif
}

void HM_BuzzerSetFrequency(float fHz) {
#ifdef HAS_BUZZER
  buzzerSetFrequency(&buzzer, fHz);
#endif
}

void HM_BuzzerStart() {
#ifdef HAS_BUZZER
  buzzerStart(&buzzer);
#endif
}

void HM_BuzzerStop() {
#ifdef HAS_BUZZER
  buzzerStop(&buzzer);
#endif
}

void HM_ServoSetAngle(int servoNum, float degrees) {
#ifdef HAS_SERVO
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
#endif
}

void HM_LedSet(int ledNum, bool set) {
  switch (ledNum) {
    case 1:
#ifdef HAS_LED_1
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, set);
#endif
      break;
    default:
      break;
  }
}

void HM_LedToggle(int ledNum) {
  switch (ledNum) {
    case 1:
#ifdef HAS_LED_1
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
#endif
      break;
    default:
      break;
  }
}

bool HM_RadioSend(Hardware_t radioType, uint8_t *data, uint16_t numBytes) {
#if defined(HAS_CC1120) || defined(HAS_CC1200)
  TiRadioCtrl_t *radioPtr;
  switch (radioType) {
#ifdef HAS_RADIO_433
    case RADIO_433:
      radioPtr = &radio433;
      break;
#endif
#ifdef HAS_RADIO_915
    case RADIO_915:
      radioPtr = &radio915;
      break;
#endif
    default:
      return false;
  }

  tiRadio_AddTxPacket(radioPtr, data, radioPtr->payloadSize);
  return true;
#endif /* Has radio */
  return false;
}

void HM_RadioUpdate() {
  // Update our radio(s)
#ifdef HAS_RADIO_915
  tiRadio_Update(&radio915);
#endif
#ifdef HAS_RADIO_433
  tiRadio_Update(&radio433);
#endif
}

void HM_RadioRegisterConsumer(Hardware_t radio, CircularBuffer_t *rxBuffer) {
  TiRadioCtrl_t *pRadio;
  switch (radio) {
#ifdef HAS_RADIO_433
    case RADIO_433:
      pRadio = &radio433;
      break;
#endif
#ifdef HAS_RADIO_915
    case RADIO_915:
      pRadio = &radio915;
      break;
#endif
    default:
      return;
  }

  tiRadio_RegisterConsumer(pRadio, rxBuffer);
}

void HM_RadioSetChannel(Hardware_t radio, int channel) {
  // We set channels by starting from channel 0's frequency and incrementing by
  // a fixed bandwidth between channels

  float centerFrequency;
  TiRadioCtrl_t *rad;
  enum TIRADIO_Band band;

#ifdef HAS_RADIO_433
  if (radio == RADIO_433) {
    centerFrequency = RAD433_CHAN0;
    rad = &radio433;
    band = TIRADIO_BAND_410_480MHz;
  }
#endif
#ifdef HAS_RADIO_915
  if (radio == RADIO_915) {
    centerFrequency = RAD915_CHAN0;
    rad = &radio915;
    band = TIRADIO_BAND_820_960MHz;
  }
#endif

  centerFrequency += channel * RADIO_CHANNEL_BANDWIDTH;

  if (rad != NULL) tiRadio_SetRadioFrequency(rad, band, centerFrequency);
}

bool HM_UsbIsConnected() {
#ifdef HAS_USB
  return usbIsConnected();
#else
  return false;
#endif
}

bool HM_UsbTransmit(uint8_t *data, uint16_t numBytes) {
#ifdef HAS_USB
  return usbTransmit(data, numBytes);
#else
  return false;
#endif
}

CircularBuffer_t *HM_UsbGetRxBuffer() {
#ifdef HAS_USB
  return usbGetRxBuffer();
#else
  return false;
#endif
}

bool HM_BluetoothSend(const uint8_t *data, uint16_t numBytes) {
  // TODO: Implement HM_BluetoothSend
  return false;
}

void HM_SetImu1Sampling(bool enable) { bImu1Sampling = enable; }

void HM_SetImu2Sampling(bool enable) { bImu2Sampling = enable; }

void HM_SetHighGSampling(bool enable) { bBatteryVoltageSampling = enable; }

void HM_SetBaro1Sampling(bool enable) { bBaro1Sampling = enable; }

void HM_SetBaro2Sampling(_Bool enable) { bBaro2Sampling = enable; }

void HM_SetGpsSampling(bool enable) { bGpsSampling = enable; }

void HM_SetBatteryVoltageSampling(bool enable) {
  bBatteryVoltageSampling = enable;
}

void HM_SetPyroContinuitySampling(bool enable) {
  bPyroContinuitySampling = enable;
}

void HM_IWDG_Refresh() { HAL_IWDG_Refresh(IWDG_HANDLE); }

void HM_PyroFire(int channel, uint32_t duration) {
#ifdef HAS_PYRO
  Pyro_start(&pyroCtrl[channel], duration);
#endif
}

void HM_PyroUpdate() {
#ifdef HAS_PYRO
  for (int i = 0; i < MAX_PYRO; i++) {
    Pyro_tick(&pyroCtrl[i]);
  }
#endif
}

static void HM_SimReadSensorData() {
  size_t buffCount =
      cbCount(simRxBuffer);  // This might change if in middle of reception, so
                             // use consistently across function

  // If buffer full, throw everything away since it's probably bad
  if (cbFull(simRxBuffer)) {
    cbFlush(simRxBuffer);
  } else {
    // Buffer isn't full, continue as normal
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
  if (!inSim) {
    // IMU 1 data
    if (bImu1Sampling) {
#if (IMU_1 == IMU_LSM9DS1)
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
#endif
    }

    // IMU 2 data
    if (bImu2Sampling) {
#if (IMU_2 == IMU_LSM9DS1)
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
#endif
    }

    // High G Accelerometer data
    if (bHighGSampling) {
#ifdef HAS_H3LIS331DL
      H3LIS331DL_get_data(&h3lis_1);
      sensorData.high_g_accel_x_raw = h3lis_1.rawVal.x;
      sensorData.high_g_accel_x = h3lis_1.val.x;
      sensorData.high_g_accel_y_raw = h3lis_1.rawVal.y;
      sensorData.high_g_accel_y = h3lis_1.val.y;
      sensorData.high_g_accel_z_raw = h3lis_1.rawVal.z;
      sensorData.high_g_accel_z = h3lis_1.val.z;
#endif
    }

#if (BARO_1 == BARO_MS5607_SPI)
    // Baro 1 data
    if (bBaro1Sampling) {
      MS5607_get_data(&ms5607_1);
      sensorData.baro1_pres = ms5607_1.altData.baro;
      sensorData.baro1_temp = ms5607_1.altData.temp;
    }
#endif

#if (BARO_2 == BARO_MS5607_SPI)
    // Baro 2 data
    if (bBaro2Sampling) {
      MS5607_get_data(&ms5607_2);
      sensorData.baro2_pres = ms5607_2.altData.baro;
      sensorData.baro2_temp = ms5607_2.altData.temp;
    }
#endif

#ifdef HAS_GPS
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
#endif

#ifdef HAS_ADC_DEVICE
    // ADC data
    float adcVal = 0;
    if (bBatteryVoltageSampling) {
#if (FCB_VERSION <= 0)
      adcStartSingleRead(&adcBatteryVoltage);
      if (adcGetValue(&adcBatteryVoltage, &adcVal, 5))
        sensorData.battery_voltage = (double)adcVal;
      else
        sensorData.battery_voltage = 0;
#else
      // TODO: Battery voltage from external ADC
#endif /* FCB_VERSION */
    }
    if (bPyroContinuitySampling) {
      for (int i = 0; i < sizeof(sensorData.pyro_continuity) / sizeof(bool);
           i++) {
        adcStartSingleRead(&adcPyro[i]);
        if (adcGetValue(&adcPyro[i], &adcVal, 5))
          sensorData.pyro_continuity[i] = adcVal > PYRO_CONTINUITY_THRESHOLD;
        else
          sensorData.pyro_continuity[i] = false;
      }
    }
#endif

    // Timestamp data
    // TODO: Make sensor data timestamp get time from PPS-updated timer
    sensorData.timestamp_s = HM_Millis();
  } else {
    // Simming
    HM_SimReadSensorData();
  }
}

SensorProperties_t *HM_GetSensorProperties() { return &sensorProperties; }

void HM_EnableSimMode(CircularBuffer_t *rxBuffer) {
  inSim = true;
  simRxBuffer = rxBuffer;
}

SensorData_t *HM_GetSensorData() { return &sensorData; }

void HM_DisableSimMode(CircularBuffer_t *rxBuffer) { inSim = false; }

bool HM_InSimMode() { return inSim; }
