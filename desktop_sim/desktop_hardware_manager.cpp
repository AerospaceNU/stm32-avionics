//
// Created by matth on 3/25/2022.
//

#include "desktop_hardware_manager.h"

#include <chrono>  // NOLINT
#include <fstream>

#include "circular_buffer.h"
#include "hardware_manager.h"

/* Device includes */

#if HAS_DEV(ACCEL_DESKTOP_FILE) || HAS_DEV(BAROMETER_DESKTOP_FILE) || \
    HAS_DEV(GPS_DESKTOP_FILE) || HAS_DEV(IMU_DESKTOP_FILE) ||         \
    HAS_DEV(PYRO_CONT_DESKTOP_FILE) || HAS_DEV(VBAT_DESKTOP_FILE)
#include "flight_replay.h"
#endif  // HAS_DEV(XXX_DESKTOP_FILE)

#if HAS_DEV(FLASH_DESKTOP_FILE_BACKED)
#include "file_backed_flash.h"
#endif  // HAS_DEV(NUM_FLASH_DESKTOP_FILE_BACKED)

#if HAS_DEV(PYRO_DESKTOP_PRINT)
#include "print_pyro.h"
#endif  // HAS_DEV(PYRO_DESKTOP_PRINT)

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
#include "tcp_socket.h"
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)

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

FileBackedFlash *internalFlash;
bool do_networking;
CircularBuffer_s bleBuffer;
uint8_t bleArray[1024] = {0};

#if HAS_DEV(ACCEL_DESKTOP_FILE) || HAS_DEV(BAROMETER_DESKTOP_FILE) || \
    HAS_DEV(GPS_DESKTOP_FILE) || HAS_DEV(IMU_DESKTOP_FILE) ||         \
    HAS_DEV(PYRO_CONT_DESKTOP_FILE) || HAS_DEV(VBAT_DESKTOP_FILE)
static CsvReplay *flightReplay;
#endif  // HAS_DEV(XXX_DESKTOP_FILE)

#if HAS_DEV(FLASH_DESKTOP_FILE_BACKED)
static FileBackedFlash *externalFlash[NUM_FLASH_DESKTOP_FILE_BACKED];
#endif  // HAS_DEV(FLASH_DESKTOP_FILE_BACKED)

#if HAS_DEV(PYRO_DESKTOP_PRINT)
static PrintPyroCtrl_s printPyro[NUM_PYRO_DESKTOP_PRINT];
#endif  // HAS_DEV(PYRO_DESKTOP_PRINT)

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
static TcpSocket *radioSocket[NUM_RADIO_DESKTOP_SOCKET];
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)

static SensorData_s sensorData = {0};
static SensorProperties_s sensorProperties;

std::string int_flash_path;  // NOLINT
std::string ext_flash_path;  // NOLINT
std::string output_file;     // NOLINT

