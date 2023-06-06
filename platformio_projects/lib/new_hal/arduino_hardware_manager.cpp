//
// Created by matth on 3/25/2022.
//

#include "arduino_hardware_manager.h"

#include <fstream>

#include "board_config_common.h"
#include "circular_buffer.h"
#include "device_manager.h"
#include "hardware_manager.h"

#define USB_SERIAL_BUFFER_SIZE 500

void stdArrayToAxis3d(Axis3dReal_s *s, std::array<double, 3> array) {
  s->x = array[0];
  s->y = array[1];
  s->z = array[2];
}

// Device manager object to provide a different (better?) abstraction for
// hardware drivers
DeviceManager deviceManager;

DeviceManager *getDeviceManager() { return &deviceManager; }

// Hardware statuses
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
#if HAS_DEV(MAG)
bool hardwareStatusMag[NUM_MAG];
#endif  // HAS_DEV(MAG)

// SensorData struct for the FCB (Contains all the FCB sensor data)
static SensorData_s sensorData = {0};
static SensorProperties_s sensorProperties;

// Circular buffer for serial reads
static uint8_t serialBuffer[USB_SERIAL_BUFFER_SIZE];
static CircularBuffer_s serialCircularBuffer;

// Circular buffer for radio reads
CircularBuffer_s *radioCircularBuffer;

void hm_hardwareInit() {
  pinMode(LED_BUILTIN, OUTPUT);
  cb_init(&serialCircularBuffer, serialBuffer, USB_SERIAL_BUFFER_SIZE,
          sizeof(uint8_t));

  deviceManager.init();

  if (deviceManager.getBarometer(0)) {
    hardwareStatusBarometer[0] =
        deviceManager.getBarometer(0)->getSensorStatus();
  }

  if (deviceManager.getGps(0)) {
    hardwareStatusGps[0] = deviceManager.getGps(0)->getSensorStatus();
  }

  // TODO: Maybe this can be better?
  if (deviceManager.getAccelerometer(0)) {
    hardwareStatusImu[0] = deviceManager.getAccelerometer(0)->getSensorStatus();
    sensorProperties.imuAccelFs[0] =
        deviceManager.getAccelerometer(0)->getAccelerometerFullScale();
  }

  if (deviceManager.getMagnetometer(0)) {
    hardwareStatusMag[0] = deviceManager.getMagnetometer(0)->getSensorStatus();
  }

  if (deviceManager.getNumberBatteryMonitorAdcs() > 0) {
    hardwareStatusVbat[0] = true;
  }

  hardwareStatusUsb[0] = true;
  hardwareStatusLed[0] = true;
}

uint32_t hm_millis() { return millis(); }

bool hm_flashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                       uint8_t *pData) {
  memset(pData, 0xff, numBytes);
  return false;
}

bool hm_flashWriteStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                        uint8_t *data) {
  return false;
}

bool hm_flashEraseSectorStart(int flashId, uint32_t sectorNum) { return false; }
bool hm_flashEraseChipStart(int flashId) { return false; }
bool hm_flashIsReadComplete(int flashId) { return true; }
bool hm_flashIsWriteComplete(int flashId) { return true; }
bool hm_flashIsEraseComplete(int flashId) { return true; }

void hm_buzzerSetFrequency(int buzzerId, float fHz) {}
void hm_buzzerStart(int buzzerId) {}
void hm_buzzerStop(int buzzerId) {}

void hm_servoSetAngle(int servoId, float degrees) {}

bool led_on = false;

void hm_ledSet(int ledId, bool set) {}
void hm_ledToggle(int ledId) {
  digitalWrite(LED_BUILTIN, led_on);
  led_on = !led_on;
}

bool hm_radioSend(int radioNum, uint8_t *data, uint16_t numBytes) {
  if (deviceManager.getRadio(0)) {
    return deviceManager.getRadio(0)->sendData(data, numBytes);
  }

  return false;
}

void hm_radioUpdate() {
  for (auto &radio : deviceManager.radios) {
    if (radio->isDataAvailable()) {
      static RadioRecievedPacket_s packet;
      packet.radioId = 0;
      packet.rssi = radio->getLastRssi();
      packet.crc = true;
      packet.lqi = 0;

      memset(packet.data, 0, sizeof(packet.data));

      uint8_t len = sizeof(packet.data);
      radio->readData(packet.data, len);

      cb_enqueue(radioCircularBuffer, (unknownPtr_t)&packet);
    }
  }
}

