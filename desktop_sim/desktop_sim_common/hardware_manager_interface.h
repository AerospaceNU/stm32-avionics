#pragma once

#include <stdint.h>

#include "circular_buffer.h"
#include "hardware_manager.h"

class HardwareManagerInterface {
public:
  HardwareManagerInterface() = default;

  virtual inline void hm_hardwareInit() {};
  virtual uint32_t hm_millis() { return 0; }
  virtual inline bool hm_flashReadStart(int flashId, uint32_t startLoc,
                                        uint32_t numBytes, uint8_t *pData) {
    return false;
  }
  virtual inline bool hm_flashWriteStart(int flashId, uint32_t startLoc,
                                         uint32_t numBytes, uint8_t *data) {
    return false;
  }
  virtual inline bool hm_flashEraseSectorStart(int flashId,
                                               uint32_t sectorNum) {
    return false;
  }
  virtual inline bool hm_flashEraseChipStart(int flashId) { return false; }
  virtual inline bool hm_flashIsReadComplete(int flashId) { return true; }
  virtual inline bool hm_flashIsWriteComplete(int flashId) { return true; }
  virtual inline bool hm_flashIsEraseComplete(int flashId) { return true; }
  virtual inline void hm_buzzerSetFrequency(int buzzerId, float fHz) {}
  virtual inline void hm_buzzerStart(int buzzerId) {}
  virtual inline void hm_buzzerStop(int buzzerId) {}
  virtual inline void hm_servoSetAngle(int servoId, float degrees) {}
  virtual inline void hm_ledSet(int ledId, bool set) {}
  virtual inline void hm_ledToggle(int ledId) {}
  virtual inline void hm_readSensorData() {}
  virtual inline bool hm_radioSend(int radioNum, uint8_t *data,
                                   uint16_t numBytes) {
    return false;
  }
  virtual inline void hm_radioUpdate() {}
  virtual inline void hm_radioRegisterConsumer(int radioNum,
                                               CircularBuffer_s *rxBuffer){};
  virtual inline void hm_radioSetChannel(int radioNum, int channel) {}
  virtual inline bool hm_usbIsConnected(int usbId) { return false; }
  virtual inline bool hm_usbTransmit(int usbId, uint8_t *data,
                                     uint16_t numBytes) {
    return false;
  }
  virtual inline CircularBuffer_s *hm_usbGetRxBuffer(int usbId) { return 0; }
  virtual inline bool hm_bleClientConnected(int bleClientId) { return false; }
  virtual inline bool hm_bleClientSend(int bleClientId, const uint8_t *data,
                                       uint16_t numBytes) {
    return false;
  }
  virtual inline CircularBuffer_s *hm_bleClientGetRxBuffer(int bleClientId) { return 0; }
    
  virtual inline void hm_bleTick() {}
  virtual inline LineCutterData_s *hm_getLineCutterData(int lineCutterId) { return 0; }
  virtual inline LineCutterFlightVars_s *hm_getLineCutterFlightVariables(
      int lineCutterId) {
    return 0;
  }
  virtual inline bool hm_lineCutterSendString(int lineCutterNumber,
                                              char *string) {
    return true;
  }
  virtual inline bool hm_lineCuttersSendCut(int chan) {
    return true;
  }
  virtual inline void hm_watchdogRefresh() {}
  virtual inline void hm_pyroFire(int pyroId, uint32_t duration) {}
  virtual inline void hm_pyroSet(int pyroId, bool enable) {}
  virtual inline void hm_pyroSetPwm(int pyroId, uint32_t frequency,
                                    uint32_t pulseWidth, uint32_t duration) {}
  virtual inline void hm_pyroUpdate() {}
  virtual inline void hm_dcMotorSetPercent(int dcMotorId, double percent) {}
  virtual inline SensorData_s *hm_getSensorData() { return 0; }
  virtual inline SensorProperties_s *hm_getSensorProperties() { return 0; }
  virtual inline void hm_enableSimMode(CircularBuffer_s *rxBuffer) {}
  virtual inline void hm_disableSimMode() {}
  virtual inline bool hm_inSimMode() { return false; }
  virtual inline void hm_observeTickComplete(uint64_t tickNum) {}
};
