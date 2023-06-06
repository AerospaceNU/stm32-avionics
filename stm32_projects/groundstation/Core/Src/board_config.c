/*
 * board_config.c
 */

#include "board_config_common.h"
#include "main.h"

#ifdef HAL_ADC_MODULE_ENABLED
#include "adc.h"
#endif  // HAL_ADC_MODULE_ENABLED

#ifdef HAL_I2C_MODULE_ENABLED
#include "i2c.h"
#endif  // HAL_I2C_MODULE_ENABLED

#ifdef HAL_IWDG_MODULE_ENABLED
#include "iwdg.h"
#endif  // HAL_SPI_MODULE_ENABLED

#ifdef HAL_SPI_MODULE_ENABLED
#include "spi.h"
#endif  // HAL_SPI_MODULE_ENABLED

#ifdef HAL_TIM_MODULE_ENABLED
#include "tim.h"
#endif  // HAL_TIM_MODULE_ENABLED

#ifdef HAL_UART_MODULE_ENABLED
#include "usart.h"
#endif  // HAL_UART_MODULE_ENABLED

/* Accelerometers */

#if HAS_DEV(ACCEL_H3LIS331DL)
SPI_HandleTypeDef* accelH3lis331dlHspi[NUM_ACCEL_H3LIS331DL];
GPIO_TypeDef* accelH3lis331dlCsGpioPort[NUM_ACCEL_H3LIS331DL];
uint16_t accelH3lis331dlCsPin[NUM_ACCEL_H3LIS331DL];
#endif  // HAS_DEV(ACCEL_H3LIS331DL)

#if HAS_DEV(ACCEL)
const Orientation_s accelBoardToLocal[NUM_ACCEL][3];
#endif  // HAS_DEV(ACCEL)

/* Barometers */

#if HAS_DEV(BAROMETER_MS5607)
SPI_HandleTypeDef* barometerMs5607Hspi[NUM_BAROMETER_MS5607] = {&hspi4};
GPIO_TypeDef* barometerMs5607CsGpioPort[NUM_BAROMETER_MS5607] = {
    BARO1_CS_GPIO_Port};
uint16_t barometerMs5607CsPin[NUM_BAROMETER_MS5607] = {BARO1_CS_Pin};
#endif  // HAS_DEV(BAROMETER_MS5607)

/* BLE Chips */

#if HAS_DEV(BLE_CHIP_NRF)
UART_HandleTypeDef* bleChipNrfHuart[NUM_BLE_CHIP_NRF];
#endif  // HAS_DEV(BLE_CHIP_NRF)

/* BLE Clients */

#if HAS_DEV(BLE_CLIENT_STD)
uint8_t bleClientStdAddress[NUM_BLE_CLIENT_STD];
#endif  // HAS_DEV(BLE_CLIENT_STD)

/* Buzzers */

#if HAS_DEV(BUZZER_PWM)
TIM_HandleTypeDef* buzzerPwmHtim[NUM_BUZZER_PWM];
uint32_t buzzerPwmChannel[NUM_BUZZER_PWM];
#endif  // HAS_DEV(BUZZER_PWM)

/* DC Motors */

#if HAS_DEV(DC_MOTOR_PWM)
TIM_HandleTypeDef* dcMotorPwmHtim[NUM_DC_MOTOR_PWM];
uint32_t dcMotorPwmChannel[NUM_DC_MOTOR_PWM];
#endif  // HAS_DEV(DC_MOTOR_PWM)

/* Flash */

#if HAS_DEV(FLASH_S25FLX)
SPI_HandleTypeDef* flashS25flxHspi[NUM_FLASH_S25FLX];
GPIO_TypeDef* flashS25flxCsGpioPort[NUM_FLASH_S25FLX];
uint16_t flashS25flxCsPin[NUM_FLASH_S25FLX];
#endif  // HAS_DEV(FLASH_S25FL)

const uint32_t kFlashSizeBytes[NUM_FLASH];

