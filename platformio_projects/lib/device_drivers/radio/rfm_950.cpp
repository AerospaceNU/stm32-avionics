//
// Created by nathan on 5/14/23.
//

#include "rfm_950.h"
static SPIClass spi(HSPI);
Rfm950::Rfm950() : Radio(63) {}

void Rfm950::init(int reset_pin, int cs_pin, int interrupt_pin) {
  // We do a little dynamic memory allocation
  spi.begin(8, 3, 4, 5);
  rf95 = new SX1262(
      new Module(cs_pin, interrupt_pin, reset_pin, RADIOLIB_NC, spi));

  // Basically copied from Adafruit example code
  pinMode(reset_pin, OUTPUT);
  digitalWrite(reset_pin, HIGH);
  delay(100);
  digitalWrite(reset_pin, LOW);
  delay(10);
  digitalWrite(reset_pin, HIGH);
  delay(10);

  // Initialize the chip
  sensorStatus = rf95->beginFSK(915.0, 38.400, 20.0, 100.0, 17, 16);

  rf95->setSyncWord(0x930B51DE);

  // Radio settings
  // Fast transmit, short range config
  // rf95->setFrequency(915.0);
  // rf95->setModemConfig(RH_RF95::Bw500Cr45Sf128);
  // rf95->setTxPower(23, false);  // High power
}

bool Rfm950::sendData(uint8_t *data, size_t len) {
  rf95->transmit(data, len);

  return true;
}

bool Rfm950::isDataAvailable() { return rf95->available(); }

bool Rfm950::readData(uint8_t *buffer, size_t buffer_length) {
  uint8_t len = buffer_length;

  auto success = rf95->receive(buffer, len) == RADIOLIB_ERR_NONE;

  return success;
}
