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

#include <stdint.h>

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static uint16_t calculateCRC(uint8_t crcData, uint16_t crcReg) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80))
      crcReg = (crcReg << 1) ^ 0x8005;
    else
      crcReg = (crcReg << 1);
    crcData <<= 1;
  }
  return crcReg;
}

typedef struct __attribute__((packed)) {
  uint8_t data[48] = {0};
  uint8_t crcHi;
  uint8_t crcLo;
} TestPacket_s;
#include "ti_fec.h"
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
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
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
  while (1) {
    HAL_Delay(1000);

    uint8_t fec_output[104];

    {
      uint32_t t1 = HAL_GetTick();

      // =============

      // Fill packet with data
      TestPacket_s packet;
      uint8_t packet_str[] = {3, 1, 2, 3};
      memcpy(packet.data, packet_str, sizeof(packet_str));
      size_t inputNum = sizeof(TestPacket_s);
      uint8_t *input = (uint8_t *)&packet;

      FecEncoder encoder;

      uint32_t t1_us = DWT->CYCCNT;

      // Generate CRC
      {
        uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
        for (int i = 0; i < sizeof(packet.data); i++) {
          checksum = calculateCRC(packet.data[i], checksum);
        }

        packet.crcHi = checksum >> 8;
        packet.crcLo = checksum & 0xff;
      }

      // borrowed pointer to internal array
      encoder.Encode(input, sizeof(packet));
      memcpy(fec_output, encoder.OutputArray(), sizeof(fec_output));

      uint32_t t2_us = DWT->CYCCNT;

      // =============

      uint32_t t2 = HAL_GetTick();

      printf("Tx dt: %lu ms %lu us\n", t2 - t1,
             (int)((double)(t2_us - t1_us) / SystemCoreClock * 1e6));
    }

    {
      uint32_t t1 = HAL_GetTick();
      uint32_t t1_us = DWT->CYCCNT;

      // =============

      FecDecoder decoder;
      // uint8_t decoded_data[sizeof(packet)];
      TestPacket_s rxPacket;
      decoder.FecDecode(fec_output, reinterpret_cast<uint8_t *>(&rxPacket),
                        sizeof(rxPacket));

      uint32_t t2 = HAL_GetTick();
      uint32_t t2_us = DWT->CYCCNT;

      printf("Decoded packet: [%5d bytes]\n", sizeof(rxPacket));
      for (int i = 0; i < sizeof(rxPacket); i++)
        printf("%02X%s", ((uint8_t *)&rxPacket)[i],
               (i % 8 == 7)   ? "\n"
               : (i % 2 == 1) ? " "
                              : " ");
      printf("\n\n");

      // Perform CRC check (Optional)
      {
        uint16_t checksum = 0xFFFF;  // Init value for CRC calculation
        for (int i = 0; i < sizeof(rxPacket.data); i++) {
          checksum = calculateCRC(rxPacket.data[i], checksum);
        }

        uint16_t txChecksum =
            (((uint16_t)rxPacket.crcHi) << 8) | rxPacket.crcLo;
        printf("hi %u low %u total %lu\n", rxPacket.crcHi, rxPacket.crcLo,
               txChecksum);

        printf("Local checksum %lu, remote checksum %lu\n", checksum,
               txChecksum);
        if (checksum == txChecksum) {
          // Do something to indicate that the CRC is OK
          printf("Crc OK!\n");
        } else {
          printf("Crc bad!\n");
        }
      }

      // =============

      printf("Rx dt: %lu ms %lu us\n", t2 - t1,
             (int)((double)(t2_us - t1_us) / SystemCoreClock * 1e6));
    }

    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
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
