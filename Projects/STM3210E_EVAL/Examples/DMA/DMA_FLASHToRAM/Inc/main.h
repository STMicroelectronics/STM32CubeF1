/**
  ******************************************************************************
  * @file    DMA/DMA_FLASHToRAM/Inc/main.h
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
#include "stm3210e_eval.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* DMA channel parameters definitions. You can modify these parameters to select*/
/*a different DMA channel.*/
#define DMA_INSTANCE               DMA1_Channel1
#define DMA_INSTANCE_IRQ           DMA1_Channel1_IRQn
#define DMA_INSTANCE_IRQHANDLER    DMA1_Channel1_IRQHandler

#define BUFFER_SIZE              32

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
