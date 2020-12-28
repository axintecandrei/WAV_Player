/*
 * lld_I2S.c
 *
 *  Created on: 15 aug. 2020
 *      Author: Andrei Axinte
 */

#include "lld_I2S.h"

/* I2S PLL parameters for different I2S Sampling Frequency */
const uint32_t  I2SFreq[8] =
{
   8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000
};
const uint32_t  I2SPLLN[8] =
{
   256, 429, 213, 429, 426, 271, 258, 344
};
const uint32_t  I2SPLLR[8] =
{
   5, 4, 4, 4, 4, 6, 3, 1
};

static void lld_I2S_pllClockConfig(uint32_t audioFreq);
static void lld_I2S_freqUpdate(uint32_t AudioFreq);
/* I2S Audio library function definitions */
/**
 * @brief Initialises I2S Audio settings
 * @param audioFreq - WAV file Audio sampling rate (44.1KHz, 48KHz, ...)
 * @param volume - CS43L22 Codec volume settings (0 - 100)
 * @retval state - true: Successfully, false: Failed
 */
void LLD_I2S_INIT(uint32_t audioFreq)
{
   /*Update PLL Clock Frequency setting*/
   lld_I2S_pllClockConfig(audioFreq);
   /*Update I2S peripheral sampling frequency*/
   lld_I2S_freqUpdate(audioFreq);
}



/* Static functions */

/**
 * @brief I2S Clock Config
 */
static void lld_I2S_pllClockConfig(uint32_t audioFreq)
{
   RCC_PeriphCLKInitTypeDef  rccclkinit;
   uint8_t                   index = 0, freqindex = 0xFF;


   for(index = 0; index < 8; index++)
   {
      if(I2SFreq[index] == audioFreq)
      {
         freqindex = index;
      }
   }
   /* Enable PLLI2S clock */
   HAL_RCCEx_GetPeriphCLKConfig(&rccclkinit);
   /* PLLI2S_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
   if ((freqindex & 0x7) == 0)
   {
      /* I2S clock config
      PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) × (PLLI2SN/PLLM)
      I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      rccclkinit.PeriphClockSelection = RCC_PERIPHCLK_I2S;
      rccclkinit.PLLI2S.PLLI2SN = I2SPLLN[freqindex];
      rccclkinit.PLLI2S.PLLI2SR = I2SPLLR[freqindex];
      HAL_RCCEx_PeriphCLKConfig(&rccclkinit);
   }
   else
   {
      /* I2S clock config
      PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) × (PLLI2SN/PLLM)
      I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      rccclkinit.PeriphClockSelection = RCC_PERIPHCLK_I2S;
      rccclkinit.PLLI2S.PLLI2SN = 258;
      rccclkinit.PLLI2S.PLLI2SR = 3;
      HAL_RCCEx_PeriphCLKConfig(&rccclkinit);
   }
}

/**
 * @brief update I2S peripheral with selected Sampling Frequency
 */
static void lld_I2S_freqUpdate(uint32_t AudioFreq)
{
   /* Initialize the hAudioOutI2s Instance parameter */
   hAudioI2S.Instance         = SPI3;

   /* Disable I2S block */
   __HAL_I2S_DISABLE(&hAudioI2S);
   CLEAR_BIT(hAudioI2S.Instance->I2SCFGR, SPI_I2SCFGR_I2SE);

   /* I2S3 peripheral configuration */
   hAudioI2S.Init.Mode        = I2S_MODE_MASTER_TX;
   hAudioI2S.Init.Standard    = I2S_STANDARD_PHILIPS;
   hAudioI2S.Init.DataFormat  = I2S_DATAFORMAT_16B;
   hAudioI2S.Init.MCLKOutput  = I2S_MCLKOUTPUT_DISABLE;
   hAudioI2S.Init.AudioFreq   = AudioFreq;
   hAudioI2S.Init.CPOL        = I2S_CPOL_LOW;
   hAudioI2S.Init.ClockSource = I2S_CLOCK_PLL;
   hAudioI2S.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
   /* Initialize the I2S peripheral with the structure above */
   HAL_I2S_Init(&hAudioI2S);
}

/**
* @brief I2S MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s)
{
   GPIO_InitTypeDef  GPIO_InitStruct =
   {
      0
   };


   if(hi2s->Instance == SPI3)
   {
      /* USER CODE BEGIN SPI3_MspInit 0 */

      /* USER CODE END SPI3_MspInit 0 */
      /* Peripheral clock enable */
      __HAL_RCC_SPI3_CLK_ENABLE();

      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();
      /**I2S3 GPIO Configuration
      PA4     ------> I2S3_WS
      PC10     ------> I2S3_CK
      PC12     ------> I2S3_SD
      */
      GPIO_InitStruct.Pin = GPIO_PIN_4;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      /* I2S3 DMA Init */
      /* SPI3_TX Init */
      hdma_spi3_tx.Instance = DMA1_Stream5;
      hdma_spi3_tx.Init.Channel = DMA_CHANNEL_0;
      hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
      hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      hdma_spi3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
      hdma_spi3_tx.Init.Mode = DMA_CIRCULAR;
      hdma_spi3_tx.Init.Priority = DMA_PRIORITY_HIGH;
      hdma_spi3_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
      hdma_spi3_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
      hdma_spi3_tx.Init.MemBurst = DMA_MBURST_SINGLE;
      hdma_spi3_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
      HAL_DMA_Init(&hdma_spi3_tx);

      __HAL_LINKDMA(hi2s,hdmatx,hdma_spi3_tx);

      /* USER CODE BEGIN SPI3_MspInit 1 */

      /* USER CODE END SPI3_MspInit 1 */
   }
}

/**
* @brief I2S MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspDeInit(I2S_HandleTypeDef* hi2s)
{
   if(hi2s->Instance == SPI3)
   {
      /* USER CODE BEGIN SPI3_MspDeInit 0 */

      /* USER CODE END SPI3_MspDeInit 0 */
      /* Peripheral clock disable */
      __HAL_RCC_SPI3_CLK_DISABLE();

      /**I2S3 GPIO Configuration
      PA4     ------> I2S3_WS
      PC10     ------> I2S3_CK
      PC12     ------> I2S3_SD
      */
      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_12);

      /* I2S3 DMA DeInit */
      HAL_DMA_DeInit(hi2s->hdmatx);
      /* USER CODE BEGIN SPI3_MspDeInit 1 */

      /* USER CODE END SPI3_MspDeInit 1 */
   }
}

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

   /* DMA controller clock enable */
   __HAL_RCC_DMA1_CLK_ENABLE();

   /* DMA interrupt init */
   /* DMA1_Stream5_IRQn interrupt configuration */
   HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

