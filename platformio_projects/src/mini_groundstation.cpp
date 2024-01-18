/**
 * Code for the new ESP32 Mini Groundstation boards
 *
 * Does the same thing as the other ground stations (radio -> USB bridge), with devices configured for this specific piece of hardware
 */

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
Groundstation groundstation;

// Drivers
Sx1262 sx1262;

void setup() {
  // Init drivers
  sx1262.init(SX1262_RST, SX1262_CS, SX1262_INT);

  // Add them to device manager
  auto deviceManager = getDeviceManager();
  deviceManager->addRadio(&sx1262);

  // Run FCB code
  groundstation.init();
}

void loop() { groundstation.runOnce(); }