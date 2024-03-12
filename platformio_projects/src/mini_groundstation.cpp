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
  // spi.begin(RAD_SCK, RAD_MISO, RAD_MOSI, SX1262_CS);

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
    Serial.print(arr[i]); Serial.print(" ");
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
    Serial.print(arr[i]); Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  Serial.println("Waiting in loop");
  Serial.readStringUntil('\n');

  uint8_t arr[] = {0xc0, 0};
  RadioSpiTransfer(arr, sizeof(arr));

  // wait 1uS then check busy
  delayMicroseconds(1);
  while (digitalRead(SX1262_BUSY)) delayMicroseconds(1);
  Serial.println("Busy low again!");

  delay(2000);
}
