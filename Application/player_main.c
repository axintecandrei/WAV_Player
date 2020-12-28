/*
 * player_main.c
 *
 *  Created on: 15 aug. 2020
 *      Author: Andrei Axinte
 */
#include <player_main.h>

/* #define audio_path "harvest.wav" */
/* #define audio_path "lightv.wav" */
/* #define audio_path "sample.wav" */
/* #define audio_path "110.wav" */
/* #define audio_path "110v.wav" */
char  * audio_path[2] =
{
   "opt1.wav",
   "opt2.wav"
};
extern ApplicationTypeDef  USBState;
extern char                USBHPath[4]; /* USBH logical drive path */

/* FatFS variable */
FATFS  AudioStorageFATFS;


static uint8_t ABSinfo_ProcessCommands(void);
static void ABSinfo_ProcessVolume(void);

void ABSinfo_Init_s(void);
void ABSinfo_Idle_s(void);
void ABSinfo_Prepare_s(void);
void ABSinfo_Play_s(void);
void ABSinfo_Error_s(void);
void ABSinfo_Disconnect_s(void);


void ABSinfo_INIT(void)
{
   ABSinfo.States = Init;
   ABSinfo.StateMachine[0] = ABSinfo_Init_s;
   ABSinfo.StateMachine[1] = ABSinfo_Idle_s;
   ABSinfo.StateMachine[2] = ABSinfo_Prepare_s;
   ABSinfo.StateMachine[3] = ABSinfo_Play_s;
   ABSinfo.StateMachine[4] = ABSinfo_Error_s;
   ABSinfo.SelectOption = 0;
   ABSinfo.Enable = 0;
   ABSinfo.ABS_trigger = 0;
   wavPlayer_init();
   BlueLed_Off();
   RedLed_Off();
}

void Player_MAIN(void)
{
   MX_USB_HOST_Process();
/*USB disk internal states.
 *  If there is no memory connected there is nothing to be played from*/
   switch (USBState)
   {
   case APPLICATION_READY:
      ABSinfo.StateMachine[ABSinfo.States]();
      break;
   case APPLICATION_START:
      if(f_mount(&AudioStorageFATFS, (TCHAR const*)USBHPath, 0) == FR_OK)
      {
         BlueLed_On();
      }
      else
      {
    	  RedLed_On();
      }

      break;
   case APPLICATION_DISCONNECT:
	   ABSinfo_Disconnect_s();
	   BlueLed_Off();
	   break;
   case APPLICATION_IDLE:
	   BlueLed_Off();
      /*reset all internal stats of ABSinfo state machine and players state machine
       * check the commands, if any arrive, set error*/
      break;
   }

}

static uint8_t ABSinfo_ProcessCommands(void)
{
   uint8_t  enable, selectOption;
   uint8_t  commands_status = 0;

   /*get main switch state
    * 0 - Sound information not needed
    * 1 - Sound information needed, prepare everything*/
   enable = HAL_GPIO_ReadPin(OnOff_ABSinfo_GPIO_Port,OnOff_ABSinfo_Pin);
   ABSinfo.Enable = enable;
   /*get select switch state
    * 0 - Option #1
    * 1 - Option #2 */
   selectOption = HAL_GPIO_ReadPin(Sel_audio_option_GPIO_Port,Sel_audio_option_Pin);

   /*get ABS trigger and set player state*/
   ABSinfo.ABS_trigger = HAL_GPIO_ReadPin(ABS_trigger_GPIO_Port,ABS_trigger_Pin);


   commands_status = (selectOption<<2) |(ABSinfo.ABS_trigger<<1) | enable;
   return commands_status;

}

static void ABSinfo_ProcessVolume(void)
{
   uint16_t         adc_value = 0;
   static uint16_t  adc_value_prev = 0;


   /*read the pot*/
   adc_value = ADC_GetSingleConv();

   /*filter so the audio output would not flicker*/
   if (ABSolut(adc_value - adc_value_prev) > 10)
   {
      wavPlayer.volume  = ((float)adc_value/4096.0F);
      adc_value_prev = adc_value;
   }
   else
   {
      wavPlayer.volume  = ((float)adc_value_prev/4096.0F);
   }
}
/*
 * First state entered after power on
   Wait for commands
   It will be visited once per ign cycle
 * */
void ABSinfo_Init_s(void)
{
   uint8_t  commands = 0;


   commands = ABSinfo_ProcessCommands();
   ABSinfo.SelectOption = commands>>2;
   if ((commands & Triggered) == Triggered)
   {
      ABSinfo.States = Prepare;
   }
}
/*
 * Pause or standby
 * Visited if the Commands off (00/01/10) but previously the state was Play
 * If AudioOption is different: go into Prepare state to reopen the proper file and stop current dma transmit
*/
void ABSinfo_Idle_s(void)
{
   uint8_t  commands = 0;


   commands = ABSinfo_ProcessCommands();

   if ((commands & Triggered) == Triggered)
   {
      if (ABSinfo.SelectOption != (commands>>2))
      {
         ABSinfo.States = Prepare;
         ABSinfo.SelectOption = commands>>2;
         wavPlayer_stop();
      }
      else
      {
         ABSinfo.States = Play;
         wavPlayer_resume();
      }
   }
}

void ABSinfo_Prepare_s(void)
{
   if (wavPlayer_fileSelect(audio_path[ABSinfo.SelectOption]))
   {
      wavPlayer_play();
      ABSinfo.States = Play;
   }
   else
   {
      /*error; file could not be opened*/
      ABSinfo.States = Error;
   }
}

void ABSinfo_Play_s(void)
{
   PLAYER_CONTROL_e player_state;
   ABSinfo_ProcessVolume();
   player_state = wavPlayer_process();

   if (ABSinfo.SelectOption == 0) /*in case "ABS like" sound option is selected play as long the trigger is active*/
   {
	   if((ABSinfo_ProcessCommands() & Triggered) < Triggered)
	   {
	      ABSinfo.States = Idle;
	      wavPlayer_pause();
	   }
   }else /*in case "Assistent voice" sound option is selected play until the end of file if duration of ABS event is sorter than the file duration*/
   {
	   if(((ABSinfo_ProcessCommands() & Triggered) < Triggered) &&  (player_state == PLAYER_CONTROL_EndOfFile))
	   {
	      ABSinfo.States = Idle;
	      wavPlayer_pause();
	   }
   }

}
void ABSinfo_Disconnect_s(void)
{
	wavPlayer_stop();
	ABSinfo.States = Init;
}

void ABSinfo_Error_s(void)
{
	RedLed_On();
}

