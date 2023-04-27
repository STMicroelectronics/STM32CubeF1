/**
  ******************************************************************************
  * @file    BSP/Src/flash.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use Flash SPI features.
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

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define  FLASH_WriteSector127 0x7F0000 /* End of Sector 0x7FFFFF */
#define  FLASH_WriteSector126 0x7E0000 /* End of Sector 0x7EFFFF */
#define  FLASH_WriteSector125 0x7D0000 /* End of Sector 0x7DFFFF */
#define  FLASH_WriteSector124 0x7C0000 /* End of Sector 0x7CFFFF */
#define  FLASH_WriteSector123 0x7B0000 /* End of Sector 0x7BFFFF */
#define  FLASH_WriteSector122 0x7A0000 /* End of Sector 0x7AFFFF */
#define  FLASH_WriteSector121 0x790000 /* End of Sector 0x79FFFF */
#define  FLASH_WriteSector120 0x780000 /* End of Sector 0x78FFFF */
#define  FLASH_WriteSector119 0x770000 /* End of Sector 0x77FFFF */
#define  FLASH_WriteSector118 0x760000 /* End of Sector 0x76FFFF */
#define  FLASH_WriteSector117 0x750000 /* End of Sector 0x75FFFF */
#define  FLASH_WriteSector116 0x740000 /* End of Sector 0x74FFFF */
#define  FLASH_WriteSector115 0x730000 /* End of Sector 0x73FFFF */
#define  FLASH_WriteSector114 0x720000 /* End of Sector 0x72FFFF */
#define  FLASH_WriteSector113 0x710000 /* End of Sector 0x71FFFF */
#define  FLASH_WriteSector112 0x700000 /* End of Sector 0x70FFFF */
#define  FLASH_WriteSector111 0x6F0000 /* End of Sector 0x6FFFFF */
#define  FLASH_WriteSector110 0x6E0000 /* End of Sector 0x6EFFFF */
#define  FLASH_WriteSector109 0x6D0000 /* End of Sector 0x6DFFFF */
#define  FLASH_WriteSector108 0x6C0000 /* End of Sector 0x6CFFFF */
#define  FLASH_WriteSector107 0x6B0000 /* End of Sector 0x6BFFFF */
#define  FLASH_WriteSector106 0x6A0000 /* End of Sector 0x6AFFFF */
#define  FLASH_WriteSector105 0x690000 /* End of Sector 0x69FFFF */
#define  FLASH_WriteSector104 0x680000 /* End of Sector 0x68FFFF */
#define  FLASH_WriteSector103 0x670000 /* End of Sector 0x67FFFF */
#define  FLASH_WriteSector102 0x660000 /* End of Sector 0x66FFFF */
#define  FLASH_WriteSector101 0x650000 /* End of Sector 0x65FFFF */
#define  FLASH_WriteSector100 0x640000 /* End of Sector 0x64FFFF */
#define  FLASH_WriteSector99  0x630000 /* End of Sector 0x63FFFF */
#define  FLASH_WriteSector98  0x620000 /* End of Sector 0x62FFFF */
#define  FLASH_WriteSector97  0x610000 /* End of Sector 0x61FFFF */
#define  FLASH_WriteSector96  0x600000 /* End of Sector 0x60FFFF */
#define  FLASH_WriteSector95  0x5F0000 /* End of Sector 0x5FFFFF */
#define  FLASH_WriteSector94  0x5E0000 /* End of Sector 0x5EFFFF */
#define  FLASH_WriteSector93  0x5D0000 /* End of Sector 0x5DFFFF */
#define  FLASH_WriteSector92  0x5C0000 /* End of Sector 0x5CFFFF */
#define  FLASH_WriteSector91  0x5B0000 /* End of Sector 0x5BFFFF */
#define  FLASH_WriteSector90  0x5A0000 /* End of Sector 0x5AFFFF */
#define  FLASH_WriteSector89  0x590000 /* End of Sector 0x59FFFF */
#define  FLASH_WriteSector88  0x580000 /* End of Sector 0x58FFFF */
#define  FLASH_WriteSector87  0x570000 /* End of Sector 0x57FFFF */
#define  FLASH_WriteSector86  0x560000 /* End of Sector 0x56FFFF */
#define  FLASH_WriteSector85  0x550000 /* End of Sector 0x55FFFF */
#define  FLASH_WriteSector84  0x540000 /* End of Sector 0x54FFFF */
#define  FLASH_WriteSector83  0x530000 /* End of Sector 0x53FFFF */
#define  FLASH_WriteSector82  0x520000 /* End of Sector 0x52FFFF */
#define  FLASH_WriteSector81  0x510000 /* End of Sector 0x51FFFF */
#define  FLASH_WriteSector80  0x500000 /* End of Sector 0x50FFFF */
#define  FLASH_WriteSector79  0x4F0000 /* End of Sector 0x4FFFFF */
#define  FLASH_WriteSector78  0x4E0000 /* End of Sector 0x4EFFFF */
#define  FLASH_WriteSector77  0x4D0000 /* End of Sector 0x4DFFFF */
#define  FLASH_WriteSector76  0x4C0000 /* End of Sector 0x4CFFFF */
#define  FLASH_WriteSector75  0x4B0000 /* End of Sector 0x4BFFFF */
#define  FLASH_WriteSector74  0x4A0000 /* End of Sector 0x4AFFFF */
#define  FLASH_WriteSector73  0x490000 /* End of Sector 0x49FFFF */
#define  FLASH_WriteSector72  0x480000 /* End of Sector 0x48FFFF */
#define  FLASH_WriteSector71  0x470000 /* End of Sector 0x47FFFF */
#define  FLASH_WriteSector70  0x460000 /* End of Sector 0x46FFFF */
#define  FLASH_WriteSector69  0x450000 /* End of Sector 0x45FFFF */
#define  FLASH_WriteSector68  0x440000 /* End of Sector 0x44FFFF */
#define  FLASH_WriteSector67  0x430000 /* End of Sector 0x43FFFF */
#define  FLASH_WriteSector66  0x420000 /* End of Sector 0x42FFFF */
#define  FLASH_WriteSector65  0x410000 /* End of Sector 0x41FFFF */
#define  FLASH_WriteSector64  0x400000 /* End of Sector 0x40FFFF */
#define  FLASH_WriteSector63  0x3F0000 /* End of Sector 0x3FFFFF */
#define  FLASH_WriteSector62  0x3E0000 /* End of Sector 0x3EFFFF */
#define  FLASH_WriteSector61  0x3D0000 /* End of Sector 0x3DFFFF */
#define  FLASH_WriteSector60  0x3C0000 /* End of Sector 0x3CFFFF */
#define  FLASH_WriteSector59  0x3B0000 /* End of Sector 0x3BFFFF */
#define  FLASH_WriteSector58  0x3A0000 /* End of Sector 0x3AFFFF */
#define  FLASH_WriteSector57  0x390000 /* End of Sector 0x39FFFF */
#define  FLASH_WriteSector56  0x380000 /* End of Sector 0x38FFFF */
#define  FLASH_WriteSector55  0x370000 /* End of Sector 0x37FFFF */
#define  FLASH_WriteSector54  0x360000 /* End of Sector 0x36FFFF */
#define  FLASH_WriteSector53  0x350000 /* End of Sector 0x35FFFF */
#define  FLASH_WriteSector52  0x340000 /* End of Sector 0x34FFFF */
#define  FLASH_WriteSector51  0x330000 /* End of Sector 0x33FFFF */
#define  FLASH_WriteSector50  0x320000 /* End of Sector 0x32FFFF */
#define  FLASH_WriteSector49  0x310000 /* End of Sector 0x31FFFF */
#define  FLASH_WriteSector48  0x300000 /* End of Sector 0x30FFFF */
#define  FLASH_WriteSector47  0x2F0000 /* End of Sector 0x2FFFFF */
#define  FLASH_WriteSector46  0x2E0000 /* End of Sector 0x2EFFFF */
#define  FLASH_WriteSector45  0x2D0000 /* End of Sector 0x2DFFFF */
#define  FLASH_WriteSector44  0x2C0000 /* End of Sector 0x2CFFFF */
#define  FLASH_WriteSector43  0x2B0000 /* End of Sector 0x2BFFFF */
#define  FLASH_WriteSector42  0x2A0000 /* End of Sector 0x2AFFFF */
#define  FLASH_WriteSector41  0x290000 /* End of Sector 0x29FFFF */
#define  FLASH_WriteSector40  0x280000 /* End of Sector 0x28FFFF */
#define  FLASH_WriteSector39  0x270000 /* End of Sector 0x27FFFF */
#define  FLASH_WriteSector38  0x260000 /* End of Sector 0x26FFFF */
#define  FLASH_WriteSector37  0x250000 /* End of Sector 0x25FFFF */
#define  FLASH_WriteSector36  0x240000 /* End of Sector 0x24FFFF */
#define  FLASH_WriteSector35  0x230000 /* End of Sector 0x23FFFF */
#define  FLASH_WriteSector34  0x220000 /* End of Sector 0x22FFFF */
#define  FLASH_WriteSector33  0x210000 /* End of Sector 0x21FFFF */
#define  FLASH_WriteSector32  0x200000 /* End of Sector 0x20FFFF */
#define  FLASH_WriteSector31  0x1F0000 /* End of Sector 0x1FFFFF */
#define  FLASH_WriteSector30  0x1E0000 /* End of Sector 0x1EFFFF */
#define  FLASH_WriteSector29  0x1D0000 /* End of Sector 0x1DFFFF */
#define  FLASH_WriteSector28  0x1C0000 /* End of Sector 0x1CFFFF */
#define  FLASH_WriteSector27  0x1B0000 /* End of Sector 0x1BFFFF */
#define  FLASH_WriteSector26  0x1A0000 /* End of Sector 0x1AFFFF */
#define  FLASH_WriteSector25  0x190000 /* End of Sector 0x19FFFF */
#define  FLASH_WriteSector24  0x180000 /* End of Sector 0x18FFFF */
#define  FLASH_WriteSector23  0x170000 /* End of Sector 0x17FFFF */
#define  FLASH_WriteSector22  0x160000 /* End of Sector 0x16FFFF */
#define  FLASH_WriteSector21  0x150000 /* End of Sector 0x15FFFF */
#define  FLASH_WriteSector20  0x140000 /* End of Sector 0x14FFFF */
#define  FLASH_WriteSector19  0x130000 /* End of Sector 0x13FFFF */
#define  FLASH_WriteSector18  0x120000 /* End of Sector 0x12FFFF */
#define  FLASH_WriteSector17  0x110000 /* End of Sector 0x11FFFF */
#define  FLASH_WriteSector16  0x100000 /* End of Sector 0x10FFFF */
#define  FLASH_WriteSector15  0x0F0000 /* End of Sector 0x0FFFFF */
#define  FLASH_WriteSector14  0x0E0000 /* End of Sector 0x0EFFFF */
#define  FLASH_WriteSector13  0x0D0000 /* End of Sector 0x0DFFFF */
#define  FLASH_WriteSector12  0x0C0000 /* End of Sector 0x0CFFFF */
#define  FLASH_WriteSector11  0x0B0000 /* End of Sector 0x0BFFFF */
#define  FLASH_WriteSector10  0x0A0000 /* End of Sector 0x0AFFFF */
#define  FLASH_WriteSector9   0x090000 /* End of Sector 0x09FFFF */
#define  FLASH_WriteSector8   0x080000 /* End of Sector 0x08FFFF */
#define  FLASH_WriteSector7   0x070000 /* End of Sector 0x07FFFF */
#define  FLASH_WriteSector6   0x060000 /* End of Sector 0x06FFFF */
#define  FLASH_WriteSector5   0x050000 /* End of Sector 0x05FFFF */
#define  FLASH_WriteSector4   0x040000 /* End of Sector 0x04FFFF */
#define  FLASH_WriteSector3   0x030000 /* End of Sector 0x03FFFF */
#define  FLASH_WriteSector2   0x020000 /* End of Sector 0x02FFFF */
#define  FLASH_WriteSector1   0x010000 /* End of Sector 0x01FFFF */
#define  FLASH_WriteSector0   0x000000 /* End of Sector 0x00FFFF */

