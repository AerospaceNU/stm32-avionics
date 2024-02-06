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

// FCB Code
// Groundstation groundstation;

// Drivers
// Sx1262 sx1262;

static SPIClass spi(HSPI);

void setup() {
  Serial.begin(115200);

  spi.begin(8, 3, 4, SX1262_CS);

  //  Serial.begin(115200);

  // Init drivers
  //  sx1262.init(SX1262_RST, SX1262_CS, SX1262_INT);

  // Add them to device manager
  //  auto deviceManager = getDeviceManager();
  //  deviceManager->addRadio(&sx1262);

  // Run FCB code
  //  groundstation.init();

  pinMode(SX1262_RST, OUTPUT);
  pinMode(SX1262_CS, OUTPUT);

  digitalWrite(SX1262_CS, HIGH);

  delay(100);
  digitalWrite(SX1262_RST, LOW);
  delay(100);
  digitalWrite(SX1262_RST, HIGH);
  delay(100);
}

SPISettings spiSettings(100000, MSBFIRST, SPI_MODE0);

void loop() {
  //  groundstation.runOnce();

  int size = 3;
  uint8_t data[size];
  memset(data, 0, size);
  data[0] = 0xC0;

  spi.beginTransaction(spiSettings);

  digitalWrite(SX1262_CS, LOW);
  Serial.print(spi.transfer(0xC0));
  Serial.print(" ");
  Serial.print(spi.transfer(0));
  Serial.print(" ");
  Serial.print(spi.transfer(0));
  Serial.println();
  digitalWrite(SX1262_CS, HIGH);

  spi.endTransaction();

  //  Serial.print(data[0]);
  //  Serial.print(" ");
  //  Serial.print(data[1]);
  //  Serial.print(" ");
  //  Serial.println(data[2]);

  delay(1000);
}
