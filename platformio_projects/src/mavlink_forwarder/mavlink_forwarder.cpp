#include <Arduino.h>

#include "RadioLib.h"
#include "cpp_circular_buffer.h"

enum class DIO0Map {
  // Packet reception complete interrupt
  // At the end of the payload, the RxDone interrupt is generated together with
  // the interrupt PayloadCrcError if the payload
  // CRC is not valid. However, even when the CRC is not valid, the data are
  // written in the FIFO data buffer for post processing.
  // Following the RxDone interrupt the radio goes to stand-by mode.
  RxDone = 0,
  // FIFO Payload transmission complete interrupt
  // Upon completion the TxDone interrupt is issued and the radio returns to
  // Stand-by mode.
  TxDone = 1,
  // CAD (checking for preamble symbols) done. Check for CadDetected to see
  // result. See page 39
  CadDone = 2
};

enum PacketType : uint8_t { TRANSPARENT_SERIAL = 0, MAVLINK, STATUS };

struct __attribute__((packed)) LastPacketStats {
  int len;
  int rssi;
  int noiseFloor;
};

// Pin definitions
// Radio
#define RFM95_RST 4
#define RFM95_CS 8
#define RFM95_INT 3  // dio0, super limiting. see page 98

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
  // 11kbps raw, or 150ms per 200 byte packet
  state |= radio.setBandwidth(250);
  state |= radio.setSpreadingFactor(7);
  state |= radio.setCodingRate(5);

  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("Unable to set configuration, code "));
    Serial.println(state);
    while (true);
  }

  // Set DIO0 to generate an interrupt on RX done. This is the configured
  // default but be paranoid page 99 and 41 for mappings
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

void enqueueRadioRxToSerialBuffer() {
  // How much data we have to send
  int radioLen = radioToSerialBytes.count();
  // How empty the serial TX FIFO is
  int serTxLenMax = Serial1.availableForWrite();

  int len = min(radioLen, serTxLenMax);

  uint8_t data[len];

  if (radioToSerialBytes.peek(data, len)) {
    radioToSerialBytes.dequeue(len);

    // should already have at least this much space
    Serial1.write(data, len);
  } else {
    // couldn't dequeue? try starting over
    Serial.println("Error dequeueing radio->seriail buff");
    radioToSerialBytes.flush();
  }
}

void enqueueSerialToRxBuffer() {
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
 * @brief Transmit up to (max length) of data from our rx buffer. Will ALWAYS
 * emit a packet, even if empty.
 */
void transmitOnePacket() {
  uint8_t data[RADIOLIB_SX127X_MAX_PACKET_LENGTH];

  // add "type" byte to the start of every packet (zero length packets not
  // allowed, so this ensures we can always ping-pong)
  // TODO: mavlink detection and packet framing
  data[0] = PacketType::TRANSPARENT_SERIAL;

  // Determine packet length, leaving room for the type byte
  int len = min(serialToRadioBytes.count(),
                static_cast<size_t>(RADIOLIB_SX127X_MAX_PACKET_LENGTH - 1));

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
void radioPacketRxDone() { receivedFlag = true; }

/**
 * @brief Wait a bit for a packet to come from the groundstation
 *
 */
void readoutPacket() {
  uint8_t data[RADIOLIB_SX127X_MAX_PACKET_LENGTH];
  // read out the last packet and empties fifo i think
  int ret = radio.readData(data, 0);

  if ((ret == RADIOLIB_ERR_NONE) || (ret == RADIOLIB_ERR_CRC_MISMATCH) ||
      (ret == RADIOLIB_ERR_LORA_HEADER_DAMAGED)) {
    int len = radio.getPacketLength();
    int rssi = radio.getRSSI();
    float snr_db = radio.getSNR();
    int noiseFloor = rssi / snr_db;

    Serial.printf("> [len=%i] rssi=%i noise=%i status %i\n", len, rssi,
                  noiseFloor, ret);
    localStatistics.len = len;
    localStatistics.rssi = rssi;
    localStatistics.noiseFloor = noiseFloor;

    if (data[0] == PacketType::TRANSPARENT_SERIAL ||
        data[0] == PacketType::MAVLINK) {
      for (size_t i = 1; i < len; i++) {
        if (!radioToSerialBytes.enqueue(data[i])) {
          Serial.println("Serial1 TX buffer overflow?");
          break;
        }
      }
    } else if (data[0] == PacketType::STATUS) {
      // statistics
      remoteStatistics = *(reinterpret_cast<LastPacketStats*>(data + 1));
    }
  } else {
    // RX error
    Serial.printf("Rx error %i\n", ret);
  }
}

void boatLoop() {
  transmitOnePacket();

  auto start = millis();
  unsigned int gsWaitTimeout = 1000;

  receivedFlag = false;
  radio.startReceive();

  // Modem status = 1 means "clear" (page 105)
  while (!receivedFlag && ((millis() - start) < gsWaitTimeout)) {
    enqueueSerialToRxBuffer();
    enqueueRadioRxToSerialBuffer();

    int status = radio.getModemStatus();
    Serial.print("Modem status: ");
    Serial.println(status, BIN);

    delay(50);
  }
  if (receivedFlag) {
    readoutPacket();
  }
}

void gsLoop() {
  receivedFlag = false;
  radio.startReceive();

  while (!receivedFlag) {
    enqueueSerialToRxBuffer();
    enqueueRadioRxToSerialBuffer();

    int status = radio.getModemStatus();
    Serial.print("Modem status: ");
    Serial.println(status, BIN);

    delay(50);
  }

  if (receivedFlag) {
    readoutPacket();
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
