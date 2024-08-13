#include "DynamixelMotor.h"
#include "stm32f4xx_hal.h"

extern "C" {  // another way
void entrypoint(UART_HandleTypeDef* huart);
};



void entrypoint(UART_HandleTypeDef* huart) {
  DynamixelMotor m{huart};
//  while (true) {
//  m.spinToPosition(10);
//  HAL_Delay(5000);
//  }
  m.ping();
}
