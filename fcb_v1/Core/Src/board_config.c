/*
 * board_config.c
 */

#include "board_config_common.h"
#include "board_config_types.h"
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

/* Accelerometer */

#if HAS_DEV(ACCEL_H3LIS331DL)
SPI_HandleTypeDef* accelH3lis331dlHspi[NUM_ACCEL_H3LIS331DL] = {&hspi3};
GPIO_TypeDef* accelH3lis331dlCsGpioPort[NUM_ACCEL_H3LIS331DL] = {
    HIGH_G_CS_GPIO_Port};
uint16_t accelH3lis331dlCsPin[NUM_ACCEL_H3LIS331DL] = {HIGH_G_CS_Pin};
#endif  // HAS_DEV(ACCEL_H3LIS331DL)

#if HAS_DEV(ACCEL)
const Orientation_s accelBoardToLocal[NUM_ACCEL][3] = {
    {{AXIS_Y, -1}, {AXIS_X, -1}, {AXIS_Z, -1}},
};
int accelFilterPriority[NUM_ACCEL] = {1};
#endif  // HAS_DEV(ACCEL)

/* Barometer */

#if HAS_DEV(BAROMETER_MS5607)
SPI_HandleTypeDef* barometerMs5607Hspi[NUM_BAROMETER_MS5607] = {&hspi3, &hspi4};
GPIO_TypeDef* barometerMs5607CsGpioPort[NUM_BAROMETER_MS5607] = {
    BARO1_CS_GPIO_Port, BARO2_CS_GPIO_Port};
uint16_t barometerMs5607CsPin[NUM_BAROMETER_MS5607] = {BARO1_CS_Pin,
                                                       BARO2_CS_Pin};
#endif  // HAS_DEV(BAROMETER_MS5607)

/* BLE Chips */

#if HAS_DEV(BLE_CHIP_NRF)
UART_HandleTypeDef* bleChipNrfHuart[NUM_BLE_CHIP_NRF] = {&huart7};
#endif  // HAS_DEV(BLE_CHIP_NRF)

/* BLE Clients */

#if HAS_DEV(BLE_CLIENT_STD)
uint8_t bleClientStdAddress[NUM_BLE_CLIENT_STD] = {1};
#endif  // HAS_DEV(BLE_CLIENT_STD)

/* Buzzer */

#if HAS_DEV(BUZZER_PWM)
TIM_HandleTypeDef* buzzerPwmHtim[NUM_BUZZER_PWM] = {&htim1};
uint32_t buzzerPwmChannel[NUM_BUZZER_PWM] = {TIM_CHANNEL_1};
#endif  // HAS_DEV(BUZZER_PWM)

/* DC Motor */

#if HAS_DEV(DC_MOTOR_PWM)
TIM_HandleTypeDef* dcMotorPwmHtim[NUM_DC_MOTOR_PWM];
uint32_t dcMotorPwmChannel[NUM_DC_MOTOR_PWM];
#endif  // HAS_DEV(DC_MOTOR_PWM)

/* Flash */

#if HAS_DEV(FLASH_S25FLX)
SPI_HandleTypeDef* flashS25flxHspi[NUM_FLASH_S25FLX] = {&hspi1};
GPIO_TypeDef* flashS25flxCsGpioPort[NUM_FLASH_S25FLX] = {FLASH_CS_GPIO_Port};
uint16_t flashS25flxCsPin[NUM_FLASH_S25FLX] = {FLASH_CS_Pin};
#endif  // HAS_DEV(FLASH_S25FL)

const uint32_t kFlashSizeBytes[NUM_FLASH] = {0x4000000};

/* GPS */

#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
UART_HandleTypeDef* gpsStdHuart[NUM_GPS_STD + NUM_GPS_UBLOX] = {&huart4};
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

/* IMU */

