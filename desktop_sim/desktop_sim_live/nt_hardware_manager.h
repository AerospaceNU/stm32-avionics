#pragma once

#include "board_config.h"
#include "circular_buffer.h"
#include "data_structures.h"
#include "file_backed_flash.h"
#include "flight_replay.h"
#include "hardware_manager.h"
#include "hardware_manager_interface.h"
#include "nt_pyro.h"
#include "print_pyro.h"
#include "sim_timing.h"
#if HAS_DEV(NT_INTERFACE)
#include "nt_interface.h"
#endif  // HAS_DEV(NT_INTERFACE)

class NtHardwareManager : public HardwareManagerInterface {
 public:
  inline NtHardwareManager(std::string_view flightCsv, std::string_view metaCSV,
                           std::string_view intFlashPath,
                           std::string_view extFlashPath)
      : HardwareManagerInterface(),
        int_flash_path(intFlashPath),
        ext_flash_path(extFlashPath),
        output_file(flightCsv) {}

 private:
  std::string int_flash_path;  // NOLINT
  std::string ext_flash_path;  // NOLINT
  std::string output_file;     // NOLINT

  FileBackedFlash *internalFlash;
  bool do_networking;
  CircularBuffer_s bleBuffer;
  uint8_t bleArray[1024] = {0};

  CircularBuffer_s usbBuffer;
  uint8_t usbArray[1024] = {0};

#if HAS_DEV(ACCEL_DESKTOP_FILE) || HAS_DEV(BAROMETER_DESKTOP_FILE) || \
    HAS_DEV(GPS_DESKTOP_FILE) || HAS_DEV(IMU_DESKTOP_FILE) ||         \
    HAS_DEV(PYRO_CONT_DESKTOP_FILE) || HAS_DEV(VBAT_DESKTOP_FILE)
  CsvReplay *flightReplay;
#endif  // HAS_DEV(XXX_DESKTOP_FILE)

#if HAS_DEV(FLASH_DESKTOP_FILE_BACKED)
  FileBackedFlash *externalFlash[NUM_FLASH_DESKTOP_FILE_BACKED];
#endif  // HAS_DEV(FLASH_DESKTOP_FILE_BACKED)

#if HAS_DEV(PYRO_DESKTOP_PRINT)
  PrintPyroCtrl_s printPyro[NUM_PYRO_DESKTOP_PRINT];
#endif  // HAS_DEV(PYRO_DESKTOP_PRINT)

#if HAS_DEV(PYRO_DESKTOP_NT)
  NtPyroCtrl_s ntPyro[NUM_PYRO_DESKTOP_NT];
#endif  // HAS_DEV(NUM_PYRO_DESKTOP_NT)

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
  TcpSocket *radioSocket[NUM_RADIO_DESKTOP_SOCKET];
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)

#if HAS_DEV(NT_INTERFACE)
  RocketNTInterface ntInterface;
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)

  SensorData_s sensorData = {0};
  SensorProperties_s sensorProperties;

 public:
  void hm_hardwareInit() override;
  uint32_t hm_millis() override;
  bool hm_flashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                         uint8_t *pData) override;
  bool hm_flashWriteStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                          uint8_t *data) override;
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
  bool hm_radioSend(int radioNum, uint8_t *data, uint16_t numBytes) override;
  void hm_radioUpdate() override;
  void hm_radioRegisterConsumer(int radioNum,
                                CircularBuffer_s *rxBuffer) override;
  inline void hm_radioSetChannel(int radioNum, int channel) override {}
  bool hm_usbIsConnected(int usbId) override;
  bool hm_usbTransmit(int usbId, uint8_t *data, uint16_t numBytes) override;
  CircularBuffer_s *hm_usbGetRxBuffer(int usbId) override;
  bool hm_bleClientConnected(int bleClientId) override;
  bool hm_bleClientSend(int bleClientId, const uint8_t *data,
                        uint16_t numBytes) override;
  CircularBuffer_s *hm_bleClientGetRxBuffer(int bleClientId) override;
  inline void hm_bleTick() override {}
  LineCutterData_s *hm_getLineCutterData(int lineCutterId) override;
  LineCutterFlightVars_s *hm_getLineCutterFlightVariables(
      int lineCutterId) override;
  bool hm_lineCutterSendString(int lineCutterNumber, char *string) override;

  bool hm_lineCuttersSendCut(int chan) override;

  inline void hm_watchdogRefresh() override {}

  void hm_pyroFire(int pyroId, uint32_t duration) override;
  void hm_pyroSet(int pyroId, bool enable) override;
  void hm_pyroSetPwm(int pyroId, uint32_t frequency, uint32_t pulseWidth,
                     uint32_t duration) override;
  void hm_pyroUpdate() override;
  inline void hm_dcMotorSetPercent(int dcMotorId, double percent) override {}
  SensorData_s *hm_getSensorData() override;
  SensorProperties_s *hm_getSensorProperties() override;
  inline void hm_enableSimMode(CircularBuffer_s *rxBuffer) override {}
  inline void hm_disableSimMode() override {}
  inline bool hm_inSimMode() override { return false; }
  void hm_observeTickComplete(uint64_t tickNum) override;
};
