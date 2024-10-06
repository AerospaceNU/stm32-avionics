#ifndef STM32_PROJECTS_DEVICE_DRIVERS_DYNAMIXEL_MOTOR_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_DYNAMIXEL_MOTOR_H_

#include <cstdint>

#include "dynamixel_command_queue.h"

enum Toggle : uint8_t {
  OFF = 0,
  ON = 1,
};

enum OperatingMode : uint8_t {
  CURRENT = 0,
  VELOCITY = 1,
  POSITION = 3,
  EXT_POSITION = 4
};

class DynamixelMotor {
 public:
  DynamixelMotor() {}

  bool init(const uint8_t id, DynamixelCommandQueue* commandQueue);

  static const constexpr uint32_t kMaxPayloadSize = 50;
  struct DynamixelPacket_t {
    uint8_t header[4];
    uint8_t id;
    uint8_t length_l;
    uint8_t length_h;
    uint8_t instruction;
    uint8_t payload[kMaxPayloadSize + 2];  // +2 for CRC space;
  };

  uint8_t ping();

  uint8_t torqueEnable(Toggle toggle);

  uint8_t setOperatingMode(OperatingMode mode);

  uint8_t goalPosition(double degrees);

 private:
  uint8_t m_id = 0;
  DynamixelCommandQueue* m_commandQueue = nullptr;
  DynamixelPacket_t m_txPacket = {};
  DynamixelPacket_t m_rxPacket = {};
  std::function<void(uint16_t)> m_readCallback;

  uint8_t processReadData(uint16_t size);

  uint8_t write(DynamixelPacket_t& buf);

  uint16_t prepareTxPacket();

  uint16_t updateCrc(uint16_t crc_accum, uint8_t* data_blk_ptr,
                     uint16_t data_blk_size);
};

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_DYNAMIXEL_MOTOR_H_
