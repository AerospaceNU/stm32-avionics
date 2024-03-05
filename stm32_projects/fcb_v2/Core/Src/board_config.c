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

SPI_HandleTypeDef* accelAdx375Hspi[NUM_ACCEL_ADX375] = {&hspi2};
GPIO_TypeDef* accelAdx375CsGpioPort[NUM_ACCEL_ADX375] = {
    HIGH_G_CS_GPIO_Port};
uint16_t accelAdx375CsPin[NUM_ACCEL_ADX375] = {HIGH_G_CS_Pin};

// Index 0 of this struct is read to determine the sensor axis to measure accel in board X
// So if board X = - sensor Y, the first index should be {AXIS_Y, -1}
const Orientation_s accelBoardToLocal[NUM_ACCEL][3] = {
    {{AXIS_X, -1}, {AXIS_Y, 1}, {AXIS_Z, -1}},
};
int accelFilterPriority[NUM_ACCEL] = {1};

/* Barometer */

SPI_HandleTypeDef* barometerMs5607Hspi[NUM_BAROMETER_MS5607] = {&hspi2};
GPIO_TypeDef* barometerMs5607CsGpioPort[NUM_BAROMETER_MS5607] = {
    BARO1_CS_GPIO_Port};
uint16_t barometerMs5607CsPin[NUM_BAROMETER_MS5607] = {BARO1_CS_Pin};

/* BLE Chips */

UART_HandleTypeDef* bleChipNrfHuart[NUM_BLE_CHIP_NRF] = {&huart1};

/* BLE Clients */

uint8_t bleClientStdAddress[NUM_BLE_CLIENT_STD] = {1};

/* Buzzer */

TIM_HandleTypeDef* buzzerPwmHtim[NUM_BUZZER_PWM] = {&htim17};
uint32_t buzzerPwmChannel[NUM_BUZZER_PWM] = {TIM_CHANNEL_1};

/* Flash */

SPI_HandleTypeDef* flashS25flxHspi[NUM_FLASH_S25FLX] = {&hspi1};
GPIO_TypeDef* flashS25flxCsGpioPort[NUM_FLASH_S25FLX] = {FLASH_CS_GPIO_Port};
uint16_t flashS25flxCsPin[NUM_FLASH_S25FLX] = {FLASH_CS_Pin};

SPI_HandleTypeDef* flashMb85rsxHspi[NUM_FLASH_MB85RSX] = {&hspi2};
GPIO_TypeDef* flashMb85rsxCsGpioPort[NUM_FLASH_MB85RSX] = {FRAM_CS_GPIO_Port};
uint16_t flashMb85rsxCsPin[NUM_FLASH_MB85RSX] = {FRAM_CS_Pin};

const uint32_t kFlashSizeBytes[NUM_FLASH] = {0x4000000};

// TODO FRAM size configuration!!

/* GPS */

UART_HandleTypeDef* gpsStdHuart[NUM_GPS_STD + NUM_GPS_UBLOX] = {&huart3};

/* IMU */

SPI_HandleTypeDef* imuIcm42688Hspi[NUM_IMU_ICM42688] = {&hspi2};
GPIO_TypeDef* imuIcm42688CsGpioPort[NUM_IMU_ICM42688] = {IMU1_CS_GPIO_Port};
uint16_t imuIcm42688CsPin[NUM_IMU_ICM42688] = {IMU1_CS_Pin};

// Index 0 of this struct is read to determine the sensor axis to measure accel in board X
// So if board X = - sensor Y, the first index should be {AXIS_Y, -1}
const Orientation_s imuBoardToLocal[NUM_IMU][3] = {
    {{AXIS_Y, -1}, {AXIS_X, -1}, {AXIS_Z, -1}}};
int imuAccelFilterPriority[NUM_IMU] = {2};

SPI_HandleTypeDef* magLis3mdlHi2c[NUM_MAG_LIS3MDL] = {&hspi2};
const Orientation_s magBoardToLocal[NUM_MAG][3] = {
    {{AXIS_Y, 1}, {AXIS_X, -1}, {AXIS_Z, -1}}};

/* LEDs */

GPIO_TypeDef* ledDigitalGpioPort[NUM_LED_DIGITAL] = {
		USER_LED_GPIO_Port};
uint16_t ledDigitalPin[NUM_LED_DIGITAL] = {USER_LED_Pin};

/* Line Cutters */

uint8_t lineCutterBleAddress[NUM_LINE_CUTTER_BLE] = {2, 3};

