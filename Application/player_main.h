/*
 * player_main.h
 *
 *  Created on: 15 aug. 2020
 *      Author: Andrei Axinte
 */

#ifndef PLAYER_MAIN_H_
#define PLAYER_MAIN_H_

#include "lld_inputs.h"
#include "wavDecoder.h"
#include "fatfs.h"
#include "usb_host.h"

#define ABSolut(x)   (((x) < 0) ? (-(x)) : (x))

typedef void (*state_func_ptr)(void);

typedef enum
{
   Disabled,
   Enabled,
   Triggered = 3
}commands_e;

typedef enum
{
   Init = 0,
   Idle,
   Prepare,
   Play,
   Error
}app_states_e;

typedef struct
{
   app_states_e States;
   volatile uint8_t ABS_trigger;
   uint8_t SelectOption;
   uint8_t Enable;
   state_func_ptr StateMachine[5];
}ABSinfo_t;

void Player_MAIN(void);
void ABSinfo_INIT(void);


ABSinfo_t  ABSinfo;

#endif /* PLAYER_MAIN_H_ */

