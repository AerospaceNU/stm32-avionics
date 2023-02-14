#include "hardware_manager.h"

#include "hal_callbacks.h"
#include "radio_packet_types.h"

/* Device includes */

#if HAS_DEV(ACCEL_H3LIS331DL)
#include "accel_h3lis331dl.h"
#endif  // HAS_DEV(ACCEL_H3LIS331DL)

#if HAS_DEV(BAROMETER_MS5607)
#include "barometer_ms5607.h"
#endif  // HAS_DEV(BAROMETER_MS5607)

#if HAS_DEV(BLE_CHIP_NRF)
#include "ble_chip_nrf.h"
#endif  // HAS_DEV(BLE_CHIP_NRF)
#if HAS_DEV(BLE_CLIENT_STD)
#include "ble_client_std.h"
#endif  // HAS_DEV(BLE_CONSOLE)

#if HAS_DEV(BUZZER_PWM)
#include "buzzer_pwm.h"
#endif  // HAS_DEV(BUZZER_PWM)

#if HAS_DEV(DC_MOTOR_PWM)
#include "dc_motor_pwm.h"
#endif  // HAS_DEV(DC_MOTOR_PWM)

#if HAS_DEV(FLASH_S25FLX)
#include "flash_s25flx.h"
#endif  // HAS_DEV(FLASH_S25FLX)

#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
#include "gps.h"
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

#if HAS_DEV(IMU_LSM9DS1)
#include "imu_lsm9ds1.h"
#endif  // HAS_DEV(IMU_LSM9DS1)

#if HAS_DEV(LINE_CUTTER_BLE)
#include "line_cutter_ble.h"
#endif  // HAS_DEV(LINE_CUTTER_BLE)

#if HAS_DEV(PYRO_DIGITAL)
#include "pyro_digital.h"
#endif  // HAS_DEV(PYRO_DIGITAL)

#if HAS_DEV(LED_DIGITAL)
#include "led.h"
#endif  // HAS_DEV(LED_DIGITAL)

#if HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915)
#include "ti_radio.h"
#endif
#if (RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200)
#include "radioconfig/smartrf_cc1200_cfg_434_38_4kbps.h"
#include "radioconfig/smartrf_cc1200_cfg_915_38_4kbps.h"
#endif
#if (RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120)
#include "radioconfig/smartrf_cc1120_cfg_434_38_4kbps.h"
#endif  // HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915)

#if HAS_DEV(SERVO_PWM)
#include "servo_pwm.h"
#endif  // HAS_DEV(SERVO_PWM)

#if HAS_DEV(USB_STD)
#include "usb_std.h"
#endif  // HAS_DEV(USB_STD)

#if HAS_DEV(VBAT_INA226)
#include "vbat_ina226.h"
#endif  // HAS_DEV(USB_STD)

#if HAS_DEV(PYRO_CONT_ADC) || HAS_DEV(VBAT_ADC)
#include "adc_device.h"
#endif  // HAS_DEV(VBAT_ADC)

/* Hardware statuses */

#if HAS_DEV(ACCEL)
bool hardwareStatusAccel[NUM_ACCEL];
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
bool hardwareStatusBarometer[NUM_BAROMETER];
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(BLE_CHIP)
bool hardwareStatusBleChip[NUM_BLE_CHIP];
#endif  // HAS_DEV(BLE)
#if HAS_DEV(BLE_CLIENT)
bool hardwareStatusBleClient[NUM_BLE_CLIENT];
#endif  // HAS_DEV(BLE_CLIENT)
#if HAS_DEV(BUZZER)
bool hardwareStatusBuzzer[NUM_BUZZER];
#endif  // HAS_DEV(BUZZER)
#if HAS_DEV(DC_MOTOR)
bool hardwareStatusDcMotor[NUM_DC_MOTOR];
#endif  // HAS_DEV(DC_MOTOR)
#if HAS_DEV(FLASH)
bool hardwareStatusFlash[NUM_FLASH];
#endif  // HAS_DEV(FLASH)
#if HAS_DEV(GPS)
bool hardwareStatusGps[NUM_GPS];
#endif  // HAS_DEV(GPS)
#if HAS_DEV(IMU)
bool hardwareStatusImu[NUM_IMU];
#endif  // HAS_DEV(IMU)
#if HAS_DEV(LED)
bool hardwareStatusLed[NUM_LED];
#endif  // HAS_DEV(LED)
#if HAS_DEV(LINE_CUTTER)
bool hardwareStatusLineCutter[NUM_LINE_CUTTER];
#endif  // HAS_DEV(LINE_CUTTER)
#if HAS_DEV(PYRO)
bool hardwareStatusPyro[NUM_PYRO];
#endif  // HAS_DEV(PYRO)
#if HAS_DEV(PYRO_CONT)
bool hardwareStatusPyroCont[NUM_PYRO_CONT];
#endif  // HAS_DEV(PYRO_CONT)
#if HAS_DEV(RADIO)
bool hardwareStatusRadio[NUM_RADIO];
#endif  // HAS_DEV(RADIO)
#if HAS_DEV(SERVO)
bool hardwareStatusServo[NUM_SERVO];
#endif  // HAS_DEV(SERVO)
#if HAS_DEV(USB)
bool hardwareStatusUsb[NUM_USB];
#endif  // HAS_DEV(USB)
#if HAS_DEV(VBAT)
bool hardwareStatusVbat[NUM_VBAT];
#endif  // HAS_DEV(VBAT)

/* Hardware objects */

/* Accelerometers */
#if HAS_DEV(ACCEL_H3LIS331DL)
static AccelH3lis331dlCtrl_s accelH3lis331dl[NUM_ACCEL_H3LIS331DL];
#endif  // HAS_DEV(ACCEL_H3LIS331DL)

