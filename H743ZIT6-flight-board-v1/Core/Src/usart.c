/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart8;
DMA_HandleTypeDef hdma_uart8_rx;

/* UART8 init function */
void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart8.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspInit 0 */

  /* USER CODE END UART8_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART8;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART8 clock enable */
    __HAL_RCC_UART8_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**UART8 GPIO Configuration
    PE0     ------> UART8_RX
    PE1     ------> UART8_TX
    */
    GPIO_InitStruct.Pin = GPS_RX_Pin|GPS_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* UART8 DMA Init */
    /* UART8_RX Init */
    hdma_uart8_rx.Instance = DMA1_Stream5;
    hdma_uart8_rx.Init.Request = DMA_REQUEST_UART8_RX;
    hdma_uart8_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart8_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart8_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart8_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart8_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart8_rx.Init.Mode = DMA_CIRCULAR;
    hdma_uart8_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_uart8_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart8_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart8_rx);

  /* USER CODE BEGIN UART8_MspInit 1 */

  /* USER CODE END UART8_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspDeInit 0 */

  /* USER CODE END UART8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART8_CLK_DISABLE();

    /**UART8 GPIO Configuration
    PE0     ------> UART8_RX
    PE1     ------> UART8_TX
    */
    HAL_GPIO_DeInit(GPIOE, GPS_RX_Pin|GPS_TX_Pin);

    /* UART8 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
  /* USER CODE BEGIN UART8_MspDeInit 1 */

  /* USER CODE END UART8_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
