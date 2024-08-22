#include "DynamixelMotor.h"
#include "stm32f4xx_hal.h"

extern "C" {  // another way
void entrypoint(UART_HandleTypeDef* huart);
};

void entrypoint(UART_HandleTypeDef* huart) {
  DynamixelMotor m{huart};
  m.torqueEnable(Toggle::ON);
  HAL_Delay(1000);
  double degrees = 0;
  while (degrees < 360) {
    m.goalPosition(degrees);
    HAL_Delay(5000);
    degrees += 60;
  }
}
