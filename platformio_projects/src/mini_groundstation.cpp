/**
 * Code for the new ESP32 Mini Groundstation boards
 *
 * Does the same thing as the other ground stations (radio -> USB bridge), with devices configured for this specific piece of hardware
 */

// https://github.com/espressif/esptool/issues/836

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <groundstation.h>
#include <SPI.h>
#include "radio_defines.h"

// Pin definitions
// Radio
#define SX1262_RST 2
#define SX1262_CS 6
#define SX1262_INT 7
#define SX1262_BUSY 5
#define SX1262_DIO1 7
#define SX1262_DIO3 10
#define RAD_MISO 3
#define RAD_MOSI 4
#define RAD_SCK 8

// FCB Code
// Groundstation groundstation;

// Drivers
// Sx1262 sx1262;

// static SPIClass spi(HSPI);

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

  pinMode(SX1262_RST, OUTPUT);
  pinMode(SX1262_CS, OUTPUT);
  pinMode(SX1262_BUSY, INPUT);
  pinMode(SX1262_DIO1, INPUT);
  pinMode(SX1262_DIO3, INPUT);

  Serial.setTimeout(30000);
  Serial.print("Waiting");
  Serial.readStringUntil('\n');
  Serial.print("Making CS high");

  digitalWrite(SX1262_CS, HIGH);

  SPI.begin(RAD_SCK, RAD_MISO, RAD_MOSI, SX1262_CS);

  delay(100);
  digitalWrite(SX1262_RST, LOW);
  delay(100);
  digitalWrite(SX1262_RST, HIGH);
  delay(100);
}

#define spi SPI
SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);

