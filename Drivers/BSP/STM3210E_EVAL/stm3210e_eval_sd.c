/**
  ******************************************************************************
  * @file    stm3210e_eval_sd.c
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file includes the uSD card driver.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================  
  (#) This driver is used to drive the micro SD external card mounted on STM3210E-EVAL 
     evaluation board.
     
  (#) This driver does not need a specific component driver for the micro SD device
     to be included with.

  (#) Initialization steps:
       (++) Initialize the micro SD card using the BSP_SD_Init() function. This 
            function includes the MSP layer hardware resources initialization and the
            SDIO interface configuration to interface with the external micro SD. It 
            also includes the micro SD initialization sequence.
       (++) To check the SD card presence you can use the function BSP_SD_IsDetected() which 
            returns the detection status 
       (++) If SD presence detection interrupt mode is desired, you must configure the 
            SD detection interrupt mode by calling the function BSP_SD_ITConfig(). The interrupt 
            is generated as an external interrupt whenever the micro SD card is 
            plugged/unplugged in/from the evaluation board. The SD detection interrupt
            is handeled by calling the function BSP_SD_DetectIT() which is called in the IRQ
            handler file, the user callback is implemented in the function BSP_SD_DetectCallback().
       (++) The function BSP_SD_GetCardInfo() is used to get the micro SD card information 
            which is stored in the structure "HAL_SD_CardInfoTypedef".
  
  (#) Micro SD card operations
       (++) The micro SD card can be accessed with read/write block(s) operations once 
            it is reay for access. The access cand be performed whether using the polling 
            mode by calling the functions BSP_SD_ReadBlocks()/BSP_SD_WriteBlocks(), or by DMA 
            transfer using the functions BSP_SD_ReadBlocks_DMA()/BSP_SD_WriteBlocks_DMA()
       (++) The DMA transfer complete is used with interrupt mode. Once the SD transfer
            is complete, the SD interrupt is handeled using the function BSP_SD_IRQHandler(),
            the DMA Tx/Rx transfer complete are handeled using the functions
            BSP_SD_DMA_Tx_IRQHandler()/BSP_SD_DMA_Rx_IRQHandler(). The corresponding user callbacks 
            are implemented by the user at application level. 
       (++) The SD erase block(s) is performed using the function BSP_SD_Erase() with specifying
            the number of blocks to erase.
       (++) The SD runtime status is returned when calling the function BSP_SD_GetCardState().
   [..] 
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm3210e_eval_sd.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM3210E_EVAL
  * @{
  */ 
  
/** @defgroup STM3210E_EVAL_SD STM3210E EVAL SD
  * @{
  */ 

/** @defgroup STM3210E_EVAL_SD_Private_Variables STM3210E EVAL SD Private Variables
  * @{
  */       
SD_HandleTypeDef uSdHandle;
/**
  * @}
  */ 

/** @defgroup STM3210E_EVAL_SD_Private_Functions STM3210E EVAL SD Private Functions
  * @{
  */ 
uint8_t SD_DMAConfigRx(SD_HandleTypeDef *hsd);
uint8_t SD_DMAConfigTx(SD_HandleTypeDef *hsd);

/**
  * @}
  */

/** @defgroup STM3210E_EVAL_SD_Exported_Functions STM3210E EVAL SD Exported Functions
  * @{
  */

/**
  * @brief  Initializes the SD card device.
  * @retval SD status.
  */
uint8_t BSP_SD_Init(void)
{ 
  uint8_t state = MSD_OK;
  
  /* uSD device interface configuration */
  uSdHandle.Instance = SDIO;

  uSdHandle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
  uSdHandle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
  uSdHandle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.BusWide             = SDIO_BUS_WIDE_1B;
  uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.ClockDiv            = SDIO_TRANSFER_CLK_DIV;
  
  /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    return MSD_ERROR;
  }
  
  /* Msp SD initialization */
  BSP_SD_MspInit(NULL);

  if(HAL_SD_Init(&uSdHandle) != HAL_OK)
  {
    state = MSD_ERROR;
  }
  
  /* Configure SD Bus width */
  if(state == MSD_OK)
  {
    /* Enable wide operation */
    if(HAL_SD_ConfigWideBusOperation(&uSdHandle, SDIO_BUS_WIDE_4B) != HAL_OK)
    {
      state = MSD_ERROR;
    }
    else
    {
      state = MSD_OK;
    }
  }
  
  return  state;
}

/**
  * @brief  Configures Interrupt mode for SD detection pin.
  * @retval Returns 0
  */
uint8_t BSP_SD_ITConfig(void)
{ 
  GPIO_InitTypeDef gpioinitstruct = {0};
  
  /* Configure Interrupt mode for SD detection pin */ 
  gpioinitstruct.Mode      = GPIO_MODE_IT_RISING_FALLING;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpioinitstruct.Pin       = SD_DETECT_PIN;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpioinitstruct);
    
  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SD_DETECT_IRQn, 0xE, 0);
  HAL_NVIC_EnableIRQ(SD_DETECT_IRQn);
  
  return 0;
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_SD_IsDetected(void)
{
  __IO uint8_t status = SD_PRESENT;

  /* Check SD card detect pin */
  if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET) 
  {
    status = SD_NOT_PRESENT;
  }
  
  return status;
}

/** @brief  SD detect IT treatment
  */
void BSP_SD_DetectIT(void)
{
  /* SD detect IT callback */
  BSP_SD_DetectCallback();
  
}


/** @brief  SD detect IT detection callback
  */