#if HAS_DEV(IMU_LSM9DS1)
SPI_HandleTypeDef* imuLsm9ds1AgHspi[NUM_IMU_LSM9DS1] = {&hspi3, &hspi3};
GPIO_TypeDef* imuLsm9ds1AgCsGpioPort[NUM_IMU_LSM9DS1] = {IMU1_AG_CS_GPIO_Port,
                                                         IMU1_AG_CS_GPIO_Port};
uint16_t imuLsm9ds1AgCsPin[NUM_IMU_LSM9DS1] = {IMU1_AG_CS_Pin, IMU1_AG_CS_Pin};
SPI_HandleTypeDef* imuLsm9ds1MagHspi[NUM_IMU_LSM9DS1] = {&hspi3, &hspi3};
GPIO_TypeDef* imuLsm9ds1MagCsGpioPort[NUM_IMU_LSM9DS1] = {IMU1_M_CS_GPIO_Port,
                                                          IMU1_M_CS_GPIO_Port};
uint16_t imuLsm9ds1MagCsPin[NUM_IMU_LSM9DS1] = {IMU1_M_CS_Pin, IMU1_M_CS_Pin};
#endif  // HAS_DEV(IMU_LSM9DS1)

#if HAS_DEV(IMU)
const Orientation_s imuBoardToLocal[NUM_IMU][3] = {
    {{AXIS_X, 1}, {AXIS_Y, 1}, {AXIS_Z, -1}},
    {{AXIS_X, 1}, {AXIS_Y, 1}, {AXIS_Z, -1}}};
int imuAccelFilterPriority[NUM_IMU] = {2, 2};
#endif  // HAS_DEV(IMU)

const Orientation_s magBoardToLocal[NUM_MAG][3] = {
	    {{AXIS_X, -1}, {AXIS_Y, 1}, {AXIS_Z, -1}},
	    {{AXIS_X, -1}, {AXIS_Y, 1}, {AXIS_Z, -1}}};

/* LEDs */

#if HAS_DEV(LED_DIGITAL)
GPIO_TypeDef* ledDigitalGpioPort[NUM_LED_DIGITAL] = {
    LED1_GPIO_Port, LED2_GPIO_Port, LED3_GPIO_Port, LED4_GPIO_Port,
    LED5_GPIO_Port};
uint16_t ledDigitalPin[NUM_LED_DIGITAL] = {LED1_Pin, LED2_Pin, LED3_Pin,
                                           LED4_Pin, LED5_Pin};
#endif  // HAS_DEV(LED_DIGITAL)

/* Line Cutters */

#if HAS_DEV(LINE_CUTTER_BLE)
uint8_t lineCutterBleAddress[NUM_LINE_CUTTER_BLE] = {2, 3};
#endif  // HAS_DEV(LINE_CUTTER_BLE)

/* Pyro */

#if HAS_DEV(PYRO_DIGITAL)
GPIO_TypeDef* pyroDigitalGpioPort[NUM_PYRO_DIGITAL] = {
    FIRE1_GPIO_Port, FIRE2_GPIO_Port, FIRE3_GPIO_Port,
    FIRE4_GPIO_Port, FIRE5_GPIO_Port, FIRE6_GPIO_Port};
TIM_HandleTypeDef* pyroDigitalTickTim = &htim2;
uint16_t pyroDigitalPin[NUM_PYRO_DIGITAL] = {FIRE1_Pin, FIRE2_Pin, FIRE3_Pin,
                                             FIRE4_Pin, FIRE5_Pin, FIRE6_Pin};
#endif  // HAS_DEV(PYRO_DIGITAL)

/* Radio */

