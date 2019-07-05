/**
  ******************************************************************************
  * @file    stlm75.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    24-November-2014
  * @brief   This file contains all the functions prototypes for the stlm75.c
  *          temperature sensor driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#ifndef __STLM75_H
#define __STLM75_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Common/tsensor.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup STLM75
  * @{
  */
  
/** @defgroup STLM75_Exported_Constants
  * @{
  */
/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/***************************** Read Access Only *******************************/
#define LM75_REG_TEMP       0x00  /*!< Temperature Register of LM75 */

/***************************** Read/Write Access ******************************/
#define LM75_REG_CONF       0x01  /*!< Configuration Register of LM75 */
#define LM75_REG_THYS       0x02  /*!< Temperature Register of LM75 */
#define LM75_REG_TOS        0x03  /*!< Over-temp Shutdown threshold Register of LM75 */

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/
/** @defgroup Conversion_Mode_Selection 
  * @{
  */
#define STLM75_CONTINUOUS_MODE                  ((uint8_t)0x00)
#define STLM75_ONE_SHOT_MODE                    ((uint8_t)0x01)
/**
  * @}
  */

/** @defgroup Operation_Mode 
  * @{
  */
#define STLM75_COMPARATOR_MODE                  ((uint8_t)0x00)
#define STLM75_INTERRUPT_MODE                   ((uint8_t)0x02)
/**
  * @}
  */

/**
  * @}
  */
 
/** @defgroup STLM75_Exported_Functions
  * @{
  */
/* Sensor Configuration Functions */ 
void                      STLM75_Init(uint16_t DeviceAddr, TSENSOR_InitTypeDef *pInitStruct);
uint8_t                   STLM75_IsReady(uint16_t DeviceAddr, uint32_t Trials);
/* Sensor Request Functions */
uint8_t                   STLM75_ReadStatus(uint16_t DeviceAddr);
uint16_t                  STLM75_ReadTemp(uint16_t DeviceAddr);

/* Temperature Sensor driver structure */
extern TSENSOR_DrvTypeDef Stlm75Drv;

/* Temperature Sensor IO functions */
void                      TSENSOR_IO_Init(void);
void                      TSENSOR_IO_Write(uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint16_t Length);
void                      TSENSOR_IO_Read(uint16_t DevAddress, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t Length);
uint16_t                  TSENSOR_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
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
  
#endif /* __STTS751_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
