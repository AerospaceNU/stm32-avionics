/*
 * board_config.c
 */

#include "board_config_common.h"
#include "board_config_types.h"
#include "main.h"
#include "board_config.h"

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
SPI_HandleTypeDef* accelH3lis331dlHspi[NUM_ACCEL_H3LIS331DL] = {&hspi2};
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
SPI_HandleTypeDef* barometerMs5607Hspi[NUM_BAROMETER_MS5607] = {&hspi2, &hspi2};
GPIO_TypeDef* barometerMs5607CsGpioPort[NUM_BAROMETER_MS5607] = {
    BARO1_CS_GPIO_Port, BARO2_CS_GPIO_Port};
uint16_t barometerMs5607CsPin[NUM_BAROMETER_MS5607] = {BARO1_CS_Pin,
                                                       BARO2_CS_Pin};
#endif  // HAS_DEV(BAROMETER_MS5607)

/* BLE Chips */

#if HAS_DEV(BLE_CHIP_NRF)
UART_HandleTypeDef* bleChipNrfHuart[NUM_BLE_CHIP_NRF] = {&huart1};
#endif  // HAS_DEV(BLE_CHIP_NRF)

/* BLE Clients */

#if HAS_DEV(BLE_CLIENT_STD)
uint8_t bleClientStdAddress[NUM_BLE_CLIENT_STD] = {1};
#endif  // HAS_DEV(BLE_CLIENT_STD)

/* Buzzer */

#if HAS_DEV(BUZZER_PWM)
TIM_HandleTypeDef* buzzerPwmHtim[NUM_BUZZER_PWM] = {&htim17};
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

// TODO FRAM configuration!!

/* GPS */

#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
UART_HandleTypeDef* gpsStdHuart[NUM_GPS_STD + NUM_GPS_UBLOX] = {&huart3};
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

/* IMU */

#if HAS_DEV(IMU_ICM20600)
SPI_HandleTypeDef* imuIcm20600Hspi[NUM_IMU_ICM20600] = {&hspi2, &hspi2};
GPIO_TypeDef* imuIcm20600CsGpioPort[NUM_IMU_ICM20600] = {IMU1_CS_GPIO_Port,
                                                         IMU1_CS_GPIO_Port};
uint16_t imuIcm20600CsPin[NUM_IMU_ICM20600] = {IMU1_CS_Pin, IMU2_CS_Pin};
#endif  // HAS_DEV(IMU_ICM20600)

#if HAS_DEV(IMU)
// TODO actually determine orientations!
const Orientation_s imuBoardToLocal[NUM_IMU][3] = {
    {{AXIS_X, 1}, {AXIS_Y, 1}, {AXIS_Z, -1}},
    {{AXIS_X, 1}, {AXIS_Y, 1}, {AXIS_Z, -1}}};
int imuAccelFilterPriority[NUM_IMU] = {2, 2};
#endif  // HAS_DEV(IMU)

/* LEDs */

#if HAS_DEV(LED_DIGITAL)
GPIO_TypeDef* ledDigitalGpioPort[NUM_LED_DIGITAL] = {
		USER_LED_GPIO_Port};
uint16_t ledDigitalPin[NUM_LED_DIGITAL] = {USER_LED_Pin};
#endif  // HAS_DEV(LED_DIGITAL)

/* Line Cutters */

#if HAS_DEV(LINE_CUTTER_BLE)
uint8_t lineCutterBleAddress[NUM_LINE_CUTTER_BLE] = {2, 3};
#endif  // HAS_DEV(LINE_CUTTER_BLE)

/* Pyro */

#if HAS_DEV(PYRO_DIGITAL)
GPIO_TypeDef* pyroDigitalGpioPort[NUM_PYRO_DIGITAL] = {
    FIRE1_GPIO_Port, FIRE2_GPIO_Port, FIRE3_GPIO_Port,
    FIRE4_GPIO_Port};
