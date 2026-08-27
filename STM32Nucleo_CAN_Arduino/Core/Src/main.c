/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile bool canTxComplete = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
uint8_t Get_RandomData();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//CAN variable setup
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxData[8];
uint8_t RxData[8];

uint32_t TxMailbox;

uint8_t intMIN = 0;
uint8_t intMAX = 254;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	srand(time(NULL));
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */



  if (HAL_CAN_Start(&hcan1) != HAL_OK){
	  Error_Handler();
  }

  //Enable CAN interrupts (Receivem, Transmit, Errors)
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_ERROR | CAN_IT_LAST_ERROR_CODE | CAN_IT_BUSOFF);


  //CAN Tx Data
  TxHeader.DLC = 8; //Data length
  TxHeader.IDE = CAN_ID_STD; //Standard length Identifier
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.StdId = 0x446; //ID for the F446RE



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	//Blink LED if flag is set
	if (canTxComplete){

		canTxComplete = false;

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	}

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 18;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  //CAN Filter Config (Rx)
  CAN_FilterTypeDef canfilterconfig;

  canfilterconfig.FilterActivation = CAN_FILTER_ENABLE; //Activate Filter
  canfilterconfig.FilterBank = 10;  // anything between 0 to SlaveStartFilterBank
  canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0; //Any CAN frame msg that passes filter is placed in this buffer
  canfilterconfig.FilterIdHigh = 0x101<<5; //Filter only the Uno Q ID (101)
  canfilterconfig.FilterIdLow = 0x0000;
  canfilterconfig.FilterMaskIdHigh = 0x7FF<<5; //11 Bits
  canfilterconfig.FilterMaskIdLow = 0x0000;
  canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK; //Set filter to check ID+Mask
  canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canfilterconfig.SlaveStartFilterBank = 20;  // 13 to 27 are assigned to slave CAN (CAN 2) OR 0 to 12 are assgned to CAN1

  HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);


  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//an EXTI (User Button) press function for sending data to the Arduino via CAN
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if (GPIO_Pin == GPIO_PIN_13){

		HAL_StatusTypeDef canStatus;

		//CAN Data
		TxData[0] = Get_RandomData();
		TxData[1] = Get_RandomData();
		TxData[2] = Get_RandomData();
		TxData[3] = Get_RandomData();
		TxData[4] = Get_RandomData();
		TxData[5] = Get_RandomData();
		TxData[6] = Get_RandomData();
		TxData[7] = Get_RandomData();

		//UART
		char msg[100];

		canStatus = HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);

		if (canStatus == HAL_OK){

			//UART - Successful CAN Tx
			sprintf(msg, "[STM32]CAN QUEUED MSG ID=0x%03lX DLC=%lu DATA=%02X %02X %02X %02X %02X %02X %02X %02X Mailbox=%lu\r\n", TxHeader.StdId, TxHeader.DLC, TxData[0], TxData[1], TxData[2], TxData[3], TxData[4], TxData[5], TxData[6], TxData[7], TxMailbox);

		}
		else{

			//UART - Error
			sprintf(msg, "[STM32][ERROR] HAL Status=%d CAN Error=0x%08lX\r\n", canStatus, HAL_CAN_GetError(&hcan1));
		}


		HAL_UART_Transmit(&huart2,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);


	}


}

//CAN RANDOM DATA - Get a random data byte
uint8_t Get_RandomData(){

	uint8_t randomint = intMIN + rand() % (intMAX + 1 - intMIN);

	return randomint;
}

//CAN RECEIVE - Callback for receiving messages
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_RxFifo0MsgPendingCallback could be implemented in the
            user file
   */
  char msg[150];

  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK){

	  sprintf(msg, "[UNOQ][CAN RX] ID=0x%03lX DLC=%lu DATA=%02X %02X\r\n", RxHeader.StdId, RxHeader.DLC, RxData[0], RxData[1]);

  }
  else{

	  sprintf(msg, "[STM32][ERROR] Rx Error=0x%08lX\r\n", HAL_CAN_GetError(&hcan1));
  }



  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}


//CAN MAILBOXES - CHECKING FOR MESSAGES SENT FROM STM32
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_TxMailbox0CompleteCallback could be implemented in the
            user file
   */
  	  //Set flag to true for LED
  	  canTxComplete = true;

  	  char msg[] = "[STM32][CAN TX COMPLETE] Mailbox 0\r\n";

      HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_TxMailbox0CompleteCallback could be implemented in the
            user file
   */
  	 //Set flag to true for LED
   	 canTxComplete = true;

  	 char msg[] = "[STM32][CAN TX COMPLETE] Mailbox 1\r\n";

     HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_TxMailbox0CompleteCallback could be implemented in the
            user file
   */
  	 //Set flag to true for LED
   	 canTxComplete = true;

  	 char msg[] = "[STM32][CAN TX COMPLETE] Mailbox 2\r\n";

  	 HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}



//HANDLE CAN ERRORS
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcan);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CAN_ErrorCallback could be implemented in the user file
   */
  uint32_t error = HAL_CAN_GetError(hcan);

  char msg[100];

  sprintf(msg, "[STM32][ERROR] HAL error = 0x%08lX\r\n", error);

  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg),HAL_MAX_DELAY);

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
