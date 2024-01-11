/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <groundstation.h>
#include <radio/rfm_950.h>

// Pin definitions
// Radio
#define RFM95_RST 2
#define RFM95_CS 6
#define RFM95_INT 7

// FCB Code
Groundstation groundstation;

// Drivers
Rfm950 rfm950;

void setup() {
  // Init drivers
  rfm950.init(RFM95_RST, RFM95_CS, RFM95_INT);

  // Add them to device manager
  auto deviceManager = getDeviceManager();
  deviceManager->addRadio(&rfm950);

  // Run FCB code
  groundstation.init();
}

void loop() { groundstation.runOnce(); }
