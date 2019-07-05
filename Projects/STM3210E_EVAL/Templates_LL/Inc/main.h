/**
  ******************************************************************************
  * @file    Templates_LL/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

/* LL drivers common to all LL examples */
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_crc.h"
#include "stm32f1xx_ll_dac.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_iwdg.h"
#include "stm32f1xx_ll_rtc.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_wwdg.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief LED1 
  */

#define LED1_PIN                           LL_GPIO_PIN_6
#define LED1_GPIO_PORT                     GPIOF
#define LED1_GPIO_CLK_ENABLE()             LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOF)

/**
  * @brief LED2 
  */

#define LED2_PIN                           LL_GPIO_PIN_7
#define LED2_GPIO_PORT                     GPIOF
#define LED2_GPIO_CLK_ENABLE()             LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOF)

/**
  * @brief LED3 
  */

#define LED3_PIN                           LL_GPIO_PIN_8
#define LED3_GPIO_PORT                     GPIOF
#define LED3_GPIO_CLK_ENABLE()             LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOF)

/**
  * @brief LED4 
  */

#define LED4_PIN                           LL_GPIO_PIN_9
#define LED4_GPIO_PORT                     GPIOF
#define LED4_GPIO_CLK_ENABLE()             LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOF)

/**
  * @brief Tamper push-button
  */
#define TAMPER_BUTTON_PIN                         LL_GPIO_PIN_13
#define TAMPER_BUTTON_GPIO_PORT                   GPIOC
#define TAMPER_BUTTON_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC)   
#define TAMPER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_13
#define TAMPER_BUTTON_EXTI_IRQn                   EXTI15_10_IRQn
#define TAMPER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(TAMPER_BUTTON_EXTI_LINE)   
#define TAMPER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(TAMPER_BUTTON_EXTI_LINE)   
#define TAMPER_BUTTON_SYSCFG_SET_EXTI()           do {                                                                           \
                                                       LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                       \
                                                       LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTC, LL_GPIO_AF_EXTI_LINE13);  \
                                                     } while(0)
#define USER_BUTTON_IRQHANDLER                  EXTI15_10_IRQHandler

/**
  * @brief Key push-button
  */
#define KEY_BUTTON_PIN                         LL_GPIO_PIN_8
#define KEY_BUTTON_GPIO_PORT                   GPIOG
#define KEY_BUTTON_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB)   
#define KEY_BUTTON_EXTI_LINE                   LL_EXTI_LINE_8
#define KEY_BUTTON_EXTI_IRQn                   EXTI9_5_IRQn
#define KEY_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(KEY_BUTTON_EXTI_LINE)   
#define KEY_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(KEY_BUTTON_EXTI_LINE)   
#define KEY_BUTTON_SYSCFG_SET_EXTI()           do {                                                                           \
                                                     LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                      \
                                                     LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTG, LL_GPIO_AF_EXTI_LINE8);  \
                                                  } while(0)
#define KEY_BUTTON_IRQHANDLER                  EXTI9_5_IRQQHandler

/**
  * @brief Wake-up push-button
  */
#define WAKEUP_BUTTON_PIN                         LL_GPIO_PIN_0
#define WAKEUP_BUTTON_GPIO_PORT                   GPIOA
#define WAKEUP_BUTTON_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA)   
#define WAKEUP_BUTTON_EXTI_LINE                   LL_EXTI_LINE_0
#define WAKEUP_BUTTON_EXTI_IRQn                   EXTI0_IRQn
#define WAKEUP_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(WAKEUP_BUTTON_EXTI_LINE)   
#define WAKEUP_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(WAKEUP_BUTTON_EXTI_LINE)   
#define WAKEUP_BUTTON_SYSCFG_SET_EXTI()           do {                                                                           \
                                                        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                      \
                                                        LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA, LL_GPIO_AF_EXTI_LINE0);  \
                                                     } while(0)
#define WAKEUP_BUTTON_IRQHANDLER                  EXTI0_IRQHandler

/**
  * @brief Joystick Right push-button
  */
