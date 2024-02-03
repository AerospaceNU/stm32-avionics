/*
 * radio_manager.h
 */

#ifndef COMMON_SYSTEM_TASKS_RADIO_MANAGER_H_
#define COMMON_SYSTEM_TASKS_RADIO_MANAGER_H_

#include <stdint.h>

#include "board_config_common.h"
#include "circular_buffer.h"
#include "data_structures.h"
#include "filters.h"
#include "hardware_manager.h"
#include "hm_timer.h"
#include "radio_packet_types.h"

#define RADIO_MAX_CALLBACKS 10
typedef void (*RadioCallback_t)(RadioDecodedRecievedPacket_s*);

struct PacketTimerCollection {
  HmTickTimer orientationTimer{10};
  HmTickTimer positionTimer{10};
  HmTimer altInfoTimer{1213};
  HmTimer lineCutterDataTimer{1000};
  HmTimer lineCutterVarsTimer{1000};
  HmTickTimer pyroContTimer{1};
};

uint16_t calculateRadioPacketCRC(RadioDecodedPacket_s &packet);

class RadioManager {
 public:
  RadioManager() = default;

  void init(int id);
  void tick();

  void transmitData(SensorData_s* sensorData, FilterData_s* filterData,
                    uint8_t state);

  // Send a string over radio
  // Note that this BLOCKS for up to 150ms!!
  void transmitString(uint8_t* data, size_t len);

  void addMessageCallback(RadioCallback_t callback);

  static void InitAll();
  static void TickAll();

 private:
  int radioId;

  // The radio will enqueue packets here automatically
  CircularBuffer_s rxBuffer;
  uint8_t rxArray[RX_BUFF_LEN * sizeof(RadioRecievedOTAPacket)];

  // Output callbacks to notify other bits of code that a packet got recieved
  RadioCallback_t callbacks[RADIO_MAX_CALLBACKS];
  size_t numCallbacks;

  // keep track of how long its been since we last transmited a kind of packet
  PacketTimerCollection timer;

  // Helper packet that sticks around and we modify in place to trasnmit
  // information
  RadioDecodedPacket_s transmitPacket;

  /**
   * Perform all common preprocessing to send a packet. Calculate and set CRC,
   * and perform any data encoding.
   */
  void sendInternal(RadioDecodedPacket_s& packet);
};

// stupid hack for static list of all radio managers
extern RadioManager radioManagers[NUM_RADIO];

#endif  // COMMON_SYSTEM_TASKS_RADIO_MANAGER_H_
