/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "isr.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern HCD_HandleTypeDef  hhcd_USB_OTG_FS;
extern DMA_HandleTypeDef  hdma_spi3_tx;
extern TIM_HandleTypeDef  htim5;


/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
   /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

   /* USER CODE END DMA1_Stream5_IRQn 0 */
   HAL_DMA_IRQHandler(&hdma_spi3_tx);
   /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

   /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
   /* USER CODE BEGIN EXTI15_10_IRQn 0 */

   /* USER CODE END EXTI15_10_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
   /* USER CODE BEGIN EXTI15_10_IRQn 1 */

   /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
   /* USER CODE BEGIN TIM5_IRQn 0 */

   /* USER CODE END TIM5_IRQn 0 */
   HAL_TIM_IRQHandler(&htim5);
   /* USER CODE BEGIN TIM5_IRQn 1 */

   /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles USB On The Go FS global interrupt.
  */
void OTG_FS_IRQHandler(void)
{
   /* USER CODE BEGIN OTG_FS_IRQn 0 */

   /* USER CODE END OTG_FS_IRQn 0 */
   HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);
   /* USER CODE BEGIN OTG_FS_IRQn 1 */

   /* USER CODE END OTG_FS_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

