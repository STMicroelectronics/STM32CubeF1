/**
  ******************************************************************************
  * @file    FSMC/FSMC_NAND/Src/stm32f1xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @defgroup FSMC_NAND
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  NAND MSP Init
  * @param  hnand: pointer to a NAND_HandleTypeDef structure that contains
  *                the configuration information for NAND module.
  * @retval None
  */
void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  
  /* Enable FSMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  
/*-- GPIO Configuration ------------------------------------------------------*/
/*!< CLE, ALE, D0->D3, NOE, NWE and NCE2  NAND pin configuration  */
  gpioinitstruct.Pin =  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15 |  
                        GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | 
                        GPIO_PIN_7;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct); 

/*!< D4->D7 NAND pin configuration  */  
  gpioinitstruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;

  HAL_GPIO_Init(GPIOE, &gpioinitstruct);


/*!< NWAIT NAND pin configuration */
  gpioinitstruct.Pin = GPIO_PIN_6;
  gpioinitstruct.Mode      = GPIO_MODE_INPUT;

  HAL_GPIO_Init(GPIOD, &gpioinitstruct); 

/*!< INT2 NAND pin configuration */  
  gpioinitstruct.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOG, &gpioinitstruct);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
