//
// Created by matth on 3/25/2022.
//

#include <hardware_manager.h>

#include <chrono>

#include "board_config.h"
#include "file_backed_flash.h"
#include "flight_replay.h"
#include "fstream"
#include "hardware_manager.h"
#include "radio_manager.h"
#include "sensor_types.h"
#include "tcp_socket.h"

FileBackedFlash *externalFlash;
FileBackedFlash *internalFlash;
static CsvReplay *flightReplay;

static SensorData_t sensorData = {0};
static SensorProperties_t sensorProperties;

static TcpSocket *radioSocket;

std::string int_flash_path;
std::string ext_flash_path;
std::string output_file;
bool do_networking = true;

extern "C" {

bool hardwareStatus[NUM_HARDWARE] = {true, true, true, true, true, true, true};

#include "stdint.h"
#define GPIO_TypeDef uint32_t
#include "pyro.h"
uint32_t zero = 0;
PyroCtrl_t pyroCtrl[MAX_PYRO];

void HM_HardwareInit() {
  printf("STARTING: output %s, ext flash %s, int flash %s\n",
         output_file.c_str(), ext_flash_path.c_str(), int_flash_path.c_str());
  externalFlash = new FileBackedFlash(ext_flash_path, FLASH_SIZE_BYTES);
  internalFlash = new FileBackedFlash(int_flash_path, FLASH_SIZE_BYTES);
  flightReplay = new CsvReplay(output_file);
  
  if (do_networking) {
    radioSocket = new TcpSocket{8080};
  }

  // TODO this resets the state log, but we start at the
  // beginning of the CSV anyways, so we shouldn't ever
  // try to resad from it regardless
  internalFlash->Reinit(true);

  pyroCtrl[0].port = &zero;
  pyroCtrl[0].pin = 0;
  pyroCtrl[1].port = &zero;
  pyroCtrl[1].pin = 1;
  Pyro_init(&pyroCtrl[0]);
  Pyro_init(&pyroCtrl[1]);

  // Fullscale of 16 gees, hardcoded for now
  // TODO we shouldn't do this, and should instead record this in metadata
  sensorProperties.imu1_accel_fs = 16 * 9.8;
  sensorProperties.imu2_accel_fs = 156.96;
}

bool HM_RadioSend(Hardware_t radio, uint8_t *data, uint16_t numBytes) {
  static RecievedPacket_t packet;
  packet.crc = true;
  packet.lqi = 4;
  packet.radioId = radio;
  packet.rssi = 10;

  memcpy(packet.data, data, numBytes);

  if (radioSocket) {
    radioSocket->writeData((uint8_t *)&packet, sizeof(packet));
  }

  return true;
}

void HM_PyroFire(int channel, uint32_t duration) {
  Pyro_start(&pyroCtrl[channel], duration);
}

void HM_PyroUpdate() {
  for (int i = 0; i < MAX_PYRO; i++) {
    Pyro_tick(&pyroCtrl[i]);
  }
}

void HM_RadioRegisterConsumer(Hardware_t radio, CircularBuffer_t *rxBuffer) {}
void HM_RadioSetChannel(Hardware_t radio, int channel) {}
bool HM_UsbTransmit(uint8_t *data, uint16_t numBytes) {
  std::cout << data << std::endl;

  return true;
}

bool HM_BluetoothSend(uint8_t address, const uint8_t *data, uint16_t numBytes) {
  return true;
}

void HM_BluetoothTick() {

}

bool HM_LineCuttersSendCut(int chan) {
  printf("Line cutter chan %i cut sent!\n", chan);
  return true;
}

bool HM_LineCutterSendString(int id, char *string) {
  return true;
}

CircularBuffer_t *HM_UsbGetRxBuffer() { return 0; }

uint32_t HM_Millis() {
  using namespace std::chrono;
  milliseconds ms =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  return ms.count();
}

bool HM_FlashReadStart(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
  return externalFlash->ReadStart(startLoc, numBytes, pData);
}
bool HM_FlashWriteStart(uint32_t startLoc, uint32_t numBytes, uint8_t *data) {
  return externalFlash->WriteStart(startLoc, numBytes, data);
}

bool HM_FlashEraseSectorStart(uint32_t sectorNum) { return false; }

bool HM_FlashEraseChipStart() { return false; }

bool HM_FlashIsReadComplete() { return true; }

bool HM_FlashIsWriteComplete() { return true; }

bool HM_FlashIsEraseComplete() { return true; }

void HM_BuzzerSetFrequency(float fHz) {}

void HM_BuzzerStart() {}

void HM_BuzzerStop() {}

void HM_ServoSetAngle(int servoNum, float degrees) {}

void HM_LedSet(int ledNum, bool set) {}

void HM_LedToggle(int ledNum) {
  // std::cout << "LED" << std::endl;
}

void HM_RadioUpdate() {}

bool HM_UsbIsConnected() { return false; }

void HM_SetImu1Sampling(bool enable) {}

void HM_SetImu2Sampling(bool enable) {}

void HM_SetHighGSampling(bool enable) {}

void HM_SetBaro1Sampling(bool enable) {}

void HM_SetBaro2Sampling(_Bool enable) {}

void HM_SetGpsSampling(bool enable) {}

void HM_SetBatteryVoltageSampling(bool enable) {}

void HM_SetPyroContinuitySampling(bool enable) {}

void HM_IWDG_Refresh() {}

static void HM_SimReadSensorData() {}

#include "data_log.h"

void HM_ReadSensorData() { flightReplay->GetNext(&sensorData); }

SensorProperties_t *HM_GetSensorProperties() { return &sensorProperties; }

bool *HM_GetHardwareStatus() { return hardwareStatus; }

void HM_EnableSimMode(CircularBuffer_t *rxBuffer) {}

void HM_DisableSimMode() {}

SensorData_t *HM_GetSensorData() { return &sensorData; }

bool HM_InSimMode() { return false; }

}