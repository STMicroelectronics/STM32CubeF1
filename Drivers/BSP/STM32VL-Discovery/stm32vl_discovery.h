/**
  ******************************************************************************
  * @file    stm32vl_discovery.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    14-April-2017
  * @brief   This file contains definitions for STM32VL-Discovery's Leds, push-buttons
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32VL_DISCOVERY_H
#define __STM32VL_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32VL_DISCOVERY
  * @{
  */ 
  
/** @defgroup STM32VL_DISCOVERY_Exported_Types STM32VL DISCOVERY Exported Types
  * @{
  */
typedef enum 
{
  LED3 = 0,
  LED4 = 1,
  
  LED_GREEN = LED3,
  LED_BLUE  = LED4
    
} Led_TypeDef;

typedef enum 
{
  BUTTON_USER = 0,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;
/**
  * @}
  */ 

/** @defgroup STM32VL_DISCOVERY_Exported_Constants STM32VL DISCOVERY Exported Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32VL-Discovery board  
  */ 
#if !defined (USE_STM32VL_DISCO)
 #define USE_STM32VL_DISCO
#endif
  
/** @defgroup STM32VL_DISCOVERY_LED STM32VL DISCOVERY LED
  * @{
  */
#define LEDn                             2

#define LED3_PIN                         GPIO_PIN_9           /* PC.09 */
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()  

#define LED4_PIN                         GPIO_PIN_8           /* PC.08 */
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()  

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) LED3_GPIO_CLK_ENABLE(); \
                                               else LED4_GPIO_CLK_ENABLE(); } while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? LED3_GPIO_CLK_DISABLE() : LED4_GPIO_CLK_DISABLE())
/**
  * @}
  */
  
/** @defgroup STM32VL_DISCOVERY_BUTTON STM32VL DISCOVERY BUTTON
  * @{
  */  
#define BUTTONn                          1
/**
 * @brief Key push-button
 */
#define USER_BUTTON_PIN                  GPIO_PIN_0           /* PA.00 */
#define USER_BUTTON_GPIO_PORT            GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn            EXTI0_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)     do {USER_BUTTON_GPIO_CLK_ENABLE();} while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    (USER_BUTTON_GPIO_CLK_DISABLE())

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/** @addtogroup STM32VL_DISCOVERY_Exported_Functions
  * @{
  */ 
uint32_t  BSP_GetVersion(void);
void      BSP_LED_Init(Led_TypeDef Led);
void      BSP_LED_On(Led_TypeDef Led);
void      BSP_LED_Off(Led_TypeDef Led);
void      BSP_LED_Toggle(Led_TypeDef Led);
void      BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Mode);
uint32_t  BSP_PB_GetState(Button_TypeDef Button);
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
  
#endif /* __STM32VL_DISCOVERY_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
