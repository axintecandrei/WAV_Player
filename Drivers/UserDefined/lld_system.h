/*
 * lld_system.h
 *
 *  Created on: 15 aug. 2020
 *      Author: uidp7521
 */

#ifndef USERDEFINED_LLD_SYSTEM_H_
#define USERDEFINED_LLD_SYSTEM_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"



void SystemClock_Config(void);
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
#endif /* USERDEFINED_LLD_SYSTEM_H_ */

