/**
  ******************************************************************************
  * @file    stm3210c_eval_audio.c
  * @author  MCD Application Team
  * @version V6.1.0
  * @date    14-April-2017
  * @brief   This file provides the Audio driver for the STM3210C-Eval 
  *          board.  
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================  
   [..] 
  (#) This driver supports STM32F107xC devices on STM3210C-Eval Kit:
       (++) to play an audio file (all functions names start by BSP_AUDIO_OUT_xxx)

  [..] 
   (#) PLAY A FILE:
       (++) Call the function BSP_AUDIO_OUT_Init(
              OutputDevice: physical output mode (OUTPUT_DEVICE_SPEAKER, 
                           OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_AUTO or 
                           OUTPUT_DEVICE_BOTH)
              Volume: initial volume to be set (0 is min (mute), 100 is max (100%)
              AudioFreq: Audio frequency in Hz (8000, 16000, 22500, 32000 ...)
              this parameter is relative to the audio file/stream type.
               )
           This function configures all the hardware required for the audio application 
           (codec, I2C, I2S, GPIOs, DMA and interrupt if needed). This function returns 0
           if configuration is OK.
           If the returned value is different from 0 or the function is stuck then the 
           communication with the codec (try to un-plug the power or reset device in this case).
              (+++) OUTPUT_DEVICE_SPEAKER: only speaker will be set as output for the 
                    audio stream.
              (+++) OUTPUT_DEVICE_HEADPHONE: only headphones will be set as output for 
                    the audio stream.
              (+++) OUTPUT_DEVICE_AUTO: Selection of output device is made through external 
                    switch (implemented into the audio jack on the evaluation board). 
                    When the Headphone is connected it is used as output. 
                    When the headphone is disconnected from the audio jack, the output is
                    automatically switched to Speaker.
              (+++) OUTPUT_DEVICE_BOTH: both Speaker and Headphone are used as outputs 
                    for the audio stream at the same time.
       (++) Call the function BSP_AUDIO_OUT_Play(
                pBuffer: pointer to the audio data file address
                Size: size of the buffer to be sent in Bytes
               )
              to start playing (for the first time) from the audio file/stream.
       (++) Call the function BSP_AUDIO_OUT_Pause() to pause playing 
       (++) Call the function BSP_AUDIO_OUT_Resume() to resume playing.
            Note. After calling BSP_AUDIO_OUT_Pause() function for pause, 
            only BSP_AUDIO_OUT_Resume() should be called for resume 
            (it is not allowed to call BSP_AUDIO_OUT_Play() in this case).
            Note. This function should be called only when the audio file is played 
            or paused (not stopped).
       (++) For each mode, you may need to implement the relative callback functions 
            into your code.
            The Callback functions are named BSP_AUDIO_OUT_XXXCallBack() and only 
            their prototypes are declared in the stm3210c_eval_audio.h file. 
            (refer to the example for more details on the callbacks implementations)
       (++) To Stop playing, to modify the volume level, the frequency or to mute, 
            use the functions BSP_AUDIO_OUT_Stop(), BSP_AUDIO_OUT_SetVolume(), 
            AUDIO_OUT_SetFrequency() BSP_AUDIO_OUT_SetOutputMode and BSP_AUDIO_OUT_SetMute().
       (++) The driver API and the callback functions are at the end of the 
            stm3210c_eval_audio.h file.
   
       (++) This driver provide the High Audio Layer: consists of the function API 
            exported in the stm3210c_eval_audio.h file (BSP_AUDIO_OUT_Init(), 
            BSP_AUDIO_OUT_Play() ...)
       (++) This driver provide also the Media Access Layer (MAL): which consists 
            of functions allowing to access the media containing/providing the 
            audio file/stream. These functions are also included as local functions into
            the stm3210c_eval_audio.c file (I2SOUT_Init()...)
       
  [..] 
                     ##### Known Limitations #####
  ==============================================================================  
   (#) When using the Speaker, if the audio file quality is not high enough, the 
       speaker output may produce high and uncomfortable noise level. To avoid 
       this issue, to use speaker output properly, try to increase audio file 
       sampling rate (typically higher than 48KHz).
       This operation will lead to larger file size.
       
   (#) Communication with the audio codec (through I2C) may be corrupted if it 
       is interrupted by some user interrupt routines (in this case, interrupts 
       could be disabled just before the start of communication then re-enabled 
       when it is over). Note that this communication is only done at the 
       configuration phase (BSP_AUDIO_OUT_Init() or BSP_AUDIO_OUT_Stop()) 
       and when Volume control modification is performed (BSP_AUDIO_OUT_SetVolume() 
       or BSP_AUDIO_OUT_SetMute()or BSP_AUDIO_OUT_SetOutputMode()). 
       When the audio data is played, no communication is required with the audio codec.
       
   (#) Parsing of audio file is not implemented (in order to determine audio file
       properties: Mono/Stereo, Data size, File size, Audio Frequency, Audio Data 
       header size ...). The configuration is fixed for the given audio file.
       
   (#) Mono audio streaming is not supported (in order to play mono audio streams, 
       each data should be sent twice on the I2S or should be duplicated on the 
       source buffer. Or convert the stream in stereo before playing).
       
   (#) Supports only 16-bit audio data size.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm3210c_eval_audio.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM3210C_EVAL
  * @{
  */

