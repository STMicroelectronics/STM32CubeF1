/**
  ******************************************************************************
  * @file    ADC/ADC_AnalogWatchdog/Inc/main.h
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
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Trigger for ADC:                                                           */
/*  - If this literal is defined: ADC is operating in not continuous mode     */
/*    and conversions are trigger by external trigger: timer.                 */
/*  - If this literal is not defined: ADC is operating in continuous mode     */
/*    and first conversion is trigger by software trigger.                    */
#define ADC_TRIGGER_FROM_TIMER



/* User can use this section to tailor ADCx instance under use and associated
   resources */

/* ## Definition of ADC related resources ################################### */
/* Definition of ADCx clock resources */
#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC1_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC1_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNELa                   ADC_CHANNEL_4

/* Definition of ADCx channels pins */
#define ADCx_CHANNELa_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADCx_CHANNELa_GPIO_PORT         GPIOA
#define ADCx_CHANNELa_PIN               GPIO_PIN_4

/* Definition of ADCx DMA resources */
#define ADCx_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx_DMA                        DMA1_Channel1

#define ADCx_DMA_IRQn                   DMA1_Channel1_IRQn
#define ADCx_DMA_IRQHandler             DMA1_Channel1_IRQHandler

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                       ADC1_2_IRQn
#define ADCx_IRQHandler                 ADC1_2_IRQHandler


#if defined(ADC_TRIGGER_FROM_TIMER)
/* ## Definition of TIM related resources ################################### */
/* Definition of TIMx clock resources */
#define TIMx                            TIM3    /* Caution: Timer instance must be on APB1 (clocked by PCLK1) due to frequency computation in function "TIM_Config()" */
#define TIMx_CLK_ENABLE()               __HAL_RCC_TIM3_CLK_ENABLE()

#define TIMx_FORCE_RESET()              __HAL_RCC_TIM3_FORCE_RESET()
#define TIMx_RELEASE_RESET()            __HAL_RCC_TIM3_RELEASE_RESET()

#define ADC_EXTERNALTRIGCONV_Tx_TRGO    ADC_EXTERNALTRIGCONV_T3_TRGO

#endif /* ADC_TRIGGER_FROM_TIMER */



/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