__weak void BSP_SD_DetectCallback(void)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
  the BSP_SD_DetectCallback could be implemented in the user file
  */ 
  
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  NumOfBlocks: Number of SD blocks to read
  * @param  Timeout: Timeout for read operation
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  if(HAL_SD_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) != HAL_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write
  * @param  Timeout: Timeout for write operation
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  if(HAL_SD_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{ 
  uint8_t state = MSD_OK;
  
  /* Invalidate the dma tx handle*/
  uSdHandle.hdmatx = NULL;
    
  /* Prepare the dma channel for a read operation */
  state = ((SD_DMAConfigRx(&uSdHandle) == MSD_OK) ? MSD_OK : MSD_ERROR);
  
  if(state == MSD_OK)
  {
    /* Read block(s) in DMA transfer mode */
    state = ((HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks) == HAL_OK) ? MSD_OK : MSD_ERROR);
  }
  
  return state; 
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write 
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{ 
  uint8_t state = MSD_OK;
  
  /* Invalidate the dma rx handle*/
  uSdHandle.hdmarx = NULL;
    
  /* Prepare the dma channel for a read operation */
  state = ((SD_DMAConfigTx(&uSdHandle) == MSD_OK) ? MSD_OK : MSD_ERROR);
  
  if(state == MSD_OK)
  { 
    /* Write block(s) in DMA transfer mode */
    state = ((HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks) == HAL_OK) ? MSD_OK : MSD_ERROR);
  }
  
  return state;  
}

/**
  * @brief  Erases the specified memory area of the given SD card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  if(HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != HAL_OK)
  {
    return MSD_ERROR;
  }
  else
  {
    return MSD_OK;
  }
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  */
uint8_t BSP_SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}
  

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  * @retval None 
  */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
  /* Get SD card Information */
  HAL_SD_GetCardInfo(&uSdHandle, CardInfo);
}

/**
  * @}
  */  


/** @addtogroup STM3210E_EVAL_SD_Private_Functions
  * @{
  */
  
/**
  * @brief  Initializes the SD MSP.
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void BSP_SD_MspInit(void *Params)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  
  /* Enable SDIO clock */
  __HAL_RCC_SDIO_CLK_ENABLE();
  
  /* Enable DMA2 clocks */
  __DMAx_TxRx_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __SD_DETECT_GPIO_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  
  /* GPIOC configuration */
  gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
   
  HAL_GPIO_Init(GPIOC, &gpioinitstruct);

  /* GPIOD configuration */
  gpioinitstruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* SD Card detect pin configuration */
  gpioinitstruct.Mode      = GPIO_MODE_INPUT;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpioinitstruct.Pin       = SD_DETECT_PIN;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpioinitstruct);
    
  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SDIO_IRQn, 0xC, 0);
  HAL_NVIC_EnableIRQ(SDIO_IRQn);
  
  /* DMA initialization should be done here but , as there is only one channel for RX and TX it is configured and done directly when required*/
}

/**
  * @brief SD_DMAConfigRx
  * @par Function Description
  *   This function configure the DMA to receive data from the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
uint8_t SD_DMAConfigRx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_rx;
  HAL_StatusTypeDef status = HAL_ERROR;
  
  if(hsd->hdmarx == NULL)
  {
    /* Configure DMA Rx parameters */
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_rx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

    hdma_rx.Instance = SD_DMAx_Rx_INSTANCE;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsd, hdmarx, hdma_rx);

    /* Stop any ongoing transfer and reset the state*/
    HAL_DMA_Abort(&hdma_rx);
    
    /* Deinitialize the Channel for new transfer */
    HAL_DMA_DeInit(&hdma_rx);

    /* Configure the DMA Channel */
    status = HAL_DMA_Init(&hdma_rx);
    
    /* NVIC configuration for DMA transfer complete interrupt */
    HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn, 0xD, 0);
    HAL_NVIC_EnableIRQ(SD_DMAx_Rx_IRQn);
  }
  else
  {
    status = HAL_OK;
  }
  
  return (status != HAL_OK? MSD_ERROR : MSD_OK);
}

/**
  * @brief SD_DMAConfigTx
  * @par Function Description
  *   This function configure the DMA to transmit data to the SD card
  * @retval
  *  SD_ERROR or SD_OK
  */
uint8_t SD_DMAConfigTx(SD_HandleTypeDef *hsd)
{
  static DMA_HandleTypeDef hdma_tx;
  HAL_StatusTypeDef status;
  
  if(hsd->hdmatx == NULL)
  {
    /* Configure DMA Tx parameters */
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_tx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    
    hdma_tx.Instance = SD_DMAx_Tx_INSTANCE;
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hsd, hdmatx, hdma_tx);
    
    /* Stop any ongoing transfer and reset the state*/
    HAL_DMA_Abort(&hdma_tx);
    
    /* Deinitialize the Channel for new transfer */
    HAL_DMA_DeInit(&hdma_tx);
    
    /* Configure the DMA Channel */
    status = HAL_DMA_Init(&hdma_tx); 
    
    /* NVIC configuration for DMA transfer complete interrupt */
    HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn, 0xD, 0);
    HAL_NVIC_EnableIRQ(SD_DMAx_Tx_IRQn);  
  }
  else
  {
    status = HAL_OK;
  }
  
  return (status != HAL_OK? MSD_ERROR : MSD_OK);
}

/**
  * @brief SD Abort callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_AbortCallback();
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_WriteCpltCallback();
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_ReadCpltCallback();
}

/**
  * @brief BSP SD Abort callbacks
  * @retval None
  */
__weak void BSP_SD_AbortCallback(void)
{

}

/**
  * @brief BSP Tx Transfer completed callbacks
  * @retval None
  */
__weak void BSP_SD_WriteCpltCallback(void)
{

}

/**
  * @brief BSP Rx Transfer completed callbacks
  * @retval None
  */
__weak void BSP_SD_ReadCpltCallback(void)
{

}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
