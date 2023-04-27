/**
  ******************************************************************************
  * @file    FSMC/FSMC_NAND/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F1xx FSMC HAL API to access
  *          by read and write operation the NAND external memory device.
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

/** @addtogroup FSMC_NAND
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WRITE_READ_ADDR         ((uint32_t)0x8000)
#define NAND_ST_MAKERID         ((uint32_t)0x20)
#define NAND_ST_DEVICEID        ((uint32_t)0x76)

#define NAND_PAGE_SIZE          ((uint16_t)0x0200) /* 512 bytes per page w/o Spare Area */
#define NAND_BLOCK_SIZE         ((uint16_t)0x0020) /* 32x512 bytes pages per block */
#define NAND_PLANE_SIZE         ((uint16_t)0x0200) /* 1024 Block per plane */
#define NAND_SPARE_AREA_SIZE    ((uint16_t)0x0010) /* last 16 bytes as spare area */
#define NAND_MAX_PLANE          ((uint16_t)0x1000) /* 4 planes of 1024 block */

#define NB_PAGE                 ((uint32_t)2)
#define BUFFER_SIZE             (NAND_PAGE_SIZE * NB_PAGE)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NAND_HandleTypeDef nandHandle;
FSMC_NAND_PCC_TimingTypeDef NAND_Timing;
/* NAND IDs structure */
static NAND_IDTypeDef NAND_Id;
static NAND_AddressTypeDef NAND_Address;
/* Read/Write Buffers */
uint8_t nand_aTxBuffer[BUFFER_SIZE];
uint8_t nand_aRxBuffer[BUFFER_SIZE];

/* Status variables */
__IO uint32_t uwWriteReadStatus = 0;

/* Counter index */
uint32_t uwIndex = 0;

/* Private function prototypes -----------------------------------------------*/
void    SystemClock_Config(void);
static  void Error_Handler(void);
static  void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLenght, uint32_t Offset);
static  TestStatus Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t BufferLength);
static  void NAND_GetAddress (uint32_t Address, NAND_AddressTypeDef *pNandAddress);

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
  HAL_Init();

  /* Configure LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /*##-1- Configure the NAND device ##########################################*/
  /* NAND device configuration */
  nandHandle.Instance  = FSMC_NAND_DEVICE;
  
  /*NAND Configuration */  
  NAND_Timing.SetupTime     = 0;
  NAND_Timing.WaitSetupTime = 2; 
  NAND_Timing.HoldSetupTime = 1;
  NAND_Timing.HiZSetupTime  = 0;
  
  nandHandle.Init.NandBank        = FSMC_NAND_BANK2;
  nandHandle.Init.Waitfeature     = FSMC_NAND_PCC_WAIT_FEATURE_ENABLE;
  nandHandle.Init.MemoryDataWidth = FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
  nandHandle.Init.EccComputation  = FSMC_NAND_ECC_ENABLE;
  nandHandle.Init.ECCPageSize     = FSMC_NAND_ECC_PAGE_SIZE_512BYTE;
  nandHandle.Init.TCLRSetupTime   = 0;
  nandHandle.Init.TARSetupTime    = 0;
  
  nandHandle.Config.BlockNbr      = NAND_MAX_PLANE;
  nandHandle.Config.BlockSize     = NAND_BLOCK_SIZE;
  nandHandle.Config.PlaneSize     = NAND_PLANE_SIZE;
  nandHandle.Config.PageSize      = NAND_PAGE_SIZE; 
  nandHandle.Config.SpareAreaSize = NAND_SPARE_AREA_SIZE;
  
  /* Initialize the NAND controller */
  if(HAL_NAND_Init(&nandHandle, &NAND_Timing, &NAND_Timing) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Read NAND memory ID */
  if(HAL_NAND_Read_ID(&nandHandle, &NAND_Id) != HAL_OK)
  {
    /* NAND read ID Error */
    Error_Handler();
  }

   /* Test the NAND ID correctness */
  if((NAND_Id.Maker_Id != NAND_ST_MAKERID) || (NAND_Id.Device_Id != NAND_ST_DEVICEID))
  {
    /* NAND ID not correct */
    Error_Handler();
  }
  
  /*##-2- Convert Address to NAND address#######################################*/ 
  NAND_GetAddress(WRITE_READ_ADDR, &NAND_Address);
  
  /*##-3- Erase NAND memory ###################################################*/ 
  if(HAL_NAND_Erase_Block(&nandHandle, &NAND_Address) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-4- NAND memory read/write access  ######################################*/   
  /* Fill the buffer to write */
  Fill_Buffer(nand_aTxBuffer, BUFFER_SIZE, 0xD210);   
  
  /* Write data to the NAND memory */
  if(HAL_NAND_Write_Page_8b(&nandHandle, &NAND_Address, nand_aTxBuffer, NB_PAGE) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Read back data from the NAND memory */
  if(HAL_NAND_Read_Page_8b(&nandHandle, &NAND_Address, nand_aRxBuffer, NB_PAGE) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-3- Checking data integrity ############################################*/
  if(Buffercmp(nand_aTxBuffer, nand_aRxBuffer, BUFFER_SIZE) != PASSED)
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
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < uwBufferLenght; index++ )
  {
    pBuffer[index] = index + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  uwBufferLenght: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static TestStatus Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t uwBufferLenght)
{
  uint32_t counter = 0;
  while(uwBufferLenght--)
  {
    if(*pBuffer != *pBuffer1)
    {
      return FAILED;
    }
    
    pBuffer++;
    pBuffer1++;
    counter++;
  }

  return PASSED;  
}

/**
  * @brief  Translate logical address into a phy one.
  * @param  Address
  * @param  pNandAddress
  * @retval Status
  */
static void NAND_GetAddress (uint32_t Address, NAND_AddressTypeDef *pNandAddress)
{
  pNandAddress->Page  = (Address % (NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE))) / (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE);
  pNandAddress->Block = (Address % (NAND_PLANE_SIZE * NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE))) / (NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE));
  pNandAddress->Plane = Address / (NAND_PLANE_SIZE * NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE));
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