extern "C" {

void HM_HardwareInit() {
  printf("STARTING: output %s, ext flash %s, int flash %s\n",
         output_file.c_str(), ext_flash_path.c_str(), int_flash_path.c_str());

  internalFlash = new FileBackedFlash(int_flash_path, kFlashSizeBytes[0]);

  // TODO this resets the state log, but we start at the
  // beginning of the CSV anyways, so we shouldn't ever
  // try to resad from it regardless
  internalFlash->Reinit(true);

  // TODO stick in ifdef
  cbInit(&bleBuffer, bleArray, sizeof(bleArray), 1);
  const char* testString = "--linecutter -i 1 -c \"!arm\"\n";
  for(int i = 0; i < strlen(testString); i++) {
    cbEnqueue(&bleBuffer, &testString[i]);
  }

#if HAS_DEV(ACCEL_DESKTOP_FILE) || HAS_DEV(BAROMETER_DESKTOP_FILE) || \
    HAS_DEV(GPS_DESKTOP_FILE) || HAS_DEV(IMU_DESKTOP_FILE) ||         \
    HAS_DEV(PYRO_CONT_DESKTOP_FILE) || HAS_DEV(VBAT_DESKTOP_FILE)
  flightReplay = new CsvReplay(output_file);
#endif  // HAS_DEV(XXX_DESKTOP_FILE)

#if HAS_DEV(FLASH_DESKTOP_FILE_BACKED)
  for (int i = 0; i < NUM_FLASH_DESKTOP_FILE_BACKED; i++) {
    externalFlash[i] = new FileBackedFlash(ext_flash_path, kFlashSizeBytes[i]);
    hardwareStatusFlash[FIRST_ID_FLASH_DESKTOP_FILE_BACKED + i] = true;
  }
#endif  // HAS_DEV(FLASH_DESKTOP_FILE_BACKED)

#if HAS_DEV(PYRO_DESKTOP_PRINT)
  for (int i = 0; i < NUM_PYRO_DESKTOP_PRINT; i++) {
    printPyro_init(&printPyro[i], i);
    hardwareStatusPyro[FIRST_ID_PYRO_DESKTOP_PRINT + i] = true;
  }
#endif  // HAS_DEV(PYRO_DESKTOP_PRINT)

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
  if (do_networking) {
    for (int i = 0; i < NUM_RADIO_DESKTOP_SOCKET; i++) {
      radioSocket[i] = new TcpSocket{radioDesktopSocketPort[i]};
      hardwareStatusRadio[FIRST_ID_RADIO_DESKTOP_SOCKET + i] = true;
    }
  }
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)

#if HAS_DEV(ACCEL)
  // TODO we shouldn't do this, and should instead record this in metadata
  for (int i = 0; i < NUM_ACCEL; i++) {
    sensorProperties.accelFs[i] = 981;  // 100G * 9.81 m/s^2
  }
#endif  // HAS_DEV(ACCEL)

#if HAS_DEV(IMU)
  // TODO we shouldn't do this, and should instead record this in metadata
  for (int i = 0; i < NUM_IMU; i++) {
    sensorProperties.imuAccelFs[i] = 156.96;  // 16 * 9.81
  }
#endif  // HAS_DEV(IMU)
}

uint32_t HM_Millis() {
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return ms.count();
}

bool HM_FlashReadStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                       uint8_t *pData) {
#if HAS_DEV(FLASH_DESKTOP_FILE_BACKED)
  if (IS_DEVICE(flashId, FLASH_DESKTOP_FILE_BACKED)) {
    return externalFlash[flashId - FIRST_ID_FLASH_DESKTOP_FILE_BACKED]
        ->ReadStart(startLoc, numBytes, pData);
  }
#endif  // HAS_DEV(FLASH_DESKTOP_FILE_BACKED)

  return false;
}

bool HM_FlashWriteStart(int flashId, uint32_t startLoc, uint32_t numBytes,
                        uint8_t *data) {
#if HAS_DEV(FLASH_DESKTOP_FILE_BACKED)
  if (IS_DEVICE(flashId, FLASH_DESKTOP_FILE_BACKED)) {
    return externalFlash[flashId - FIRST_ID_FLASH_DESKTOP_FILE_BACKED]
        ->WriteStart(startLoc, numBytes, data);
  }
#endif  // HAS_DEV(FLASH_DESKTOP_FILE_BACKED)

  return false;
}

bool HM_FlashEraseSectorStart(int flashId, uint32_t sectorNum) { return false; }
bool HM_FlashEraseChipStart(int flashId) { return false; }
bool HM_FlashIsReadComplete(int flashId) { return true; }
bool HM_FlashIsWriteComplete(int flashId) { return true; }
bool HM_FlashIsEraseComplete(int flashId) { return true; }

void HM_BuzzerSetFrequency(int buzzerId, float fHz) {}
void HM_BuzzerStart(int buzzerId) {}
void HM_BuzzerStop(int buzzerId) {}