/* Barometers */
#if HAS_DEV(BAROMETER_MS5607)
static BarometerMs5607Ctrl_s barometerMs5607[NUM_BAROMETER_MS5607];
#endif  // HAS_DEV(BAROMETER_MS5607)

/* BLE Chips */
#if HAS_DEV(BLE_CHIP_NRF)
static BleChipNrfCtrl_t bleChipNrf[NUM_BLE_CHIP_NRF];
#endif  // HAS_DEV(BLE_CHIP_NRF)

/* BLE Clients */
#if HAS_DEV(BLE_CLIENT_STD)
static BleClientStdCtrl_s bleClientStd[NUM_BLE_CLIENT_STD];
#endif  // HAS_DEV(BLE_CLIENT_STD)

/* Buzzers */
#if HAS_DEV(BUZZER_PWM)
static BuzzerPwmCtrl_s buzzerPwm[NUM_BUZZER_PWM];
#endif  // HAS_DEV(BUZZER_PWM)

/* DC Motors */
#if HAS_DEV(DC_MOTOR_PWM)
static DcMotorPwmCtrl_s dcMotorPwm[NUM_DC_MOTOR_PWM];
#endif  // HAS_DEV(DC_MOTOR_PWM)

/* Flash memory */
#if HAS_DEV(FLASH_S25FLX)
static FlashS25flxCtrl_s flashS25flx[NUM_FLASH_S25FLX];
#endif  // HAS_DEV(FLASH_S25FLX)

/* GPS */
#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
static GpsCtrl_s gps[NUM_GPS_STD + NUM_GPS_UBLOX];
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

/* IMUs */
#if HAS_DEV(IMU_LSM9DS1)
static ImuLsm9ds1Ctrl_s imuLsm9ds1[NUM_IMU_LSM9DS1];
#endif  // HAS_DEV(IMU_LSM9DS1)

/* Line Cutters */
#if HAS_DEV(LINE_CUTTER_BLE)
static LineCutterBleCtrl_t lineCutterBle[NUM_LINE_CUTTER_BLE];
#endif  // HAS_DEV(LINE_CUTTER_BLE)

/* Pyros */
#if HAS_DEV(PYRO_DIGITAL)
static PyroDigitalCtrl_s pyroDigital[NUM_PYRO_DIGITAL];
#endif  // HAS_DEV(PYRO_DIGITAL)

/* Leds */
#if HAS_DEV(LED_DIGITAL)
static LedCtrl_s ledDigital[NUM_LED_DIGITAL];
#endif  // HAS_DEV(LED_DIGITAL)

/* Pyro continuity */
#if HAS_DEV(PYRO_CONT_ADC)
static AdcDevCtrl_s pyroContAdc[NUM_PYRO_CONT_ADC];
#endif  // HAS_DEV(PYRO_CONT_ADC)

/* Radio */
#if HAS_DEV(RADIO_TI_433)
static TiRadioCtrl_s radioTi433[NUM_RADIO_TI_433];
#endif  // HAS_DEV(RADIO_TI_433)
#if HAS_DEV(RADIO_TI_915)
static TiRadioCtrl_s radioTi915[NUM_RADIO_TI_915];
#endif  // HAS_DEV(RADIO_TI_915)

/* Servos */
#if HAS_DEV(SERVO_PWM)
static ServoPwmCtrl_t servoPwm[NUM_SERVO_PWM];
#endif  // HAS_DEV(SERVO_PWM)

/* VBat Sensors */
#if HAS_DEV(VBAT_ADC)
static AdcDevCtrl_s vbatAdc[NUM_VBAT_ADC];
static AdcDevCtrl_s vbatAdcCurrent[NUM_VBAT_ADC];
#endif  // HAS_DEV(VBAT_ADC)
#if HAS_DEV(VBAT_INA226)
static VbatIna226Ctrl_s vbatIna226[NUM_VBAT_INA226];
#endif  // HAS_DEV(VBAT_INA226)

/* Sensor info */
static SensorProperties_s sensorProperties;
static SensorData_s sensorData;
static size_t SENSOR_DATA_SIZE = sizeof(SensorData_s);

/* Hardware manager sim mode trackers */
static bool inSim = false;
static CircularBuffer_s *simRxBuffer = NULL;

