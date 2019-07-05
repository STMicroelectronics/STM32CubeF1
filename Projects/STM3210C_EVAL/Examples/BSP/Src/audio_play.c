/**
  ******************************************************************************
  * @file    BSP/Src/audio_play.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use AUDIO features for the play.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

}WAVE_FormatTypeDef;

/* Private define ------------------------------------------------------------*/
/* Audio file size and start address are defined here since the Audio file is 
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_ADDRESS    0x08015000   /* Audio file address */  
#define AUDIO_FILE_SIZE       (FLASH_BANK1_END - AUDIO_FILE_ADDRESS)
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t UserPressButton;
extern __IO uint32_t PauseResumeStatus;
extern __IO uint8_t UserOutputMode;
uint8_t UserOutputModePreviousState = OUTPUT_DEVICE_AUTO;

  
/* Variables used in norma mode to manage audio file during DMA transfer*/
uint32_t AudioTotalSize           = 0xFFFF; /* This variable holds the total size of the audio file */
uint32_t AudioRemSize             = 0xFFFF; /* This variable holds the remaining data in audio file */
uint16_t *CurrentPos ;             /* This variable holds the current position of audio pointer */

extern __IO uint8_t volume;
extern __IO uint8_t VolumeChange;

/* Private function prototypes -----------------------------------------------*/
static void AudioPlay_SetHint(void);
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio Hardware.
  *   The main objective of this test is to check the hardware connection of the 
  *   Audio peripheral.
  * @param  None
  * @retval None
  */
void AudioPlay_demo(void)
{  
  WAVE_FormatTypeDef *waveformat =  NULL;
  uint8_t Volume_string[20] = {0};

  AudioPlay_SetHint();

  /* Configuration of the EXTI for the joystick SEL push button for pause/resume */
  /* UP/DOWN push buttons for change the volume */
  BSP_JOY_Init(JOY_MODE_EXTI);
  
  /* Retrieve Wave Sample rate*/
  waveformat = (WAVE_FormatTypeDef*) AUDIO_FILE_ADDRESS;
  AudioPlay_DisplayInfos(waveformat);

  /* Initialize Audio Device */
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, volume, waveformat->SampleRate) != 0)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Audio Codec not detected", CENTER_MODE); 
    Error_Handler();
  }
  
  /* Set the total number of data to be played */
  AudioTotalSize = (AUDIO_FILE_SIZE / 2);
  /* Set the current audio pointer position */
  CurrentPos = (uint16_t *)(AUDIO_FILE_ADDRESS);

  /* Initialize Volume */
  if(BSP_AUDIO_OUT_SetVolume(volume) != 0)
  {
    Error_Handler();
  }

  /* Start the audio player */
  if(BSP_AUDIO_OUT_Play(CurrentPos,DMA_MAX((AudioTotalSize))) != 0)
  {
    Error_Handler();
  }

  /* Turn ON LED green: start of Audio file play */
  BSP_LED_On(LED_GREEN);

  /* Update the remaining number of data to be played */
  AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);  
  /* Update the current audio pointer position */
  CurrentPos += DMA_MAX(AudioTotalSize);

  BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Playback on-going", LEFT_MODE);
  sprintf((char *) Volume_string, " Volume : %d%% ", volume);
  BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, Volume_string, RIGHT_MODE);

  /* Infinite loop */
  while(!CheckForUserInput())
  { 
    if (PauseResumeStatus == PAUSE_STATUS)
    {
      /* Turn ON LED orange: Audio play in pause */
      BSP_LED_On(LED_ORANGE);
      
      /* Pause playing */
      if(BSP_AUDIO_OUT_Pause() != 0)
      {
        Error_Handler();
      }
      BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Playback paused  ", LEFT_MODE);
      PauseResumeStatus = IDLE_STATUS;
    }
    else if (PauseResumeStatus == RESUME_STATUS)
    {
      /* Turn OFF LED orange: Audio play running */
      BSP_LED_Off(LED_ORANGE);
      
      /* Resume playing */
      if(BSP_AUDIO_OUT_Resume() != 0)
      {
        Error_Handler();
      }
      BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Playback on-going", LEFT_MODE);
      PauseResumeStatus = IDLE_STATUS;
    }
  
    if (VolumeChange != 0)
    {
      VolumeChange = 0;
      if(BSP_AUDIO_OUT_SetVolume(volume) != 0)
      {
        Error_Handler();
      }
      sprintf((char *) Volume_string, " Volume : %d%% ", volume);
      BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, Volume_string, RIGHT_MODE);
    }

    if (UserOutputMode != UserOutputModePreviousState)
    {
      /* Change audio output */
      BSP_AUDIO_OUT_SetOutputMode(UserOutputMode);
      
      UserOutputModePreviousState = UserOutputMode;
    }
  }
  
  /* Reset the EXTI configuration for Joystick SEL, UP and DOWN push buttons */
  /* Configuration of the joystick in GPIO mode and no more EXTI */
  BSP_JOY_Init(JOY_MODE_GPIO);
  
  /* Stop Player before close Test */
  if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW) != AUDIO_OK)
  {
    /* Audio Stop error */
    Error_Handler();
  }
  else
  {
    /* Turn OFF LED green: stop of Audio file play */
    BSP_LED_Off(LED_GREEN);
    BSP_LED_Off(LED_ORANGE);
  }
}

