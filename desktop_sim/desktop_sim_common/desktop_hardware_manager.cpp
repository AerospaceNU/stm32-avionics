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

namespace {
std::shared_ptr<HardwareManagerInterface> p =
    std::make_shared<HardwareManagerInterface>();
}

void hm_sim_setHM(std::shared_ptr<HardwareManagerInterface> hmInterface) {
  p = hmInterface;
}

extern "C" {

void hm_hardwareInit() { p->hm_hardwareInit(); }
uint32_t hm_millis() { return p->hm_millis(); }
bool hm_flashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                       uint8_t *pData) {
  return p->hm_flashReadStart(flashId, startLoc, numBytes, pData);
}
bool hm_flashWriteStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                        uint8_t *data) {
  return p->hm_flashWriteStart(flashId, startLoc, numBytes, data);
}
bool hm_flashEraseSectorStart(int flashId, uint32_t sectorNum) {
  return p->hm_flashEraseSectorStart(flashId, sectorNum);
}
bool hm_flashEraseChipStart(int flashId) {
  return p->hm_flashEraseChipStart(flashId);
}
bool hm_flashIsReadComplete(int flashId) {
  return p->hm_flashIsReadComplete(flashId);
}
bool hm_flashIsWriteComplete(int flashId) {
  return p->hm_flashIsWriteComplete(flashId);
}
bool hm_flashIsEraseComplete(int flashId) {
  return p->hm_flashIsEraseComplete(flashId);
}
void hm_buzzerSetFrequency(int buzzerId, float fHz) {
  p->hm_buzzerSetFrequency(buzzerId, fHz);
}
void hm_buzzerStart(int buzzerId) { p->hm_buzzerStart(buzzerId); }
void hm_buzzerStop(int buzzerId) { p->hm_buzzerStop(buzzerId); }
void hm_servoSetAngle(int servoId, float degrees) {
  p->hm_servoSetAngle(servoId, degrees);
}
void hm_ledSet(int ledId, bool set) { p->hm_ledSet(ledId, set); }
void hm_ledToggle(int ledId) { p->hm_ledToggle(ledId); }
void hm_readSensorData() { p->hm_readSensorData(); }
bool hm_radioSend(int radioNum, uint8_t *data, uint16_t numBytes) {
  return p->hm_radioSend(radioNum, data, numBytes);
}
void hm_radioUpdate() { p->hm_radioUpdate(); }
void hm_radioRegisterConsumer(int radioNum, CircularBuffer_s *rxBuffer) {
  p->hm_radioRegisterConsumer(radioNum, rxBuffer);
}
void hm_radioSetChannel(int radioNum, int channel) {
  p->hm_radioSetChannel(radioNum, channel);
}
bool hm_usbIsConnected(int usbId) { return p->hm_usbIsConnected(usbId); }
bool hm_usbTransmit(int usbId, uint8_t *data, uint16_t numBytes) {
  return p->hm_usbTransmit(usbId, data, numBytes);
}
CircularBuffer_s *hm_usbGetRxBuffer(int usbId) {
  return p->hm_usbGetRxBuffer(usbId);
}
bool hm_bleClientConnected(int bleClientId) {
  return p->hm_bleClientConnected(bleClientId);
}
bool hm_bleClientSend(int bleClientId, const uint8_t *data, uint16_t numBytes) {
  return p->hm_bleClientSend(bleClientId, data, numBytes);
}
CircularBuffer_s *hm_bleClientGetRxBuffer(int bleClientId) {
  return p->hm_bleClientGetRxBuffer(bleClientId);
}
void hm_bleTick() { p->hm_bleTick(); }
LineCutterData_s *hm_getLineCutterData(int lineCutterId) {
  return p->hm_getLineCutterData(lineCutterId);
}
LineCutterFlightVars_s *hm_getLineCutterFlightVariables(int lineCutterId) {
  return p->hm_getLineCutterFlightVariables(lineCutterId);
}
bool hm_lineCutterSendString(int lineCutterNumber, char *string) {
  return p->hm_lineCutterSendString(lineCutterNumber, string);
}
bool hm_lineCuttersSendCut(int chan) { return p->hm_lineCuttersSendCut(chan); }
void hm_watchdogRefresh() { p->hm_watchdogRefresh(); }
void hm_pyroFire(int pyroId, uint32_t duration) {
  p->hm_pyroFire(pyroId, duration);
}
void hm_pyroSet(int pyroId, bool enable) { p->hm_pyroSet(pyroId, enable); }
void hm_pyroSetPwm(int pyroId, uint32_t frequency, uint32_t pulseWidth,
                   uint32_t duration) {
  p->hm_pyroSetPwm(pyroId, frequency, pulseWidth, duration);
}
void hm_pyroUpdate() { p->hm_pyroUpdate(); }
void hm_dcMotorSetPercent(int dcMotorId, double percent) {
  p->hm_dcMotorSetPercent(dcMotorId, percent);
}
SensorData_s *hm_getSensorData() { return p->hm_getSensorData(); }
SensorProperties_s *hm_getSensorProperties() {
  return p->hm_getSensorProperties();
}
void hm_enableSimMode(CircularBuffer_s *rxBuffer) {
  p->hm_enableSimMode(rxBuffer);
}
void hm_disableSimMode() { p->hm_disableSimMode(); }
bool hm_inSimMode() { return p->hm_inSimMode(); }
void hm_observeTickComplete(uint64_t tickNum) {
  p->hm_observeTickComplete(tickNum);
}
}