/* GPS */

#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
UART_HandleTypeDef* gpsStdHuart[NUM_GPS_STD + NUM_GPS_UBLOX] = {&huart8};
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

/* IMUs */

#if HAS_DEV(IMU_LSM9DS1)
SPI_HandleTypeDef* imuLsm9ds1AgHspi[NUM_IMU_LSM9DS1];
GPIO_TypeDef* imuLsm9ds1AgCsGpioPort[NUM_IMU_LSM9DS1];
uint16_t imuLsm9ds1AgCsPin[NUM_IMU_LSM9DS1];
SPI_HandleTypeDef* imuLsm9ds1MagHspi[NUM_IMU_LSM9DS1];
GPIO_TypeDef* imuLsm9ds1MagCsGpioPort[NUM_IMU_LSM9DS1];
uint16_t imuLsm9ds1MagCsPin[NUM_IMU_LSM9DS1];
#endif  // HAS_DEV(IMU_LSM9DS1)

#if HAS_DEV(IMU)
const Orientation_s imuBoardToLocal[NUM_IMU][3];
#endif  // HAS_DEV(IMU)

/* LEDs */

#if HAS_DEV(LED_DIGITAL)
GPIO_TypeDef* ledDigitalGpioPort[NUM_LED_DIGITAL];
uint16_t ledDigitalPin[NUM_LED_DIGITAL];
#endif  // HAS_DEV(LED_DIGITAL)

/* Line Cutters */

#if HAS_DEV(LINE_CUTTER_BLE)
uint8_t lineCutterBleAddress[NUM_LINE_CUTTER_BLE];
#endif  // HAS_DEV(LINE_CUTTER_BLE)

/* Pyros */

#if HAS_DEV(PYRO_DIGITAL)
GPIO_TypeDef* pyroDigitalGpioPort[NUM_PYRO_DIGITAL];
uint16_t pyroDigitalPin[NUM_PYRO_DIGITAL];
#endif  // HAS_DEV(PYRO_DIGITAL)

/* Pyro continuity */
#if HAS_DEV(PYRO_CONT_ADC)
ADC_HandleTypeDef* pyroContAdcHadc[NUM_PYRO_CONT_ADC];
uint8_t pyroContAdcRank[NUM_PYRO_CONT_ADC];
#endif  // HAS_DEV(PYRO_CONT_ADC)

/* Radios */

#if HAS_DEV(RADIO_TI_433)
SPI_HandleTypeDef* radioTi433Hspi[NUM_RADIO_TI_433] = {&hspi3};
GPIO_TypeDef* radioTi433CsGpioPort[NUM_RADIO_TI_433] = {RAD433_CS_GPIO_Port};
uint16_t radioTi433CsPin[NUM_RADIO_TI_433] = {RAD433_CS_Pin};
GPIO_TypeDef* radioTi433RstGpioPort[NUM_RADIO_TI_433] = {RAD433_RST_GPIO_Port};
uint16_t radioTi433RstPin[NUM_RADIO_TI_433] = {RAD433_RST_Pin};
GPIO_TypeDef* radioTi433MisoGpioPort[NUM_RADIO_TI_433] = {
    RAD433_MISO_GPIO_Port};