/** @defgroup STM3210C_EVAL_AUDIO STM3210C EVAL AUDIO
  * @brief This file includes the low layer audio driver available on STM3210C-Eval
  * eval board.
  * @{
  */
  
/** @defgroup STM3210C_EVAL_AUDIO_Private_Variables STM3210C EVAL AUDIO Private Variables
  * @{
  */
/*### PLAY ###*/
static AUDIO_DrvTypeDef         *pAudioDrv;
I2S_HandleTypeDef               hAudioOutI2s;

/**
  * @}
  */ 

/** @defgroup STM3210C_EVAL_AUDIO_Private_Function_Prototypes STM3210C EVAL AUDIO Private Function Prototypes
  * @{
  */ 
static void  I2SOUT_Init(uint32_t AudioFreq);
static void  I2SOUT_DeInit(void);

/**
  * @}
  */ 

/** @defgroup STM3210C_EVAL_AUDIO_OUT_Exported_Functions STM3210C EVAL AUDIO OUT Exported Functions
  * @{
  */ 

/**
  * @brief  Configure the audio peripherals.
  * @param  OutputDevice: OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
  uint8_t ret = AUDIO_ERROR;

  /* Disable I2S */
  I2SOUT_DeInit();
  
  /* I2S data transfer preparation:
  Prepare the Media to be used for the audio transfer from memory to I2S peripheral */
  hAudioOutI2s.Instance = I2SOUT;
  if(HAL_I2S_GetState(&hAudioOutI2s) == HAL_I2S_STATE_RESET)
  {
    /* Init the I2S MSP: this __weak function can be redefined by the application*/
    BSP_AUDIO_OUT_MspInit(&hAudioOutI2s, NULL);
  }  
  /* Configure the I2S peripheral */
  I2SOUT_Init(AudioFreq);

  if(((cs43l22_drv.ReadID(AUDIO_I2C_ADDRESS)) & CS43L22_ID_MASK) == CS43L22_ID)
  {  
    /* Initialize the audio driver structure */
    pAudioDrv = &cs43l22_drv; 
    ret = AUDIO_OK;
  }
  else
  {
    ret = AUDIO_ERROR;
  }
  
  if(ret == AUDIO_OK)
  {
    pAudioDrv->Init(AUDIO_I2C_ADDRESS, OutputDevice, Volume, AudioFreq);
  }
  
  return ret;
}

/**
  * @brief  De-initialize the audio peripherals.
  */
void BSP_AUDIO_OUT_DeInit(void)
{
  I2SOUT_DeInit();
  /* DeInit the I2S MSP : this __weak function can be rewritten by the application */
  BSP_AUDIO_OUT_MspDeInit(&hAudioOutI2s, NULL);
}

/**
  * @brief  Starts playing audio stream from a data buffer for a determined size. 
  * @param  pBuffer: Pointer to the buffer 
  * @param  Size: Number of audio data BYTES.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size)
{
  /* Call the audio Codec Play function */
  if(pAudioDrv->Play(AUDIO_I2C_ADDRESS, pBuffer, Size) != 0)
  {
    return AUDIO_ERROR;
  }
  else 
  {
    /* Update the Media layer and enable it for play */  
    HAL_I2S_Transmit_DMA(&hAudioOutI2s, pBuffer, DMA_MAX(Size)); 
    
    return AUDIO_OK;
  }
}

/**
  * @brief Sends n-Bytes on the I2S interface.
  * @param pData: pointer on data address 
  * @param Size: number of data to be written
  */
void BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size)
{
  HAL_I2S_Transmit_DMA(&hAudioOutI2s, pData, Size); 
}

/**
  * @brief  This function Pauses the audio file stream. In case
  *         of using DMA, the DMA Pause feature is used.
  * @note When calling BSP_AUDIO_OUT_Pause() function for pause, only
  *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of BSP_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behavior).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Pause(void)
{    
  /* Call the Audio Codec Pause/Resume function */
  if(pAudioDrv->Pause(AUDIO_I2C_ADDRESS) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Call the Media layer pause function */
    HAL_I2S_DMAPause(&hAudioOutI2s);
    
    /* Return AUDIO_OK if all operations are OK */
    return AUDIO_OK;
  }
}

