/**
  ******************************************************************************
  * @file    stm3210e_eval_tsensor.h
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file contains all the functions prototypes for the 
  *          stm3210e_tsensor.c firmware driver.
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
#ifndef __STM3210E_EVAL_TSENSOR_H
#define __STM3210E_EVAL_TSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm3210e_eval.h"
#include "../Components/stlm75/stlm75.h"   

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM3210E_EVAL
  * @{
  */ 
  
/** @addtogroup STM3210E_EVAL_TSENSOR
  * @{
  */  

/** @defgroup STM3210E_EVAL_TSENSOR_Exported_Types STM3210E EVAL TSENSOR Exported Types
  * @{
  */
   
/** 
  * @brief  TSENSOR Status  
  */ 
typedef enum
{
  TSENSOR_OK = 0,
  TSENSOR_ERROR
}TSENSOR_Status_TypDef;

/**
  * @}
  */
  
/** @defgroup STM3210E_EVAL_TSENSOR_Exported_Constants STM3210E EVAL TSENSOR Exported Constants
  * @{
  */ 
/* Temperature Sensor hardware I2C address */ 
#define TSENSOR_I2C_ADDRESS_A01 0x90
#define TSENSOR_I2C_ADDRESS_A02 0x92

/* Maximum number of trials use for STLM75_IsReady function */
#define TSENSOR_MAX_TRIALS      50
/**
  * @}
  */


/** @addtogroup STM3210E_EVAL_I2C_TSENSOR_Exported_Functions
  * @{
  */ 
uint32_t BSP_TSENSOR_Init(void);
uint8_t  BSP_TSENSOR_ReadStatus(void);
uint16_t BSP_TSENSOR_ReadTemp(void);
 
#ifdef __cplusplus
}
#endif

#endif /* __STM3210E_EVAL_TSENSOR_H */
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
