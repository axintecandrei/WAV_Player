/*
 * main_init.c
 *
 *  Created on: 15 aug. 2020
 *      Author: uidp7521
 */
#include "main_init.h"

void MAIN_INIT(void)
{
   SystemClock_Config();
   /* Initialize all configured peripherals */
   MX_GPIO_Init();
   MX_DMA_Init();
   MX_ADC1_Init();
   MX_FATFS_Init();
   MX_USB_HOST_Init();
   ABSinfo_INIT();
}