TIM_HandleTypeDef* pyroDigitalTickTim = &htim2;
uint16_t pyroDigitalPin[NUM_PYRO_DIGITAL] = {FIRE1_Pin, FIRE2_Pin, FIRE3_Pin,
                                             FIRE4_Pin};
#endif  // HAS_DEV(PYRO_DIGITAL)

/* Pyro continuinty */

#if HAS_DEV(PYRO_CONT_ADC)
ADC_HandleTypeDef* pyroContAdcHadc[NUM_PYRO_CONT_ADC] = {
    &hadc1, &hadc1, &hadc1, &hadc1};
uint8_t pyroContAdcRank[NUM_PYRO_CONT_ADC] = {1, 2, 3, 4};
#endif  // HAS_DEV(PYRO_CONT_ADC)

/* Radio */

#if HAS_DEV(RADIO_TI_433)
SPI_HandleTypeDef* radioTi433Hspi[NUM_RADIO_TI_433] = {&hspi3};
GPIO_TypeDef* radioTi433CsGpioPort[NUM_RADIO_TI_433] = {RAD433_CS_GPIO_Port};
uint16_t radioTi433CsPin[NUM_RADIO_TI_433] = {RAD433_CS_Pin};
GPIO_TypeDef* radioTi433RstGpioPort[NUM_RADIO_TI_433] = {RAD433_RST_GPIO_Port};
uint16_t radioTi433RstPin[NUM_RADIO_TI_433] = {RAD433_RST_Pin};
GPIO_TypeDef* radioTi433MisoGpioPort[NUM_RADIO_TI_433] = {
    RAD433_MISO_GPIO_Port};
uint16_t radioTi433MisoPin[NUM_RADIO_TI_433] = {RAD433_MISO_Pin};
GPIO_TypeDef* radioTi433Gp0GpioPort[NUM_RADIO_TI_433] = {RAD433_IO0_GPIO_Port};
uint16_t radioTi433Gp0Pin[NUM_RADIO_TI_433] = {RAD433_IO0_Pin};
GPIO_TypeDef* radioTi433Gp2GpioPort[NUM_RADIO_TI_433] = {RAD433_IO2_GPIO_Port};
uint16_t radioTi433Gp2Pin[NUM_RADIO_TI_433] = {RAD433_IO2_Pin};
GPIO_TypeDef* radioTi433Gp3GpioPort[NUM_RADIO_TI_433] = {RAD433_IO3_GPIO_Port};
uint16_t radioTi433Gp3Pin[NUM_RADIO_TI_433] = {RAD433_IO3_Pin};
#endif  // HAS_DEV(RADIO_TI_433)

/* Servo */

#if HAS_DEV(SERVO_PWM)
TIM_HandleTypeDef* servoPwmHtim[NUM_SERVO_PWM] = {&htim3, &htim3, &htim3,
                                                  &htim3};
uint32_t servoPwmChannel[NUM_SERVO_PWM] = {TIM_CHANNEL_4, TIM_CHANNEL_3,
                                           TIM_CHANNEL_2, TIM_CHANNEL_1};
float servoPwmMinPulseMs[NUM_SERVO_PWM] = {0.75, 0.75, 0.75, 0.75};
float servoPwmMaxPulseMs[NUM_SERVO_PWM] = {2.25, 2.25, 2.25, 2.25};
#endif  // HAS_DEV(SERVO_PWM)

/* VBat Sensors */

#if HAS_DEV(VBAT_ADC)
ADC_HandleTypeDef* vbatAdcHadc[NUM_VBAT_ADC] = {&hadc1, &hadc1};
uint8_t vbatAdcRank[NUM_VBAT_ADC] = {5, 6};
#endif  // HAS_DEV(VBAT_ADC)

/* Watchdogs */

#if HAS_DEV(WATCHDOG_INTERNAL)
IWDG_HandleTypeDef* watchdogInternalHiwdg[NUM_WATCHDOG_INTERNAL] = {&hiwdg};
#endif  // HAS_DEV(WATCHDOG_INTERNAL)
