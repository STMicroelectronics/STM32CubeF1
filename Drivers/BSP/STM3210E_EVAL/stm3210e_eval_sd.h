/**
  ******************************************************************************
  * @file    stm3210e_eval_sd.c
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file includes the uSD card driver.
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
#ifndef __STM3210E_EVAL_SD_H
#define __STM3210E_EVAL_SD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM3210E_EVAL
  * @{
  */
    
/** @addtogroup STM3210E_EVAL_SD
  * @{
  */    

/* Exported types ------------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_SD_Exported_Types STM3210E EVAL SD Exported Types
  * @{
  */

/** 
  * @brief SD Card information structure 
  */
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef

/** 
  * @brief SD status structure definition
  */
#define MSD_OK         ((uint8_t)0x00)
#define MSD_ERROR      ((uint8_t)0x01)

/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/  
 
/** @defgroup STM3210E_EVAL_SD_Exported_Constants STM3210E EVAL SD Exported Constants
  * @{
  */ 

/** 
  * @brief  SD transfer state definition  
  */     
#define  SD_TRANSFER_OK                ((uint8_t)0x00)
#define  SD_TRANSFER_BUSY              ((uint8_t)0x01)

#define SD_DETECT_PIN                    GPIO_PIN_11
#define SD_DETECT_GPIO_PORT              GPIOF
#define __SD_DETECT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define SD_DETECT_IRQn                   EXTI15_10_IRQn
   
#define SD_DATATIMEOUT           100000000U

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)

/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __HAL_RCC_DMA2_CLK_ENABLE
#define SD_DMAx_Tx_INSTANCE               DMA2_Channel4
#define SD_DMAx_Rx_INSTANCE               DMA2_Channel4
#define SD_DMAx_Tx_IRQn                   DMA2_Channel4_5_IRQn
#define SD_DMAx_Rx_IRQn                   DMA2_Channel4_5_IRQn
#define BSP_SD_IRQHandler                 SDIO_IRQHandler
#define SD_DMAx_Tx_IRQHandler             DMA2_Channel4_5_IRQHandler
#define SD_DMAx_Rx_IRQHandler             DMA2_Channel4_5_IRQHandler
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup STM3210E_EVAL_SD_Exported_Functions
  * @{
  */
uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ITConfig(void);
void    BSP_SD_DetectIT(void);
void    BSP_SD_DetectCallback(void);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t BSP_SD_GetCardState(void);
void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);
uint8_t BSP_SD_IsDetected(void);
   
/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
void    BSP_SD_MspInit(void *Params);
void    BSP_SD_AbortCallback(void);
void    BSP_SD_WriteCpltCallback(void);
void    BSP_SD_ReadCpltCallback(void); 

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

#endif /* __STM3210E_EVAL_SD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
