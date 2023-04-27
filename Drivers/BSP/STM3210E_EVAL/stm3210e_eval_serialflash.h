/**
  ******************************************************************************
  * @file    stm3210e_eval_serialflash.h
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm3210e_eval_flash driver.
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
#ifndef __STM3210E_EVAL_SERIAL_FLASH_H
#define __STM3210E_EVAL_SERIAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm3210e_eval.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM3210E_EVAL
  * @{
  */
  
/** @addtogroup STM3210E_EVAL_SERIAL_FLASH
  * @{
  */  

/** @defgroup STM3210E_EVAL_SERIAL_FLASH_Exported_Constants STM3210E EVAL SERIAL FLASH Exported Constants
  * @{
  */
#define   FLASH_OK         0x00
#define   FLASH_ERROR      0x01
/**
  * @}
  */ 

/** @addtogroup STM3210E_EVAL_SERIAL_FLASH_Exported_Functions
  * @{
  */
uint8_t  BSP_SERIAL_FLASH_Init(void);
uint8_t  BSP_SERIAL_FLASH_EraseSector(uint32_t SectorAddr);
uint8_t  BSP_SERIAL_FLASH_EraseBulk(void);
uint8_t  BSP_SERIAL_FLASH_WritePage(uint32_t uwStartAddress, uint8_t* pData, uint32_t uwDataSize);
uint8_t  BSP_SERIAL_FLASH_WriteData(uint32_t uwStartAddress, uint8_t* pData, uint32_t uwDataSize);
uint8_t  BSP_SERIAL_FLASH_ReadData( uint32_t uwStartAddress, uint8_t* pData, uint32_t uwDataSize);
uint32_t BSP_SERIAL_FLASH_ReadID(void);

/* Link function for EEPROM peripheral over SPI */
HAL_StatusTypeDef         FLASH_SPI_IO_Init(void);
uint8_t                   FLASH_SPI_IO_WriteByte(uint8_t Data);
uint8_t                   FLASH_SPI_IO_ReadByte(void);
HAL_StatusTypeDef         FLASH_SPI_IO_ReadData(uint32_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
void                      FLASH_SPI_IO_WriteEnable(void);
HAL_StatusTypeDef         FLASH_SPI_IO_WaitForWriteEnd(void);
uint32_t                  FLASH_SPI_IO_ReadID(void);
/**
  * @}
  */

  
#ifdef __cplusplus
}
#endif

#endif /* __STM3210E_EVAL_SERIAL_FLASH_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

