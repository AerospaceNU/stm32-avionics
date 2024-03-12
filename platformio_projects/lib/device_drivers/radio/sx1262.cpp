//
// Created by nathan on 5/14/23.
//

#if 0

#include "sx1262.h"

#ifdef HSPI
static SPIClass spi(HSPI);
#endif

Sx1262::Sx1262() : Radio(63) {}

void Sx1262::init(int reset_pin, int cs_pin, int interrupt_pin) {
  // We do a little dynamic memory allocation
#ifdef HSPI
  spi.begin(8, 3, 4, cs_pin);  // SCK, MISO, MOSI, CS
  sx1262 = new SX1262(new Module(cs_pin, interrupt_pin, reset_pin, RADIOLIB_NC, spi));
#else  // IDK if this works
  sx1262 = new SX1262(new Module(cs_pin, interrupt_pin, reset_pin, RADIOLIB_NC));
#endif

  // Basically copied from Adafruit example code
  sx1262->reset();

  // Initialize the chip
  initErrorCode = sx1262->beginFSK(915.0, 38.400, 20.0, 100.0, 17, 16);
  sensorStatus = initErrorCode == 0;

  // Radio settings
  uint8_t syncWord[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  sx1262->setSyncWord(syncWord, 8);

  sx1262->setCRC(2, 0xFFFF, 0x8005, false);
}

bool Sx1262::sendData(uint8_t *data, size_t len) {
  Serial.print("HI ");
  Serial.print(initErrorCode);
  Serial.print(" | ");
  Serial.println(sx1262->getDataRate());
  delay(10);

  sx1262->transmit(data, len);

  return true;
}

bool Sx1262::isDataAvailable() { return sx1262->available(); }

bool Sx1262::readData(uint8_t *buffer, size_t buffer_length) {
  uint8_t len = buffer_length;

  auto success = sx1262->receive(buffer, len) == RADIOLIB_ERR_NONE;
  lastRssi = sx1262->getRSSI();

  return success;
}
#endif