#define RIGHT_JOY_PIN                         LL_GPIO_PIN_13
#define RIGHT_JOY_GPIO_PORT                   GPIOG
#define RIGHT_JOY_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOG)   
#define RIGHT_JOY_EXTI_LINE                   LL_EXTI_LINE_13
#define RIGHT_JOY_EXTI_IRQn                   EXTI15_10_IRQn
#define RIGHT_JOY_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(RIGHT_JOY_EXTI_LINE)   
#define RIGHT_JOY_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(RIGHT_JOY_EXTI_LINE)   
#define RIGHT_JOY_SYSCFG_SET_EXTI()           do {                                                                            \
                                                    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                       \
                                                    LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTG, LL_GPIO_AF_EXTI_LINE13);  \
                                                  } while(0)
#define RIGHT_JOY_IRQHANDLER                  EXTI15_10_IRQHandler

/**
  * @brief Joystick Left push-button
  */
#define LEFT_JOY_PIN                         LL_GPIO_PIN_14
#define LEFT_JOY_GPIO_PORT                   GPIOG
#define LEFT_JOY_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOG)   
#define LEFT_JOY_EXTI_LINE                   LL_EXTI_LINE_14
#define LEFT_JOY_EXTI_IRQn                   EXTI15_10_IRQn
#define LEFT_JOY_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(LEFT_JOY_EXTI_LINE)   
#define LEFT_JOY_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(LEFT_JOY_EXTI_LINE)   
#define LEFT_JOY_SYSCFG_SET_EXTI()           do {                                                                           \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                       \
                                                  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTG, LL_GPIO_AF_EXTI_LINE14);  \
                                                } while(0)
#define LEFT_JOY_IRQHANDLER                  EXTI15_10_IRQHandler

/**
  * @brief Joystick Up push-button
  */
#define UP_JOY_PIN                         LL_GPIO_PIN_15
#define UP_JOY_GPIO_PORT                   GPIOG
#define UP_JOY_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOG)   
#define UP_JOY_EXTI_LINE                   LL_EXTI_LINE_15
#define UP_JOY_EXTI_IRQn                   EXTI15_10_IRQn
#define UP_JOY_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(UP_JOY_EXTI_LINE)   
#define UP_JOY_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(UP_JOY_EXTI_LINE)   
#define UP_JOY_SYSCFG_SET_EXTI()           do {                                                                           \
                                                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                       \
                                                LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTG, LL_GPIO_AF_EXTI_LINE15);  \
                                               } while(0)
#define UP_JOY_IRQHANDLER                  EXTI15_10_IRQHandler

/**
  * @brief Joystick Down push-button
  */
#define DOWN_JOY_PIN                         LL_GPIO_PIN_3
#define DOWN_JOY_GPIO_PORT                   GPIOD
#define DOWN_JOY_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD)   
#define DOWN_JOY_EXTI_LINE                   LL_EXTI_LINE_3
#define DOWN_JOY_EXTI_IRQn                   EXTI3_IRQn
#define DOWN_JOY_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(DOWN_JOY_EXTI_LINE)   
#define DOWN_JOY_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(DOWN_JOY_EXTI_LINE)   
#define DOWN_JOY_SYSCFG_SET_EXTI()           do {                                                                        \
                                                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                      \
                                                LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTD, LL_GPIO_AF_EXTI_LINE3);  \
                                               } while(0)
#define DOWN_JOY_IRQHANDLER                  EXTI3_IRQHandler

/**
  * @brief Joystick Sel push-button
  */
#define SEL_JOY_PIN                         LL_GPIO_PIN_7
#define SEL_JOY_GPIO_PORT                   GPIOG
#define SEL_JOY_GPIO_CLK_ENABLE()           LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOG)   
#define SEL_JOY_EXTI_LINE                   LL_EXTI_LINE_7
#define SEL_JOY_EXTI_IRQn                   EXTI9_5_IRQn
#define SEL_JOY_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(SEL_JOY_EXTI_LINE)   
#define SEL_JOY_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(SEL_JOY_EXTI_LINE)   
#define SEL_JOY_SYSCFG_SET_EXTI()           do {                                                                        \
                                                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);                      \
                                                LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTG, LL_GPIO_AF_EXTI_LINE7);  \
                                               } while(0)
#define SEL_JOY_IRQHANDLER                  EXTI9_5_IRQHandler

/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000
  
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
