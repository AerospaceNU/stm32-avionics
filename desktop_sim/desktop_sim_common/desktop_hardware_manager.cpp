//
// Created by matth on 3/25/2022.
//

#include "desktop_hardware_manager.h"

#include <memory>

#include "hardware_manager_interface.h"

// std::unique_ptr<HardwareManagerInterface> m_hmInterface;

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