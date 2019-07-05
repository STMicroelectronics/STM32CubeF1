/**
  ******************************************************************************
  * @file    stm3210e_eval_audio.h
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file contains the common defines and functions prototypes for
  *          stm3210e_eval_audio.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3210E_EVAL_AUDIO_H
#define __STM3210E_EVAL_AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include audio component Driver */
#include "../Components/ak4343/ak4343.h"
#include "stm3210e_eval.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM3210E_EVAL
  * @{
  */
    
/** @addtogroup STM3210E_EVAL_AUDIO
  * @{
  */

/** @defgroup STM3210E_EVAL_AUDIO_OUT_Exported_Constants STM3210E EVAL AUDIO OUT Exported Constants 
  * @{
  */ 


/*------------------------------------------------------------------------------
             AUDIO OUT CONFIGURATION
------------------------------------------------------------------------------*/

/* I2S peripheral configuration defines */
#define I2SOUT                          SPI2
#define I2SOUT_CLK_ENABLE()             __HAL_RCC_SPI2_CLK_ENABLE()
#define I2SOUT_CLK_DISABLE()            __HAL_RCC_SPI2_CLK_DISABLE()
#define I2SOUT_SCK_SD_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2SOUT_MCK_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2SOUT_WS_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2SOUT_WS_PIN                   GPIO_PIN_12             /* PB.12*/
#define I2SOUT_SCK_PIN                  GPIO_PIN_13             /* PB.13*/
#define I2SOUT_SD_PIN                   GPIO_PIN_15             /* PB.15*/
#define I2SOUT_MCK_PIN                  GPIO_PIN_6              /* PC.06*/
#define I2SOUT_SCK_SD_GPIO_PORT         GPIOB
#define I2SOUT_WS_GPIO_PORT             GPIOB
#define I2SOUT_MCK_GPIO_PORT            GPIOC

/* I2S DMA Channel definitions */
#define I2SOUT_DMAx_CLK_ENABLE()        __HAL_RCC_DMA1_CLK_ENABLE()
#define I2SOUT_DMAx_CHANNEL             DMA1_Channel5
#define I2SOUT_DMAx_IRQ                 DMA1_Channel5_IRQn
#define I2SOUT_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_HALFWORD
#define I2SOUT_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_HALFWORD
#define DMA_MAX_SZE                     0xFFFF

#define I2SOUT_IRQHandler               DMA1_Channel5_IRQHandler

/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
#define AUDIO_OUT_IRQ_PREPRIO         0xE   /* Select the preemption priority level(0 is the highest) */

/*------------------------------------------------------------------------------
             CONFIGURATION: Audio Driver Configuration parameters
------------------------------------------------------------------------------*/

/* Audio status definition */     
#define AUDIO_OK                              0
#define AUDIO_ERROR                           1
#define AUDIO_TIMEOUT                         2

   
/*------------------------------------------------------------------------------
                    OPTIONAL Configuration defines parameters
------------------------------------------------------------------------------*/


/**
  * @}
  */ 

/** @defgroup STM3210E_EVAL_AUDIO_Exported_Macros STM3210E EVAL AUDIO Exported Macros
  * @{
  */
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)

/**
  * @}
  */

/** @addtogroup STM3210E_EVAL_AUDIO_OUT_Exported_Functions 
  * @{
  */ 
uint8_t BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
void    BSP_AUDIO_OUT_DeInit(void);
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size);
void    BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size);
uint8_t BSP_AUDIO_OUT_Pause(void);
uint8_t BSP_AUDIO_OUT_Resume(void);
uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option);
uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume);
void    BSP_AUDIO_OUT_SetFrequency(uint32_t AudioFreq);
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Command);
uint8_t BSP_AUDIO_OUT_SetOutputMode(uint8_t Output);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function is called when the requested data has been completely transferred.*/
void    BSP_AUDIO_OUT_TransferComplete_CallBack(void);

/* This function is called when half of the requested buffer has been transferred. */
void    BSP_AUDIO_OUT_HalfTransfer_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    BSP_AUDIO_OUT_Error_CallBack(void);

/* These function can be modified in case the current settings need to be changed for specific
   application needs */
void    BSP_AUDIO_OUT_MspInit(I2S_HandleTypeDef *hi2s, void *Params);
void    BSP_AUDIO_OUT_MspDeInit(I2S_HandleTypeDef *hi2s, void *Params);

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

#ifdef __cplusplus
}
#endif
   
#endif /* __STM3210E_EVAL_AUDIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