uint16_t radioTi433MisoPin[NUM_RADIO_TI_433] = {RAD433_MISO_Pin};
GPIO_TypeDef* radioTi433Gp0GpioPort[NUM_RADIO_TI_433] = {RAD433_GP0_GPIO_Port};
uint16_t radioTi433Gp0Pin[NUM_RADIO_TI_433] = {RAD433_GP0_Pin};
GPIO_TypeDef* radioTi433Gp2GpioPort[NUM_RADIO_TI_433] = {RAD433_GP2_GPIO_Port};
uint16_t radioTi433Gp2Pin[NUM_RADIO_TI_433] = {RAD433_GP2_Pin};
GPIO_TypeDef* radioTi433Gp3GpioPort[NUM_RADIO_TI_433] = {RAD433_GP3_GPIO_Port};
uint16_t radioTi433Gp3Pin[NUM_RADIO_TI_433] = {RAD433_GP3_Pin};
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
SPI_HandleTypeDef* radioTi915Hspi[NUM_RADIO_TI_915] = {&hspi1};
GPIO_TypeDef* radioTi915CsGpioPort[NUM_RADIO_TI_915] = {RAD915_CS_GPIO_Port};
uint16_t radioTi915CsPin[NUM_RADIO_TI_915] = {RAD915_CS_Pin};
GPIO_TypeDef* radioTi915RstGpioPort[NUM_RADIO_TI_915] = {RAD915_RST_GPIO_Port};
uint16_t radioTi915RstPin[NUM_RADIO_TI_915] = {RAD915_RST_Pin};
GPIO_TypeDef* radioTi915MisoGpioPort[NUM_RADIO_TI_915] = {
    RAD915_MISO_GPIO_Port};
uint16_t radioTi915MisoPin[NUM_RADIO_TI_915] = {RAD915_MISO_Pin};
GPIO_TypeDef* radioTi915Gp0GpioPort[NUM_RADIO_TI_915] = {RAD915_GP0_GPIO_Port};
uint16_t radioTi915Gp0Pin[NUM_RADIO_TI_915] = {RAD915_GP0_Pin};
GPIO_TypeDef* radioTi915Gp2GpioPort[NUM_RADIO_TI_915] = {RAD915_GP2_GPIO_Port};
uint16_t radioTi915Gp2Pin[NUM_RADIO_TI_915] = {RAD915_GP2_Pin};
GPIO_TypeDef* radioTi915Gp3GpioPort[NUM_RADIO_TI_915] = {RAD915_GP3_GPIO_Port};
uint16_t radioTi915Gp3Pin[NUM_RADIO_TI_915] = {RAD915_GP3_Pin};
#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
GPIO_TypeDef* radioTi915HgmGpioPort[NUM_RADIO_TI_915];
uint16_t radioTi915HgmPin[NUM_RADIO_TI_915];
#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
#endif  // HAS_DEV(RADIO_TI_915)

/* Servos */

#if HAS_DEV(SERVO_PWM)
TIM_HandleTypeDef* servoPwmHtim[NUM_SERVO_PWM];
uint32_t servoPwmChannel[NUM_SERVO_PWM];
float servoPwmMinPulseMs[NUM_SERVO_PWM];
float servoPwmMaxPulseMs[NUM_SERVO_PWM];
#endif  // HAS_DEV(SERVO_PWM)

/* VBat Sensors */

#if HAS_DEV(VBAT_STM_ADC)
ADC_HandleTypeDef* vbatAdcHadc[NUM_VBAT_STM_ADC];
uint8_t vbatAdcRank[NUM_VBAT_STM_ADC];
ADC_HandleTypeDef* vbatAdcCurrentHadc[NUM_VBAT_STM_ADC];
uint8_t vbatAdcCurrentRank[NUM_VBAT_STM_ADC];
#endif  // HAS_DEV(VBAT_STM_ADC)

#if HAS_DEV(VBAT_INA226)
I2C_HandleTypeDef* vbatIna226Hi2c[NUM_VBAT_INA226];
#endif  // HAS_DEV(VBAT_INA226)

/* Watchdogs */

#if HAS_DEV(WATCHDOG_INTERNAL)
IWDG_HandleTypeDef* watchdogInternalHiwdg[NUM_WATCHDOG_INTERNAL] = {&hiwdg1};
#endif  // HAS_DEV(WATCHDOG_INTERNAL)

/* USB description strings */
#include "usbd_composite.h"
const char* CDC_ACM_STR_DESC[USBD_CDC_ACM_COUNT] = {
    "Groundstation Command Line",
    "Groundstation Telemetry",
};
