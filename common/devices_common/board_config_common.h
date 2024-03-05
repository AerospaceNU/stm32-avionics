/*
 * board_config_common.h
 */

#ifndef COMMON_DEVICES_COMMON_BOARD_CONFIG_COMMON_H_
#define COMMON_DEVICES_COMMON_BOARD_CONFIG_COMMON_H_

#include "board_config.h"
#include "data_structures.h"

#ifdef USE_STM32_HAL_HEADER
#include HAL_HEADER
#endif

/* Common Utilities */

// Checks if given device ID is of type DEV_TYPE
#define IS_DEVICE(DEV_ID, DEV_TYPE)  \
  (DEV_ID >= FIRST_ID_##DEV_TYPE) && \
      (DEV_ID < FIRST_ID_##DEV_TYPE + NUM_##DEV_TYPE)

#define HAS_DEV(DEV_TYPE) (NUM_##DEV_TYPE > 0)

/* Define hardware if not configured */
// Real hardware
#ifndef NUM_ACCEL_H3LIS331DL
#define NUM_ACCEL_H3LIS331DL 0
#endif  // NUM_ACCEL_H3LIS331DL
#ifndef NUM_ACCEL_ADXL375
#define NUM_ACCEL_ADXL375 0
#endif  // NUM_ACCEL_ADXL375
#ifndef NUM_OTHER_ACCEL
#define NUM_OTHER_ACCEL 0
#endif  // NUM_OTHER_ACCEL
#ifndef NUM_BAROMETER_MS5607
#define NUM_BAROMETER_MS5607 0
#endif  // NUM_BAROMETER_MS5607
#ifndef NUM_OTHER_BAROMETER
#define NUM_OTHER_BAROMETER 0
#endif  // NUM_OTHER_BAROMETER
#ifndef NUM_BLE_CHIP_NRF
#define NUM_BLE_CHIP_NRF 0
#endif  // NUM_BLE_CHIP_NRF
#ifndef NUM_BLE_CLIENT_STD
#define NUM_BLE_CLIENT_STD 0
#endif  // NUM_BLE_CLIENT_STD
#ifndef NUM_BUZZER_PWM
#define NUM_BUZZER_PWM 0
#endif  // NUM_BUZZER_PWM
#ifndef NUM_DC_MOTOR_PWM
#define NUM_DC_MOTOR_PWM 0
#endif  // NUM_DC_MOTOR_PWM
#ifndef NUM_FLASH_S25FLX
#define NUM_FLASH_S25FLX 0
#endif  // NUM_FLASH_S25FLX
#ifndef NUM_FLASH_MB85RSX
#define NUM_FLASH_MB85RSX 0
#endif  // NUM_FLASH_MB85RSX
#ifndef NUM_GPS_STD
#define NUM_GPS_STD 0
#endif  // NUM_GPS_STD
#ifndef NUM_GPS_UBLOX
#define NUM_GPS_UBLOX 0
#endif  // NUM_GPS_UBLOX
#ifndef NUM_OTHER_GPS
#define NUM_OTHER_GPS 0
#endif
#ifndef NUM_IMU_LSM9DS1
#define NUM_IMU_LSM9DS1 0
#endif  // NUM_IMU_LSM9DS1
#ifndef NUM_IMU_ICM20948
#define NUM_IMU_ICM20948 0
#endif  // NUM_IMU_ICM20948
#ifndef NUM_IMU_ICM42688
#define NUM_IMU_ICM42688 0
#endif  // NUM_IMU_ICM42688
#ifndef NUM_OTHER_IMU
#define NUM_OTHER_IMU 0
#endif  // NUM_OTHER_IMU
#ifndef NUM_IMU_ICM20600
#define NUM_IMU_ICM20600 0
#endif  // NUM_IMU_ICM20600
#ifndef NUM_MAG_IIS2MDC
#define NUM_MAG_IIS2MDC 0
#endif  // NUM_MAG_IIS2MDC
#ifndef NUM_MAG_LIS3MDL
#define NUM_MAG_LIS3MDL 0
#endif  // NUM_MAG_LIS3MDL
#ifndef NUM_OTHER_MAG
#define NUM_OTHER_MAG 0
#endif  // NUM_OTHER_MAG
#ifndef NUM_LED_DIGITAL
#define NUM_LED_DIGITAL 0
#endif  // NUM_LED_DIGITAL
#ifndef NUM_OTHER_LED
#define NUM_OTHER_LED 0
#endif  // NUM_OTHER_LED
#ifndef NUM_LINE_CUTTER_BLE
#define NUM_LINE_CUTTER_BLE 0
#endif  // NUM_LINE_CUTTER_BLE
#ifndef NUM_LINE_CUTTER_DESKTOP
#define NUM_LINE_CUTTER_DESKTOP 0
#endif  // NUM_LINE_CUTTER_DESKTOP
#ifndef NUM_PYRO_DIGITAL
#define NUM_PYRO_DIGITAL 0
#endif  // NUM_PYRO_DIGITAL
#ifndef NUM_PYRO_CONT_HADC
#define NUM_PYRO_CONT_HADC 0
#endif  // NUM_PYRO_CONT_HADC
#ifndef NUM_RADIO_TI_433
#define NUM_RADIO_TI_433 0
#endif  // NUM_RADIO_TI_433
#ifndef NUM_RADIO_TI_915
#define NUM_RADIO_TI_915 0
#endif  // NUM_RADIO_TI_915
#ifndef NUM_SERVO_PWM
#define NUM_SERVO_PWM 0
#endif  // NUM_SERVO_PWM
#ifndef NUM_USB_STD
#define NUM_USB_STD 0
#endif  // NUM_USB_STD
#ifndef NUM_VBAT_STM_ADC
#define NUM_VBAT_STM_ADC 0
#endif  // NUM_VBAT_STM_ADC
#ifndef NUM_OTHER_VBAT_ADC
#define NUM_OTHER_VBAT_ADC 0
#endif  // NUM_OTHER_VBAT_ADC
#ifndef NUM_CURRENT_ADC
#define NUM_CURRENT_ADC 0
#endif  // NUM_CURRENT_ADC
#ifndef NUM_VBAT_INA226
#define NUM_VBAT_INA226 0
#endif  // NUM_VBAT_INA226
#ifndef NUM_WATCHDOG_INTERNAL
#define NUM_WATCHDOG_INTERNAL 0
#endif  // NUM_WATCHDOG_INTERNAL
#ifndef BLE_CLI_ID
#define BLE_CLI_ID 0
#endif  // BLE_CLI_ID
#ifndef RADIO_CLI_ID
#define RADIO_CLI_ID 0
#endif  // RADIO_CLI_ID
#ifndef USB_CLI_ID
#define USB_CLI_ID 0
#endif  // USB_CLI_ID
#ifndef RADIO_TI_TYPE
#define RADIO_TI_TYPE RADIO_TI_TYPE_CC1120
#endif  // RADIO_TI_TYPE
// Simmed desktop hardware
#ifndef NUM_ACCEL_DESKTOP_FILE
#define NUM_ACCEL_DESKTOP_FILE 0
#endif  // NUM_ACCEL_DESKTOP_FILE
#ifndef NUM_BAROMETER_DESKTOP_FILE
#define NUM_BAROMETER_DESKTOP_FILE 0
#endif  // NUM_BAROMETER_DESKTOP_FILE
#ifndef NUM_FLASH_DESKTOP_FILE_BACKED
#define NUM_FLASH_DESKTOP_FILE_BACKED 0
#endif  // NUM_FLASH_DESKTOP_FILE_BACKED
#ifndef NUM_GPS_DESKTOP_FILE
#define NUM_GPS_DESKTOP_FILE 0
#endif  // NUM_GPS_DESKTOP_FILE
#ifndef NUM_IMU_DESKTOP_FILE
#define NUM_IMU_DESKTOP_FILE 0
#endif  // NUM_IMU_DESKTOP_FILE
#ifndef NUM_MAG_DESKTOP_FILE
#define NUM_MAG_DESKTOP_FILE 0
#endif  // NUM_MAG_DESKTOP_FILE
#ifndef NUM_PYRO_DESKTOP_PRINT
#define NUM_PYRO_DESKTOP_PRINT 0
#endif  // NUM_PYRO_DESKTOP_PRINT
#ifndef NUM_PYRO_CONT_DESKTOP_FILE
#define NUM_PYRO_CONT_DESKTOP_FILE 0
#endif  // NUM_PYRO_CONT_DESKTOP_FILE
#ifndef NUM_RADIO_DESKTOP_SOCKET
#define NUM_RADIO_DESKTOP_SOCKET 0
#endif  // NUM_RADIO_DESKTOP_SOCKET
#ifndef NUM_OTHER_RADIO
#define NUM_OTHER_RADIO 0
#endif  // NUM_OTHER_RADIO
#ifndef NUM_VBAT_DESKTOP_FILE
#define NUM_VBAT_DESKTOP_FILE 0
#endif  // NUM_VBAT_DESKTOP_FILE
#ifndef MAX_PACKET_SIZE
#define MAX_PACKET_SIZE 128
#endif

// Flash stuff
#ifndef FLASH_MAX_SECTOR_BYTES
#define FLASH_MAX_SECTOR_BYTES 0x40000
#endif
#ifndef FLASH_MIN_PAGE_SIZE_BYTES
#define FLASH_MIN_PAGE_SIZE_BYTES 0x200
#endif
#ifndef NUM_STM_HADC
#define NUM_STM_HADC 0
#endif  // NUM_STM_HADC

/* Accelerometer */

#define NUM_ACCEL \
  (NUM_ACCEL_H3LIS331DL + NUM_ACCEL_ADXL375 + NUM_ACCEL_DESKTOP_FILE + NUM_OTHER_ACCEL)

#define FIRST_ID_ACCEL_H3LIS331DL 0
#define FIRST_ID_ACCEL_DESKTOP_FILE \
  (FIRST_ID_ACCEL_H3LIS331DL + NUM_ACCEL_H3LIS331DL)
#define FIRST_ID_ACCEL_ADXL375 \
  (FIRST_ID_ACCEL_DESKTOP_FILE + NUM_ACCEL_DESKTOP_FILE)

#if HAS_DEV(ACCEL_H3LIS331DL)
extern SPI_HandleTypeDef* accelH3lis331dlHspi[NUM_ACCEL_H3LIS331DL];
extern GPIO_TypeDef* accelH3lis331dlCsGpioPort[NUM_ACCEL_H3LIS331DL];
extern uint16_t accelH3lis331dlCsPin[NUM_ACCEL_H3LIS331DL];
#endif  // HAS_DEV(ACCEL_H3LIS331DL)

#if HAS_DEV(ACCEL)
extern const Orientation_s accelBoardToLocal[NUM_ACCEL][3];
extern int accelFilterPriority[NUM_ACCEL];
#endif  // HAS_DEV(ACCEL)

/* Barometer */

#define NUM_BAROMETER \
  (NUM_BAROMETER_MS5607 + NUM_BAROMETER_DESKTOP_FILE + NUM_OTHER_BAROMETER)

#define FIRST_ID_BAROMETER_MS5607 0
#define FIRST_ID_BAROMETER_DESKTOP_FILE \
  (FIRST_ID_BAROMETER_MS5607 + NUM_BAROMETER_MS5607)

#if HAS_DEV(BAROMETER_MS5607)
extern SPI_HandleTypeDef* barometerMs5607Hspi[NUM_BAROMETER_MS5607];
extern GPIO_TypeDef* barometerMs5607CsGpioPort[NUM_BAROMETER_MS5607];
extern uint16_t barometerMs5607CsPin[NUM_BAROMETER_MS5607];
#endif  // HAS_DEV(BAROMETER_MS5607)

/* BLE Chips */

#define NUM_BLE_CHIP NUM_BLE_CHIP_NRF

#define FIRST_ID_BLE_CHIP_NRF 0

#if HAS_DEV(BLE_CHIP_NRF)
extern UART_HandleTypeDef* bleChipNrfHuart[NUM_BLE_CHIP_NRF];
#endif  // HAS_DEV(BLE_CHIP_NRF)

#if NUM_BLE_CHIP > 1
#error "Why would you put multiple BLE chips on the same MCU?"
#endif  // NUM_BLE_CHIP > 1

/* BLE Clients */

#define NUM_BLE_CLIENT NUM_BLE_CLIENT_STD

#define FIRST_ID_BLE_CLIENT_STD 0

#if HAS_DEV(BLE_CLIENT_STD)
extern uint8_t bleClientStdAddress[NUM_BLE_CLIENT_STD];
#endif  // HAS_DEV(BLE_CLIENT_STD)

/* Buzzer */

#define NUM_BUZZER NUM_BUZZER_PWM

#define FIRST_ID_BUZZER_PWM 0

#if HAS_DEV(BUZZER_PWM)
extern TIM_HandleTypeDef* buzzerPwmHtim[NUM_BUZZER_PWM];
extern uint32_t buzzerPwmChannel[NUM_BUZZER_PWM];
#endif  // HAS_DEV(BUZZER_PWM)

/* DC Motor */

#define NUM_DC_MOTOR NUM_DC_MOTOR_PWM

#define FIRST_ID_DC_MOTOR_PWM 0

#if HAS_DEV(DC_MOTOR_PWM)
extern TIM_HandleTypeDef* dcMotorPwmHtim[NUM_DC_MOTOR_PWM];
extern uint32_t dcMotorPwmChannel[NUM_DC_MOTOR_PWM];
#endif  // HAS_DEV(DC_MOTOR_PWM)

/* Flash */

#define NUM_FLASH \
  (NUM_FLASH_S25FLX + NUM_FLASH_DESKTOP_FILE_BACKED + NUM_FLASH_MB85RSX)

#define FIRST_ID_FLASH_S25FLX 0
#define FIRST_ID_FLASH_DESKTOP_FILE_BACKED \
  (FIRST_ID_FLASH_S25FLX + NUM_FLASH_S25FLX)
#define FIRST_ID_FLASH_MB85RSX \
  (FIRST_ID_FLASH_DESKTOP_FILE_BACKED + NUM_FLASH_DESKTOP_FILE_BACKED)

#if HAS_DEV(FLASH_S25FLX)
extern SPI_HandleTypeDef* flashS25flxHspi[NUM_FLASH_S25FLX];
extern GPIO_TypeDef* flashS25flxCsGpioPort[NUM_FLASH_S25FLX];
extern uint16_t flashS25flxCsPin[NUM_FLASH_S25FLX];
#endif  // HAS_DEV(FLASH_S25FL)

#if HAS_DEV(FLASH_MB85RSX)
extern SPI_HandleTypeDef* flashMb85rsxHspi[NUM_FLASH_MB85RSX];
extern GPIO_TypeDef* flashMb85rsxCsGpioPort[NUM_FLASH_MB85RSX];
extern uint16_t flashMb85rsxCsPin[NUM_FLASH_MB85RSX];
#endif  // HAS_DEV(FLASH_MB85RSX)

extern const uint32_t kFlashSizeBytes[NUM_FLASH];

/* GPS */

#define NUM_GPS \
  (NUM_GPS_STD + NUM_GPS_UBLOX + NUM_GPS_DESKTOP_FILE + NUM_OTHER_GPS)

#define FIRST_ID_GPS_STD 0
#define FIRST_ID_GPS_UBLOX (FIRST_ID_GPS_STD + NUM_GPS_STD)
#define FIRST_ID_GPS_DESKTOP_FILE (FIRST_ID_GPS_UBLOX + NUM_GPS_UBLOX)

#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
extern UART_HandleTypeDef* gpsStdHuart[NUM_GPS_STD + NUM_GPS_UBLOX];
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

/* IMU */

#define NUM_IMU                                            \
  (NUM_IMU_LSM9DS1 + NUM_IMU_ICM20948 + NUM_IMU_ICM20600 + NUM_IMU_ICM42688 + \
   NUM_IMU_DESKTOP_FILE + NUM_OTHER_IMU)  // NOLINT

#define NUM_MAG_LSM9DS1 NUM_IMU_LSM9DS1
#define NUM_MAG_ICM20948 NUM_IMU_ICM20948
#define FIRST_ID_MAG_LSM9DS1 0
#define FIRST_ID_MAG_ICM20948 (FIRST_ID_MAG_LSM9DS1 + NUM_MAG_LSM9DS1)
#define FIRST_ID_MAG_IIS2DS1 (FIRST_ID_MAG_ICM20948 + NUM_MAG_ICM20948)
#define FIRST_ID_MAG_LIS3MDL (FIRST_ID_MAG_IIS2DS1 + NUM_MAG_IIS2MDC)

#define FIRST_ID_IMU_LSM9DS1 0
#define FIRST_ID_IMU_ICM20948 (FIRST_ID_IMU_LSM9DS1 + NUM_IMU_LSM9DS1)
#define FIRST_ID_IMU_ICM20600 (FIRST_ID_IMU_ICM20948 + NUM_IMU_ICM20948)
#define FIRST_ID_IMU_ICM42688 (FIRST_ID_IMU_ICM20600 + NUM_IMU_ICM20600)
#define FIRST_ID_IMU_DESKTOP_FILE (FIRST_ID_IMU_ICM20600 + NUM_IMU_ICM20600)

// Magnetometers
#define NUM_MAG                                           \
  (NUM_MAG_IIS2MDC + NUM_IMU_LSM9DS1 + NUM_MAG_LIS3MDL + NUM_IMU_ICM20948 + \
   NUM_MAG_DESKTOP_FILE + NUM_OTHER_MAG)  // NOLINT

#if HAS_DEV(MAG)
extern const Orientation_s magBoardToLocal[NUM_MAG][3];
#endif  // HAS_DEV(MAG)

#if HAS_DEV(IMU_LSM9DS1)
extern SPI_HandleTypeDef* imuLsm9ds1AgHspi[NUM_IMU_LSM9DS1];
extern GPIO_TypeDef* imuLsm9ds1AgCsGpioPort[NUM_IMU_LSM9DS1];
extern uint16_t imuLsm9ds1AgCsPin[NUM_IMU_LSM9DS1];
extern SPI_HandleTypeDef* imuLsm9ds1MagHspi[NUM_IMU_LSM9DS1];
extern GPIO_TypeDef* imuLsm9ds1MagCsGpioPort[NUM_IMU_LSM9DS1];
extern uint16_t imuLsm9ds1MagCsPin[NUM_IMU_LSM9DS1];
#endif  // HAS_DEV(IMU_LSM9DS1)

#if HAS_DEV(IMU_ICM20600)
extern SPI_HandleTypeDef* imuIcm20600Hspi[NUM_IMU_ICM20600];
extern GPIO_TypeDef* imuIcm20600CsGpioPort[NUM_IMU_ICM20600];
extern uint16_t imuIcm20600CsPin[NUM_IMU_ICM20600];
#endif  // HAS_DEV(IMU_ICM20600)

#if HAS_DEV(IMU)
extern const Orientation_s imuBoardToLocal[NUM_IMU][3];
extern int imuAccelFilterPriority[NUM_IMU];
#endif  // HAS_DEV(IMU)

/* LEDs */

#define NUM_LED NUM_LED_DIGITAL + NUM_OTHER_LED

#define FIRST_ID_LED_DIGITAL 0

#if HAS_DEV(LED_DIGITAL)
extern GPIO_TypeDef* ledDigitalGpioPort[NUM_LED_DIGITAL];
extern uint16_t ledDigitalPin[NUM_LED_DIGITAL];
#endif  // HAS_DEV(LED_DIGITAL)

/* Line Cutters */

#define NUM_LINE_CUTTER (NUM_LINE_CUTTER_BLE + NUM_LINE_CUTTER_DESKTOP)

#define FIRST_ID_LINE_CUTTER_BLE 0

#if HAS_DEV(LINE_CUTTER_BLE)
extern uint8_t lineCutterBleAddress[NUM_LINE_CUTTER_BLE];
#endif  // HAS_DEV(LINE_CUTTER_BLE)

/* Pyro */

#define NUM_PYRO (NUM_PYRO_DIGITAL + NUM_PYRO_DESKTOP_PRINT)

#define FIRST_ID_PYRO_DIGITAL 0
#define FIRST_ID_PYRO_DESKTOP_PRINT (FIRST_ID_PYRO_DIGITAL + NUM_PYRO_DIGITAL)

#if HAS_DEV(PYRO_DIGITAL)
extern GPIO_TypeDef* pyroDigitalGpioPort[NUM_PYRO_DIGITAL];
extern TIM_HandleTypeDef* pyroDigitalTickTim;
extern uint16_t pyroDigitalPin[NUM_PYRO_DIGITAL];
#endif  // HAS_DEV(PYRO_DIGITAL)

// ADCs

#define NUM_ADC (NUM_STM_HADC)

/* Pyro continuinty */

#define NUM_PYRO_CONT (NUM_PYRO_CONT_HADC + NUM_PYRO_CONT_DESKTOP_FILE)

// We store continuity as a uint8, so more than 8 won't work
#if NUM_PYRO_CONT > 8
#error "Must have at most 8 pyros!"
#endif

#define FIRST_ID_PYRO_CONT_HADC 0
#define FIRST_ID_PYRO_CONT_DESKTOP_FILE \
  (FIRST_ID_PYRO_CONT_ADC + NUM_PYRO_CONT_ADC)

#if HAS_DEV(PYRO_CONT_ADC)
extern ADC_HandleTypeDef* pyroContAdcHadc[NUM_PYRO_CONT_ADC];
extern uint8_t pyroContAdcRank[NUM_PYRO_CONT_ADC];
#endif  // HAS_DEV(PYRO_CONT_ADC)

/* Radio */

#define NUM_RADIO                                                   \
  (NUM_RADIO_TI_433 + NUM_RADIO_TI_915 + NUM_RADIO_DESKTOP_SOCKET + \
   NUM_OTHER_RADIO)  // NOLINT

#define FIRST_ID_RADIO_TI_433 0
#define FIRST_ID_RADIO_TI_915 (FIRST_ID_RADIO_TI_433 + NUM_RADIO_TI_433)
#define FIRST_ID_RADIO_DESKTOP_SOCKET (FIRST_ID_RADIO_TI_915 + NUM_RADIO_TI_915)

#if HAS_DEV(RADIO_TI_433)
extern SPI_HandleTypeDef* radioTi433Hspi[NUM_RADIO_TI_433];
extern GPIO_TypeDef* radioTi433CsGpioPort[NUM_RADIO_TI_433];
extern uint16_t radioTi433CsPin[NUM_RADIO_TI_433];
extern GPIO_TypeDef* radioTi433RstGpioPort[NUM_RADIO_TI_433];
extern uint16_t radioTi433RstPin[NUM_RADIO_TI_433];
extern GPIO_TypeDef* radioTi433MisoGpioPort[NUM_RADIO_TI_433];
extern uint16_t radioTi433MisoPin[NUM_RADIO_TI_433];
extern GPIO_TypeDef* radioTi433Gp0GpioPort[NUM_RADIO_TI_433];
extern uint16_t radioTi433Gp0Pin[NUM_RADIO_TI_433];
extern GPIO_TypeDef* radioTi433Gp2GpioPort[NUM_RADIO_TI_433];
extern uint16_t radioTi433Gp2Pin[NUM_RADIO_TI_433];
extern GPIO_TypeDef* radioTi433Gp3GpioPort[NUM_RADIO_TI_433];
extern uint16_t radioTi433Gp3Pin[NUM_RADIO_TI_433];
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
extern SPI_HandleTypeDef* radioTi915Hspi[NUM_RADIO_TI_915];
extern GPIO_TypeDef* radioTi915CsGpioPort[NUM_RADIO_TI_915];
extern uint16_t radioTi915CsPin[NUM_RADIO_TI_915];
extern GPIO_TypeDef* radioTi915RstGpioPort[NUM_RADIO_TI_915];
extern uint16_t radioTi915RstPin[NUM_RADIO_TI_915];
extern GPIO_TypeDef* radioTi915MisoGpioPort[NUM_RADIO_TI_915];
extern uint16_t radioTi915MisoPin[NUM_RADIO_TI_915];
extern GPIO_TypeDef* radioTi915Gp0GpioPort[NUM_RADIO_TI_915];
extern uint16_t radioTi915Gp0Pin[NUM_RADIO_TI_915];
extern GPIO_TypeDef* radioTi915Gp2GpioPort[NUM_RADIO_TI_915];
extern uint16_t radioTi915Gp2Pin[NUM_RADIO_TI_915];
extern GPIO_TypeDef* radioTi915Gp3GpioPort[NUM_RADIO_TI_915];
extern uint16_t radioTi915Gp3Pin[NUM_RADIO_TI_915];
#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
extern GPIO_TypeDef* radioTi915HgmGpioPort[NUM_RADIO_TI_915];
extern uint16_t radioTi915HgmPin[NUM_RADIO_TI_915];
#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
#endif  // HAS_DEV(RADIO_TI_915)

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
extern int radioDesktopSocketPort[NUM_RADIO_DESKTOP_SOCKET];
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)

/* Servo */

#define NUM_SERVO NUM_SERVO_PWM

#define FIRST_ID_SERVO_PWM 0

#if HAS_DEV(SERVO_PWM)
extern TIM_HandleTypeDef* servoPwmHtim[NUM_SERVO_PWM];
extern uint32_t servoPwmChannel[NUM_SERVO_PWM];
extern float servoPwmMinPulseMs[NUM_SERVO_PWM];
extern float servoPwmMaxPulseMs[NUM_SERVO_PWM];
#endif  // HAS_DEV(SERVO_PWM)

/* USB */

#define NUM_USB NUM_USB_STD

#define FIRST_ID_USB_STD 0

// USB can currently only support a single instance, so throw pre-processor
// error if more than 1 are defined
#if NUM_USB > 1
#error "Trying to stir up trouble with multiple USBs defined, huh?"
#endif  // NUM_USB > 1

/* VBat Sensors */

#define NUM_VBAT                                             \
  (NUM_OTHER_VBAT_ADC + NUM_VBAT_STM_ADC + NUM_VBAT_INA226 + \
   NUM_VBAT_DESKTOP_FILE)  // NOLINT
#define NUM_CURRENT_SENSE (NUM_CURRENT_ADC)

#define FIRST_ID_VBAT_STM_ADC 0
#define FIRST_ID_VBAT_INA226 (FIRST_ID_VBAT_STM_ADC + NUM_VBAT_STM_ADC)
#define FIRST_ID_VBAT_DESKTOP_FILE (FIRST_ID_VBAT_INA226 + NUM_VBAT_INA226)
#define FIRST_ID_VBAT_EXTRA (FIRST_ID_VBAT_DESKTOP_FILE + NUM_VBAT_DESKTOP_FILE)

#if HAS_DEV(STM_HADC)
extern ADC_HandleTypeDef* stmHadcInstances[NUM_STM_HADC];
extern uint8_t vbatAdcRank[NUM_VBAT_STM_ADC];

#define FIRST_ID_STM_HADC 0

typedef struct {
  uint8_t stmAdcIdx;  // Index in the stmHadcInstances array
  uint8_t rank;

  float min;
  float max;
} StmHadcEntry_s;

#endif  // HAS_DEV(STM_HADC)
#if HAS_DEV(VBAT_STM_ADC)
extern StmHadcEntry_s vbatHadcEntries[NUM_VBAT_STM_ADC];
#endif
#if HAS_DEV(PYRO_CONT_HADC)
extern StmHadcEntry_s pyroHadcEntries[NUM_PYRO_CONT_HADC];
#endif

#if HAS_DEV(VBAT_INA226)
extern I2C_HandleTypeDef* vbatIna226Hi2c[NUM_VBAT_INA226];
#endif  // HAS_DEV(VBAT_INA226)

/* ADC calculations */

// #define NUM_STM_HADCS (NUM_VBAT_STM_ADC + NUM_PYRO_CONT_HADC)
// #define FIRST_ARRAY_INDEX_PYRO_CONT_ADC 0
// #define FIRST_ARRAY_INDEX_VBAT (FIRST_ARRAY_INDEX_PYRO_CONT_ADC +
// NUM_PYRO_CONT_ADC)

/* Watchdogs */

#define NUM_WATCHDOG NUM_WATCHDOG_INERNAL

#define FIRST_ID_WATCHDOG_INTERNAL 0

#if HAS_DEV(WATCHDOG_INTERNAL)
extern IWDG_HandleTypeDef* watchdogInternalHiwdg[NUM_WATCHDOG_INTERNAL];
#endif  // HAS_DEV(WATCHDOG_INTERNAL)

#endif  // COMMON_DEVICES_COMMON_BOARD_CONFIG_COMMON_H_
