/*
 * wav_player.h
 *
 *  Created on: 17 Apr 2020
 *      Author: A
 */

#ifndef WAVDECODER_H_
#define WAVDECODER_H_

#include <stdbool.h>
#include <stdint.h>
#include "fatfs.h"
#include "lld_I2S.h"

/*Defines */

#define AUDIO_BUFFER_SIZE  (4096)
#define WAV_HEADER_OFFSET  (44)

/* WAV Player process states */
typedef enum
{
   PLAYER_CONTROL_Idle = 0,
   PLAYER_CONTROL_HalfBuffer,
   PLAYER_CONTROL_FullBuffer,
   PLAYER_CONTROL_EndOfFile,
}PLAYER_CONTROL_e;


typedef struct wavPlayer_s
{
   UINT bytesRead;
   uint32_t DataSize;       /*size of data; actual audio samples - in bytes*/
   __IO uint32_t audioRemainSize;
   uint32_t samplingFreq;
   uint32_t bitRate;
   float volume;
   __IO PLAYER_CONTROL_e playerControlSM;
   /* uint8_t audioBuffer[AUDIO_BUFFER_SIZE]; */
}wavPlayer_t;

typedef struct
{
   uint32_t ChunkID;        /* 0 */
   uint32_t FileSize;       /* 4 */
   uint32_t FileFormat;     /* 8 */
   uint32_t SubChunk1ID;    /* 12 */
   uint32_t SubChunk1Size;  /* 16*/
   uint16_t AudioFormat;    /* 20 */
   uint16_t NbrChannels;    /* 22 */
   uint32_t SampleRate;     /* 24 */

   uint32_t ByteRate;       /* 28 */
   uint16_t BlockAlign;     /* 32 */
   uint16_t BitPerSample;   /* 34 */
   uint32_t SubChunk2ID;    /* 36 */
   uint32_t SubChunk2Size;  /* 40 */

}WAV_HeaderTypeDef;  /*total of 44 bytes*/

/**
 * @brief Select WAV file to play
 * @retval returns true when file is found in USB Drive
 */
bool wavPlayer_fileSelect(const char* filePath);

/**
 * @brief WAV File Play
 */
void wavPlayer_play(void);

/**
 * @brief WAV stop
 */
void wavPlayer_stop(void);

/**
 * @brief Process WAV
 */
PLAYER_CONTROL_e wavPlayer_process(void);


/**
 * @brief WAV pause/resume
 */
void wavPlayer_repeat(void);
void wavPlayer_stop(void);
void wavPlayer_pause(void);
void wavPlayer_resume(void);
void wavPlayer_init(void);
wavPlayer_t  wavPlayer;

#endif /* WAVDECODER_H_ */