#define  FLASH_AddrSector12   0x0C0035  /* Addr inside Sector 12, In case of erase sector at this address Sector 12 shall be erase*/

#define  BufferSize (countof(Tx_Buffer)-1)

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Private variables ---------------------------------------------------------*/
uint8_t Tx_Buffer[] = "STM32F10x SPI Firmware Library Example: communication with an M25P SPI FLASH";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t FlashID = 0;
__IO uint8_t Index = 0x0;

volatile TestStatus TransferStatus1 = PASSED, TransferStatus2 = PASSED, TransferStatus3 = PASSED, Test = PASSED;


/* Private functions ---------------------------------------------------------*/
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void Flush_Rx_Buffers(uint8_t* pBuffer, uint16_t BufferLength);
static void Flash_SetHint(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void FLASH_demo(void)
{
  /* Set Display for Demo Flash SPI */
  Flash_SetHint();
  
  /* Initialize the SPI FLASH driver */
  BSP_SERIAL_FLASH_Init();

  /* Get SPI Flash ID */
  FlashID = BSP_SERIAL_FLASH_ReadID();
  
  /* Check the SPI Flash ID */
  if (FlashID == FLASH_SPI_M25P64_ID)
  {
    /* Initialization OK as BSP_SERIAL_FLASH_ReadID return the right value */
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"FLASH Initialization : OK.", LEFT_MODE);
    
    /* Perform a write in the Flash followed by an erase of the written data */
    /* Write Tx_Buffer data to SPI FLASH memory at beginning of Sector12*/
    BSP_SERIAL_FLASH_WritePage(FLASH_WriteSector12, Tx_Buffer, BufferSize);
    
    /* Erase SPI FLASH Sector12 by giving an Addr inside range of Sector12 Addr */
    BSP_SERIAL_FLASH_EraseSector(FLASH_AddrSector12);
    
    /* Read Erase data from SPI FLASH memory, shall be 0xFF */
    BSP_SERIAL_FLASH_ReadData(FLASH_WriteSector12, Rx_Buffer, BufferSize);

    /* Check the correctness of erasing operation data */
    for (Index = 0; Index < BufferSize; Index++)
    {
      if (Rx_Buffer[Index] != 0xFF)
      {
        TransferStatus1 = FAILED;
      }
    }
    /* Flush Data inside Rx_Buffer */
    Flush_Rx_Buffers(Rx_Buffer, BufferSize);
    
    if(TransferStatus1 == PASSED)
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"FLASH ERASE : OK.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"FLASH ERASE : FAILED.", LEFT_MODE);
      Test = FAILED;
    } 

    /* Perform a write in the Flash followed by a read of the written data */
    /* Write Tx_Buffer data to SPI FLASH memory at Sector0 Addr*/
    BSP_SERIAL_FLASH_WritePage(FLASH_WriteSector0, Tx_Buffer, BufferSize);
    
    /* Read data from SPI FLASH memory at Sector0 Addr*/
    BSP_SERIAL_FLASH_ReadData(FLASH_WriteSector0, Rx_Buffer, BufferSize);
    
    /* Check the correctness of written data */
    TransferStatus2 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
    
    /* Flush Data inside Rx_Buffer */
    Flush_Rx_Buffers(Rx_Buffer, BufferSize);

    if(TransferStatus2 == PASSED)
    {
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"FLASH WRITE : OK.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"FLASH READ  : OK.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"FLASH WRITE : FAILED.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"FLASH READ  : FAILED.", LEFT_MODE);
      Test = FAILED;
    }
    
    if(Test == PASSED)
    {
      BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"FLASH Test : OK.", LEFT_MODE);
    }
    else
    {    
      BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"FLASH Test : FAILED.", LEFT_MODE);
    } 
  }
  else
  {
    /* Error Init: Test FAILED */
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"FLASH Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"FLASH Test : FAILED.", LEFT_MODE);
  }

  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @brief  Display FLASH Demo Hint
  * @param  None
  * @retval None
  */
static void Flash_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"FLASH", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on FLASH SPI", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}

/**
  * @brief  Flushes the receive buffers.
  * @param  None
  * @retval None
  */
static void Flush_Rx_Buffers(uint8_t* pBuffer, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    *pBuffer = 0;
    pBuffer++;
  }
}
/**
  * @}
  */ 
/**
  * @}
  */ 
