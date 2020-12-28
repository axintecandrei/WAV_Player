/*
 * lld_inputs.c
 *
 *  Created on: 15 aug. 2020
 *      Author: Andrei Axinte
 */
#include "lld_inputs.h"

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void MX_ADC1_Init(void)
{

   /* USER CODE BEGIN ADC1_Init 0 */

   /* USER CODE END ADC1_Init 0 */

   ADC_ChannelConfTypeDef  sConfig =
   {
      0
   };


   /* USER CODE BEGIN ADC1_Init 1 */

   /* USER CODE END ADC1_Init 1 */
   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
   */
   hadc1.Instance = ADC1;
   hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
   hadc1.Init.Resolution = ADC_RESOLUTION_12B;
   hadc1.Init.ScanConvMode = ENABLE;
   hadc1.Init.ContinuousConvMode = DISABLE;
   hadc1.Init.DiscontinuousConvMode = DISABLE;
   hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
   hadc1.Init.NbrOfConversion = 1;
   hadc1.Init.DMAContinuousRequests = DISABLE;
   hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
   if (HAL_ADC_Init(&hadc1) != HAL_OK)
   {
      /* Error_Handler(); */
   }

   /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
   sConfig.Channel = ADC_CHANNEL_0;
   sConfig.Rank = 1;
   sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
   if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
   {
      /* Error_Handler(); */
   }

   /* USER CODE BEGIN ADC1_Init 2 */
   ADC1->CR2 |= 1;
   /* USER CODE END ADC1_Init 2 */

}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
   GPIO_InitTypeDef  GPIO_InitStruct =
   {
      0
   };


   if(hadc->Instance == ADC1)
   {
      /* USER CODE BEGIN ADC1_MspInit 0 */

      /* USER CODE END ADC1_MspInit 0 */
      /* Peripheral clock enable */
      __HAL_RCC_ADC1_CLK_ENABLE();

      __HAL_RCC_GPIOA_CLK_ENABLE();
      /**ADC1 GPIO Configuration
      PA0-WKUP     ------> ADC1_IN0
      */
      GPIO_InitStruct.Pin = Volume_adc_pin_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(Volume_adc_pin_GPIO_Port, &GPIO_InitStruct);

      /* USER CODE BEGIN ADC1_MspInit 1 */

      /* USER CODE END ADC1_MspInit 1 */
   }
}


uint16_t ADC_GetSingleConv(void)
{
   /*Start conversion*/
   ADC1->CR2 |= 1 << 30;

   /*Wait till conversion done*/
   while(!((ADC1->SR & 0x02)>>1)) ;

   /*Return the converted data*/
   return ADC1->DR;
}

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
   if(hadc->Instance == ADC1)
   {
      /* USER CODE BEGIN ADC1_MspDeInit 0 */

      /* USER CODE END ADC1_MspDeInit 0 */
      /* Peripheral clock disable */
      __HAL_RCC_ADC1_CLK_DISABLE();

      /**ADC1 GPIO Configuration
      PA0-WKUP     ------> ADC1_IN0
      */
      HAL_GPIO_DeInit(Volume_adc_pin_GPIO_Port, Volume_adc_pin_Pin);

      /* USER CODE BEGIN ADC1_MspDeInit 1 */

      /* USER CODE END ADC1_MspDeInit 1 */
   }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStruct =
   {
      0
   };


   /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOH_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();

   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(Blue_LED_GPIO_Port, Blue_LED_Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);

   /*Configure GPIO pin : ABS_trigger_Pin */
   GPIO_InitStruct.Pin = ABS_trigger_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT; /* GPIO_MODE_IT_FALLING; */
   GPIO_InitStruct.Pull = GPIO_PULLDOWN;//GPIO_NOPULL;
   HAL_GPIO_Init(ABS_trigger_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : GPIO_MODE_INPUT_Pin */
   GPIO_InitStruct.Pin = OnOff_ABSinfo_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_PULLDOWN;
   HAL_GPIO_Init(OnOff_ABSinfo_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : Sel_audio_option_Pin */
   GPIO_InitStruct.Pin = Sel_audio_option_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_PULLDOWN;
   HAL_GPIO_Init(Sel_audio_option_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : USB_Connect_LED_Pin */
   GPIO_InitStruct.Pin = Blue_LED_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(Blue_LED_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : USB_Connect_LED_Pin */
   GPIO_InitStruct.Pin = Red_LED_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(Red_LED_GPIO_Port, &GPIO_InitStruct);
}


/*User Functions*/

void RedLed_On()
{
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
}

void RedLed_Off()
{
	HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
}

void BlueLed_On()
{
	HAL_GPIO_WritePin(Blue_LED_GPIO_Port, Blue_LED_Pin, GPIO_PIN_SET);
}

void BlueLed_Off()
{
	HAL_GPIO_WritePin(Blue_LED_GPIO_Port, Blue_LED_Pin, GPIO_PIN_RESET);
}
