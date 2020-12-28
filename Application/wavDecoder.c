/*
 * wav_player.c
 *
 *  Created on: 17 Apr 2020
 *      Author: Andrei Axinte
 */

#include "wavDecoder.h"

/* WAV File System variables */
static FIL  wavFile;
uint8_t     audioBuffer[AUDIO_BUFFER_SIZE];

static void wavPlayer_setVolume(float volume, int16_t * data, uint16_t size);
static void wavPlayer_reset(void);

void wavPlayer_init(void)
{
   wavPlayer_reset();
}
/**
 * @brief Select WAV file to play
 * @retval returns true when file is found in USB Drive
 */
bool wavPlayer_fileSelect(const char* filePath)
{
   WAV_HeaderTypeDef  wavHeader;

   /* Open WAV file */
   if(f_open(&wavFile, filePath, FA_READ) != FR_OK)
   {
      return false;
   }

   /* Read WAV file Header */
   f_read(&wavFile, &wavHeader, WAV_HEADER_OFFSET, &wavPlayer.bytesRead);
   /* Get audio data size */
   wavPlayer.DataSize = wavHeader.FileSize;
   /* Play the WAV file with frequency specified in header */
   wavPlayer.samplingFreq = wavHeader.SampleRate;
   wavPlayer.bitRate = wavHeader.BitPerSample;
   return true;
}

/**
 * @brief WAV File Play
 */
void wavPlayer_play(void)
{

   /* Initialise I2S Audio Sampling settings */
   LLD_I2S_INIT(wavPlayer.samplingFreq);
   /* Read Audio data from USB Disk */
   f_lseek(&wavFile, WAV_HEADER_OFFSET);
   f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE, &wavPlayer.bytesRead);
   wavPlayer.audioRemainSize = wavPlayer.DataSize - wavPlayer.bytesRead;
   wavPlayer_setVolume(wavPlayer.volume,(int16_t *)&audioBuffer[0],wavPlayer.bytesRead/2);
   /* Start playing the WAV */
   HAL_I2S_Transmit_DMA(&hAudioI2S, (uint16_t *)&audioBuffer[0], DMA_MAX(AUDIO_BUFFER_SIZE/AUDIODATA_SIZE));
}

/**
 * @brief Process WAV
 */
PLAYER_CONTROL_e wavPlayer_process(void)
{

   switch(wavPlayer.playerControlSM)
   {
   case PLAYER_CONTROL_Idle:
      break;

   case PLAYER_CONTROL_HalfBuffer:
      wavPlayer.bytesRead = 0;
      wavPlayer.playerControlSM = PLAYER_CONTROL_Idle;
      f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE/2, &wavPlayer.bytesRead);
      wavPlayer_setVolume(wavPlayer.volume,(int16_t *)&audioBuffer[0],wavPlayer.bytesRead/2);
      if(wavPlayer.audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
      {
         wavPlayer.audioRemainSize -= wavPlayer.bytesRead;

      }
      else
      {
         wavPlayer.audioRemainSize = 0;
         wavPlayer.playerControlSM = PLAYER_CONTROL_EndOfFile;
      }

      break;

   case PLAYER_CONTROL_FullBuffer:
      wavPlayer.bytesRead = 0;
      wavPlayer.playerControlSM = PLAYER_CONTROL_Idle;
      f_read (&wavFile, &audioBuffer[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2, &wavPlayer.bytesRead);
      wavPlayer_setVolume(wavPlayer.volume,(int16_t *)&audioBuffer[(AUDIO_BUFFER_SIZE/2)],wavPlayer.bytesRead/2);
      if(wavPlayer.audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
      {
         wavPlayer.audioRemainSize -= wavPlayer.bytesRead;

      }
      else
      {
         wavPlayer.audioRemainSize = 0;
         wavPlayer.playerControlSM = PLAYER_CONTROL_EndOfFile;
      }

      break;

   case PLAYER_CONTROL_EndOfFile:
      wavPlayer_repeat();
      wavPlayer.playerControlSM = PLAYER_CONTROL_Idle;
      break;
   }
   return wavPlayer.playerControlSM;
}


/**
 * @brief WAV stop
 */
void wavPlayer_stop(void)
{
   f_close(&wavFile);
   wavPlayer_reset();
   wavPlayer.playerControlSM = PLAYER_CONTROL_Idle;
   HAL_I2S_DMAStop(&hAudioI2S);
}

/**
 * @brief WAV pause/resume
 */
void wavPlayer_pause(void)
{
   HAL_I2S_DMAPause(&hAudioI2S);
}

void wavPlayer_resume(void)
{
   HAL_I2S_DMAResume(&hAudioI2S);
}

void wavPlayer_repeat(void)
{
   f_lseek(&wavFile, WAV_HEADER_OFFSET);
   f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE, &wavPlayer.bytesRead);
   wavPlayer_setVolume(0,(int16_t *)&audioBuffer[0],wavPlayer.bytesRead/2);
   wavPlayer.audioRemainSize = wavPlayer.DataSize - wavPlayer.bytesRead;
}
static void wavPlayer_reset(void)
{
   wavPlayer.audioRemainSize = 0;
   wavPlayer.bytesRead = 0;
   wavPlayer.DataSize = 0;
   wavPlayer.volume = 0;
}

/**
 * @brief Set Volume
 */
static void wavPlayer_setVolume(float volume, int16_t * data, uint16_t size)
{
   uint16_t  idx;


   for(idx = 0; idx < size; idx++)
   {
      data[idx] = (int16_t)((float)(data[idx]) * volume);
   }
}

/**
 * @brief Half/Full transfer Audio callback for buffer management
 */

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
   if(hi2s->Instance == SPI3)
   {
      wavPlayer.playerControlSM = PLAYER_CONTROL_FullBuffer;
   }
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
   if(hi2s->Instance == SPI3)
   {
      wavPlayer.playerControlSM =  PLAYER_CONTROL_HalfBuffer ;
   }
}

