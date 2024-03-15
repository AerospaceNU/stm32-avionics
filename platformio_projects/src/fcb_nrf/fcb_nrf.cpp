#include <bluefruit.h>

BLEDis bledis;
BLEUart bleuart;

#define STATUS_LED (17)
#define BLINKY_MS (2000)

uint32_t blinkyms;

void setupAdv(void);

void setup() {
  // Setup LED pins and reset blinky counter
  pinMode(STATUS_LED, OUTPUT);
  blinkyms = millis();

  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this manually via PIN 19
  Bluefruit.autoConnLed(true);

  Bluefruit.begin(1, 0);
  Bluefruit.setName("FCB NRF idk");

  // Configure and Start Device Information Service
  bledis.setManufacturer("AeroNU");
  bledis.setModel("FCB V2.2");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Set up Advertising Packet
  setupAdv();

  // Start Advertising
  Bluefruit.Advertising.start();

  // might be backwards
  Serial1.setPins(11, 12);
  // might be wrong
  Serial1.begin(115200);

  Serial1.setTimeout(10);
}

void setupAdv(void) {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // There is no room for Name in Advertising packet
  // Use Scan response for Name
  Bluefruit.ScanResponse.addName();
}

struct UartFrame {
  uint8_t client;
  uint16_t length;
  uint8_t payload[256];
};

void loop() {
  // Blinky!
  if (blinkyms + BLINKY_MS < millis()) {
    blinkyms = millis();
    digitalToggle(STATUS_LED);
  }

  // Forward from BLEUART to Serial
  if (bleuart.available()) {
    // assume reading until newline
    auto str = bleuart.readStringUntil('\n');

    UartFrame frame;
    frame.client = 1;
    frame.length = str.length();
    memcpy(frame.payload, str.c_str(), str.length());

    Serial1.write(reinterpret_cast<uint8_t*>(&frame),
                  frame.length + sizeof(frame.client) + sizeof(frame.length));
  }

  // Forward from Serial to BLEUART
  if (Serial.available()) {
    // Delay to get enough input data since we have a
    // limited amount of space in the transmit buffer (or so this random example
    // claims)
    delay(2);

    UartFrame frame;
    int count =
        Serial.readBytes(reinterpret_cast<uint8_t*>(&frame), sizeof(frame));
    if (count == frame.length + 3) {
      bleuart.write(frame.payload, frame.length);
      bleuart.flushTXD();
    }
    Serial.flush();
  }
}
