/**
  ******************************************************************************
  * @file    I2S/I2S_Audio/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stlogo.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup I2S_Audio
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
/* Audio file size and start offset address are defined here since the audio wave file is 
   stored in Flash memory as a constant table of 16-bit data 
 */
#define AUDIO_FILE_SIZE               147500        /* Size of audio file */
#define AUDIO_START_OFFSET_ADDRESS    44            /* Offset relative to audio file header size */
#define AUDIO_FILE_ADDRESS            0x0800E300   /* Audio file address */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwCommand = AUDIO_PAUSE;
__IO uint32_t uwVolume = 70;
  uint8_t Volume_string[20] = {0};

uint32_t AudioTotalSize = 0xFFFF; /* This variable holds the total size of the audio file */
uint32_t AudioRemSize   = 0xFFFF; /* This variable holds the remaining data in audio file */
uint16_t* CurrentPos;              /* This variable holds the current position address of audio data */

/* Variable to indicate that push buttons will be used for switching between
   Headphone and Speaker output modes. */
uint32_t uwSpHpSwitch = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Display_ExampleDescription(void);
static void AudioPlay_SetHint(void);
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  WAVE_FormatTypeDef *waveformat =  NULL;

  /* STM32F107xC HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Initialize the LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /*##-1- Initialize the Keys Push buttons and LCD #####################*/

  /* Initialize the Push buttons */
  /* KEY push button used for Pause/Resume */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  /* WAKEUP push button used for Volume High or select headphone*/
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
  /* TAMPER push button used for Volume Low or select speaker*/

  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /* Initialize the LCD */
  BSP_LCD_Init();

  /*##-2- Display welcome messages on LCD ####################################*/  
  Display_ExampleDescription();

  /* Wait for Key push-button press before starting the Example */
  while (BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_RESET)
  {
  }
  
  /* Wait for Key push-button release before starting the Example */
  while (BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_SET)
  {
  }

  /*##-3- Display Example Template ###########################################*/  
  AudioPlay_SetHint();

  /*##-4- Turn on LEDs available on STM3210C-EVAL RevC Eval board ###################*/  
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);
  BSP_LED_On(LED3);
  BSP_LED_On(LED4);

  /* Retrieve Wave Sample rate */
  waveformat = (WAVE_FormatTypeDef*)AUDIO_FILE_ADDRESS;
  
  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOs...) */
  if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, uwVolume, waveformat->SampleRate) != AUDIO_OK)
  {
    /* Initialization Error */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 116, (uint8_t*)"Initialization problem", CENTER_MODE);
    Error_Handler();
  }
  else
  {
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"Audio Codec Ready", LEFT_MODE);
  }

  /*##-5- Display information related to control and Playback state #*/
  AudioPlay_DisplayInfos(waveformat);
  
  /*##-6- Start AUDIO playback #####################################*/  
  /*
  Normal mode description:
      Start playing the audio file (using DMA).
      Using this mode, the application can run other tasks in parallel since
      the DMA is handling the Audio Transfer instead of the CPU.
      The only task remaining for the CPU will be the management of the DMA
      Transfer Complete interrupt or the Half Transfer Complete interrupt in
      order to load again the buffer and to calculate the remaining data.
  Circular mode description:
     Start playing the file from a circular buffer, once the DMA is enabled it
     always run. User has to fill periodically the buffer with the audio data
     using Transfer complete and/or half transfer complete interrupts callbacks
     (BSP_AUDIO_OUT_TransferComplete_CallBack() or BSP_AUDIO_OUT_HalfTransfer_CallBack()...
     In this case the audio data file is smaller than the DMA max buffer
     size 65535 so there is no need to load buffer continuously or manage the
     transfer complete or Half transfer interrupts callbacks. 
   */
   
  /* Set the total number of data to be played (count in half-word) */
  AudioTotalSize = (AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS)/(waveformat->NbrChannels);  
  /* Set the current audio pointer position */
  CurrentPos = (uint16_t*)(AUDIO_FILE_ADDRESS + AUDIO_START_OFFSET_ADDRESS);
  /* Start the audio player */
  BSP_AUDIO_OUT_Play((uint16_t*)CurrentPos, (uint32_t)(AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS)); 
  /* Update the remaining number of data to be played */
  AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);   
  /* Update the current audio pointer position */
  CurrentPos += DMA_MAX(AudioTotalSize);
  /* Display the state on the screen */
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Playback on-going", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  /* Infinite loop */
  while(1)
  {
    /* Check on the Pause/Resume button */
    if (BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET)
    {
      /* Wait to avoid rebound */
      while(BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET);
      if(uwCommand == AUDIO_PAUSE)
      {
        BSP_AUDIO_OUT_Pause();
        /* Display the current state of the player */      
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Playback paused  ", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

        /* Next time Resume command should be processed */
        uwCommand = AUDIO_RESUME;

        /* Push buttons will be used to switch between Speaker and Headphone modes */
        uwSpHpSwitch = 1;
      }
      else
      {
        BSP_AUDIO_OUT_Resume();
        /* Display the current state of the player */
        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Playback on-going", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

        /* Next time Pause command should be processed */
        uwCommand = AUDIO_PAUSE;

        /* Push buttons will be used to control volume level */
        uwSpHpSwitch = 0;
      }
    }

    /* Check on the Volume Low button */
    if (BSP_PB_GetState(BUTTON_WAKEUP) == GPIO_PIN_SET)
    {
      /* Check if the current state is paused (push buttons are used for volume control or for
      speaker/headphone mode switching) */
      if (uwSpHpSwitch)
      {
        /* Set output to Speaker */
        BSP_AUDIO_OUT_SetOutputMode(OUTPUT_DEVICE_SPEAKER);

        /* Display the current state of the player */
        BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)" SPEAKER ", CENTER_MODE);
      }
      else
      {
        /* Wait to avoid rebound */
        while (BSP_PB_GetState(BUTTON_WAKEUP) == GPIO_PIN_SET);

        /* Decrease volume by 5% */
        if (uwVolume > 5)
        {
          uwVolume -= 5;
        }
        else
        {
          uwVolume = 0;
        }

        /* Apply the new volume to the codec */
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        sprintf((char *) Volume_string, " Volume : %lu%% ", uwVolume);
        BSP_LCD_DisplayStringAt(6, BSP_LCD_GetYSize()-60, Volume_string, RIGHT_MODE);
      }
    }

    /* Check on the Volume High button */
    if (BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_RESET)
    {
      /* Check if the current state is paused (push buttons are used for volume control or for
      speaker/headphone mode switching) */
      if (uwSpHpSwitch)
      {
        /* Set output to Headphone */
        BSP_AUDIO_OUT_SetOutputMode(OUTPUT_DEVICE_HEADPHONE);

        /* Display the current state of the player */
        BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"HEADPHONE", CENTER_MODE);
      }
      else
      {
        /* Wait to avoid rebound */
        while (BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_RESET);

        /* Increase volume by 5% */
        if (uwVolume < 95)
        {
          uwVolume += 5;
        }
        else
        {
          uwVolume = 100;
        }

        /* Apply the new volume to the codec */
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        sprintf((char *) Volume_string, " Volume : %lu%% ", uwVolume);
        BSP_LCD_DisplayStringAt(6, BSP_LCD_GetYSize()-60, Volume_string, RIGHT_MODE);
      }
    }

    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 100 ms delay */
    HAL_Delay(100);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 25000000
  *            HSE PREDIV1                    = 5
  *            HSE PREDIV2                    = 5
  *            PLL2MUL                        = 8
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Configure PLLs ------------------------------------------------------*/
  /* PLL2 configuration: PLL2CLK = (HSE / HSEPrediv2Value) * PLL2MUL = (25 / 5) * 8 = 40 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8 MHz */
  /* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */ 

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState              = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue        = RCC_HSE_PREDIV_DIV5;
  oscinitstruct.Prediv1Source         = RCC_PREDIV1_SOURCE_PLL2;
  oscinitstruct.PLL.PLLState          = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL            = RCC_PLL_MUL9;
  oscinitstruct.PLL2.PLL2State        = RCC_PLL2_ON;
  oscinitstruct.PLL2.PLL2MUL          = RCC_PLL2_MUL8;
  oscinitstruct.PLL2.HSEPrediv2Value  = RCC_HSE_PREDIV2_DIV5;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/**
  * @brief  Display main example message
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32F107xC", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Example", CENTER_MODE);
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2016", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 15, (uint8_t *)"Press Key push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 30, (uint8_t *)"to start :", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)"AUDIO playback Example", CENTER_MODE);
}

/**
  * @brief  Display AUDIO Playback Demo Hint
  * @param  None
  * @retval None
  */
