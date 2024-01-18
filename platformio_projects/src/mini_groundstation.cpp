/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
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

#define FEATHER_M0_ADC_RESOLUTION 4096

// FCB Code
Groundstation groundstation;

// Drivers
Sx1262 sx1262;

void setup() {
  // Init drivers
  sx1262.init(RFM95_RST, RFM95_CS, RFM95_INT);

  // Add them to device manager
  auto deviceManager = getDeviceManager();
  //  deviceManager->addBatteryMonitorAdc(A7, 2.0 / FEATHER_M0_ADC_RESOLUTION);  // Voltage divider divides by 2
  deviceManager->addRadio(&sx1262);

  // Run FCB code
  groundstation.init();
}

void loop() { groundstation.runOnce(); }