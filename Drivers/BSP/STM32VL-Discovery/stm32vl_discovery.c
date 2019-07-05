/**
  ******************************************************************************
  * @file    stm32vl_discovery.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    14-April-2017
  * @brief   This file provides
  *            - set of firmware functions to manage Led and push-button
  *          available on STM32VL-Discovery board from STMicroelectronics.  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
#include "stm32vl_discovery.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32VL_DISCOVERY STM32VL Discovery
  * @brief This file provides firmware functions to manage Leds and push-buttons
  *        available on STM32VL discovery board from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32VL_DISCOVERY_Private_Defines STM32VL DISCOVERY Private Defines
  * @{
  */ 
  
/**
* @brief STM32VL-Discovery BSP Driver version number
*/
#define __STM32VL_DISCO_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32VL_DISCO_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32VL_DISCO_BSP_VERSION_SUB2   (0x02) /*!< [15:8]  sub2 version */
#define __STM32VL_DISCO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __STM32VL_DISCO_BSP_VERSION        ((__STM32VL_DISCO_BSP_VERSION_MAIN << 24)\
                                           |(__STM32VL_DISCO_BSP_VERSION_SUB1 << 16)\
                                           |(__STM32VL_DISCO_BSP_VERSION_SUB2 << 8 )\
                                           |(__STM32VL_DISCO_BSP_VERSION_RC)) 
/**
  * @}
  */ 

/** @defgroup STM32VL_DISCOVERY_Private_Variables STM32VL DISCOVERY Private Variables
  * @{
  */ 
GPIO_TypeDef*   LED_PORT[LEDn] = {LED3_GPIO_PORT,
                                  LED4_GPIO_PORT};

const uint16_t  LED_PIN[LEDn]  = {LED3_PIN,
                                  LED4_PIN};


GPIO_TypeDef*   BUTTON_PORT[BUTTONn]  = {USER_BUTTON_GPIO_PORT};
const uint16_t  BUTTON_PIN[BUTTONn]   = {USER_BUTTON_PIN};
const uint8_t   BUTTON_IRQn[BUTTONn]  = {USER_BUTTON_EXTI_IRQn};

/**
  * @}
  */ 


/** @defgroup STM32VL_DISCOVERY_Exported_Functions STM32VL DISCOVERY Exported Functions
  * @{
  */ 

/**
  * @brief  This method returns the STM32VL-Discovery BSP Driver version
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32VL_DISCO_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  gpioinitstruct.Pin    = LED_PIN[Led];
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_PORT[Led], &gpioinitstruct);

  /* Reset PIN to switch off the LED */
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED3
  *     @arg LED4
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_USER
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                     generation capability  
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef gpioinitstruct = {0};

  /* Enable the BUTTON Clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);

  gpioinitstruct.Pin    = BUTTON_PIN[Button];
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;

  if (Button_Mode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpioinitstruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);
  }
  else if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    gpioinitstruct.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_USER  
  * @retval Button state.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
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

