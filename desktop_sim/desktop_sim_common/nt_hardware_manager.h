#include "hardware_manager_interface.h"
#include "data_structures.h"

class NtHardwareManager : public HardwareManagerInterface {
  void hm_hardwareInit() override;
  uint32_t hm_millis() override;
  bool hm_flashReadStart(int flashId, uint32_t startLoc,
                                        uint32_t numBytes, uint8_t *pData) override;
  bool hm_flashWriteStart(int flashId, uint32_t startLoc,
                                         uint32_t numBytes, uint8_t *data) override;
  inline bool hm_flashEraseSectorStart(int flashId,
                                               uint32_t sectorNum) override {
    return false;
  }
  inline bool hm_flashEraseChipStart(int flashId) override { return false; }
  inline bool hm_flashIsReadComplete(int flashId) override { return true; }
  inline bool hm_flashIsWriteComplete(int flashId) override { return true; }
  inline bool hm_flashIsEraseComplete(int flashId) override { return true; }
  inline void hm_buzzerSetFrequency(int buzzerId, float fHz) override {}
  inline void hm_buzzerStart(int buzzerId) override {}
  inline void hm_buzzerStop(int buzzerId) override {}
  inline void hm_servoSetAngle(int servoId, float degrees) override {}
  inline void hm_ledSet(int ledId, bool set) override {}
  inline void hm_ledToggle(int ledId) override {}
  void hm_readSensorData() override;
  bool hm_radioSend(int radioNum, uint8_t *data,
                                   uint16_t numBytes) override;
  void hm_radioUpdate() override;
  void hm_radioRegisterConsumer(int radioNum,
                                               CircularBuffer_s *rxBuffer) override;
  inline void hm_radioSetChannel(int radioNum, int channel) override {}
  bool hm_usbIsConnected(int usbId) override;
  bool hm_usbTransmit(int usbId, uint8_t *data,
                                     uint16_t numBytes) override;
  CircularBuffer_s *hm_usbGetRxBuffer(int usbId) override;
  bool hm_bleClientConnected(int bleClientId) override;
  bool hm_bleClientSend(int bleClientId, const uint8_t *data,
                                       uint16_t numBytes) override;
  CircularBuffer_s *hm_bleClientGetRxBuffer(int bleClientId) override;
  inline void hm_bleTick() override {}
  LineCutterData_s *hm_getLineCutterData(int lineCutterId) override { return 0; }
  LineCutterFlightVars_s *hm_getLineCutterFlightVariables(
      int lineCutterId) override {
    return 0;
  }
  bool hm_lineCutterSendString(int lineCutterNumber,
                                              char *string) override;
    
  bool hm_lineCuttersSendCut(int chan) override;
    
  inline void hm_watchdogRefresh() override {}

  void hm_pyroFire(int pyroId, uint32_t duration) override;
  void hm_pyroSet(int pyroId, bool enable) override;
  void hm_pyroSetPwm(int pyroId, uint32_t frequency,
                                    uint32_t pulseWidth, uint32_t duration) override;
  void hm_pyroUpdate() override;
  inline void hm_dcMotorSetPercent(int dcMotorId, double percent) override {}
  SensorData_s *hm_getSensorData() override;
  SensorProperties_s *hm_getSensorProperties() override;
  inline void hm_enableSimMode(CircularBuffer_s *rxBuffer) override {}
  inline void hm_disableSimMode() override {}
  inline bool hm_inSimMode() override { return false; }
  void hm_delay(int ms) override;
  void hm_observeTickComplete(uint64_t tickNum) override;
};
