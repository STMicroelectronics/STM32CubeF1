/**
  ******************************************************************************
  * @file    stlm75.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    24-November-2014
  * @brief   This file provides a set of functions needed to manage the STLM75
  *          Temperature Sensor.
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

/* Includes ------------------------------------------------------------------*/
#include "stlm75.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup STLM75
  * @brief     This file provides a set of functions needed to drive the 
  *            STLM75 Temperature Sensor.
  * @{
  */

/** @defgroup STLM75_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup STLM75_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup STLM75_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup STLM75_Private_Variables
  * @{
  */ 
TSENSOR_DrvTypeDef Stlm75Drv =
{
  STLM75_Init,
  STLM75_IsReady,
  STLM75_ReadStatus,
  STLM75_ReadTemp,
};

/**
  * @}
  */

/** @defgroup STLM75_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup STLM75_Private_Functions
  * @{
  */

/**
  * @brief  Set STLM75 Initialization.
  * @param  DeviceAddr : Device ID address.
  * @param  pInitStruct: pointer to a STLM75_InitTypeDef structure 
  *         that contains the configuration setting for the STLM75.
  * @retval None
  */
void STLM75_Init(uint16_t DeviceAddr, TSENSOR_InitTypeDef *pInitStruct)
{  
  uint8_t confreg = 0;
  uint16_t tempreg = 0;

  /* Set the Configuration Register */
  confreg = (uint8_t)(pInitStruct->AlertMode | pInitStruct->ConversionMode);
  TSENSOR_IO_Write(DeviceAddr, &confreg, LM75_REG_CONF, 1);

  /* Set the Temperature Registers */
  /* Keep the sign bit and shift the temperature value (as given value is integer, the 0.5 digit is not set) */
  tempreg = (((pInitStruct->TemperatureLimitHigh & 0x007F) << 8) | (pInitStruct->TemperatureLimitHigh & 0x8000));
  TSENSOR_IO_Write(DeviceAddr, (uint8_t*)(&tempreg), LM75_REG_TOS, 2);

  tempreg = (((pInitStruct->TemperatureLimitLow & 0x007F) << 8) | (pInitStruct->TemperatureLimitLow & 0x8000));
  TSENSOR_IO_Write(DeviceAddr, (uint8_t*)(&tempreg), LM75_REG_THYS, 2);
}

/**
  * @brief  Read ID address of STLM75
  * @param  DeviceAddr : Device ID address.
  * @param  Trials: Number of trials
  * @retval ID name
  */
uint8_t STLM75_IsReady(uint16_t DeviceAddr, uint32_t Trials)
{
  /* Configure the low level interface ---------------------------------------*/
  TSENSOR_IO_Init();
  
  /* Check is Temperature Sensor is Ready to use */
  return TSENSOR_IO_IsDeviceReady(DeviceAddr, Trials);
}

/**
  * @brief  Read The Temperature Sensor Status
  * @param  DeviceAddr : Device ID address.
  * @retval Status
  */
uint8_t STLM75_ReadStatus(uint16_t DeviceAddr)
{
  uint8_t tmp = 0;

  /* Read Status register */
  TSENSOR_IO_Read(DeviceAddr, &tmp, LM75_REG_CONF, 1);

  /* Return Temperature Sensor Status */
  return (uint8_t)(tmp);
}

/**
  * @brief  Read ID address of STLM75
  * @param  DeviceAddr : Device ID address.
  * @retval ID name
  */
uint16_t STLM75_ReadTemp(uint16_t DeviceAddr)
{
  uint16_t tempreg = 0;
  uint16_t tmp = 0;

  /* Read Temperature registers */
  TSENSOR_IO_Read(DeviceAddr, (uint8_t*)(&tempreg), LM75_REG_TEMP, 2);
  
  tmp = ((tempreg & 0x00FF) << 8) | ((tempreg & 0xFF00) >> 8);
  tempreg = (((tmp & 0x7F80) >> 7) | (tmp & 0x8000));

  /* Return Temperature value */
  return (tempreg);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     
