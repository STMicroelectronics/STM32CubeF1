/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/Src/usbd_conf.c
  * @author  MCD Application Team
  * @brief   This file implements the USB Device library callbacks and MSP
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
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
#define USB_DISCONNECT_PORT                 GPIOB
#define USB_DISCONNECT_PIN                  GPIO_PIN_14

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
PCD_HandleTypeDef hpcd;
__IO uint32_t remotewakeupon = 0;

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */
static void SystemClockConfig_STOP(void);

/*******************************************************************************
                       PCD BSP Routines
*******************************************************************************/

/**
  * @brief  Initializes the PCD MSP.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_MspInit(PCD_HandleTypeDef * hpcd)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure USB DM/DP pins */
  GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
  GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Enable the USB disconnect GPIO clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USB_DISCONNECT used as USB pull-up */
  GPIO_InitStruct.Pin = USB_DISCONNECT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  HAL_GPIO_Init(USB_DISCONNECT_PORT, &GPIO_InitStruct);

  /* Enable USB Clock */
  __HAL_RCC_USB_CLK_ENABLE();

  if (hpcd->Init.low_power_enable == 1)
  {
    /* Enable EXTI for USB wakeup */
    __HAL_USB_WAKEUP_EXTI_CLEAR_FLAG();
    __HAL_USB_WAKEUP_EXTI_ENABLE_RISING_EDGE();
    __HAL_USB_WAKEUP_EXTI_ENABLE_IT();

    /* USB Wakeup Interrupt */
    HAL_NVIC_EnableIRQ(USBWakeUp_IRQn);

    /* Enable USB Wake-up interrupt */
    HAL_NVIC_SetPriority(USBWakeUp_IRQn, 0, 0);
  }

  /* Set USB Interrupt priority */
  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5, 0);

  /* Enable USB Interrupt */
  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

/**
  * @brief  De-Initializes the PCD MSP.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef * hpcd)
{
  /* Disable USB FS Clock */
  __HAL_RCC_USB_CLK_DISABLE();
}

/*******************************************************************************
                       LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/

/**
  * @brief  SetupStage callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_SetupStage((USBD_HandleTypeDef *) hpcd->pData,
                     (uint8_t *) hpcd->Setup);
}

/**
  * @brief  DataOut Stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_DataOutStage((USBD_HandleTypeDef *) hpcd->pData, epnum,
                       hpcd->OUT_ep[epnum].xfer_buff);
}

/**
  * @brief  DataIn Stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_DataInStage((USBD_HandleTypeDef *) hpcd->pData, epnum,
                      hpcd->IN_ep[epnum].xfer_buff);
}

/**
  * @brief  SOF callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_SOF((USBD_HandleTypeDef *) hpcd->pData);
}

/**
  * @brief  Reset callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_SetSpeed((USBD_HandleTypeDef *) hpcd->pData, USBD_SPEED_FULL);
  /* Reset Device */
  USBD_LL_Reset((USBD_HandleTypeDef *) hpcd->pData);
}

/**
  * @brief  Suspend callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef * hpcd)
{
  /* Inform USB library that core enters in suspend Mode */
  USBD_LL_Suspend((USBD_HandleTypeDef *) hpcd->pData);

  /* Enter in STOP mode */
  if (hpcd->Init.low_power_enable)
  {
    /* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
    SCB->SCR |=
      (uint32_t) ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
  }
  USBD_LL_Delay(50);
}

/**
  * @brief  Resume callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef * hpcd)
{
  if ((hpcd->Init.low_power_enable) && (remotewakeupon == 0))
  {
    SystemClockConfig_STOP();

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &=
      (uint32_t) ~
      ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
  }
  USBD_LL_Resume((USBD_HandleTypeDef *) hpcd->pData);
  remotewakeupon = 0;
}

/**
  * @brief  ISOOUTIncomplete callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_IsoOUTIncomplete((USBD_HandleTypeDef *) hpcd->pData, epnum);
}

/**
  * @brief  ISOINIncomplete callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum)
{
  USBD_LL_IsoINIncomplete((USBD_HandleTypeDef *) hpcd->pData, epnum);
}

/**
  * @brief  ConnectCallback callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_DevConnected((USBD_HandleTypeDef *) hpcd->pData);
}

/**
  * @brief  Disconnect callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef * hpcd)
{
  USBD_LL_DevDisconnected((USBD_HandleTypeDef *) hpcd->pData);
}

/*******************************************************************************
                       LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

/**
  * @brief  Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef * pdev)
{
  /* Set LL Driver parameters */
  hpcd.Instance = USB;
  hpcd.Init.dev_endpoints = 8;
  hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd.Init.speed = PCD_SPEED_FULL;
  hpcd.Init.low_power_enable = 0;

  /* Link The driver to the stack */
  hpcd.pData = pdev;
  pdev->pData = &hpcd;

  /* Initialize LL Driver */
  HAL_PCD_Init((PCD_HandleTypeDef *) pdev->pData);

  HAL_PCDEx_PMAConfig((PCD_HandleTypeDef *) pdev->pData, 0x00, PCD_SNG_BUF,
                      0x18);
  HAL_PCDEx_PMAConfig((PCD_HandleTypeDef *) pdev->pData, 0x80, PCD_SNG_BUF,
                      0x58);
  HAL_PCDEx_PMAConfig((PCD_HandleTypeDef *) pdev->pData, 0x81, PCD_SNG_BUF,
                      0x100);

  return USBD_OK;
}

