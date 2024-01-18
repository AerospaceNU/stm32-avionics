//
// Created by nathan on 5/14/23.
//

#include "rfm_950.h"

Rfm950::Rfm950() : Radio(RH_RF95_MAX_MESSAGE_LEN) {}

void Rfm950::init(int reset_pin, int cs_pin, int interrupt_pin) {
  // Need to construct the object here, so we do this weird thing to avoid
  // dynamic memory allocation
  new (&driver_memory) RH_RF95(cs_pin, interrupt_pin);
  rf95 = ((RH_RF95 *)&driver_memory);

  // Basically copied from Adafruit example code
  pinMode(reset_pin, OUTPUT);
  digitalWrite(reset_pin, HIGH);
  delay(100);
  digitalWrite(reset_pin, LOW);
  delay(10);
  digitalWrite(reset_pin, HIGH);
  delay(10);

  // Initialize the chip
  sensorStatus = rf95->init();

  // Radio settings
  // Fast transmit, short range config
  rf95->setFrequency(915.0);
  rf95->setModemConfig(RH_RF95::Bw500Cr45Sf128);
  rf95->setTxPower(23, false);  // High power
}

bool Rfm950::sendData(uint8_t *data, size_t len) {
  //    long start_time = millis();

  rf95->send(data, len);
  //    delay(10);
  //    rf95->waitPacketSent();  //send() automatically does the wait, so we
  //    don't wait

  //    int delta_time = millis() - start_time;

  //    Serial.print(delta_time);
  //    Serial.println(" Radio sent");

  return true;
}

bool Rfm950::isDataAvailable() { return rf95->available(); }

bool Rfm950::readData(uint8_t *buffer, size_t buffer_length) {
  uint8_t len = min((uint8_t)buffer_length, getMaxMessageLength());
  bool success = rf95->recv(buffer, &len);
  lastRssi = rf95->lastRssi();
  return success;
}