void hm_radioRegisterConsumer(int radioNum, CircularBuffer_s *rxBuffer) {
  radioCircularBuffer = rxBuffer;
}

void hm_radioSetChannel(int radioNum, int channel) {}

bool hm_usbIsConnected(int usbId) { return false; }
bool hm_usbTransmit(int usbId, uint8_t *data, uint16_t numBytes) {
  for (int i = 0; i < numBytes; i++) {
    Serial.print((char)data[i]);
  }

  return true;
}
CircularBuffer_s *hm_usbGetRxBuffer(int usbId) { return &serialCircularBuffer; }

bool hm_bleClientConnected(int bleClientId) { return false; }
bool hm_bleClientSend(int bleClientId, const uint8_t *data, uint16_t numBytes) {
  return false;
}
CircularBuffer_s *hm_bleClientGetRxBuffer(int bleClientId) { return nullptr; }
void hm_bleTick() {}

LineCutterData_s *hm_getLineCutterData(int lineCutterId) { return nullptr; }

LineCutterFlightVars_s *hm_getLineCutterFlightVariables(int lineCutterId) {
  return nullptr;
}
bool hm_lineCutterSendString(int lineCutterNumber, char *string) {
  printf("[Sent to LC %i] %s\n", lineCutterNumber, string);
  return true;
}
bool hm_lineCuttersSendCut(int chan) {
  printf("Line cutter chan %i cut sent!\n", chan);
  return true;
}

void hm_watchdogRefresh() {}

void hm_pyroFire(int pyroId, uint32_t duration) {
  // TODO: Actually fire pyro
}

void hm_pyroSet(int pyroId, bool enable) {
  // TODO: Implement
}

void hm_pyroSetPwm(int pyroId, uint32_t frequency, uint32_t pulseWidth,
                   uint32_t duration) {
  // TODO: Analogwrite()?
}

void hm_pyroUpdate() {
  // TODO: Actually do this
}

void hm_dcMotorSetPercent(int dcMotorId, double percent) {}

void hm_readSensorData() {
  // Read from serial port
  while (Serial.available()) {
    uint8_t data_byte = Serial.read();
    cb_enqueue(&serialCircularBuffer, &data_byte);
  }

  // Read sensors
  deviceManager.readSensors();

  // Get data from barometer
  if (deviceManager.getBarometer(0)) {
    auto barometer = deviceManager.getBarometer(0);

    BarometerData_s baro_data;
    baro_data.pressureAtm = barometer->getPressureAtm();
    baro_data.temperatureC = barometer->getTemperatureC();
    sensorData.barometerData[0] = baro_data;
  }

  // Get data from GPS
  if (deviceManager.getGps(0)) {
    auto gps = deviceManager.getGps(0);
    sensorData.gpsData[0] = *gps->getLastFix();
  }

  // Get data from Gyro and Accelerometer
  if (deviceManager.getAccelerometer(0) && deviceManager.getGyroscope(0)) {
    auto accel = deviceManager.getAccelerometer(0);
    auto gyro = deviceManager.getGyroscope(0);

    ImuData_s imu_data;
    stdArrayToAxis3d(&(imu_data.accelRealMps2), accel->getAccelerometerData());
    stdArrayToAxis3d(&(imu_data.angVelRealRadps), gyro->getGyroscopeData());
    sensorData.imuData[0] = imu_data;
  }

  // Get data from magnetometer
  if (deviceManager.getMagnetometer(0)) {
    auto mag = deviceManager.getMagnetometer(0);

    MagData_s mag_data;
    stdArrayToAxis3d(&mag_data.realGauss, mag->getMagnetometerData());
    sensorData.magData[0] = mag_data;
  }

  // Read battery monitor ADCs
  for (uint i = 0; i < deviceManager.getNumberBatteryMonitorAdcs(); i++) {
    sensorData.vbatData[FIRST_ID_VBAT_ADC + i] =
        deviceManager.readBatteryVoltage(i);
  }

  // Set data timestamp
  sensorData.timestampMs = hm_millis();
}

SensorData_s *hm_getSensorData() { return &sensorData; }

SensorProperties_s *hm_getSensorProperties() { return &sensorProperties; }

void hm_enableSimMode(CircularBuffer_s *rxBuffer) {}

void hm_disableSimMode() {}

bool hm_inSimMode() { return false; }
