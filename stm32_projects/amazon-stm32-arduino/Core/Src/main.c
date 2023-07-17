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

#include "ao_fec.h"
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

//--------------------------------------------------------------------------------
uint16_t culCalcCRC(uint8_t crcData, uint16_t crcReg) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80))
      crcReg = (crcReg << 1) ^ 0x8005;
    else
      crcReg = (crcReg << 1);
    crcData <<= 1;
  }
  return crcReg;
}  // culCalcCRC
uint16_t fecEncodeTable[] = {0, 3, 1, 2, 3, 0, 2, 1, 3, 0, 2, 1, 0, 3, 1, 2};
uint16_t input[260];
uint16_t i, j, val, fecReg, fecOutput;
uint32_t intOutput;
uint16_t fec[520];
uint16_t interleaved[520];
uint16_t inputNum = 0, fecNum;
uint16_t checksum;
uint16_t length;

#define DEBUG_PRINTF(a, b...) {}

void fec_encode() {
  //--------------------------------------------------------------------------------
  // Example code
  length = 3;
  input[0] = length;
  input[1] = 1;
  input[2] = 2;
  input[3] = 3;
  inputNum = length + 1;
  DEBUG_PRINTF("Input: [%5d uint8_ts]\n", inputNum);

  for (int i = 0; i < inputNum; i++)
    DEBUG_PRINTF("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  DEBUG_PRINTF("\n\n");

  // Generate CRC
  checksum = 0xFFFF;  // Init value for CRC calculation
  for (i = 0; i <= input[0]; i++) checksum = culCalcCRC(input[i], checksum);
  input[inputNum++] = checksum >> 8;      // CRC1
  input[inputNum++] = checksum & 0x00FF;  // CRC0
  DEBUG_PRINTF("Appended CRC: [%5d uint8_ts]\n", inputNum);
  for (i = 0; i < inputNum; i++)
    DEBUG_PRINTF("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  DEBUG_PRINTF("\n\n");

  // Append Trellis Terminator
  input[inputNum] = 0x0B;
  input[inputNum + 1] = 0x0B;
  fecNum = 2 * ((inputNum / 2) + 1);
  DEBUG_PRINTF("Appended Trellis terminator: [%5d uint8_ts]\n", fecNum);
  for (i = 0; i < fecNum; i++)
    DEBUG_PRINTF("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
  DEBUG_PRINTF("\n\n");

  // FEC encode
  fecReg = 0;
  for (i = 0; i < fecNum; i++) {
    fecReg = (fecReg & 0x700) | (input[i] & 0xFF);
    fecOutput = 0;
    for (j = 0; j < 8; j++) {
      fecOutput = (fecOutput << 2) | fecEncodeTable[fecReg >> 7];
      fecReg = (fecReg << 1) & 0x7FF;
    }
    fec[i * 2] = fecOutput >> 8;
    fec[i * 2 + 1] = fecOutput & 0xFF;
  }
  DEBUG_PRINTF("FEC encoder output: [%5d uint8_ts]\n", fecNum * 2);

  for (i = 0; i < fecNum * 2; i++)
    DEBUG_PRINTF("%02X%s", fec[i], (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " ");
  DEBUG_PRINTF("\n\n");

  // Perform interleaving
  for (i = 0; i < fecNum * 2; i += 4) {
    intOutput = 0;
    for (j = 0; j < 4 * 4; j++)
      intOutput = (intOutput << 2) |
                  ((fec[i + (~j & 0x03)] >> (2 * ((j & 0x0C) >> 2))) & 0x03);
    interleaved[i] = (intOutput >> 24) & 0xFF;
    interleaved[i + 1] = (intOutput >> 16) & 0xFF;
    interleaved[i + 2] = (intOutput >> 8) & 0xFF;
    interleaved[i + 3] = (intOutput >> 0) & 0xFF;
  }
  DEBUG_PRINTF("Interleaver output: [%5d uint8_ts]\n", fecNum * 2);

  for (i = 0; i < fecNum * 2; i++)
    DEBUG_PRINTF("%02X%s", interleaved[i],
           (i % 16 == 15) ? "\n"
           : (i % 4 == 3) ? " "
                          : " ");
  DEBUG_PRINTF("\n\n");
}

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
#define MAX_PACKET_SIZE 64
    uint8_t packet[64];
    uint8_t fecWorkspace[(MAX_PACKET_SIZE + 4) * 2];
    uint8_t radioSendLen = 0;

    // fill packet with some bs
    for (int i = 0; i < sizeof(packet); i++) packet[i] = rand();

    {
      unsigned long t1 = DWT->CYCCNT;
      //      radioSendLen = ao_fec_encode(packet, sizeof(packet),
      //      fecWorkspace);
      fec_encode();
//      HAL_Delay(30);
      unsigned long t2 = DWT->CYCCNT;
      printf("Tx dt: %lu ms\n",
             (long unsigned int)((t2 - t1) / SystemCoreClock * 1e6));
    }

    {
      uint8_t packet_decoded[sizeof(packet)];
      uint32_t start = HAL_GetTick();
      ao_fec_decode(fecWorkspace, radioSendLen, packet_decoded,
                    sizeof(packet_decoded), 0);
      uint32_t end = HAL_GetTick();
      printf("Rx dt: %lu ms\n", end - start);
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
