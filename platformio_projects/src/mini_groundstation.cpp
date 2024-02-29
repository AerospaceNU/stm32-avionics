/**
 * Code for the new ESP32 Mini Groundstation boards
 *
 * Does the same thing as the other ground stations (radio -> USB bridge), with devices configured for this specific piece of hardware
 */

// https://github.com/espressif/esptool/issues/836

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <groundstation.h>
#include <radio/sx1262.h>

// Pin definitions
// Radio
#define SX1262_RST 2
#define SX1262_CS 6
#define SX1262_INT 7

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
  // pinMode(RAD_MISO, OUTPUT);
  // pinMode(RAD_MOSI, OUTPUT);
  // pinMode(RAD_SCK, OUTPUT);

  Serial.setTimeout(30000);
  Serial.print("Waiting");
  Serial.readStringUntil('\n');
  Serial.print("Making CS high");

  digitalWrite(SX1262_CS, HIGH);

  SPI.begin(RAD_SCK, RAD_MISO, RAD_MOSI, SX1262_CS);
  // spi.begin(RAD_SCK, RAD_MISO, RAD_MOSI, SX1262_CS);

  delay(100);
  digitalWrite(SX1262_RST, LOW);
  delay(100);
  digitalWrite(SX1262_RST, HIGH);
  delay(100);
}

#define spi SPI
SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);

void loop() {
  Serial.println("Waiting in loop");
  Serial.readStringUntil('\n');

  digitalWrite(SX1262_CS, LOW);
  SPI.beginTransaction(spiSettings);
  Serial.print(spi.transfer(0xC0));
  Serial.print(" ");
  Serial.print(spi.transfer(0), BIN);
  Serial.println();
  SPI.endTransaction();
  digitalWrite(SX1262_CS, HIGH);


  delay(2000);
}
