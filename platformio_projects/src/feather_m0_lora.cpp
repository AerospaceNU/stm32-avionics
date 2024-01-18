/**
 * FCB Code to run on an Adafruit Feather M0 LoRa with some sensors attached
 * The actual flight hardware for this should be in the avionics box
 *
 * Originally written to support Big BeanBooster and as a test for the platformio code
 */

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <barometer/mpl115a2_barometer.h>
#include <gps/nmea_0183_gps.h>
#include <imu/bno055_imu.h>
#include <radio/rfm_950.h>
#include <scheduler.h>

// Pin definitions
// Radio
#define RFM95_RST 4
#define RFM95_CS 8
#define RFM95_INT 3

#define FEATHER_M0_ADC_RESOLUTION 4096

// FCB Code
Scheduler s;

// Drivers
Rfm950 rfm950;
Mpl115a2Barometer barometer;
Nmea0183Gps nmeaGps;
Bno055Imu bno055Imu;

void setup() {
  // Init drivers
  rfm950.init(RFM95_RST, RFM95_CS, RFM95_INT);
  nmeaGps.init(&Serial1, 9600);
  barometer.init();
  bno055Imu.init(&Wire, 0x28);

  // Add them to device manager
  auto deviceManager = getDeviceManager();
  deviceManager->addBatteryMonitorAdc(
      A7, 2.0 / FEATHER_M0_ADC_RESOLUTION);  // Voltage divider divides by 2
  deviceManager->addRadio(&rfm950);
  deviceManager->addBarometer(&barometer);
  deviceManager->addGps(&nmeaGps);
  deviceManager->addImu(&bno055Imu, true);

  // Run FCB code
  s.run();
}

void loop() {
  // We should never get here because the FCB code runs forever
  delay(10000);
  Serial.println("Why are we here");
}
