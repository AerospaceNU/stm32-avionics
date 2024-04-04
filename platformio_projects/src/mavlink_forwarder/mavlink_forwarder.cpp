#include <Arduino.h>

#include "RadioLib.h"
#include "cpp_circular_buffer.h"

enum class DIO0Map {
  // Packet reception complete interrupt
  // At the end of the payload, the RxDone interrupt is generated together with the interrupt PayloadCrcError if the payload
  // CRC is not valid. However, even when the CRC is not valid, the data are written in the FIFO data buffer for post processing.
  // Following the RxDone interrupt the radio goes to stand-by mode.
  RxDone = 0,
  // FIFO Payload transmission complete interrupt
  // Upon completion the TxDone interrupt is issued and the radio returns to Stand-by mode.
  TxDone = 1,
  // CAD (checking for preamble symbols) done. Check for CadDetected to see result. See page 39
  CadDone = 2
};

enum PacketType : uint8_t {
  TRANSPARENT_SERIAL = 0,
  MAVLINK,
  STATUS
};

struct __attribute__((packed)) LastPacketStats {
  int len;
  int rssi;
  int noiseFloor;
};

// Pin definitions
// Radio
#define RFM95_RST 4
#define RFM95_CS 8
#define RFM95_INT 3 // dio0, super limiting. see page 98

RFM95 radio = new Module(RFM95_CS, RFM95_INT, RFM95_RST);

CircularBuffer<uint8_t, 1024> serialToRadioBytes;
CircularBuffer<uint8_t, 1024> radioToSerialBytes;

LastPacketStats localStatistics;
LastPacketStats remoteStatistics;

void radioPacketRxDone();

void setup() {
  Serial.begin(115200);
  // GPIO0 = RX, GPIO1 = TX
  Serial1.begin(115200);
  Serial1.setTimeout(0);

  int state = radio.begin();
  // radio.setDio0Action();

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  state |= radio.setFrequency(914.15f);
  state |= radio.setOutputPower(10);
  state |= radio.setBandwidth(125);
  state |= radio.setSpreadingFactor(8);
  state |= radio.setCodingRate(6);  

  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("Unable to set configuration, code "));
    Serial.println(state);
    while (true);
  }

  // Set DIO0 to generate an interrupt on RX done. This is the configured default but be paranoid
  // page 99 and 41 for mappings
  state = radio.setDIOMapping(0, static_cast<uint8_t>(DIO0Map::RxDone));
  radio.setPacketReceivedAction(radioPacketRxDone);

  // start continuous reception (up until packet recieved)
  radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("Radio now in RX!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void enqueueSerialToBuffer() {
  // grab incoming data and buffer it ourselves
  int len = Serial1.available();
  char data[len];
  Serial1.readBytes(data, len);
  for (auto d : data) {
    if (!serialToRadioBytes.enqueue(d)) {
      // buffer overflow? GUH. 
      // Simply discard data
      break;
    }
  }
}

/**
 * @brief Transmit up to (max length) of data from our rx buffer
 */
void transmitOnePacket() {
  uint8_t data[RADIOLIB_SX127X_MAX_PACKET_LENGTH];


  // add "type" byte to the start of every packet (zero length packets not allowed, so this ensures we can always ping-pong)
  data[0] = PacketType::TRANSPARENT_SERIAL;

  // Determine packet length, leaving room for the type byte
  int len = min(serialToRadioBytes.count(), static_cast<size_t>(RADIOLIB_SX127X_MAX_PACKET_LENGTH - 1));

  if (len) { 
    // Pull from the rxbuffer
    serialToRadioBytes.peek(data + 1, len);
    serialToRadioBytes.dequeue(len);
  }

  // we always have a type byte at the start
  int radioLen = len + 1;

  // Send data out
  radio.transmit(data, radioLen);
}

// Dont wanna deal with syncronizing access between main thread and ISR,
volatile bool receivedFlag = false;
void radioPacketRxDone() {
  receivedFlag = true;
}

/**
 * @brief Wait a bit for a packet to come from the groundstation
 * 
 */
void readoutPacket() {
  uint8_t data[RADIOLIB_SX127X_MAX_PACKET_LENGTH];
  // read out the last packet and empties fifo i think
  int ret = radio.readData(data, 0);
  
  if (
    (ret == RADIOLIB_ERR_NONE) || 
    (ret == RADIOLIB_ERR_CRC_MISMATCH) || 
    (ret == RADIOLIB_ERR_LORA_HEADER_DAMAGED)) {
    int len = radio.getPacketLength();
    int rssi = radio.getRSSI();
    float snr_db = radio.getSNR();
    int noiseFloor = rssi/snr_db;

    Serial.printf("> [len=%i] rssi=%i noise=%i status %i", len, rssi, noiseFloor, ret);
    localStatistics = {
      .len = len,
      .rssi = rssi,
      .noiseFloor = noiseFloor
    };

    // Serial buffered internally, so just shove data in
    int written = Serial1.write(data, len);

    if (written != len) {
      Serial.println("Serial1 TX buffer overflow?");
    }
  } else {
    // timed out, ping time
  }
}

void boatLoop() {
  transmitOnePacket();
  
  auto start = millis();
  unsigned int gsWaitTimeout = 100;

  receivedFlag = false;
  radio.startReceive();

  while(!receivedFlag && (millis() - start) < gsWaitTimeout) {
    enqueueSerialToBuffer();
  }
}


void gsLoop() {
  receivedFlag = false;
  radio.startReceive();

  while(!receivedFlag) {
    enqueueSerialToBuffer();
  }
  
  transmitOnePacket();
}

void loop() {
#ifdef IS_BOAT
  boatLoop();
#else
  gsLoop();
#endif
}
