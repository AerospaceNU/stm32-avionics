/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "board_config_common.h"
#include "data_structures.h"
#include "hardware_manager.h"
#include "radio_manager.h"
#include "radio_packet_types.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
typedef struct __attribute__((__packed__)) {
  uint8_t packetType;
  float latitude;
  float longitude;
  float gps_alt;
  double groundPressure;
  double groundTemp;
} HeartbeatData_s;
typedef enum {
  RAD_433 = FIRST_ID_RADIO_TI_433,
  RAD_915 = FIRST_ID_RADIO_TI_915,
  GROUNDSTATION = 0xff
} MessageDestination_e;

typedef struct __attribute__((packed)) {
  uint8_t destination;
  uint16_t len;
  uint8_t data[256];
} GroundstationUsbCommand_s;
#define min(a, b) (a < b) ? a : b
#define CHANNEL_COMMAND_ID 1
#define RADIO_HARDWARE_COMMAND_ID 2
static void GroundstationParseCommand(GroundstationUsbCommand_s *command) {
  if (command->data[0] == CHANNEL_COMMAND_ID) {
    uint8_t radioHw = command->data[1];
    int8_t channel = command->data[2];

    hm_radioSetChannel((int)radioHw, channel);
  }

  //  if (command->data[0] == CHANNEL_COMMAND_ID) {
  //    uint8_t radioHw = command->data[1];
  //    int8_t channel = command->data[2];
  //
  //    HM_RadioSetChannel(radioHw, channel);
  //  }
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void OnDataRx(RadioRecievedPacket_s *packet) {
  hm_usbTransmit(FIRST_ID_USB_STD, (uint8_t *)packet, sizeof(*packet));
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  hm_hardwareInit();

  radioManager_init();
  for (int i = 0; i < NUM_RADIO; i++) {
    radioManager_addMessageCallback(i, OnDataRx);
  }

  uint32_t start = HAL_GetTick();
  CircularBuffer_s *buffer = hm_usbGetRxBuffer(FIRST_ID_USB_STD);

  while (1) {
    // Update sensors
    hm_watchdogRefresh();
    hm_readSensorData();
    hm_radioUpdate();

    if ((HAL_GetTick() - start) >= 2000) {
      start = HAL_GetTick();

      static HeartbeatData_s heartbeat;
      heartbeat.packetType = 200;
      //      heartbeat.latitude =
      //      hm_getSensorData()->gpsData->generalData.latitude;
      //      heartbeat.longitude =
      //      hm_getSensorData()->gpsData->generalData.longitude;
      //      heartbeat.gps_alt =
      //      hm_getSensorData()->gpsData->generalData.altitude;
      //      heartbeat.groundPressure =
      //          hm_getSensorData()->barometerData[0].pressureAtm;
      //      heartbeat.groundTemp =
      //      hm_getSensorData()->barometerData[0].temperatureC;

      // Hack to make all packets the same length when sent over USB
      static uint8_t heartbeatArr[sizeof(RadioRecievedPacket_s)] = {0};
      memset(heartbeatArr, 0, sizeof(heartbeatArr));
      memcpy(heartbeatArr, &heartbeat, sizeof(heartbeat));
      hm_usbTransmit(FIRST_ID_USB_STD, (uint8_t *)&heartbeatArr,
                     sizeof(heartbeatArr));
    }

    // A packet must mave at least a destination [1 byte] and a len [2 bytes],
    // and a non-zero quantity of data
    if (cb_count(buffer) > 3) {
      static GroundstationUsbCommand_s command;
      size_t count = min(cb_count(buffer), sizeof(command));
      cb_peek(buffer, (uint8_t *)&command, &count);

      // If we got at least enough bytes for one message to be done
      if (count >= command.len + 3) {
        if (command.destination == GROUNDSTATION) {
          GroundstationParseCommand(&command);
        } else {
          int dest = command.destination == RAD_433 ? FIRST_ID_RADIO_TI_433
                                                    : FIRST_ID_RADIO_TI_915;

          radioManager_transmitString(dest, command.data, command.len);
        }
        cb_dequeue(buffer, count);
      } else if (command.destination != GROUNDSTATION ||
                 command.destination != RAD_433 ||
                 command.destination != RAD_915) {
        cb_flush(buffer);
      }
    }

    // Process incoming data
    radioManager_tick();

    HAL_Delay(1);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* USER CODE END 3 */
  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state
   */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
     file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