void hm_hardwareInit() {
  /* Accelerometers */
#if HAS_DEV(ACCEL_H3LIS331DL)
  for (int i = 0; i < NUM_ACCEL_H3LIS331DL; i++) {
    SpiCtrl_t accelH3lis331dlSpi = {accelH3lis331dlHspi[i],
                                    accelH3lis331dlCsGpioPort[i],
                                    accelH3lis331dlCsPin[i]};
    hardwareStatusAccel[FIRST_ID_ACCEL_H3LIS331DL + i] =
        accelH3lis331dl_init(&accelH3lis331dl[i], accelH3lis331dlSpi);
    sensorProperties.accelFs[FIRST_ID_ACCEL_H3LIS331DL + i] =
        981;  // 100G * 9.81 m/s^2
  }
#endif  // HAS_DEV(ACCEL_H3LIS331DL)

  /* Barometers */
#if HAS_DEV(BAROMETER_MS5607)
  for (int i = 0; i < NUM_BAROMETER_MS5607; i++) {
    hardwareStatusBarometer[FIRST_ID_BAROMETER_MS5607 + i] =
        barometerMs5607_init(&barometerMs5607[i], barometerMs5607Hspi[i],
                             barometerMs5607CsGpioPort[i],
                             barometerMs5607CsPin[i]);
  }
#endif  // HAS_DEV(BAROMETER_MS5607)

  /* BLE Chips */
#if HAS_DEV(BLE_CHIP_NRF)
  for (int i = 0; i < NUM_BLE_CHIP_NRF; i++) {
    bleChipNrf_init(&bleChipNrf[i], bleChipNrfHuart[i]);
    hardwareStatusBleChip[FIRST_ID_BLE_CHIP_NRF + i] = true;
  }
#endif  // HAS_DEV(BLE_CHIP)

  /* BLE Clients */
#if HAS_DEV(BLE_CLIENT_STD)
  // Register each client with the 1 chip (1 chip should be enforced by config)
  for (int i = 0; i < NUM_BLE_CLIENT_STD; i++) {
#if HAS_DEV(BLE_CHIP_NRF)
    bleClientStd_init(&bleClientStd[i], (BleChip_s *)(&bleChipNrf[0]),
                      bleClientStdAddress[i]);
#endif  // HAS_DEV(BLE_CHIP_NRF)
    hardwareStatusBleClient[FIRST_ID_BLE_CLIENT_STD + i] = true;
  }
#endif  // HAS_DEV(BLE_CLIENT_STD)

  /* Buzzers */
#if HAS_DEV(BUZZER_PWM)
  for (int i = 0; i < NUM_BUZZER_PWM; i++) {
    buzzerPwm_init(&buzzerPwm[i], buzzerPwmHtim[i], buzzerPwmChannel[i], 500);
    hardwareStatusBuzzer[FIRST_ID_BUZZER_PWM + i] = true;
  }
#endif  // HAS_DEV(BUZZER_PWM)

  /* DC Motors */
#if HAS_DEV(DC_MOTOR_PWM)
  for (int i = 0; i < NUM_DC_MOTOR_PWM; i++) {
    dcMotorPwm_init(&dcMotorPwm[i], dcMotorPwmHtim[i], dcMotorPwmChannel[i]);
    hardwareStatusDcMotor[FIRST_ID_DC_MOTOR_PWM + i] = true;
  }
#endif  // HAS_DEV(DC_MOTOR_PWM)

  /* Flash */
#if HAS_DEV(FLASH_S25FLX)
  for (int i = 0; i < NUM_FLASH_S25FLX; i++) {
    flashS25flx_init(&(flashS25flx[i]), flashS25flxHspi[i],
                     flashS25flxCsGpioPort[i], flashS25flxCsPin[i],
                     kFlashSizeBytes[i]);
    hardwareStatusFlash[FIRST_ID_FLASH_S25FLX + i] = true;
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  /* GPS */
#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
  for (int i = 0; i < NUM_GPS_STD + NUM_GPS_UBLOX; i++) {
    gps_init(&gps[i], gpsStdHuart[i],
             IS_DEVICE(i, GPS_STD) ? GPS_TYPE_STD : GPS_TYPE_UBLOX);
    hardwareStatusGps[FIRST_ID_GPS_STD + i] = true;
  }
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

  /* IMUs */
#if HAS_DEV(IMU_LSM9DS1)
  for (int i = 0; i < NUM_IMU_LSM9DS1; i++) {
    imuLsm9ds1[i].ag.lsm9ds1Spi.hspi = imuLsm9ds1AgHspi[i];
    imuLsm9ds1[i].ag.lsm9ds1Spi.port = imuLsm9ds1AgCsGpioPort[i];
    imuLsm9ds1[i].ag.lsm9ds1Spi.pin = imuLsm9ds1AgCsPin[i];
    imuLsm9ds1[i].m.lsm9ds1Spi.hspi = imuLsm9ds1MagHspi[i];
    imuLsm9ds1[i].m.lsm9ds1Spi.port = imuLsm9ds1MagCsGpioPort[i];
    imuLsm9ds1[i].m.lsm9ds1Spi.pin = imuLsm9ds1MagCsPin[i];
    imuLsm9ds1[i].ag.aFs = FS_XL_16;
    imuLsm9ds1[i].ag.gFs = FS_G_2000;
    imuLsm9ds1[i].m.mFs = FS_M_8;
    lsm9ds1_init(&imuLsm9ds1[i]);
    hardwareStatusImu[FIRST_ID_IMU_LSM9DS1 + i] = true;
    sensorProperties.imuAccelFs[FIRST_ID_IMU_LSM9DS1 + i] =
        156.96;  // 16 G * 9.81 mps2/G
  }
#endif  // HAS_DEV(IMU_LSM9DS1)

  /* LEDs */
#if HAS_DEV(LED_DIGITAL)
  for (int i = 0; i < NUM_LED_DIGITAL; i++) {
    ledDigital[i].port = ledDigitalGpioPort[i];
    ledDigital[i].pin = ledDigitalPin[i];
    led_init(&ledDigital[i]);
    hardwareStatusLed[FIRST_ID_LED_DIGITAL + i] = true;
  }
#endif  // HAS_DEV(LED_DIGITAL)

  /* Pyros */
#if HAS_DEV(PYRO_DIGITAL)
  for (int i = 0; i < NUM_PYRO_DIGITAL; i++) {
    pyroDigital[i].port = pyroDigitalGpioPort[i];
    pyroDigital[i].pin = pyroDigitalPin[i];
    pyroDigital_init(&pyroDigital[i]);
    hardwareStatusPyro[FIRST_ID_PYRO_DIGITAL + i] = true;
  }
  halCallbacks_registerTimPeriodElapsedCallback(pyroDigitalTickTim,
                                                hm_pyroUpdate, NULL);
  HAL_TIM_Base_Start_IT(pyroDigitalTickTim);
#endif  // HAS_DEV(PYRO_DIGITAL)

  /* Line Cutters */
#if HAS_DEV(LINE_CUTTER_BLE)
  for (int i = 0; i < NUM_LINE_CUTTER_BLE; i++) {
    // Register each BLE line cutter with the 1 chip (1 chip should be enforced
    // by config)
#if HAS_DEV(BLE_CHIP_NRF)
    lineCutterBle_init(&lineCutterBle[i], (BleChip_s *)(&bleChipNrf[0]),
                       lineCutterBleAddress[i]);
    hardwareStatusLineCutter[FIRST_ID_LINE_CUTTER_BLE + i] = true;
#endif  // HAS_DEV(BLE_CHIP_NRF)
  }
#endif  // HAS_DEV(LINE_CUTTER_BLE)

  /* Pyro continuity */
#if HAS_DEV(PYRO_CONT_ADC)
  // Pyros - 0 min, (127k/27k*3.3V) max
  // I had to multiply by 10/3 on my V0 to get these to make sense. TODO
  // Why???
  double voltageDividerMax = 3.3 * (127.0 / 27.0) * 10 / 3;
  for (int i = 0; i < NUM_PYRO_CONT_ADC; i++) {
    adcDev_init(&pyroContAdc[i], pyroContAdcHadc[i], pyroContAdcRank[i], 0,
                voltageDividerMax, true);
    hardwareStatusPyroCont[FIRST_ID_PYRO_CONT_ADC + i] = true;
  }
#endif  // HAS_DEV(PYRO_CONT_ADC)

  /* Radios */
#if HAS_DEV(RADIO_TI_433)
  for (int i = 0; i < NUM_RADIO_TI_433; i++) {
    radioTi433[i].radhspi = radioTi433Hspi[i];
    radioTi433[i].CS_port = radioTi433CsGpioPort[i];
    radioTi433[i].CS_pin = radioTi433CsPin[i];
    radioTi433[i].RST_port = radioTi433RstGpioPort[i];
    radioTi433[i].RST_pin = radioTi433RstPin[i];
    radioTi433[i].MISO_port = radioTi433MisoGpioPort[i];
    radioTi433[i].MISO_pin = radioTi433MisoPin[i];
    radioTi433[i].GP0_port = radioTi433Gp0GpioPort[i];
    radioTi433[i].GP0_pin = radioTi433Gp0Pin[i];
    radioTi433[i].GP2_port = radioTi433Gp2GpioPort[i];
    radioTi433[i].GP2_pin = radioTi433Gp2Pin[i];
    radioTi433[i].GP3_port = radioTi433Gp3GpioPort[i];
    radioTi433[i].GP3_pin = radioTi433Gp3Pin[i];
    radioTi433[i].payloadSize = sizeof(RadioPacket_s);
    radioTi433[i].id = 0;
    radioTi433[i].packetCfg = TIRADIO_PKTLEN_FIXED;
    radioTi433[i].initialized = false;

    // All 433 chips do NOT have a 1190 -- how can they?
    radioTi433[i].has_cc1190 = false;

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
    radioTi433[i].settingsPtr = cc1200_433_1_2kbps_cfg;
    radioTi433[i].settingsSize = cc1200_433_1_2kbps_size;
#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120
    radioTi433[i].settingsPtr = cc1120_433_1_2kbps_cfg;
    radioTi433[i].settingsSize = sizeof(cc1120_433_1_2kbps_cfg);
#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1120

    // Enable our radio and configure pins
    hardwareStatusRadio[FIRST_ID_RADIO_TI_433 + i] =
        tiRadio_init(&radioTi433[i]);
  }
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
  for (int i = 0; i < NUM_RADIO_TI_915; i++) {
    radioTi915[i].radhspi = radioTi915Hspi[i];
    radioTi915[i].CS_port = radioTi915CsGpioPort[i];
    radioTi915[i].CS_pin = radioTi915CsPin[i];
    radioTi915[i].RST_port = radioTi915RstGpioPort[i];
    radioTi915[i].RST_pin = radioTi915RstPin[i];
    radioTi915[i].MISO_port = radioTi915MisoGpioPort[i];
    radioTi915[i].MISO_pin = radioTi915MisoPin[i];
    radioTi915[i].GP0_port = radioTi915Gp0GpioPort[i];
    radioTi915[i].GP0_pin = radioTi915Gp0Pin[i];
    radioTi915[i].GP2_port = radioTi915Gp2GpioPort[i];
    radioTi915[i].GP2_pin = radioTi915Gp2Pin[i];
    radioTi915[i].GP3_port = radioTi915Gp3GpioPort[i];
    radioTi915[i].GP3_pin = radioTi915Gp3Pin[i];
    radioTi915[i].payloadSize = sizeof(RadioPacket_s);
    radioTi915[i].id = 1;
    radioTi915[i].packetCfg = TIRADIO_PKTLEN_FIXED;
    radioTi915[i].initialized = false;

#if RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
    radioTi915[i].has_cc1190 = true;

    radioTi915[i].settingsPtr = cc1200_916_1_2kbps_cfg;
    radioTi915[i].settingsSize = sizeof(cc1200_916_1_2kbps_cfg);
#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200

    // Enable our radio and configure pins
    hardwareStatusRadio[FIRST_ID_RADIO_TI_915 + i] =
        tiRadio_init(&radioTi915[i]);

#if (RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200)
    // PA/LNA control pins. TODO this seems somewhat code-smell-y since we
    // have CC1200 registers in hardware manager?

    // Both registers need to be inverted since the signal is active low. This
    // is fine since we never go into sleep state anyways

    // IO0 is PA_EN, so should be high in TX
    tiRadio_configGpio(&radioTi915[i], TIRADIO_IOCFG0, TIRADIO_nPA_PD, true);
    // IO2 is LNA_EN, so high in RX
    tiRadio_configGpio(&radioTi915[i], TIRADIO_IOCFG2, TIRADIO_nLNA_PD, true);

    // Enable HGM on CC1200s TODO: Make function to enable HGM in radio
    HAL_GPIO_WritePin(radioTi915HgmGpioPort[i], radioTi915HgmPin[i],
                      GPIO_PIN_SET);
#endif  // RADIO_TI_TYPE == RADIO_TI_TYPE_CC1200
  }
#endif  // HAS_DEV(RADIO_TI_915)

  /* Servos */
#if HAS_DEV(SERVO_PWM)
  for (int i = 0; i < NUM_SERVO_PWM; i++) {
    hardwareStatusServo[FIRST_ID_SERVO_PWM + i] =
        servoPwm_init(&servoPwm[i], servoPwmHtim[i], servoPwmChannel[i], 20,
                      servoPwmMinPulseMs[i], servoPwmMaxPulseMs[i], -90, 90);
  }
#endif  // HAS_DEV(SERVO_PWM)

  /* USB */
#if HAS_DEV(USB_STD)
  usbStd_init();
  hardwareStatusUsb[FIRST_ID_USB_STD] = true;
#endif  // HAS_DEV(USB_STD)

  /* VBat Sensors */
#if HAS_DEV(VBAT_ADC)
  // Battery voltage - 0 min, seems like 10v in = 2.4v on the voltage divider?
  float vbatMax = 67;  // 4.72 Volts/volt, 67v fullscale??
  // Also the scale seems non linear: at 10vin this is right, but at 13vin it
  // reads 0.25 above the true number. Apparently this is coz our ADCs aren't
  // buffered
  // TODO why does 67 make things work?
  for (int i = 0; i < NUM_VBAT_ADC; i++) {
    adcDev_init(&vbatAdc[i], vbatAdcHadc[i], vbatAdcRank[i], 0, vbatMax, true);
    adcDev_init(&vbatAdcCurrent[i], vbatAdcCurrentHadc[i],
                vbatAdcCurrentRank[i], -12.5, 17.5, true);
    hardwareStatusVbat[FIRST_ID_VBAT_ADC + i] = true;
    hardwareStatusVbat[FIRST_ID_VBAT_ADC + i] = true;
  }
#endif  // HAS_DEV(VBAT_ADC)
#if HAS_DEV(VBAT_INA226)
  for (int i = 0; i < NUM_VBAT_INA226; i++) {
    vbatIna226[i].hi2c = vbatIna226Hi2c[i];
    vbatIna226[i].addr = 0x40;  // (01000000b)
    vbatIna226[i].rShuntVal = 0.002;
    vbatIna226[i].iMaxExpected = 10;
    hardwareStatusVbat[FIRST_ID_VBAT_INA226 + i] =
        vbatIna226_init(&vbatIna226[i]);
  }
#endif  // HAS_DEV(VBAT_INA226)
}

uint32_t hm_millis() { return HAL_GetTick(); }

bool hm_flashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                       uint8_t *pData) {
#if HAS_DEV(FLASH_S25FLX)
  if (IS_DEVICE(flashId, FLASH_S25FLX)) {
    return flashS25flx_readStart(
        &(flashS25flx[flashId - FIRST_ID_FLASH_S25FLX]), startLoc, numBytes,
        pData);
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  return false;
}

bool hm_flashWriteStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                        uint8_t *data) {
#if HAS_DEV(FLASH_S25FLX)
  if (IS_DEVICE(flashId, FLASH_S25FLX)) {
    return flashS25flx_writeStart(
        &(flashS25flx[flashId - FIRST_ID_FLASH_S25FLX]), startLoc, numBytes,
        data);
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  return false;
}

bool hm_flashEraseSectorStart(int flashId, uint32_t sectorNum) {
#if HAS_DEV(FLASH_S25FLX)
  if (IS_DEVICE(flashId, FLASH_S25FLX)) {
    return flashS25flx_eraseSectorStart(
        &(flashS25flx[flashId - FIRST_ID_FLASH_S25FLX]), sectorNum);
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  return false;
}

bool hm_flashEraseChipStart(int flashId) {
#if HAS_DEV(FLASH_S25FLX)
  if (IS_DEVICE(flashId, FLASH_S25FLX)) {
    return flashS25flx_eraseChipStart(
        &(flashS25flx[flashId - FIRST_ID_FLASH_S25FLX]));
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  return false;
}

bool hm_flashIsReadComplete(int flashId) {
#if HAS_DEV(FLASH_S25FLX)
  if (IS_DEVICE(flashId, FLASH_S25FLX)) {
#ifdef USE_S25FLx_DMA
    return flashS25flx_isReadComplete(
        &(flashS25flx[flashId - FIRST_ID_FLASH_S25FLX]));
#else
    return true;
#endif  // USE_S25FLx_DMA
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  return false;
}

bool hm_flashIsWriteComplete(int flashId) {
#if HAS_DEV(FLASH_S25FLX)
  if (IS_DEVICE(flashId, FLASH_S25FLX)) {
    return flashS25flx_isWriteComplete(
        &(flashS25flx[flashId - FIRST_ID_FLASH_S25FLX]));
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  return false;
}

bool hm_flashIsEraseComplete(int flashId) {
#if HAS_DEV(FLASH_S25FLX)
  if (IS_DEVICE(flashId, FLASH_S25FLX)) {
    return flashS25flx_isEraseComplete(
        &(flashS25flx[flashId - FIRST_ID_FLASH_S25FLX]));
  }
#endif  // HAS_DEV(FLASH_S25FLX)

  return false;
}

void hm_buzzerSetFrequency(int buzzerId, float fHz) {
#if HAS_DEV(BUZZER_PWM)
  if (IS_DEVICE(buzzerId, BUZZER_PWM)) {
    buzzerPwm_setFrequency(&buzzerPwm[buzzerId - FIRST_ID_BUZZER_PWM], fHz);
  }
#endif  // HAS_DEV(BUZZER_PWM)
}

void hm_buzzerStart(int buzzerId) {
#if HAS_DEV(BUZZER_PWM)
  if (IS_DEVICE(buzzerId, BUZZER_PWM)) {
    buzzerPwm_start(&buzzerPwm[buzzerId - FIRST_ID_BUZZER_PWM]);
  }
#endif  // HAS_DEV(BUZZER_PWM)
}

void hm_buzzerStop(int buzzerId) {
#if HAS_DEV(BUZZER_PWM)
  if (IS_DEVICE(buzzerId, BUZZER_PWM)) {
    buzzerPwm_stop(&buzzerPwm[buzzerId - FIRST_ID_BUZZER_PWM]);
  }
#endif  // HAS_DEV(BUZZER_PWM)
}

void hm_servoSetAngle(int servoId, float degrees) {
#if HAS_DEV(SERVO_PWM)
  if (IS_DEVICE(servoId, SERVO_PWM)) {
    servoPwm_setAngle(&servoPwm[servoId - FIRST_ID_SERVO_PWM], degrees);
  }
#endif  // HAS_DEV(SERVO_PWM)
}

void hm_ledSet(int ledId, bool set) {
#if HAS_DEV(LED_DIGITAL)
  if (IS_DEVICE(ledId, LED_DIGITAL)) {
    led_set(&ledDigital[ledId - FIRST_ID_LED_DIGITAL], set);
  }
#endif  // HAS_DEV(LED_DIGITAL)
}

void hm_ledToggle(int ledId) {
#if HAS_DEV(LED_DIGITAL)
  if (IS_DEVICE(ledId, LED_DIGITAL)) {
    led_toggle(&ledDigital[ledId - FIRST_ID_LED_DIGITAL]);
  }
#endif  // HAS_DEV(LED_DIGITAL)
}

bool hm_radioSend(int radioNum, uint8_t *data, uint16_t numBytes) {
#if HAS_DEV(RADIO_TI_433)
  if (IS_DEVICE(radioNum, RADIO_TI_433)) {
    TiRadioCtrl_s *pRadio = &radioTi433[radioNum - FIRST_ID_RADIO_TI_433];
    return tiRadio_addTxPacket(pRadio, data, pRadio->payloadSize);
  }
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
  if (IS_DEVICE(radioNum, RADIO_TI_915)) {
    TiRadioCtrl_s *pRadio = &radioTi915[radioNum - FIRST_ID_RADIO_TI_915];
    return tiRadio_addTxPacket(pRadio, data, pRadio->payloadSize);
  }
#endif  // HAS_DEV(RADIO_TI_915)

  return false;
}

void hm_radioUpdate() {
#if HAS_DEV(RADIO_TI_433)
  for (int i = 0; i < NUM_RADIO_TI_433; i++) {
    tiRadio_update(&radioTi433[i]);
  }
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
  for (int i = 0; i < NUM_RADIO_TI_915; i++) {
    tiRadio_update(&radioTi915[i]);
  }
#endif  // HAS_DEV(RADIO_TI_915)
}

void hm_radioRegisterConsumer(int radioNum, CircularBuffer_s *rxBuffer) {
#if HAS_DEV(RADIO_TI_433)
  if (IS_DEVICE(radioNum, RADIO_TI_433)) {
    TiRadioCtrl_s *pRadio = &radioTi433[radioNum - FIRST_ID_RADIO_TI_433];
    tiRadio_registerConsumer(pRadio, rxBuffer);
  }
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
  if (IS_DEVICE(radioNum, RADIO_TI_915)) {
    TiRadioCtrl_s *pRadio = &radioTi915[radioNum - FIRST_ID_RADIO_TI_915];
    tiRadio_registerConsumer(pRadio, rxBuffer);
  }
#endif  // HAS_DEV(RADIO_TI_915)
}

void hm_radioSetChannel(int radioNum, int channel) {
#if HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915)
  // We set channels by starting from channel 0's frequency and incrementing
  // by a fixed bandwidth between channels
  float centerFrequency;
  TiRadioCtrl_s *rad = NULL;
  TiRadioBand_e band;

#if HAS_DEV(RADIO_TI_433)
  if (IS_DEVICE(radioNum, RADIO_TI_433)) {
    centerFrequency = RAD433_CHAN0;
    rad = &radioTi433[radioNum - FIRST_ID_RADIO_TI_433];
    band = TIRADIO_BAND_410_480MHz;
  }
#endif  // HAS_DEV(RADIO_TI_433)

#if HAS_DEV(RADIO_TI_915)
  if (IS_DEVICE(radioNum, RADIO_TI_915)) {
    centerFrequency = RAD915_CHAN0;
    rad = &radioTi915[radioNum - FIRST_ID_RADIO_TI_915];
    band = TIRADIO_BAND_820_960MHz;
  }
#endif  // HAS_DEV(RADIO_TI_915)

  centerFrequency += channel * RADIO_CHANNEL_BANDWIDTH;
  tiRadio_setRadioFrequency(rad, band, centerFrequency);
#endif  // HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915)
}

bool hm_usbIsConnected(int usbId) {
#if HAS_DEV(USB_STD)
  if (IS_DEVICE(usbId, USB_STD)) {
    return usbStd_isConnected();
  }
#endif  // HAS_DEV(USB_STD)

  return false;
}

bool hm_usbTransmit(int usbId, uint8_t *data, uint16_t numBytes) {
#if HAS_DEV(USB_STD)
  if (IS_DEVICE(usbId, USB_STD)) {
    return usbStd_transmit(data, numBytes);
  }
#endif  // HAS_DEV(USB_STD)

  return false;
}

CircularBuffer_s *hm_usbGetRxBuffer(int usbId) {
#if HAS_DEV(USB_STD)
  if (IS_DEVICE(usbId, USB_STD)) {
    return usbStd_getRxBuffer();
  }
#endif  // HAS_DEV(USB_STD)

  return NULL;
}

bool hm_bleClientConnected(int bleClientId) {
#if HAS_DEV(BLE_CLIENT_STD)
  if (IS_DEVICE(bleClientId, BLE_CLIENT_STD)) {
    return bleClientStd[bleClientId - FIRST_ID_BLE_CLIENT_STD]
        .bleChip->isAddressConnected(
            bleClientStd[bleClientId - FIRST_ID_BLE_CLIENT_STD].bleChip,
            bleClientStdAddress[bleClientId - FIRST_ID_BLE_CLIENT_STD]);
  }
#endif  // HAS_DEV(BLE_CLIENT_STD)
  return false;
}

bool hm_bleClientSend(int bleClientId, const uint8_t *data, uint16_t numBytes) {
#if HAS_DEV(BLE_CLIENT_STD)
  if (IS_DEVICE(bleClientId, BLE_CLIENT_STD)) {
    return bleClientStd[bleClientId - FIRST_ID_BLE_CLIENT_STD]
        .bleChip->sendRequest(
            bleClientStd[bleClientId - FIRST_ID_BLE_CLIENT_STD].bleChip,
            bleClientStdAddress[bleClientId - FIRST_ID_BLE_CLIENT_STD], data,
            numBytes);
  }
#endif  // HAS_DEV(BLE_CLIENT_STD)
  return false;
}

CircularBuffer_s *hm_bleClientGetRxBuffer(int bleClientId) {
#if HAS_DEV(BLE_CLIENT_STD)
  if (IS_DEVICE(bleClientId, BLE_CLIENT_STD)) {
    return &bleClientStd[bleClientId - FIRST_ID_BLE_CLIENT_STD].parsedBuffer;
  }
#endif  // // HAS_DEV(BLE_CLIENT_STD)
  return NULL;
}

void hm_bleTick() {
#if HAS_DEV(BLE_CHIP_NRF)
  for (int i = 0; i < NUM_BLE_CHIP_NRF; i++) {
    bleChipNrf_tick(&bleChipNrf[i]);
  }
#endif  // HAS_DEV(BLE_CHIP_NRF)

#if HAS_DEV(BLE_CLIENT_STD)
  for (int i = 0; i < NUM_BLE_CLIENT_STD; i++) {
    bleClientStd_tick(&bleClientStd[i]);
  }
#endif  // HAS_DEV(BLE_CLIENT_STD)

#if HAS_DEV(LINE_CUTTER_BLE)
  for (int i = 0; i < NUM_LINE_CUTTER_BLE; i++) {
    lineCutterBle_tick(&lineCutterBle[i]);
  }
#endif  // HAS_DEV(LINE_CUTTER_BLE)
}

LineCutterData_s *hm_getLineCutterData(int lineCutterId) {
#if HAS_DEV(LINE_CUTTER_BLE)
  if (IS_DEVICE(lineCutterId, LINE_CUTTER_BLE)) {
    return &lineCutterBle[lineCutterId - FIRST_ID_LINE_CUTTER_BLE].lastData;
  }
#endif  // HAS_DEV(LINE_CUTTER_BLE)
  return NULL;
}

LineCutterFlightVars_s *hm_getLineCutterFlightVariables(int lineCutterId) {
#if HAS_DEV(LINE_CUTTER_BLE)
  if (IS_DEVICE(lineCutterId, LINE_CUTTER_BLE)) {
    return &lineCutterBle[lineCutterId - FIRST_ID_LINE_CUTTER_BLE].flightVars;
  }
#endif  // HAS_DEV(LINE_CUTTER_BLE)
  return NULL;
}

bool hm_lineCutterSendString(int lineCutterNumber, char *string) {
  if (!string) return false;
#if HAS_DEV(LINE_CUTTER_BLE)
  for (int i = 0; i < NUM_LINE_CUTTER_BLE; i++) {
    if (lineCutterNumber == lineCutterBle[i].lastData.lineCutterNumber) {
      return lineCutterBle_sendString(&lineCutterBle[i], string);
    }
  }
#endif  // HAS_DEV(LINE_CUTTER_BLE)
  return false;
}

bool hm_lineCuttersSendCut(int device) {
  // We want to check that both succeed, not that one succeeds
  // This means we gotta be careful about short-circuiting!
  bool ret = true;

#if HAS_DEV(LINE_CUTTER_BLE)
  /*
  for (int i = 0; i < NUM_LINE_CUTTER_BLE; i++) {
    if (chan == 1) {
      ret &= lineCutterBle_cut1(&lineCutterBle[i]);
    } else if (chan == 2) {
      ret &= lineCutterBle_cut2(&lineCutterBle[i]);
    } else {
      ret = false;
    }
  }
  */
  if (IS_DEVICE(device, LINE_CUTTER_BLE)) {
    ret &= lineCutterBle_cut1(&lineCutterBle[device]);
  }
#endif  // HAS_DEV(LINE_CUTTER_BLE)

  return ret;
}

void hm_watchdogRefresh() {
#if HAS_DEV(WATCHDOG_INTERNAL)
  for (int i = 0; i < NUM_WATCHDOG_INTERNAL; i++) {
    HAL_IWDG_Refresh(watchdogInternalHiwdg[i]);
  }
#endif  // HAS_DEV(WATCHDOG_INTERNAL)
}

void hm_pyroFire(int pyroId, uint32_t duration) {
#if HAS_DEV(PYRO_DIGITAL)
  if (IS_DEVICE(pyroId, PYRO_DIGITAL)) {
    pyroDigital_start(&pyroDigital[pyroId - FIRST_ID_PYRO_DIGITAL], duration);
  }
#endif  // HAS_DEV(PYRO_DIGITAL)
}

void hm_pyroSet(int pyroId, bool enable) {
#if HAS_DEV(PYRO_DIGITAL)
  if (IS_DEVICE(pyroId, PYRO_DIGITAL)) {
    pyroDigital_set(&pyroDigital[pyroId - FIRST_ID_PYRO_DIGITAL], enable);
  }
#endif  // HAS_DEV(PYRO_DIGITAL)
}

void hm_pyroSetPwm(int pyroId, uint32_t duration, uint32_t frequency,
                   uint32_t pulseWidth) {
#if HAS_DEV(PYRO_DIGITAL)
  if (IS_DEVICE(pyroId, PYRO_DIGITAL)) {
    pyroDigital_pwmStart(&pyroDigital[pyroId - FIRST_ID_PYRO_DIGITAL], duration,
                         frequency, pulseWidth);
  }
#endif  // HAS_DEV(PYRO_DIGITAL)
}

void hm_pyroUpdate() {
#if HAS_DEV(PYRO_DIGITAL)
  for (int i = 0; i < NUM_PYRO_DIGITAL; i++) {
    pyroDigital_tick(&pyroDigital[i]);
  }
#endif  // HAS_DEV(PYRO_DIGITAL)
}

void hm_dcMotorSetPercent(int dcMotorId, double percent) {
  if (fabs(percent) > 100) return;

#if HAS_DEV(DC_MOTOR_PWM)
  if (IS_DEVICE(dcMotorId, DC_MOTOR_PWM)) {
    dcMotorPwm_setPercent(&dcMotorPwm[dcMotorId - FIRST_ID_DC_MOTOR_PWM],
                          percent);
  }
#endif  // HAS_DEV(DC_MOTOR_PWM)
}

static void hm_simReadSensorData() {
  size_t buffCount =
      cb_count(simRxBuffer);  // This might change if in middle of reception,
                              // so use consistently across function

  // If buffer full, throw everything away since it's probably bad
  if (cb_full(simRxBuffer)) {
    cb_flush(simRxBuffer);
  } else {
    // Buffer isn't full, continue as normal
    // First throw away any old data that wasn't processed in time
    if (buffCount > SENSOR_DATA_SIZE) {
      cb_dequeue(simRxBuffer, buffCount - SENSOR_DATA_SIZE);
      buffCount = SENSOR_DATA_SIZE;
    }
    // If right amount of bytes are seen, grab them as sensor data
    if (buffCount == SENSOR_DATA_SIZE) {
      cb_peek(simRxBuffer, &sensorData, &SENSOR_DATA_SIZE);
      cb_flush(simRxBuffer);
    }
  }
}

void hm_readSensorData() {
  if (!inSim) {
    // Accelerometer data
#if HAS_DEV(ACCEL_H3LIS331DL)
    for (int i = 0; i < NUM_ACCEL_H3LIS331DL; i++) {
      accelH3lis331dl_getData(&accelH3lis331dl[i]);
      sensorData.accelData[i + FIRST_ID_ACCEL_H3LIS331DL] =
          accelH3lis331dl[i].val;
    }
#endif  // HAS_DEV(ACCEL_H3LIS331DL)

    // Barometer data
#if HAS_DEV(BAROMETER_MS5607)
    bool allBaroComplete = false;
    bool baroComplete[NUM_BAROMETER_MS5607] = {false};
    while (!allBaroComplete) {
      allBaroComplete = true;
      for (int i = 0; i < NUM_BAROMETER_MS5607; i++) {
        if (!baroComplete[i]) {
          allBaroComplete = false;
          baroComplete[i] = barometerMs5607_getData(&barometerMs5607[i]);
          sensorData.barometerData[i + FIRST_ID_BAROMETER_MS5607] =
              barometerMs5607[i].data;
        }
      }
    }
#endif  // HAS_DEV(BAROMETER_MS5607)

    // GPS data
#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
    // TODO: Poll GPS status to determine if data is good
    for (int i = 0; i < NUM_GPS_STD + NUM_GPS_UBLOX; i++) {
      gps_newData(&gps[i]);
      sensorData.gpsData[i + FIRST_ID_GPS_STD] = gps[i].data;
    }
#endif  // HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

    // IMU data
#if HAS_DEV(IMU_LSM9DS1)
    for (int i = 0; i < NUM_IMU_LSM9DS1; i++) {
      lsm9ds1_getData(&imuLsm9ds1[i]);
      sensorData.imuData[FIRST_ID_IMU_LSM9DS1 + i] = imuLsm9ds1[i].data;
    }
#endif  // HAS_DEV(IMU_LSM9DS1)

    // Pyro continuity data
#if HAS_DEV(PYRO_CONT_ADC)
    for (int i = 0; i < NUM_PYRO_CONT_ADC; i++) {
      float adcVal = 0;
      adcDev_startSingleRead(&pyroContAdc[i]);
      sensorData.pyroContData[i] = adcDev_getValue(&pyroContAdc[i], &adcVal, 5)
                                       ? adcVal > PYRO_CONTINUITY_THRESHOLD
                                       : false;
    }
#endif  // HAS_DEV(PYRO_CONT_ADC)

    // VBat data
#if HAS_DEV(VBAT_ADC)
    for (int i = 0; i < NUM_VBAT_ADC; i++) {
      float adcVal = 0;
      adcDev_startSingleRead(&vbatAdc[i]);
      sensorData.vbatData[FIRST_ID_VBAT_ADC + i] =
          adcDev_getValue(&vbatAdc[i], &adcVal, 5) ? adcVal : 0;
    }
#endif  // HAS_DEV(VBAT_ADC)
#if HAS_DEV(VBAT_INA226)
    for (int i = 0; i < NUM_VBAT_INA226; i++) {
      sensorData.vbatData[FIRST_ID_VBAT_INA226 + i] =
          vbatIna226_readBusVoltage(&vbatIna226[i]);
    }
#endif  // HAS_DEV(VBAT_INA226)

    // Timestamp data
    // TODO: Make sensor data timestamp get time from PPS-updated timer
    sensorData.timestampMs = hm_millis();
  } else {
    // Simming
    hm_simReadSensorData();
  }
}

SensorData_s *hm_getSensorData() { return &sensorData; }

SensorProperties_s *hm_getSensorProperties() { return &sensorProperties; }

void hm_enableSimMode(CircularBuffer_s *rxBuffer) {
  inSim = true;
  simRxBuffer = rxBuffer;
}

void hm_disableSimMode(CircularBuffer_s *rxBuffer) { inSim = false; }

bool hm_inSimMode() { return inSim; }
