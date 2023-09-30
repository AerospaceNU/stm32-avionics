/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <gps/nmea_0183_gps.h>
#include <scheduler.h>

// Pin definitions

// FCB Code
Scheduler s;

// Drivers
Nmea0183Gps nmeaGps;

void setup() {
  // Init drivers
  nmeaGps.init(&Serial1, 9600);

  // Add them to device manager
  auto deviceManager = getDeviceManager();
  deviceManager->addGps(&nmeaGps);

  // Run FCB code
  s.run();
}

void loop() {
  // We should never get here because the FCB code runs forever
  delay(1000);
  Serial.println("Why are we here");
}