/**
  * @brief  This function  Resumes the audio file stream.  
  * @note When calling BSP_AUDIO_OUT_Pause() function for pause, only
  *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of BSP_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behavior).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Resume(void)
{    
  /* Call the Audio Codec Pause/Resume function */
  if(pAudioDrv->Resume(AUDIO_I2C_ADDRESS) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Call the Media layer resume function */
    HAL_I2S_DMAResume(&hAudioOutI2s);
    /* Return AUDIO_OK if all operations are OK */
    return AUDIO_OK;
  }
}

/**
  * @brief  Stops audio playing and Power down the Audio Codec. 
  * @param  Option: could be one of the following parameters 
  *           - CODEC_PDWN_HW: completely shut down the codec (physically). 
  *                            Then need to reconfigure the Codec after power on.  
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option)
{
  /* Call DMA Stop to disable DMA stream before stopping codec */
  HAL_I2S_DMAStop(&hAudioOutI2s);
  
  /* Call Audio Codec Stop function */
  if(pAudioDrv->Stop(AUDIO_I2C_ADDRESS, Option) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    if(Option == CODEC_PDWN_HW)
    { 
      /* Wait at least 100us */
      HAL_Delay(1);
  
      /* Power Down the codec */
      BSP_IO_WritePin(AUDIO_RESET_PIN, GPIO_PIN_RESET);
  
    }
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Controls the current audio volume level. 
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume)
{
  /* Call the codec volume control function with converted volume value */
  if(pAudioDrv->SetVolume(AUDIO_I2C_ADDRESS, Volume) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Enables or disables the MUTE mode by software 
  * @param  Cmd: could be AUDIO_MUTE_ON to mute sound or AUDIO_MUTE_OFF to 
  *         unmute the codec and restore previous volume level.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd)
{ 
  /* Call the Codec Mute function */
  if(pAudioDrv->SetMute(AUDIO_I2C_ADDRESS, Cmd) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @note   This function modifies a global variable of the audio codec driver: OutputDev.
  * @param  Output: specifies the audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO 
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetOutputMode(uint8_t Output)
{
  /* Call the Codec output Device function */
  if(pAudioDrv->SetOutputMode(AUDIO_I2C_ADDRESS, Output) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Update the audio frequency.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @note This API should be called after the BSP_AUDIO_OUT_Init() to adjust the
  * audio frequency. 
  */
void BSP_AUDIO_OUT_SetFrequency(uint32_t AudioFreq)
{ 
  /* Update the I2S audio frequency configuration */
  I2SOUT_Init(AudioFreq);
}

/**
  * @brief  Initializes BSP_AUDIO_OUT MSP.
  * @param  hi2s: I2S handle
  * @param  Params  
  */
__weak void BSP_AUDIO_OUT_MspInit(I2S_HandleTypeDef *hi2s, void *Params)
{
  static DMA_HandleTypeDef  hdma_i2stx;
  GPIO_InitTypeDef          gpioinitstruct = {0};

  /* Enable I2SOUT clock */
  I2SOUT_CLK_ENABLE();

  /*** Configure the GPIOs ***/  
  /* Enable I2S GPIO clocks */
  I2SOUT_SCK_SD_CLK_ENABLE();
  I2SOUT_WS_CLK_ENABLE();

  /* I2SOUT pins configuration: WS, SCK and SD pins -----------------------------*/
  gpioinitstruct.Pin         = I2SOUT_SCK_PIN | I2SOUT_SD_PIN; 
  gpioinitstruct.Mode        = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull        = GPIO_NOPULL;
  gpioinitstruct.Speed       = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(I2SOUT_SCK_SD_GPIO_PORT, &gpioinitstruct);
  
  gpioinitstruct.Pin         = I2SOUT_WS_PIN ;
  HAL_GPIO_Init(I2SOUT_WS_GPIO_PORT, &gpioinitstruct);

  /* I2SOUT pins configuration: MCK pin */
  I2SOUT_MCK_CLK_ENABLE();
  gpioinitstruct.Pin         = I2SOUT_MCK_PIN; 
  HAL_GPIO_Init(I2SOUT_MCK_GPIO_PORT, &gpioinitstruct);   

  /* Enable the I2S DMA clock */
  I2SOUT_DMAx_CLK_ENABLE(); 
  
  if(hi2s->Instance == I2SOUT)
  {
    /* Configure the hdma_i2stx handle parameters */   
    hdma_i2stx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_i2stx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_i2stx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_i2stx.Init.PeriphDataAlignment = I2SOUT_DMAx_PERIPH_DATA_SIZE;
    hdma_i2stx.Init.MemDataAlignment    = I2SOUT_DMAx_MEM_DATA_SIZE;
    hdma_i2stx.Init.Mode                = DMA_NORMAL;
    hdma_i2stx.Init.Priority            = DMA_PRIORITY_HIGH;

    hdma_i2stx.Instance                 = I2SOUT_DMAx_CHANNEL;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hi2s, hdmatx, hdma_i2stx);
    
    /* Deinitialize the Channel for new transfer */
    HAL_DMA_DeInit(&hdma_i2stx);
    
    /* Configure the DMA Channel */
    HAL_DMA_Init(&hdma_i2stx);
  }
  
  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(I2SOUT_DMAx_IRQ, AUDIO_OUT_IRQ_PREPRIO, 0);
  HAL_NVIC_EnableIRQ(I2SOUT_DMAx_IRQ); 
}

/**
  * @brief  De-Initializes BSP_AUDIO_OUT MSP.
  * @param  hi2s: I2S handle
  * @param  Params
  */
__weak void BSP_AUDIO_OUT_MspDeInit(I2S_HandleTypeDef *hi2s, void *Params)
{
  GPIO_InitTypeDef  gpioinitstruct;  
  
  /* Disable I2S clock */
  I2SOUT_CLK_DISABLE();
  
  /* CODEC_I2S pins configuration: WS, SCK and SD pins */
  gpioinitstruct.Pin = I2SOUT_WS_PIN | I2SOUT_SCK_PIN | I2SOUT_SD_PIN;
  HAL_GPIO_DeInit(I2SOUT_SCK_SD_GPIO_PORT, gpioinitstruct.Pin);

  /* CODEC_I2S pins configuration: MCK pin */
  gpioinitstruct.Pin = I2SOUT_MCK_PIN;
  HAL_GPIO_DeInit(I2SOUT_MCK_GPIO_PORT, gpioinitstruct.Pin); 
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hi2s: I2S handle
  */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance == I2SOUT)
  {
    /* Manage the remaining file size and new address offset: This function 
    should be coded by user (its prototype is already declared in stm3210c_eval_audio.h) */  
    BSP_AUDIO_OUT_TransferComplete_CallBack();       
  }
}

/**
  * @brief Tx Transfer Half completed callbacks
  * @param hi2s: I2S handle
  */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance == I2SOUT)
  {
    /* Manage the remaining file size and new address offset: This function 
    should be coded by user (its prototype is already declared in stm3210c_eval_audio.h) */  
    BSP_AUDIO_OUT_HalfTransfer_CallBack();
  }
}

/**
  * @brief I2S error callbacks
  * @param hi2s: I2S handle
  */
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the error generated on DMA FIFO: This function 
     should be coded by user (its prototype is already declared in stm3210c_eval_audio.h) */  
  if(hi2s->Instance == I2SOUT)
  {
    BSP_AUDIO_OUT_Error_CallBack();
  }
}

