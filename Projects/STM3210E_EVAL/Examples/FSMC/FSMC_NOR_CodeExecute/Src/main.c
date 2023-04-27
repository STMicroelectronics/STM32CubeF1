/**
  ******************************************************************************
  * @file    FSMC/FSMC_NOR_CodeExecute/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F1xx FSMC HAL API to access
  *          by read and write operation the NOR external memory device.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup FSMC_NOR_CodeExecute
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
#define ApplicationAddress       ((uint32_t)0x64000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
__IO uint32_t JumpAddress;

NOR_HandleTypeDef hNor;
FSMC_NORSRAM_TimingTypeDef NOR_Timing;

/* Private functions ---------------------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F103xG HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */

  HAL_Init();

  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /*##-1- Configure the NOR device ##########################################*/
  /* NOR device configuration */

  hNor.Instance  = FSMC_NORSRAM_DEVICE;
  hNor.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;

  /* NOR device configuration */  
  NOR_Timing.AddressSetupTime       = 2;
  NOR_Timing.AddressHoldTime        = 1;
  NOR_Timing.DataSetupTime          = 5;
  NOR_Timing.BusTurnAroundDuration  = 1;
  NOR_Timing.CLKDivision            = 2;
  NOR_Timing.DataLatency            = 1;
  NOR_Timing.AccessMode             = FSMC_ACCESS_MODE_B;
  
  hNor.Init.NSBank                 = FSMC_NORSRAM_BANK2;
  hNor.Init.DataAddressMux         = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hNor.Init.MemoryType             = FSMC_MEMORY_TYPE_NOR;
  hNor.Init.MemoryDataWidth        = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hNor.Init.BurstAccessMode        = FSMC_BURST_ACCESS_MODE_DISABLE;
  hNor.Init.WaitSignalPolarity     = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hNor.Init.WrapMode               = FSMC_WRAP_MODE_DISABLE;
  hNor.Init.WaitSignalActive       = FSMC_WAIT_TIMING_BEFORE_WS;
  hNor.Init.WriteOperation         = FSMC_WRITE_OPERATION_ENABLE;
  hNor.Init.WaitSignal             = FSMC_WAIT_SIGNAL_DISABLE;
  hNor.Init.ExtendedMode           = FSMC_EXTENDED_MODE_DISABLE;
  hNor.Init.AsynchronousWait       = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hNor.Init.WriteBurst             = FSMC_WRITE_BURST_DISABLE;

  /* Initialize the NOR controller */
  if(HAL_NOR_Init(&hNor, &NOR_Timing, &NOR_Timing) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Jump to code loaded in NOR memory and execute it *************************/
  JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
  Jump_To_Application = (pFunction) JumpAddress;

  /* Initialize user application's Stack Pointer */
  __set_MSP(*(__IO uint32_t*) ApplicationAddress); 

  Jump_To_Application();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