/* Pyro */

GPIO_TypeDef* pyroDigitalGpioPort[NUM_PYRO_DIGITAL] = {
    FIRE1_GPIO_Port, FIRE2_GPIO_Port, FIRE3_GPIO_Port,
    FIRE4_GPIO_Port, PERIPH_EN_GPIO_Port};
TIM_HandleTypeDef* pyroDigitalTickTim = &htim2;
uint16_t pyroDigitalPin[NUM_PYRO_DIGITAL] = {FIRE1_Pin, FIRE2_Pin, FIRE3_Pin,
                                             FIRE4_Pin, PERIPH_EN_Pin};

/* Radio */

SPI_HandleTypeDef* radioTi915Hspi[NUM_RADIO_TI_915] = {&hspi3};
GPIO_TypeDef* radioTi915CsGpioPort[NUM_RADIO_TI_915] = {RAD915_CS_GPIO_Port};
uint16_t radioTi915CsPin[NUM_RADIO_TI_915] = {RAD915_CS_Pin};
GPIO_TypeDef* radioTi915RstGpioPort[NUM_RADIO_TI_915] = {RAD915_RST_GPIO_Port};
uint16_t radioTi915RstPin[NUM_RADIO_TI_915] = {RAD915_RST_Pin};
GPIO_TypeDef* radioTi915MisoGpioPort[NUM_RADIO_TI_915] = {
    RAD915_MISO_GPIO_Port};
uint16_t radioTi915MisoPin[NUM_RADIO_TI_915] = {RAD915_MISO_Pin};
GPIO_TypeDef* radioTi915Gp0GpioPort[NUM_RADIO_TI_915] = {NULL};
uint16_t radioTi915Gp0Pin[NUM_RADIO_TI_915] = {-1};
GPIO_TypeDef* radioTi915Gp2GpioPort[NUM_RADIO_TI_915] = {NULL};
uint16_t radioTi915Gp2Pin[NUM_RADIO_TI_915] = {-1};
GPIO_TypeDef* radioTi915Gp3GpioPort[NUM_RADIO_TI_915] = {RAD915_IO3_GPIO_Port};
uint16_t radioTi915Gp3Pin[NUM_RADIO_TI_915] = {RAD915_IO3_Pin};

/* Servo */

TIM_HandleTypeDef* servoPwmHtim[NUM_SERVO_PWM] = {&htim3, &htim3, &htim3,
                                                  &htim3};
uint32_t servoPwmChannel[NUM_SERVO_PWM] = {TIM_CHANNEL_4, TIM_CHANNEL_3,
                                           TIM_CHANNEL_2, TIM_CHANNEL_1};
float servoPwmMinPulseMs[NUM_SERVO_PWM] = {0.75, 0.75, 0.75, 0.75};
float servoPwmMaxPulseMs[NUM_SERVO_PWM] = {2.25, 2.25, 2.25, 2.25};

/* Various analog sensors */

ADC_HandleTypeDef* stmHadcInstances[NUM_STM_HADC] = {
    &hadc1};

// Maps vbat entry number to [hadc_idx, rank, minValue, maxValue]
StmHadcEntry_s vbatHadcEntries[NUM_VBAT_STM_ADC] = {
		{.stmAdcIdx=0, .rank=5, .min=0, .max=3.3 * (34.8 + 10.0) / 10.0},
		{.stmAdcIdx=0, .rank=6, .min=0, .max= 3.3 * (34.8 + 10.0) / 10.0}
};

// Maps pyro entry number to [hadc_idx, rank, minValue, maxValue]
StmHadcEntry_s pyroHadcEntries[NUM_PYRO_CONT_HADC] = {
	{.stmAdcIdx=0, .rank=1, .min=0, .max=3.3 * (110.0 / 10.0)},
	{.stmAdcIdx=0, .rank=2, .min=0, .max=3.3 * (110.0 / 10.0)},
	{.stmAdcIdx=0, .rank=3, .min=0, .max=3.3 * (110.0 / 10.0)},
	{.stmAdcIdx=0, .rank=4, .min=0, .max=3.3 * (110.0 / 10.0)}
};

/* Watchdogs */

#if HAS_DEV(WATCHDOG_INTERNAL)
IWDG_HandleTypeDef* watchdogInternalHiwdg[NUM_WATCHDOG_INTERNAL] = {&hiwdg};
#endif  // HAS_DEV(WATCHDOG_INTERNAL)
