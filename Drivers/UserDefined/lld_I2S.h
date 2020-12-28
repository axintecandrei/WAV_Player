/*
 * lld_I2S.h
 *
 *  Created on: 15 aug. 2020
 *      Author: Andrei Axinte
 */

#ifndef USERDEFINED_LLD_I2S_H_
#define USERDEFINED_LLD_I2S_H_

#include "stm32f4xx_hal.h"

#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB


/* Audio library defines */
#define DMA_MAX_SZE                 0xFFFF
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE) ? (_X_) : DMA_MAX_SZE)
#define AUDIODATA_SIZE              2   /* 16-bits audio data size */

I2S_HandleTypeDef  hAudioI2S;
DMA_HandleTypeDef  hdma_spi3_tx;

void MX_DMA_Init(void);
void LLD_I2S_INIT(uint32_t audioFreq);
#endif /* USERDEFINED_LLD_I2S_H_ */

