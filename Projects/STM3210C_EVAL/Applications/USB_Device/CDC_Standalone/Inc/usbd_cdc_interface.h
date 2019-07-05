/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Inc/usbd_cdc_interface.h
  * @author  MCD Application Team
  * @brief   Header for usbd_cdc_interface.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H
#define __USBD_CDC_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_5
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_RX_PIN                    GPIO_PIN_6
#define USARTx_RX_GPIO_PORT              GPIOD 

/* Definition for USARTx's NVIC: used for receiving data over Rx pin */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

/* Definition for USARTx's DMA: used for transmitting data over Tx pin */
#define USARTx_TX_DMA_STREAM             DMA1_Channel7  
#define USARTx_DMA_TX_IRQHandler         DMA1_Channel7_IRQHandler
#define USARTx_DMA_TX_IRQn               DMA1_Channel7_IRQn

/* Definition for TIMx clock resources */
#define TIMx                             TIM3
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM3_CLK_ENABLE
#define TIMx_FORCE_RESET()               __HAL_RCC_USART2_FORCE_RESET()
#define TIMx_RELEASE_RESET()             __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                        TIM3_IRQn
#define TIMx_IRQHandler                  TIM3_IRQHandler

/* Periodically, the state of the buffer "UserTxBuffer" is checked.
   The period depends on CDC_POLLING_INTERVAL */
#define CDC_POLLING_INTERVAL             5 /* in ms. The max is 65 and the min is 1 */

extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __USBD_CDC_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
