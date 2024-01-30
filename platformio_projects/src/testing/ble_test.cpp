#include "Arduino.h"
#include "STM32duinoBLE-main/src/STM32duinoBLE.h"

HCISharedMemTransportClass HCISharedMemTransport;
BLELocalDevice BLEObj(&HCISharedMemTransport);
BLELocalDevice& BLE = BLEObj;

const uint8_t BLEUART_UUID_SERVICE[] =
{
    0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0,
    0x93, 0xF3, 0xA3, 0xB5, 0x01, 0x00, 0x40, 0x6E
};

const uint8_t BLEUART_UUID_CHR_RXD[] =
{
    0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0,
    0x93, 0xF3, 0xA3, 0xB5, 0x02, 0x00, 0x40, 0x6E
};

const uint8_t BLEUART_UUID_CHR_TXD[] =
{
    0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0,
    0x93, 0xF3, 0xA3, 0xB5, 0x03, 0x00, 0x40, 0x6E
};

/*
const uint8_t BLEUART_UUID_SERVICE[] = 
9E CA DC 24 0E E5 A9 E0 93 F3 A3 B5 01 00 40 6E

const uint8_t BLEUART_UUID_CHR_RXD[] =
9E CA DC 24 0E E5 A9 E0 93 F3 A3 B5 02 00 40 6E

const uint8_t BLEUART_UUID_CHR_TXD[] =
9E CA DC 24 0E E5 A9 E0 93 F3 A3 B5 03 00 40 6E
*/



BLEService bleuart_service(BLEUuid::uuidToString(BLEUART_UUID_SERVICE, sizeof(BLEUART_UUID_SERVICE)));
BLECharacteristic bleuart_rxd_char(BLEUuid::uuidToString(BLEUART_UUID_CHR_RXD, sizeof(BLEUART_UUID_CHR_RXD)), BLEWrite | BLEWriteWithoutResponse, 32);
BLECharacteristic bleuart_txd_char(BLEUuid::uuidToString(BLEUART_UUID_CHR_TXD, sizeof(BLEUART_UUID_CHR_TXD)), BLERead | BLENotify, 32);

#define ledPin LED_BUILTIN

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Line Cutter");
  BLE.setAdvertisedService(bleuart_service);

  // add the characteristic to the service
  bleuart_service.addCharacteristic(bleuart_rxd_char);
  bleuart_service.addCharacteristic(bleuart_txd_char);

  // add service
  BLE.addService(bleuart_service);

  // set the initial value for the characeristic:
  // char* test_data = "hello!\n";
  // bleuart_rxd_char.writeValue(test_data);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (bleuart_rxd_char.written()) {
        char* val = (char*) bleuart_rxd_char.value();
        val[bleuart_rxd_char.valueLength()] = '\0';
        Serial.println("GOT RX->");
        Serial.println(val);

        bleuart_txd_char.writeValue(val);
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}


// void setup() {
//   // initialize LED digital pin as an output.
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop() {
//   // turn the LED on (HIGH is the voltage level)
//   digitalWrite(LED_BUILTIN, HIGH);

//   // wait for a second
//   delay(1000);

//   // turn the LED off by making the voltage LOW
//   digitalWrite(LED_BUILTIN, LOW);

//   // wait for a second
//   delay(200);
// }
