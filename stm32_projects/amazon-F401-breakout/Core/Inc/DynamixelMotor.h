#ifndef STM32_PROJECTS_AMAZON_F401_BREAKOUT_CORE_INC_DYNAMIXELMOTOR_H_
#define STM32_PROJECTS_AMAZON_F401_BREAKOUT_CORE_INC_DYNAMIXELMOTOR_H_

#include <cstdint>

#include "stm32f4xx_hal.h"

class DynamixelMotor {
 public:
  explicit DynamixelMotor(UART_HandleTypeDef* huart) : m_huart{huart} {}

  static const constexpr uint32_t kMaxPayloadSize = 50;

  struct DynamixelPacket_t {
    uint8_t header[4];
    uint8_t id;
    uint8_t length_l;
    uint8_t length_h;
    uint8_t instruction;
    uint8_t payload[kMaxPayloadSize + 2];  // +2 for CRC space;
  };

  uint8_t spinToPosition(double degrees);

  uint8_t ping();
  uint8_t setGoalPosition(double degrees);
  uint8_t startSpin();

 private:
  DynamixelPacket_t m_txPacket = {};
  DynamixelPacket_t m_rxPacket = {};
  UART_HandleTypeDef* m_huart;

  const uint8_t m_id = 0;
  const uint32_t m_baudRate = 9600;

  uint8_t read(DynamixelPacket_t& buf);
  uint8_t write(DynamixelPacket_t& buf);

  void printPacket(DynamixelPacket_t& buf);

  uint16_t updateCrc(uint16_t crc_accum, uint8_t* data_blk_ptr,
                     uint16_t data_blk_size);
};

#endif  // STM32_PROJECTS_AMAZON_F401_BREAKOUT_CORE_INC_DYNAMIXELMOTOR_H_
