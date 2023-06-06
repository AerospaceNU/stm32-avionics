//
// Created by nathan on 5/14/23.
//

#include "radio.h"

Radio::Radio(int max_message_length) { maxMessageLength = max_message_length; }

bool Radio::sendData(uint8_t *data, size_t len) { return false; }

bool Radio::isDataAvailable() { return false; }

bool Radio::readData(uint8_t *buffer, size_t buffer_length) { return false; }

int Radio::getMaxMessageLength() { return maxMessageLength; }

int Radio::getLastRssi() { return lastRssi; }