void HM_ServoSetAngle(int servoId, float degrees) {}

void HM_LedSet(int ledId, bool set) {}
void HM_LedToggle(int ledId) {}

bool HM_RadioSend(int radioNum, uint8_t *data, uint16_t numBytes) {
  static RadioRecievedPacket_s packet;
  packet.crc = true;
  packet.lqi = 4;
  packet.rssi = 10;
  memcpy(packet.data, data, numBytes);

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
  if (IS_DEVICE(radioNum, RADIO_DESKTOP_SOCKET)) {
    packet.radioId = radioNum;
    radioSocket[radioNum]->writeData((uint8_t *)&packet, sizeof(packet));
    return true;
  }
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)

  return false;
}

void HM_RadioUpdate() {}
void HM_RadioRegisterConsumer(int radioNum, CircularBuffer_s *rxBuffer) {}
void HM_RadioSetChannel(int radioNum, int channel) {}

bool HM_UsbIsConnected(int usbId) { return false; }
bool HM_UsbTransmit(int usbId, uint8_t *data, uint16_t numBytes) {
  std::cout << data << std::endl;
  return true;
}
CircularBuffer_s *HM_UsbGetRxBuffer(int usbId) { return nullptr; }

bool HM_BleClientConnected(int bleClientId) { return false; }
bool HM_BleClientSend(int bleClientId, const uint8_t *data, uint16_t numBytes) {
  return false;
}

CircularBuffer_s *HM_BleClientGetRxBuffer(int bleClientId) { 
  return &bleBuffer;
}
void HM_BleTick() {}

LineCutterData_s *HM_GetLineCutterData(int lineCutterId) { return nullptr; }
LineCutterFlightVars_s *HM_GetLineCutterFlightVariables(int lineCutterId) {
  return nullptr;
}
bool HM_LineCutterSendString(int lineCutterNumber, char *string) {
  printf("[Sent to LC %i] %s\n", lineCutterNumber, string);
  return true;
}
bool HM_LineCuttersSendCut(int chan) {
  printf("Line cutter chan %i cut sent!\n", chan);
  return true;
}

void HM_WatchdogRefresh() {}

void HM_PyroFire(int pyroId, uint32_t duration) {
#if HAS_DEV(PYRO_DESKTOP_PRINT)
  if (IS_DEVICE(pyroId, PYRO_DESKTOP_PRINT)) {
    printPyro_start(&printPyro[pyroId - FIRST_ID_PYRO_DESKTOP_PRINT], duration);
  }
#endif  // HAS_DEV(PYRO_DESKTOP_PRINT)
}

void HM_PyroSet(int pyroId, bool enable) {
#if HAS_DEV(PYRO_DESKTOP_PRINT)
  if (IS_DEVICE(pyroId, PYRO_DESKTOP_PRINT)) {
    printPyro_set(&printPyro[pyroId - FIRST_ID_PYRO_DESKTOP_PRINT], enable);
  }
#endif  // HAS_DEV(PYRO_DESKTOP_PRINT)
}

void HM_PyroUpdate() {
#if HAS_DEV(PYRO_DESKTOP_PRINT)
  for (int i = 0; i < NUM_PYRO_DESKTOP_PRINT; i++) {
    printPyro_tick(&printPyro[i]);
  }
#endif  // HAS_DEV(PYRO_DESKTOP_PRINT)
}

void HM_DCMotorSetPercent(int dcMotorId, double percent) {}

void HM_ReadSensorData() { flightReplay->GetNext(&sensorData); }

SensorData_s *HM_GetSensorData() { return &sensorData; }

SensorProperties_s *HM_GetSensorProperties() { return &sensorProperties; }

void HM_EnableSimMode(CircularBuffer_s *rxBuffer) {}

void HM_DisableSimMode() {}

bool HM_InSimMode() { return false; }
}  // extern "C"
