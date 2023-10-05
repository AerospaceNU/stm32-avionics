/**
 * For radio testing 2023-09-24
 */

#include <Arduino.h>
#include <arduino_hardware_manager.h>
#include <groundstation.h>
#include <radio/rfm_950.h>

// Pin definitions
// Radio
#define RFM95_RST 5
#define RFM95_CS 11
#define RFM95_INT 6

#define FEATHER_M0_ADC_RESOLUTION 4096

// FCB Code
Groundstation groundstation;

// Drivers
Rfm950 rfm950;

void setup() {
  // Init drivers
  rfm950.init(RFM95_RST, RFM95_CS, RFM95_INT);

  // Add them to device manager
  auto deviceManager = getDeviceManager();
  //    deviceManager->addBatteryMonitorAdc(A7, 2.0 /
  //    FEATHER_M0_ADC_RESOLUTION);  // Voltage divider divides by 2
  deviceManager->addRadio(&rfm950);

  // Run FCB code
  groundstation.init();
}

void loop() { groundstation.runOnce(); }
