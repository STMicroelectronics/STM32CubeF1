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
  * @brief ADC MSP initialization
  *        This function configures the hardware resources used in this example:
  *          - Enable clock of ADC peripheral
  *          - Configure the GPIO associated to the peripheral channels
  *          - Configure the DMA associated to the peripheral
  *          - Configure the NVIC associated to the peripheral interruptions
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef * hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  static DMA_HandleTypeDef DmaHandle;

  /* ##-1- Enable peripherals and GPIO Clocks ################################# */
  /* Enable clock of GPIO associated to the peripheral channels */
  ADCx_CHANNEL_GPIO_CLK_ENABLE();

  /* Enable clock of ADCx peripheral */
  ADCx_CLK_ENABLE();

  /* Enable clock of DMA associated to the peripheral */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* ##-2- Configure peripheral GPIO ########################################## */
  /* Configure GPIO pin of the selected ADC channel */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /* ##- 3- Configure DMA ##################################################### */

  /* Configure DMA parameters */
  DmaHandle.Instance = DMA1_Channel1;
  DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
  DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
  DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  DmaHandle.Init.Mode = DMA_CIRCULAR;
  DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;

  /* Deinitialize & Initialize the DMA for new transfer */
  HAL_DMA_DeInit(&DmaHandle);
  HAL_DMA_Init(&DmaHandle);

  /* Associate the initialized DMA handle to the the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);

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
  /* ##-1- Reset peripherals ################################################## */
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /* ##-2- Disable peripherals and GPIO Clocks ################################ */
  /* De-initialize the ADC3 Channel8 GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);

  /* ##-3- Disable the DMA Streams ############################################ */
  /* De-Initialize the DMA associated to the peripheral */
  if (hadc->DMA_Handle != NULL)
  {
    HAL_DMA_DeInit(hadc->DMA_Handle);
  }

  /* ##-4- Disable the NVIC ################################################### */
  /* Disable the NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);
}
