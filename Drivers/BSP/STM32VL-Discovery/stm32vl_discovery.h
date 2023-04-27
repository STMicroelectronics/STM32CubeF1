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
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
