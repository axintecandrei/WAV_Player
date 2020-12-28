/*
 * lld_inputs.h
 *
 *  Created on: 15 aug. 2020
 *      Author: Andrei AxinteS
 */

#ifndef USERDEFINED_LLD_INPUTS_H_
#define USERDEFINED_LLD_INPUTS_H_

#include "stm32f4xx_hal.h"


#define ABS_trigger_Pin GPIO_PIN_0
#define ABS_trigger_GPIO_Port GPIOB
#define OnOff_ABSinfo_Pin GPIO_PIN_0
#define OnOff_ABSinfo_GPIO_Port GPIOC
#define Sel_audio_option_Pin GPIO_PIN_1
#define Sel_audio_option_GPIO_Port GPIOC
#define Volume_adc_pin_Pin GPIO_PIN_0
#define Volume_adc_pin_GPIO_Port GPIOA
#define Blue_LED_Pin GPIO_PIN_10
#define Blue_LED_GPIO_Port GPIOB
#define Red_LED_Pin GPIO_PIN_8
#define Red_LED_GPIO_Port GPIOA


ADC_HandleTypeDef  hadc1;

void MX_GPIO_Init(void);
void MX_ADC1_Init(void);
uint16_t ADC_GetSingleConv(void);
void RedLed_On();
void RedLed_Off();
void BlueLed_On();
void BlueLed_Off();
#endif /* USERDEFINED_LLD_INPUTS_H_ */