void RadioSpiTransfer(uint8_t *arr, size_t len) {
  Serial.println("Output arr:");
  for (int i = 0; i < len; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.println();

  digitalWrite(SX1262_CS, LOW);
  SPI.beginTransaction(spiSettings);
  for (auto i = 0; i < len; i++) {
    arr[i] = SPI.transfer(arr[i]);
  }
  SPI.endTransaction();
  digitalWrite(SX1262_CS, HIGH);

  Serial.println("Got back:");
  for (int i = 0; i < len; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void RadioSendCmd(uint8_t cmd, uint8_t data) {
  uint8_t arr[] = {cmd, data};
  RadioSpiTransfer(arr, sizeof(arr));

  // wait 1uS then check busy
  delayMicroseconds(1);
  while (digitalRead(SX1262_BUSY)) delayMicroseconds(1);
  Serial.println("Busy low again!");
}

void RadioSendCmd(uint8_t cmd, uint8_t *data, size_t len) {
  uint8_t arr[len + 1];
  arr[0] = cmd;
  for (int i = 0; i < len; i++) {
    arr[i + 1] = data[i];
  }
  RadioSpiTransfer(arr, sizeof(arr));

  // wait 1uS then check busy
  delayMicroseconds(1);
  while (digitalRead(SX1262_BUSY)) delayMicroseconds(1);
  Serial.println("Busy low again!");
}

void loop() {
  Serial.println("Waiting in loop");
  Serial.readStringUntil('\n');

  // read status
  RadioSendCmd(0xc0, 0);

  // Set into standby_rc
  RadioSendCmd(0x80, 0);

  // Set into GFSK mode
  RadioSendCmd(0x8a, 0);

  // use dio2 as rf switch ctrl
  RadioSendCmd(0x9d, 1);

  // set frequency to 915MHz
  double freqVal = 915000000;
  uint32_t freq = freqVal / FREQ_STEP;
  uint8_t freqRegBuf[4];
  freqRegBuf[0] = (uint8_t)((freq >> 24) & 0xFF);
  freqRegBuf[1] = (uint8_t)((freq >> 16) & 0xFF);
  freqRegBuf[2] = (uint8_t)((freq >> 8) & 0xFF);
  freqRegBuf[3] = (uint8_t)(freq & 0xFF);
  RadioSendCmd(0x86, freqRegBuf, sizeof(freqRegBuf));

  // set TX ramp to 10us, tx power 14
  uint8_t txParamBuf[2] = {10, 0x00};
  RadioSendCmd(0x8e, txParamBuf, sizeof(txParamBuf));

  // set base address to 0/0
  uint8_t buffAddrBuf[2] = {0, 0};
  RadioSendCmd(0x8f, buffAddrBuf, sizeof(buffAddrBuf));

  // Fill fifo 0-32 with 0-32
  uint8_t txBuf[32 + 1];
  txBuf[1] = 0;
  for (int i = 0; i < sizeof(txBuf); i++) txBuf[i + 1] = i;
  RadioSendCmd(RADIO_WRITE_BUFFER, txBuf, sizeof(txBuf));

  // set modulation params
  {
    ModulationParams_t modulationParams = {
        .BitRate = 38400,
        .Fdev = 20000,
        .ModulationShaping = MOD_SHAPING_G_BT_05,
        .Bandwidth = RX_BW_156200,
    };

    uint8_t n;
    uint32_t tempVal = 0;
    uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    n = 8;
    tempVal = (uint32_t)(32 * ((double)XTAL_FREQ / (double)modulationParams.BitRate));
    buf[0] = (tempVal >> 16) & 0xFF;
    buf[1] = (tempVal >> 8) & 0xFF;
    buf[2] = tempVal & 0xFF;
    buf[3] = modulationParams.ModulationShaping;
    buf[4] = modulationParams.Bandwidth;
    tempVal = (uint32_t)((double)modulationParams.Fdev / (double)FREQ_STEP);
    buf[5] = (tempVal >> 16) & 0xFF;
    buf[6] = (tempVal >> 8) & 0xFF;
    buf[7] = (tempVal & 0xFF);

    RadioSendCmd(RADIO_SET_MODULATIONPARAMS, buf, n);
  }

  // and set packet params
  {
    PacketParams_t packetParams {
      .PreambleLength = 3,
      .PreambleMinDetect = RADIO_PREAMBLE_DETECTOR_08_BITS,
      .SyncWordLength = 4,
      .AddrComp = RADIO_ADDRESSCOMP_FILT_OFF,
      .HeaderType = RADIO_PACKET_FIXED_LENGTH,
      .PayloadLength = sizeof(txBuf) - 1,
      .CrcLength = RADIO_CRC_OFF,
      .DcFree = RADIO_DC_FREE_OFF
    };

    uint8_t n;
    uint8_t crcVal = 0;
    uint8_t buf[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    crcVal = packetParams.CrcLength;
    n = 9;
    buf[0] = (packetParams.PreambleLength >> 8) & 0xFF;
    buf[1] = packetParams.PreambleLength;
    buf[2] = packetParams.PreambleMinDetect;
    buf[3] = (packetParams.SyncWordLength << 3);  // convert from byte to bit
    buf[4] = packetParams.AddrComp;
    buf[5] = packetParams.HeaderType;
    buf[6] = packetParams.PayloadLength;
    buf[7] = crcVal;
    buf[8] = packetParams.DcFree;

    RadioSendCmd(RADIO_SET_PACKETPARAMS, buf, n);
  }

  // set sync word
	uint8_t sync[8] = { 0x93, 0x0b, 0x51, 0xde };
  uint8_t setSyncArr[sizeof(sync) + 3];
  setSyncArr[0] = RADIO_WRITE_REGISTER;
  setSyncArr[1] = (0x06C0 >> 8) & 0xff;
  setSyncArr[2] = (0x06C0) & 0xff;
  memcpy(setSyncArr + 3, sync, sizeof(sync));
  RadioSpiTransfer(setSyncArr, sizeof(setSyncArr));

  // And set tx mode, no timeout
  uint8_t setTxArr[] = {0x83, 0, 0, 0};
  RadioSpiTransfer(setTxArr, sizeof(setTxArr));

  delay(2000);
}