#if HAS_DEV(RADIO_TI_433)
SPI_HandleTypeDef* radioTi433Hspi[NUM_RADIO_TI_433];
GPIO_TypeDef* radioTi433CsGpioPort[NUM_RADIO_TI_433];
uint16_t radioTi433CsPin[NUM_RADIO_TI_433];
GPIO_TypeDef* radioTi433RstGpioPort[NUM_RADIO_TI_433];
uint16_t radioTi433RstPin[NUM_RADIO_TI_433];
GPIO_TypeDef* radioTi433MisoGpioPort[NUM_RADIO_TI_433];
uint16_t radioTi433MisoPin[NUM_RADIO_TI_433];
GPIO_TypeDef* radioTi433Gp0GpioPort[NUM_RADIO_TI_433];
uint16_t radioTi433Gp0Pin[NUM_RADIO_TI_433];
GPIO_TypeDef* radioTi433Gp2GpioPort[NUM_RADIO_TI_433];
uint16_t radioTi433Gp2Pin[NUM_RADIO_TI_433];
GPIO_TypeDef* radioTi433Gp3GpioPort[NUM_RADIO_TI_433];
uint16_t radioTi433Gp3Pin[NUM_RADIO_TI_433];
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
SPI_HandleTypeDef* radioTi915Hspi[NUM_RADIO_TI_915] = {&hspi2};
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
GPIO_TypeDef* radioTi915HgmGpioPort[NUM_RADIO_TI_915] = {HGM_GPIO_Port};
uint16_t radioTi915HgmPin[NUM_RADIO_TI_915] = {HGM_Pin};
#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
#endif  // HAS_DEV(RADIO_TI_915)

/* Servo */

#if HAS_DEV(SERVO_PWM)
TIM_HandleTypeDef* servoPwmHtim[NUM_SERVO_PWM] = {&htim8, &htim8, &htim8,
                                                  &htim8};
uint32_t servoPwmChannel[NUM_SERVO_PWM] = {TIM_CHANNEL_4, TIM_CHANNEL_3,
                                           TIM_CHANNEL_2, TIM_CHANNEL_1};
float servoPwmMinPulseMs[NUM_SERVO_PWM] = {0.75, 0.75, 0.75, 0.75};
float servoPwmMaxPulseMs[NUM_SERVO_PWM] = {2.25, 2.25, 2.25, 2.25};
#endif  // HAS_DEV(SERVO_PWM)

/* Various analog sensors */

ADC_HandleTypeDef* stmHadcInstances[NUM_STM_HADC] = {
    &hadc1};

// Maps pyro entry number to [hadc_idx, rank, minValue, maxValue]
StmHadcEntry_s pyroHadcEntries[NUM_PYRO_CONT_HADC] = {
	{0, 2, 0, 3.3 * (127.0 / 27.0) * 10 / 3},
	{0, 5, 0, 3.3 * (127.0 / 27.0) * 10 / 3},
	{0, 6, 0, 3.3 * (127.0 / 27.0) * 10 / 3},
	{0, 3, 0, 3.3 * (127.0 / 27.0) * 10 / 3},
	{0, 1, 0, 3.3 * (127.0 / 27.0) * 10 / 3},
	{0, 4, 0, 3.3 * (127.0 / 27.0) * 10 / 3},
};

/* VBat Sensors */

#if HAS_DEV(VBAT_ADC)
ADC_HandleTypeDef* vbatAdcHadc[NUM_VBAT_ADC];
uint8_t vbatAdcRank[NUM_VBAT_ADC];
ADC_HandleTypeDef* vbatAdcCurrentHadc[NUM_VBAT_ADC];
uint8_t vbatAdcCurrentRank[NUM_VBAT_ADC];
#endif  // HAS_DEV(VBAT_ADC)

#if HAS_DEV(VBAT_INA226)
I2C_HandleTypeDef* vbatIna226Hi2c[NUM_VBAT_INA226] = {&hi2c2};
#endif  // HAS_DEV(VBAT_INA226)

/* Serial pressure sensors */

#if HAS_DEV(SERIAL_DUCER)
UART_HandleTypeDef *serialDucerUart[NUM_SERIAL_DUCER] = {&huart1};
#endif

/* Watchdogs */

#if HAS_DEV(WATCHDOG_INTERNAL)
IWDG_HandleTypeDef* watchdogInternalHiwdg[NUM_WATCHDOG_INTERNAL] = {&hiwdg1};
#endif  // HAS_DEV(WATCHDOG_INTERNAL)
