#include <Arduino.h>

#include "RadioLib.h"
#include "cpp_circular_buffer.h"
#include "wiring_private.h"

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

// WindVaneSerial. TX D11 (S1.0 primary), RX D12 (S1.3 primary)
#define PIN_SERIAL2_RX (12ul)
#define PAD_SERIAL2_RX (SERCOM_RX_PAD_3)
#define PIN_SERIAL2_TX (11ul)
#define PAD_SERIAL2_TX (UART_TX_PAD_0)
Uart WindVaneSerial(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX,
             PAD_SERIAL2_TX);
void SERCOM1_Handler()
{
  WindVaneSerial.IrqHandler();
}

#define WIND_DIR_PIN A1
#define WIND_SPEED_PIN A0
#define ANALOG_READ_BITS (12)
#define ANALOG_MAX (1 << ANALOG_READ_BITS)

RFM95 radio = new Module(RFM95_CS, RFM95_INT, RFM95_RST);

CircularBuffer<uint8_t, 4000> serialToRadioBytes;
CircularBuffer<uint8_t, 4000> radioToSerialBytes;

LastPacketStats localStatistics;
LastPacketStats remoteStatistics;

void radioPacketRxDone();

void setup() {
  Serial.begin(115200);

  Serial.println("Hullo!");

  // GPIO0 = RX, GPIO1 = TX
  Serial1.begin(115200);
  Serial1.setTimeout(0);

  // D11 TX, D12 RX
  WindVaneSerial.begin(115200);
  pinPeripheral(PIN_SERIAL_RX, PIO_SERCOM); 
  pinPeripheral(PIN_SERIAL_TX, PIO_SERCOM); 

  pinMode(WIND_DIR_PIN, INPUT);
  pinMode(WIND_SPEED_PIN, INPUT);
  analogReadResolution(ANALOG_READ_BITS);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

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
  state |= radio.setBandwidth(500);
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

template <typename T>
T map_t(T x, T in_min, T in_max, T out_min, T out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int lastWindSentTime = 0;
/**
 * @brief If it's been more than 300ms since last wind send, send a wind message
 *
 */
void sendWindNMEA() {
  if ((millis() - lastWindSentTime) < 350) {
    return;
  }

  // Read in angle and remap. Might need to invert the polarity/apply
  // calibration later
  int sensor_value = analogRead(WIND_DIR_PIN);
  int speed_val = analogRead(WIND_SPEED_PIN);

  int wind_dir = map_t(sensor_value, 0, ANALOG_MAX, 360, 0);
  float speed_mps = (0.0297 * speed_val - 11.5);

  // Create message, up until checksum
  // Direction in degrees (relative), speed in tenths of a m/s
  char str[100];
  int len = snprintf(str, sizeof(str), "$WIMWV,%i,R,%.1f,M,A", wind_dir, speed_mps);

  // calculate checksum
  uint8_t checksum = 0;
  for (int i = 1; i < len; i++) {
    checksum ^= str[i];
  }

  // append checksum+newlines to message
  snprintf(str + len, sizeof(str) - len + 4, "*%X%X\r\n",
           (checksum >> 4) & 0x0f, checksum & 0x0f);

  // WindVaneSerial.print(str);
  Serial1.print(str);
  Serial.printf("Dir val %i speed val %i\n", sensor_value, speed_val);
  Serial.printf("Dir %i speed %f\n", wind_dir, speed_mps); Serial.println(str);

  // And write down last sent time
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  lastWindSentTime = millis();
}

/**
 * @brief Send data we've gotten from the radio out over Serial1
 * 
 */
void enqueueRadioRxToSerialBuffer() {
  // How much data we have to send
  int radioLen = radioToSerialBytes.count();
  // How empty the serial TX FIFO is
  
#ifdef IS_BOAT
#define SerialPort Serial1
#else
#define SerialPort Serial
#endif

  int serTxLenMax = SerialPort.availableForWrite();

  int len = min(radioLen, serTxLenMax);

  uint8_t data[len];

  if (radioToSerialBytes.peek(data, len)) {
    radioToSerialBytes.dequeue(len);

    // should already have at least this much space
    SerialPort.write(data, len);
  } else {
    // couldn't dequeue? try starting over
    Serial.println("Error dequeueing radio->seriail buff");
    radioToSerialBytes.flush();
  }
}

/**
 * @brief Move data from Serial1 (autopilot->radio) into our buffer
 * 
 */
void enqueueSerialToRxBuffer() {
  // grab incoming data and buffer it ourselves
  int len = Serial1.available();
  char data[len];
  Serial1.readBytes(data, len);
  for (auto d : data) {
    if (!serialToRadioBytes.enqueue(d)) {
      Serial.println("> Serial->Radio buffer overflow!!");
      // buffer overflow? GUH.
      // Simply discard data
      return;
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
  digitalWrite(LED_BUILTIN, HIGH);
  radio.transmit(data, radioLen);
  digitalWrite(LED_BUILTIN, LOW);
  
#ifdef IS_BOAT
  Serial.printf("After tx, buffer has %i\n", serialToRadioBytes.count());
#endif
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

    // Serial.printf("> [len=%i] rssi=%i noise=%i status %i\n", len, rssi, noiseFloor, ret);
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
  
  sendWindNMEA();
  delay(100);
  return;


  transmitOnePacket();

  auto start = millis();
  unsigned int gsWaitTimeout = 1000;

  receivedFlag = false;
  radio.startReceive();

  // Modem status = 1 means "clear" (page 105)
  // bool rxClear = true;

  // Loop until (packet rx done via interrupt) or (timedout and not in rx)
  Serial.println("Waiting for pong");
  while (!receivedFlag && ((millis() - start) < gsWaitTimeout)) {
    enqueueSerialToRxBuffer();
    enqueueRadioRxToSerialBuffer();

    // int status = radio.getModemStatus();
    // Serial.print("Status: "); Serial.println(status, BIN);
    // rxClear = (status & (1 << 4));

    delay(30);

    // Send a wind message if it's time to
    sendWindNMEA();
  }


  if (receivedFlag) {
    Serial.println("Reading out");
    readoutPacket();
  } else {
    Serial.println("Timed out waiting for pong");
  }
}

void gsLoop() {
  receivedFlag = false;
  radio.startReceive();

  while (!receivedFlag) {
    enqueueSerialToRxBuffer();
    enqueueRadioRxToSerialBuffer();

    delay(30);
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
