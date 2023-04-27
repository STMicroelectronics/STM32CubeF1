/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/Inc/main.h
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
#include "stm32f1xx_hal.h"
#include "stm3210c_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Trigger for ADC:                                                           */
/*  - If this literal is defined: ADC is operating in not continuous mode     */
/*    and conversions are trigger by external trigger: timer.                 */
/*  - If this literal is not defined: ADC is operating in continuous mode     */
/*    and first conversion is trigger by software trigger.                    */
#define ADC_TRIGGER_FROM_TIMER

/* Waveform voltage generation for test:                                      */
/*  - If this literal is defined: For this example purpose, generates a       */
/*    waveform voltage on a spare DAC channel, so user has just to connect    */
/*    a wire between DAC channel output and ADC input to run this example.    */
/*    (this avoid to user the need of an external signal generator).          */
/*  - If this literal is not defined: User has to connect an external signal  */
/*    generator on the selected ADC input to run this example.                */
#define WAVEFORM_VOLTAGE_GENERATION_FOR_TEST


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


/* Definition of ADCy clock resources */
#define ADCy                            ADC2
#define ADCy_CLK_ENABLE()               __HAL_RCC_ADC2_CLK_ENABLE()

#define ADCy_FORCE_RESET()              __HAL_RCC_ADC2_FORCE_RESET()
#define ADCy_RELEASE_RESET()            __HAL_RCC_ADC2_RELEASE_RESET()

/* Definition of ADCy channels */
#define ADCy_CHANNELa                   ADC_CHANNEL_4

/* Definition of ADCy channels pins */
#define ADCy_CHANNELa_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADCy_CHANNELa_GPIO_PORT         GPIOA
#define ADCy_CHANNELa_PIN               GPIO_PIN_4

/* Definition of ADCy NVIC resources */
#define ADCy_IRQn                       ADC1_2_IRQn
#define ADCy_IRQHandler                 ADC1_2_IRQHandler


/* #if defined(ADC_TRIGGER_FROM_TIMER) */ /* Note: Line commented for compilation purpose in HAL MSP functions */
/* ## Definition of TIM related resources ################################### */
/* Definition of TIMx clock resources */
#define TIMx                            TIM3    /* Caution: Timer instance must be on APB1 (clocked by PCLK1) due to frequency computation in function "TIM_Config()" */
#define TIMx_CLK_ENABLE()               __HAL_RCC_TIM3_CLK_ENABLE()

#define TIMx_FORCE_RESET()              __HAL_RCC_TIM3_FORCE_RESET()
#define TIMx_RELEASE_RESET()            __HAL_RCC_TIM3_RELEASE_RESET()

#define ADC_EXTERNALTRIGCONV_Tx_TRGO    ADC_EXTERNALTRIGCONV_T3_TRGO

/* #endif */ /* ADC_TRIGGER_FROM_TIMER */


#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
/* ## Definition of DAC related resources for waveform voltage generation test ## */
/* Definition of DACx clock resources */
#define DACx                            DAC
#define DACx_CLK_ENABLE()               __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

/* Definition of DACx channels */
#define DACx_CHANNELa                   DAC_CHANNEL_1

/* Definition of DACx channels pins */
#define DACx_CHANNELa_PIN               GPIO_PIN_4
#define DACx_CHANNELa_GPIO_PORT         GPIOA



/* ## Definition of TIM related resources for waveform voltage generation test ## */
#define TIM_test_signal_generation                            TIM6    /* Caution: Timer instance must be on APB1 (clocked by PCLK1) due to frequency computation in function "WaveformVoltageGenerationForTest()" */
#define TIM_test_signal_generation_CLK_ENABLE()               __HAL_RCC_TIM6_CLK_ENABLE()

#define TIM_test_signal_generation_FORCE_RESET()              __HAL_RCC_TIM6_FORCE_RESET()
#define TIM_test_signal_generation_RELEASE_RESET()            __HAL_RCC_TIM6_RELEASE_RESET()

#define DACx_TRIGGER_Tx_TRGO            DAC_TRIGGER_T6_TRGO

/* Definition of DACx DMA resources */
#define DACx_CHANNELa_DMA_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE()
#define DACx_CHANNELa_DMA                        DMA2_Channel3
#define DACx_CHANNELb_DMA                        DMA2_Channel4

#define DACx_CHANNELa_DMA_IRQn                   DMA2_Channel3_IRQn
#define DACx_CHANNELa_DMA_IRQHandler             DMA2_Channel3_IRQHandler

#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
