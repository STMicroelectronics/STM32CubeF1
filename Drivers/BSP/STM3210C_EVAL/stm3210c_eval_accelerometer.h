/**
  ******************************************************************************
  * @file    stm3210c_eval_accelerometer.h
  * @author  MCD Application Team
  * @version V6.1.0
  * @date    14-April-2017
  * @brief   This file contains all the functions prototypes for the stm3210c_eval_accelerometer.c
  *          firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3210C_EVAL_ACCELEROMETER_H
#define __STM3210C_EVAL_ACCELEROMETER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm3210c_eval.h"
/* Include Accelerometer component driver */
#include "../Components/lis302dl/lis302dl.h"   

   /** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM3210C_EVAL
  * @{
  */ 

/** @addtogroup STM3210C_EVAL_ACCELEROMETER
  * @{
  */
  

/** @defgroup STM3210C_EVAL_ACCELEROMETER_Exported_Types STM3210C EVAL ACCELEROMETER Exported Types
  * @{
  */
typedef enum 
{
  ACCELERO_OK = 0,
  ACCELERO_ERROR = 1,
  ACCELERO_TIMEOUT = 2
} 
ACCELERO_StatusTypeDef;

/**
  * @}
  */
  
/** @addtogroup STM3210C_EVAL_ACCELEROMETER_Exported_Functions
  * @{
  */
/* Acc functions */  
uint8_t   BSP_ACCELERO_Init(void);
uint8_t   BSP_ACCELERO_ReadID(void);
void      BSP_ACCELERO_Reset(void);
void      BSP_ACCELERO_Click_ITConfig(void);
void      BSP_ACCELERO_Click_ITClear(void);
void      BSP_ACCELERO_GetXYZ(int16_t *pDataXYZ);

#endif /* __STM3210C_EVAL_ACCELEROMETER_H */
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


