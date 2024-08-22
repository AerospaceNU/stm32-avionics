#ifndef STM32_PROJECTS_AMAZON_F401_BREAKOUT_CORE_INC_DYNAMIXELMOTOR_H_
#define STM32_PROJECTS_AMAZON_F401_BREAKOUT_CORE_INC_DYNAMIXELMOTOR_H_

#include <cstdint>

#include "stm32f4xx_hal.h"

enum Toggle : uint8_t {
  OFF = 0,
  ON = 1,
};

class DynamixelMotor {
 public:
  explicit DynamixelMotor(UART_HandleTypeDef* huart);

  static const constexpr uint32_t kMaxPayloadSize = 1000;
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

  uint8_t goalPosition(double degrees);

 private:
  DynamixelPacket_t m_txPacket = {};
  DynamixelPacket_t m_rxPacket = {};
  UART_HandleTypeDef* m_huart;

  const uint8_t m_id = 1;

  uint8_t read(DynamixelPacket_t& buf);

  uint8_t processReadData(uint16_t size);

  uint8_t write(DynamixelPacket_t& buf);

  uint16_t prepareTxPacket();

  uint16_t updateCrc(uint16_t crc_accum, uint8_t* data_blk_ptr,
                     uint16_t data_blk_size);
};

#endif  // STM32_PROJECTS_AMAZON_F401_BREAKOUT_CORE_INC_DYNAMIXELMOTOR_H_
