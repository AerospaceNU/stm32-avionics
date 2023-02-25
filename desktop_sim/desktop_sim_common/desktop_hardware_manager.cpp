//
// Created by matth on 3/25/2022.
//

#include "desktop_hardware_manager.h"

#include <memory>

#include "hardware_manager_interface.h"

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


std::unique_ptr<HardwareManagerInterface> m_hmInterface;

void hm_sim_setHM(std::unique_ptr<HardwareManagerInterface> hmInterface) {
  m_hmInterface = std::move(hmInterface);
}


extern "C" {

void hm_hardwareInit(){};
uint32_t hm_millis() { return 0; }
bool hm_flashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                       uint8_t *pData) {
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
void hm_ledSet(int ledId, bool set) {}
void hm_ledToggle(int ledId) {}
void hm_readSensorData() {}
bool hm_radioSend(int radioNum, uint8_t *data, uint16_t numBytes) {
  return false;
}
void hm_radioUpdate() {}
void hm_radioRegisterConsumer(int radioNum, CircularBuffer_s *rxBuffer){};
void hm_radioSetChannel(int radioNum, int channel) {}
bool hm_usbIsConnected(int usbId) { return false; }
bool hm_usbTransmit(int usbId, uint8_t *data, uint16_t numBytes) {
  return false;
}
CircularBuffer_s *hm_usbGetRxBuffer(int usbId) { return 0; }
bool hm_bleClientConnected(int bleClientId) { return false; }
bool hm_bleClientSend(int bleClientId, const uint8_t *data, uint16_t numBytes) {
  return false;
}
CircularBuffer_s *hm_bleClientGetRxBuffer(int bleClientId) { return 0; }
void hm_bleTick() {}
LineCutterData_s *hm_getLineCutterData(int lineCutterId) { return 0; }
LineCutterFlightVars_s *hm_getLineCutterFlightVariables(int lineCutterId) {
  return 0;
}
bool hm_lineCutterSendString(int lineCutterNumber, char *string) {
  return true;
}
bool hm_lineCuttersSendCut(int chan) { return true; }
void hm_watchdogRefresh() {}
void hm_pyroFire(int pyroId, uint32_t duration) {}
void hm_pyroSet(int pyroId, bool enable) {}
void hm_pyroSetPwm(int pyroId, uint32_t frequency, uint32_t pulseWidth,
                   uint32_t duration) {}
void hm_pyroUpdate() {}
void hm_dcMotorSetPercent(int dcMotorId, double percent) {}
SensorData_s *hm_getSensorData() { return 0; }
SensorProperties_s *hm_getSensorProperties() { return 0; }
void hm_enableSimMode(CircularBuffer_s *rxBuffer) {}
void hm_disableSimMode() {}
bool hm_inSimMode() { return false; }
void hm_delay(int ms) {}
void hm_observeTickComplete(uint64_t tickNum) {}
}