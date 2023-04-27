/**
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/Src/stm32f1xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.    
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

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief ADC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef * hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  static DMA_HandleTypeDef hdma_adc;

  /* ##-1- Enable peripherals and GPIO Clocks ################################# */
  /* Enable GPIO clock */
  ADCx_CHANNEL_GPIO_CLK_ENABLE();

  /* ADC1 Periph clock enable */
  ADCx_CLK_ENABLE();

  /* Enable DMA1 clock */
  DMAx_CLK_ENABLE();

  /* ##-2- Configure peripheral GPIO ########################################## */
  /* ADC1 Channe14 GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /* ##-3- Configure the DMA streams ########################################## */
  /* Set the parameters to be configured */
  hdma_adc.Instance = ADCx_DMA_CHANNEL;
  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_adc.Init.Mode = DMA_CIRCULAR;
  hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;

  /* Deinitialize & Initialize the DMA for new transfer */
  HAL_DMA_DeInit(&hdma_adc);
  HAL_DMA_Init(&hdma_adc);

  /* Associate the initialized DMA handle to the the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

  /* ##-4- Configure the NVIC for DMA ######################################### */
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
}

/**
  * @brief ADC MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef * hadc)
{
  static DMA_HandleTypeDef hdma_adc;

  /* ##-1- Reset peripherals ################################################## */
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /* ##-2- Disable peripherals and GPIO Clocks ################################ */
  /* De-initialize the ADC3 Channel8 GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);

  /* ##-3- Disable the DMA Streams ############################################ */
  /* De-Initialize the DMA Stream associate to transmission process */
  HAL_DMA_DeInit(&hdma_adc);

  /* ##-4- Disable the NVIC for DMA ########################################### */
  HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);
}
