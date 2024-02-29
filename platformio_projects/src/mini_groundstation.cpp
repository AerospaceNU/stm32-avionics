/**
 * Code for the new ESP32 Mini Groundstation boards
 *
 * Does the same thing as the other ground stations (radio -> USB bridge), with devices configured for this specific piece of hardware
 */

// https://github.com/espressif/esptool/issues/836

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <groundstation.h>
#include <radio/custom_SX126x.hpp>
#include <SPI.h>

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

SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);

class ArduinoSx126x : public SX126x {
	uint8_t HalGpioRead(GpioPinFunction_t func) {
    switch (func) {
      case GPIO_PIN_RESET: return digitalRead(SX1262_RST);
      case GPIO_PIN_BUSY: return digitalRead(SX1262_BUSY);
      case GPIO_PIN_DIO1: return digitalRead(SX1262_DIO1);
      case GPIO_PIN_DIO3: return digitalRead(SX1262_DIO1);
      default: return 0;
    }
  }
	void HalGpioWrite(GpioPinFunction_t func, uint8_t value) {
    switch (func) {
      case GPIO_PIN_RESET: digitalWrite(SX1262_RST, value); break;
      case GPIO_PIN_BUSY: digitalWrite(SX1262_BUSY, value); break;
      case GPIO_PIN_DIO1: digitalWrite(SX1262_DIO1, value); break;
      case GPIO_PIN_DIO3: digitalWrite(SX1262_DIO1, value); break;
      default: break;
    }
  }
	void HalSpiTransfer(uint8_t *buffer_in, const uint8_t *buffer_out, uint16_t size) {
    digitalWrite(SX1262_CS, LOW);
    SPI.beginTransaction(spiSettings);
    SPI.transferBytes(buffer_out, buffer_in, size);
    SPI.endTransaction();
    digitalWrite(SX1262_CS, HIGH);
  }
};

ArduinoSx126x radio;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

  Serial.setTimeout(90000);
  Serial.println("Waiting");
  Serial.readStringUntil('\n');
  Serial.print("Continuing!");

  SPI.begin(RAD_SCK, RAD_MISO, RAD_MOSI, SX1262_CS);

  radio.Init();
}

void loop() {
  Serial.println("Waiting in loop");
  Serial.readStringUntil('\n');

	SX126x::ModulationParams_t ModulationParams2;
	ModulationParams2.PacketType = SX126x::PACKET_TYPE_LORA;
	ModulationParams2.Params.LoRa.CodingRate = SX126x::LORA_CR_4_6;
	ModulationParams2.Params.LoRa.Bandwidth = SX126x::LORA_BW_500;
	ModulationParams2.Params.LoRa.SpreadingFactor = SX126x::LORA_SF10;
	ModulationParams2.Params.LoRa.LowDatarateOptimize = 0;

	SX126x::PacketParams_t PacketParams2;
	PacketParams2.PacketType = SX126x::PACKET_TYPE_LORA;
	auto &l = PacketParams2.Params.LoRa;
	l.PayloadLength = 253;
	l.HeaderType = SX126x::LORA_PACKET_FIXED_LENGTH;
	l.PreambleLength = 12;
	l.CrcMode = SX126x::LORA_CRC_ON;
	l.InvertIQ = SX126x::LORA_IQ_NORMAL;

  radio.SetModulationParams(ModulationParams2);
  radio.SetPacketParams(PacketParams2);
  radio.SetBufferBaseAddresses(0, 128);
  Serial.print("Errors on configuration: "); Serial.println(radio.GetDeviceErrors().Value);

  uint8_t testPayload[16];
  for (int i = 0; i < sizeof(testPayload); i++) testPayload[i] = i;
  radio.SetPayload(testPayload, sizeof(testPayload));
  radio.SetTx(0);
  Serial.print("Errors on TX: "); Serial.println(radio.GetDeviceErrors().Value);

  delay(5000);
}
