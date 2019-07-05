/**
  ******************************************************************************
  * @file    stm3210c_eval_io.h
  * @author  MCD Application Team
  * @version V6.1.0
  * @date    14-April-2017
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm3210c_eval_io.c driver.
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
#ifndef __STM3210C_EVAL_IO_H
#define __STM3210C_EVAL_IO_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include "stm3210c_eval.h"
#include "../Components/stmpe811/stmpe811.h"


/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM3210C_EVAL
  * @{
  */
    
/** @addtogroup STM3210C_EVAL_IO
  * @{
  */    

/* Exported types ------------------------------------------------------------*/

/** @defgroup STM3210C_EVAL_IO_Exported_Types STM3210C EVAL IO Exported Types
  * @{
  */
typedef enum 
{
  IO_OK       = 0x00,
  IO_ERROR    = 0x01,
  IO_TIMEOUT  = 0x02

}IO_StatusTypeDef;

/**
  * @}
  */ 

/** @defgroup STM3210C_EVAL_IO_Exported_Constants STM3210C EVAL IO Exported Constants
  * @{
  */
/* Virtual pin offset STMPE811, IOExpander1 */
#define IO1_PIN_OFFSET               0
/* Virtual pin offset STMPE811, IOExpander2 */
#define IO2_PIN_OFFSET               8

/* Pins definition STMPE811, IOExpander1 */
#define IO1_PIN_0                     (uint32_t)(0x00000001 << IO1_PIN_OFFSET)
#define IO1_PIN_1                     (uint32_t)(0x00000002 << IO1_PIN_OFFSET)
#define IO1_PIN_2                     (uint32_t)(0x00000004 << IO1_PIN_OFFSET)
#define IO1_PIN_3                     (uint32_t)(0x00000008 << IO1_PIN_OFFSET)
#define IO1_PIN_4                     (uint32_t)(0x00000010 << IO1_PIN_OFFSET)
#define IO1_PIN_5                     (uint32_t)(0x00000020 << IO1_PIN_OFFSET)
#define IO1_PIN_6                     (uint32_t)(0x00000040 << IO1_PIN_OFFSET)
#define IO1_PIN_7                     (uint32_t)(0x00000080 << IO1_PIN_OFFSET)
#define IO1_PIN_ALL                   (uint32_t)(0x000000FF << IO1_PIN_OFFSET)

/* Pins definition STMPE16000, IOExpander2 */
#define IO2_PIN_0                     (uint32_t)(0x00000001 << IO2_PIN_OFFSET)
#define IO2_PIN_1                     (uint32_t)(0x00000002 << IO2_PIN_OFFSET)
#define IO2_PIN_2                     (uint32_t)(0x00000004 << IO2_PIN_OFFSET)
#define IO2_PIN_3                     (uint32_t)(0x00000008 << IO2_PIN_OFFSET)
#define IO2_PIN_4                     (uint32_t)(0x00000010 << IO2_PIN_OFFSET)
#define IO2_PIN_5                     (uint32_t)(0x00000020 << IO2_PIN_OFFSET)
#define IO2_PIN_6                     (uint32_t)(0x00000040 << IO2_PIN_OFFSET)
#define IO2_PIN_7                     (uint32_t)(0x00000080 << IO2_PIN_OFFSET)
#define IO2_PIN_ALL                   (uint32_t)(0x000000FF << IO2_PIN_OFFSET)

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/

/** @addtogroup STM3210C_EVAL_IO_Exported_Functions
  * @{
  */

uint8_t  BSP_IO_Init(void);
void     BSP_IO_ITClear(uint32_t IO_Pin);
uint32_t BSP_IO_ITGetStatus(uint32_t IO_Pin);
uint8_t  BSP_IO_ConfigPin(uint32_t IO_Pin, IO_ModeTypedef IO_Mode);
void     BSP_IO_WritePin(uint32_t IO_Pin, uint8_t PinState);
uint32_t BSP_IO_ReadPin(uint32_t IO_Pin);
void     BSP_IO_TogglePin(uint32_t IO_Pin);

#ifdef __cplusplus
}
#endif
#endif /* __STM3210C_EVAL_IO_H */

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