/**
  * @brief  Manages the DMA full Transfer complete event.
  */
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  */
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
}

/**
  * @brief  Manages the DMA FIFO error event.
  */
__weak void BSP_AUDIO_OUT_Error_CallBack(void)
{
}

/******************************************************************************
                            Static Function
*******************************************************************************/

/**
  * @brief  Initializes the Audio Codec audio interface (I2S)
  * @param  AudioFreq: Audio frequency to be configured for the I2S peripheral. 
  */
static void I2SOUT_Init(uint32_t AudioFreq)
{
  /* Initialize the hAudioOutI2s Instance parameter */
  hAudioOutI2s.Instance         = I2SOUT;

 /* Disable I2S block */
  __HAL_I2S_DISABLE(&hAudioOutI2s);
  
  /* I2SOUT peripheral configuration */
  hAudioOutI2s.Init.AudioFreq   = AudioFreq;
  hAudioOutI2s.Init.CPOL        = I2S_CPOL_LOW;
  hAudioOutI2s.Init.DataFormat  = I2S_DATAFORMAT_16B;
  hAudioOutI2s.Init.MCLKOutput  = I2S_MCLKOUTPUT_ENABLE;
  hAudioOutI2s.Init.Mode        = I2S_MODE_MASTER_TX;
  hAudioOutI2s.Init.Standard    = I2S_STANDARD;
  
  /* Initialize the I2S peripheral with the structure above */
  HAL_I2S_Init(&hAudioOutI2s);
}
  
/**
  * @brief  Deinitialize the Audio Codec audio interface (I2S).
  */
static void I2SOUT_DeInit(void)
{
  /* Initialize the hAudioOutI2s Instance parameter */
  hAudioOutI2s.Instance = I2SOUT;

  /* Disable I2S peripheral */
  __HAL_I2S_DISABLE(&hAudioOutI2s);

  HAL_I2S_DeInit(&hAudioOutI2s);
}

/**
  * @}
  */ 
 
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
