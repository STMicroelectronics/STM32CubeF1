/**
  ******************************************************************************
  * @file    SMARTCARD_T0/Inc/platform_config.h 
  * @author  MCD Application Team
  * @brief   Evaluation board specific configuration file.
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
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SC_USART                           USART3
#define SC_USART_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE()
#define SC_USART_CLK_DISABLE();            __HAL_RCC_USART3_CLK_DISABLE()
#define SC_USART_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define SC_USART_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()
#define SC_USART_IRQn                      USART3_IRQn
#define SC_USART_IRQHandler                USART3_IRQHandler

#define SC_USART_TX_PIN                    GPIO_PIN_8                
#define SC_USART_TX_GPIO_PORT              GPIOD                       
#define SC_USART_TX_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()

#define SC_USART_CK_PIN                    GPIO_PIN_10                
#define SC_USART_CK_GPIO_PORT              GPIOD
#define SC_USART_CK_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()


/* Smartcard Interface GPIO pins */
#define SC_3_5V_PIN                        GPIO_PIN_0
#define SC_3_5V_GPIO_PORT                  GPIOC
#define SC_3_5V_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()

#define SC_RESET_PIN                       GPIO_PIN_9
#define SC_RESET_GPIO_PORT                 GPIOD
#define SC_RESET_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE()

#define SC_CMDVCC_PIN                      GPIO_PIN_14
#define SC_CMDVCC_GPIO_PORT                GPIOE
#define SC_CMDVCC_CLK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()

#define SC_OFF_PIN                         GPIO_PIN_7
#define SC_OFF_GPIO_PORT                   GPIOE
#define SC_OFF_CLK_ENABLE()                __HAL_RCC_GPIOE_CLK_ENABLE()

#define SC_OFF_EXTI_IRQn                   EXTI9_5_IRQn 
#define SC_OFF_EXTI_IRQHandler             EXTI9_5_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */
