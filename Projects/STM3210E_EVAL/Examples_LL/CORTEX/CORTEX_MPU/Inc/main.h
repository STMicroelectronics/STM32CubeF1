/**
  ******************************************************************************
  * @file    Examples_LL/CORTEX/CORTEX_MPU/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_pwr.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief LED Types Definition
 */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,

  LED_GREEN  = LED1,
  LED_ORANGE = LED2,
  LED_RED    = LED3,
  LED_BLUE   = LED4

} Led_TypeDef;
/* Exported constants --------------------------------------------------------*/
#define LEDn                                 4

#define LED1_PIN                             LL_GPIO_PIN_6

#define LED2_PIN                             LL_GPIO_PIN_7

#define LED3_PIN                             LL_GPIO_PIN_8

#define LED4_PIN                             LL_GPIO_PIN_9

#define LEDx_GPIO_PORT                       GPIOF
#define LEDx_GPIO_CLK_ENABLE()               LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOF)


/**
  * @brief Toggle periods for various blinking modes
  */

#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW 500
#define LED_BLINK_ERROR 1000


/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         LL_GPIO_PIN_8
#define USER_BUTTON_GPIO_PORT                   GPIOG
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOG)
#define USER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_8
#define USER_BUTTON_EXTI_IRQn                   EXTI9_5_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)   
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE) 
#define USER_BUTTON_SYSCFG_SET_EXTI()           do {                                                                     \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                  \
                                                  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTG, LL_GPIO_AF_EXTI_LINE8);  \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER                  EXTI9_5_IRQHandler

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/* Memory Management Handler treatment. */
void MemManage_Callback(void); 
/* USER button IRQ Handler treatment. */
void UserButton_Callback(void); 

#endif /* __MAIN_H */
