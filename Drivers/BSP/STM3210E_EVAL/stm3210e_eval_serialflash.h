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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
