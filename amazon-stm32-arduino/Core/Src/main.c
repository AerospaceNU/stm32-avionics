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
#include <stdbool.h>
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Defined deep within STM32 driver code, but we yank it out here
extern USBD_HandleTypeDef hUsbDeviceFS;

#include "usbd_cdc_if.h"

// Overwrite _write so printf prints to USB
int _write(int file, char *ptr, int len) {
  // Check here to make sure that USB things are actually set up
  if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) return 0;

  // Try to send -- CDC_Transmit_FS is what actually sends our data
  if (USBD_OK == CDC_Transmit_FS((uint8_t *)ptr, len)) return len;

  // Failure, report sending zero bytes
  return 0;
}

#include "../system/tasks/radio_packet_types.h"
#include "circular_buffer.h"
#include "data_structures.h"
#include "radioconfig/smartrf_cc1120_cfg_434_38_4kbps.h"
#include "ti_radio.h"

TiRadioCtrl_s radio;

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

  radio.CS_pin = RAD_CS_Pin;
  radio.CS_port = RAD_CS_GPIO_Port;
  radio.RST_pin = RAD_RST_Pin;
  radio.RST_port = RAD_RST_GPIO_Port;
  radio.radhspi = &hspi1;
  radio.settingsPtr = cc1120_433_1_2kbps_cfg;
  radio.settingsSize = sizeof(cc1120_433_1_2kbps_cfg);
  radio.payloadSize = sizeof(RadioPacket_s);
  radio.id = 0;
  radio.packetCfg = TIRADIO_PKTLEN_FIXED;

  bool success = tiRadio_init(&radio);
  if (!success) {
    printf("REEEEEEEE\n");
    return 0;
  }
  tiRadio_setRadioFrequency(&radio, TIRADIO_BAND_410_480MHz, 433 * 1e6);
  tiRadio_setOutputPower(&radio, 14);

  RadioRecievedPacket_s buff[10];
  CircularBuffer_s circBuff;
  cb_init(&circBuff, buff, sizeof(buff), sizeof(buff[0]));
  tiRadio_registerConsumer(&radio, &circBuff);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    RadioPacket_s tx;
    tx.packetType = TELEMETRY_ID_STRING;
    static int i = 0;
    i++;
    tx.payload.cliString.id = i;
    char *str = "Hellowowowowowowo";
    memcpy(tx.payload.cliString.string, str, strlen(str));
    tx.payload.cliString.len = strlen(str);
    tiRadio_addTxPacket(&radio, (uint8_t *)&tx, sizeof(tx));

    // Wait for packet for (100x * 10ms each) = 1 second
    for (int i = 0; i < 100; i++) {
      tiRadio_update(&radio);
      HAL_Delay(10);

      while (cb_count(&circBuff)) {
        RadioRecievedPacket_s packet;
        size_t one = 1;
        cb_peek(&circBuff, &packet, &one);
        if (one) {
          printf("Got packet, rssi=%i lqi=%i\r\n", packet.rssi, packet.lqi);
          cb_dequeue(&circBuff, 1);
        } else {
          break;
        }
      }
    }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  /* User can add his own implementation to report the HAL error return state */
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
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