/**
  * @brief  Display audio play demo hint
  * @param  None
  * @retval None
  */
static void AudioPlay_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Audio Play", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press KEY to stop playback", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Press SEL to pause/resume playback", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press UP/DOWN to change Volume", CENTER_MODE);   
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Display audio play demo hint
  * @param  format : structure containing informations of the file
  * @retval None
  */
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format)
{
  uint8_t string[50] = {0};

  sprintf((char *) string, "Sampling frequency : %ld Hz", format->SampleRate);
  BSP_LCD_DisplayStringAt(20, 100, string, CENTER_MODE);

  if (format->NbrChannels == 2)
  {
    sprintf((char *) string, "Format : %d bits stereo", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 115, string, CENTER_MODE);
  }
  else if (format->NbrChannels == 1)
  {
    sprintf((char *) string, "Format : %d bits mono", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 115, string, CENTER_MODE);
  }
  
  /* How to use Joystick for change speaker */
  sprintf((char *) string, "Joystick Right for speaker only");
  BSP_LCD_DisplayStringAt(20, 145, string, CENTER_MODE);
  sprintf((char *) string, "Joystick Left for headset only");
  BSP_LCD_DisplayStringAt(20, 160, string, CENTER_MODE);
}

/*--------------------------------
Callbacks implementation:
The callbacks prototypes are defined in the stm3210c_eval_audio.h file
and their implementation should be done in the user code if they are needed.
Below some examples of callback implementations.
--------------------------------------------------------*/
/**
* @brief  Calculates the remaining file size and new position of the pointer.
* @param  None
* @retval None
*/
void BSP_AUDIO_OUT_TransferComplete_CallBack()
{
  uint32_t replay = 0;
  
  if (AudioRemSize > 0)
  {
    /* Replay from the current position */
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)CurrentPos, DMA_MAX(AudioRemSize));
  
    /* Update the current pointer position */
    CurrentPos += DMA_MAX(AudioRemSize);        
  
    /* Update the remaining number of data to be played */
    AudioRemSize -= DMA_MAX(AudioRemSize);
  }
  else
  {
    /* Request to replay audio file from beginning */
    replay = 1;
  }
  
  /* Audio sample used for play*/
  if(replay == 1)
  {
    /* Replay from the beginning */
    /* Set the current audio pointer position */
    CurrentPos = (uint16_t *)(AUDIO_FILE_ADDRESS);
    /* Replay from the beginning */
    if (BSP_AUDIO_OUT_Play(CurrentPos,DMA_MAX(AudioTotalSize)) != 0)
    {
      Error_Handler();
    }

    /* Toggle Green Led, each time a replay is requested */
    BSP_LED_Toggle(LED_GREEN);
    
    /* Update the remaining number of data to be played */
    AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);  
    /* Update the current audio pointer position */
    CurrentPos += DMA_MAX(AudioTotalSize);
  }

}

/**
* @brief  Manages the DMA FIFO error interrupt.
* @param  None
* @retval None
*/
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  /* Stop the program with an infinite loop */
  Error_Handler();
}

/**
  * @}
  */ 


/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
