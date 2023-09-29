/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <scheduler.h>

// Pin definitions

// FCB Code
Scheduler s;

// Drivers

void setup() {
  // Init drivers


  // Add them to device manager
  auto deviceManager = getDeviceManager();

  // Run FCB code
  s.run();
}

void loop() {
  // We should never get here because the FCB code runs forever
  delay(10000);
  Serial.println("Why are we here");
}
