/**
  ******************************************************************************
  * @file    FSMC/FSMC_NOR/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F1xx FSMC HAL API to access
  *          by read and write operation the NOR external memory device.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup FSMC_NOR
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x400)
#define WRITE_READ_ADDR     ((uint32_t)0x8000)
#define MANUFACTURER_CODE   ((uint16_t)0x0020)
#define DEVICE_CODE1        ((uint16_t)0x227E)
#define DEVICE_CODE2        ((uint16_t)0x2221)
#define DEVICE_CODE3        ((uint16_t)0x2200)  /*00h for M29W128GL.*/

#define NOR_BANK_ADDR       ((uint32_t)0x64000000)
#define NOR_TIMEOUT_VALUE   ((uint32_t)0xFFFF)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NOR_HandleTypeDef hNor;
FSMC_NORSRAM_TimingTypeDef NOR_Timing;
/* NOR IDs structure */
static NOR_IDTypeDef NOR_Id;

/* Read/Write Buffers */
uint16_t aTxBuffer[BUFFER_SIZE];
uint16_t aRxBuffer[BUFFER_SIZE];

/* Status variables */
__IO uint32_t uwWriteReadStatus = 0;

/* Counter index */
uint32_t uwIndex = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint16_t uwOffset);
static TestStatus Buffercmp(uint16_t *pBuffer1, uint16_t *pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

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

  uint16_t *pdata = NULL;
  uint32_t index  = 0;
  uint32_t startaddress = 0;

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

  /* Read NOR memory ID */
  if(HAL_NOR_Read_ID(&hNor, &NOR_Id) != HAL_OK)
  {
    /* NOR read ID Error */
    Error_Handler();
  }

   /* Test the NOR ID correctness */
  if((NOR_Id.Manufacturer_Code != (uint16_t)MANUFACTURER_CODE) ||
     (NOR_Id.Device_Code1 != (uint16_t)DEVICE_CODE1) ||
     (NOR_Id.Device_Code2 != (uint16_t)DEVICE_CODE2) ||
     (NOR_Id.Device_Code3 != (uint16_t)DEVICE_CODE3))
  {
    /* NOR ID not correct */
    Error_Handler();
  }

  /* Return to read mode */
  HAL_NOR_ReturnToReadMode(&hNor);
  
  /* Erase the NOR memory block to write on */
  HAL_NOR_Erase_Block(&hNor, WRITE_READ_ADDR, NOR_BANK_ADDR);
  
  /* Return the NOR memory status */  
  if(HAL_NOR_GetStatus(&hNor, NOR_BANK_ADDR, NOR_TIMEOUT_VALUE) != HAL_NOR_STATUS_SUCCESS)
  {
    /* Erase Error */
    Error_Handler();
  }

  /*##-2- NOR memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xC20F);

  /* Write data to the NOR memory */
  pdata = aTxBuffer;
  index = BUFFER_SIZE;
  startaddress = NOR_BANK_ADDR + WRITE_READ_ADDR;
  while(index > 0)
  {
    /* Write data to NOR */
    HAL_NOR_Program(&hNor, (uint32_t *)startaddress, pdata);
    
    /* Read NOR device status */
    if(HAL_NOR_GetStatus(&hNor, NOR_BANK_ADDR, NOR_TIMEOUT_VALUE) != HAL_NOR_STATUS_SUCCESS)
    {
      Error_Handler();
    }
    
    /* Update the counters */
    index--;
    startaddress += 2;
    pdata++; 
  }

  /* Read back data from the NOR memory */
  if(HAL_NOR_ReadBuffer(&hNor, NOR_BANK_ADDR + WRITE_READ_ADDR, aRxBuffer, BUFFER_SIZE) != HAL_OK)
  {
      Error_Handler();
  }

  /*##-3- Checking data integrity ############################################*/
  uwWriteReadStatus = Buffercmp(aTxBuffer, aRxBuffer, BUFFER_SIZE);

  if(uwWriteReadStatus != PASSED)
  {
    /* KO */
    /* Turn on LED2 */
    BSP_LED_On(LED2);
  }
  else
  {
    /* OK */
    /* Turn on LED1 */
    BSP_LED_On(LED1);
  }

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

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint16_t uwOffset)
{
  uint16_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLength; tmpIndex++)
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
static TestStatus Buffercmp(uint16_t *pBuffer1, uint16_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
