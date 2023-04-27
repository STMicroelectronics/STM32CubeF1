/**
  ******************************************************************************
  * @file    stm3210e_eval_tsensor.c
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file provides a set of functions needed to manage the I2C STLM75 
  *          temperature sensor mounted on STM3210E_EVAL board . 
  *          It implements a high level communication layer for read and write 
  *          from/to this sensor. The needed STM32L152xD hardware resources (I2C and 
  *          GPIO) are defined in stm3210e_eval.h file, and the initialization is 
  *          performed in TSENSOR_IO_Init() function declared in stm3210e_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          TSENSOR_IO_Init() function. 
  *
  *     +--------------------------------------------------------------------+
  *     |                        Pin assignment                              |
  *     +----------------------------------------+--------------+------------+
  *     |  STM3210E I2C Pins                     |   STLM75     |   Pin      |
  *     +----------------------------------------+--------------+------------+
  *     | EVAL_I2C2_SDA_PIN                      |   SDA        |    1       |
  *     | EVAL_I2C2_SCL_PIN                      |   SCL        |    2       |
  *     | EVAL_I2C2_SMBUS_PIN                    |   OS/INT     |    3       |
  *     | .                                      |   GND        |    4 (0V)  |
  *     | .                                      |   A2         |    5       |
  *     | .                                      |   A1         |    6       |
  *     | .                                      |   A0         |    7       |
  *     | .                                      |   VDD        |    8 (3.3V)|
  *     +----------------------------------------+--------------+------------+
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

/* Includes ------------------------------------------------------------------*/
#include "stm3210e_eval_tsensor.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM3210E_EVAL
  * @{
  */

/** @defgroup STM3210E_EVAL_TSENSOR STM3210E EVAL TSENSOR
  * @brief      This file includes the STLM75 Temperature Sensor driver of 
  *             STM3210E_EVAL board.
  * @{
  */ 


  
/** @defgroup STM3210E_EVAL_TSENSOR_Private_Variables STM3210E EVAL TSENSOR Private Variables
  * @{
  */ 
static TSENSOR_DrvTypeDef  *tsensor_drv; 
__IO uint16_t  TSENSORAddress = 0;
/**
  * @}
  */ 


/** @defgroup STM3210E_EVAL_I2C_TSENSOR_Exported_Functions STM3210E EVAL I2C TSENSOR Exported Functions
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the I2C Temperature Sensor driver.
  * @retval TSENSOR status
  */
uint32_t BSP_TSENSOR_Init(void)
{ 
  uint8_t ret = TSENSOR_ERROR;
  TSENSOR_InitTypeDef tsensor_initstruct = {0};

  /* Temperature Sensor Initialization */
  if(Stlm75Drv.IsReady(TSENSOR_I2C_ADDRESS_A01, TSENSOR_MAX_TRIALS) == HAL_OK)
  {
    /* Initialize the temperature sensor driver structure */
    TSENSORAddress = TSENSOR_I2C_ADDRESS_A01;
    tsensor_drv = &Stlm75Drv;

    ret = TSENSOR_OK;
  }
  else
  {
    if(Stlm75Drv.IsReady(TSENSOR_I2C_ADDRESS_A02, TSENSOR_MAX_TRIALS) == HAL_OK)
    {
      /* Initialize the temperature sensor driver structure */
      TSENSORAddress = TSENSOR_I2C_ADDRESS_A02;
      tsensor_drv = &Stlm75Drv;

      ret = TSENSOR_OK;
    }
    else
    {
      ret = TSENSOR_ERROR;
    }
  }

  if (ret == TSENSOR_OK)
  {
    /* Configure Temperature Sensor : Conversion 9 bits in continuous mode */
    /* Alert outside range Limit Temperature 12° <-> 24°c */
    tsensor_initstruct.AlertMode             = STLM75_INTERRUPT_MODE;
    tsensor_initstruct.ConversionMode        = STLM75_CONTINUOUS_MODE;
    tsensor_initstruct.TemperatureLimitHigh  = 24;
    tsensor_initstruct.TemperatureLimitLow   = 12;
        
    /* TSENSOR Init */   
    tsensor_drv->Init(TSENSORAddress, &tsensor_initstruct);

    ret = TSENSOR_OK;
  }
  
  return ret;
}

/**
  * @brief  Returns the Temperature Sensor status.
  * @retval The Temperature Sensor status.
  */
uint8_t BSP_TSENSOR_ReadStatus(void)
{
  return (tsensor_drv->ReadStatus(TSENSORAddress));
}

/**
  * @brief  Read Temperature register of STLM75.
  * @retval STLM75 measured temperature value.
  */
uint16_t BSP_TSENSOR_ReadTemp(void)
{ 
  return tsensor_drv->ReadTemp(TSENSORAddress);

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