static void AudioPlay_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"AUDIO Playback", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example provides basic", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"implementation of audio features", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"through the I2S peripheral ", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  /* Prepare LCD to display */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

/**
  * @brief  Display audio file and control information
  * @param  format : structure containing information of the audio file
  * @retval None
  */
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format)
{
  uint8_t string[50] = {0};

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  sprintf((char *) string, "Sampling frequency : %lu Hz", format->SampleRate);
  BSP_LCD_DisplayStringAt(20, 115, string, LEFT_MODE);

  if (format->NbrChannels == 2)
  {
    sprintf((char *) string, "Format : %d bits stereo", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 130, string, LEFT_MODE);
  }
  else if (format->NbrChannels == 1)
  {
    sprintf((char *) string, "Format : %d bits mono", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 130, string, LEFT_MODE);
  }
  
  sprintf((char *) Volume_string, " Volume : %lu%% ", uwVolume);
  BSP_LCD_DisplayStringAt(6, BSP_LCD_GetYSize()-60, Volume_string, RIGHT_MODE);
  BSP_LCD_DisplayStringAt(20, 200, (uint8_t *)"KEY: Pause/Resume playback", LEFT_MODE);
  BSP_LCD_DisplayStringAt(20, 215, (uint8_t *)"WAKEUP/TAMPER: Volume or Speaker/Headset", LEFT_MODE);   
}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm3210c_eval_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief  Manages the full Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  /* Calculate the remaining audio data in the file and the new size
     for the DMA transfer. If the Audio files size is less than the DMA max
     data transfer size, so there is no calculation to be done, just restart
     from the beginning of the file ... */
  /* Check if the end of file has been reached */
  if(AudioRemSize > 0)
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
    /* Set the current audio pointer position */
    CurrentPos = (uint16_t*)(AUDIO_FILE_ADDRESS + AUDIO_START_OFFSET_ADDRESS);
  /* Replay from the beginning */
    BSP_AUDIO_OUT_Play((uint16_t*)CurrentPos,  (uint32_t)(AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS));
    /* Update the remaining number of data to be played */
    AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);  
    /* Update the current audio pointer position */
    CurrentPos += DMA_MAX(AudioTotalSize);
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
  /* Generally this interrupt routine is used to load the buffer when
  a streaming scheme is used: When first Half buffer is already transferred load
  the new data to the first half of buffer while DMA is transferring data from
  the second half. And when Transfer complete occurs, load the second half of
  the buffer while the DMA is transferring from the first half ... */
  /*
    ...........
                   */
}

/**
  * @brief  Manages the DMA FIFO error event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  /* Display message on the LCD screen */
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"     DMA  ERROR     ");

  /* Stop the program with an infinite loop */
  while (1)
  {
  }

  /* could also generate a system reset to recover from the error */
  /* .... */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Stop the program with an infinite loop */
  while(1)
  {
  }

  /* could also generate a system reset to recover from the error */
  /* .... */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