/**
  * @brief  De-Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef * pdev)
{
  HAL_PCD_DeInit((PCD_HandleTypeDef *) pdev->pData);
  return USBD_OK;
}

/**
  * @brief  Starts the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef * pdev)
{
  HAL_PCD_Start((PCD_HandleTypeDef *) pdev->pData);
  return USBD_OK;
}

/**
  * @brief  Stops the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef * pdev)
{
  HAL_PCD_Stop((PCD_HandleTypeDef *) pdev->pData);
  return USBD_OK;
}

/**
  * @brief  Opens an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  ep_type: Endpoint Type
  * @param  ep_mps: Endpoint Max Packet Size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef * pdev,
                                  uint8_t ep_addr,
                                  uint8_t ep_type, uint16_t ep_mps)
{
  HAL_PCD_EP_Open((PCD_HandleTypeDef *) pdev->pData, ep_addr, ep_mps, ep_type);

  return USBD_OK;
}

/**
  * @brief  Closes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  HAL_PCD_EP_Close((PCD_HandleTypeDef *) pdev->pData, ep_addr);
  return USBD_OK;
}

/**
  * @brief  Flushes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  HAL_PCD_EP_Flush((PCD_HandleTypeDef *) pdev->pData, ep_addr);
  return USBD_OK;
}

/**
  * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  HAL_PCD_EP_SetStall((PCD_HandleTypeDef *) pdev->pData, ep_addr);
  return USBD_OK;
}

/**
  * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef * pdev,
                                        uint8_t ep_addr)
{
  HAL_PCD_EP_ClrStall((PCD_HandleTypeDef *) pdev->pData, ep_addr);
  return USBD_OK;
}

/**
  * @brief  Returns Stall condition.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Stall (1: Yes, 0: No)
  */
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *) pdev->pData;

  if ((ep_addr & 0x80) == 0x80)
  {
    return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
  }
  else
  {
    return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
  }
}

/**
  * @brief  Assigns a USB address to the device.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef * pdev,
                                         uint8_t dev_addr)
{
  HAL_PCD_SetAddress((PCD_HandleTypeDef *) pdev->pData, dev_addr);
  return USBD_OK;
}

/**
  * @brief  Transmits data over an endpoint.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be sent
  * @param  size: Data size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef * pdev,
                                    uint8_t ep_addr,
                                    uint8_t * pbuf, uint16_t size)
{
  HAL_PCD_EP_Transmit((PCD_HandleTypeDef *) pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;
}

/**
  * @brief  Prepares an endpoint for reception.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be received
  * @param  size: Data size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef * pdev,
                                          uint8_t ep_addr,
                                          uint8_t * pbuf, uint16_t size)
{
  HAL_PCD_EP_Receive((PCD_HandleTypeDef *) pdev->pData, ep_addr, pbuf, size);
  return USBD_OK;
}

/**
  * @brief  Returns the last transferred packet size.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Received Data Size
  */
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
  return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef *) pdev->pData, ep_addr);
}

/**
  * @brief  Delays routine for the USB Device Library.
  * @param  Delay: Delay in ms
  * @retval None
  */
void USBD_LL_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/**
  * @brief  static single allocation.
  * @param  size: size of allocated memory
  * @retval None
  */
void *USBD_static_malloc(uint32_t size)
{
  static uint32_t mem[MAX_STATIC_ALLOC_SIZE];
  return mem;
}

/**
  * @brief  Dummy memory free
  * @param  *p pointer to allocated  memory address
  * @retval None
  */
void USBD_static_free(void *p)
{

}

/**
  * @brief  Software Device Connection
  * @param  hpcd: PCD handle
  * @param  state: connection state (0 : disconnected / 1: connected)
  * @retval None
  */
void HAL_PCDEx_SetConnectionState(PCD_HandleTypeDef * hpcd, uint8_t state)
{
  if (state != 0)
  {
    /* Enabling DP Pull-Down bit to Connect internal pull-up on USB DP line */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
  }
  else
  {
    /* Disable DP Pull-Down bit */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  }
}

/**
  * @brief  Configures system clock after wakeup from STOP mode.
  * @param  None
  * @retval None
  */
static void SystemClockConfig_STOP(void)
{
  RCC_ClkInitTypeDef clkinitstruct = { 0 };
  RCC_OscInitTypeDef oscinitstruct = { 0 };

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;

  oscinitstruct.PLL.PLLState = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  HAL_RCC_OscConfig(&oscinitstruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
   * clocks dividers */
  clkinitstruct.ClockType =
    (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 |
     RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2);
}


/**
  * @brief  GPIO EXTI Callback function
  *         Handle remote-wakeup through key button
  * @param  GPIO_Pin
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == KEY_BUTTON_PIN)
  {
    if ((((USBD_HandleTypeDef *) hpcd.pData)->dev_remote_wakeup == 1) &&
        (((USBD_HandleTypeDef *) hpcd.pData)->dev_state ==
         USBD_STATE_SUSPENDED))
    {
      if ((&hpcd)->Init.low_power_enable)
      {
        /* Reset SLEEPDEEP bit of Cortex System Control Register */
        SCB->SCR &=
          (uint32_t) ~
          ((uint32_t) (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

        SystemClockConfig_STOP();
      }

      /* Activate Remote wakeup */
      HAL_PCD_ActivateRemoteWakeup((&hpcd));

      /* remote wakeup delay */
      HAL_Delay(10);

      /* Disable Remote wakeup */
      HAL_PCD_DeActivateRemoteWakeup((&hpcd));

      /* change remote_wakeup feature to 0 */
      ((USBD_HandleTypeDef *) hpcd.pData)->dev_remote_wakeup = 0;
      remotewakeupon = 1;
    }
  }
}
