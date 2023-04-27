/**
  ******************************************************************************
  * @file    stm3210e_eval_nand.h
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm3210e_eval_nand.c driver.
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
#ifndef __STM3210E_EVAL_NAND_H
#define __STM3210E_EVAL_NAND_H

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
    
/** @addtogroup STM3210E_EVAL_NAND
  * @{
  */    

/* Exported constants --------------------------------------------------------*/ 
/** @defgroup STM3210E_EVAL_NAND_Exported_Constants STM3210E EVAL NAND Exported Constants
  * @{
  */
 
/** 
  * @brief  NAND status structure definition  
  */     
#define   NAND_OK         0x00U

#define NAND_DEVICE_ADDR  ((uint32_t)NAND_DEVICE1)  
  
/** 
  * @brief  FSMC NAND memory parameters  
  */  
#define NAND_PAGE_SIZE             ((uint16_t)0x0200) /* 512 bytes per page w/o Spare Area */
#define NAND_BLOCK_SIZE            ((uint16_t)0x0020) /* 32x512 bytes pages per block */
#define NAND_PLANE_SIZE            ((uint16_t)0x0400) /* 1024 Block per plane */
#define NAND_SPARE_AREA_SIZE       ((uint16_t)0x0010) /* last 16 bytes as spare area */
#define NAND_MAX_PLANE             ((uint16_t)0x1000) /* 4 planes of 1024 block */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup STM3210E_EVAL_NAND_Exported_Functions
  * @{
  */  
uint8_t BSP_NAND_Init(void);
uint8_t BSP_NAND_ReadData(NAND_AddressTypeDef BlockAddress, uint8_t *pData, uint32_t uwDataSize);
uint8_t BSP_NAND_WriteData(NAND_AddressTypeDef BlockAddress, uint8_t *pData, uint32_t uwDataSize);
uint8_t BSP_NAND_Erase_Block(NAND_AddressTypeDef BlockAddress);
uint8_t BSP_NAND_Erase_Chip(void);
uint8_t BSP_NAND_Read_ID(NAND_IDTypeDef *pNAND_ID);

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

#endif /* __STM3210E_EVAL_NAND_H */


