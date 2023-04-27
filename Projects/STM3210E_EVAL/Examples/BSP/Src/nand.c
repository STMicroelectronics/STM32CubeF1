/**
  ******************************************************************************
  * @file    BSP/Src/nand.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the NAND Driver
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
/* Private define ------------------------------------------------------------*/
#define NB_PAGE             ((uint32_t)2)
#define BUFFER_SIZE         (NAND_PAGE_SIZE * NB_PAGE)
#define WRITE_READ_ADDR     ((uint32_t)0x4000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t nand_aTxBuffer[BUFFER_SIZE];
uint8_t nand_aRxBuffer[BUFFER_SIZE];
static uint8_t ubIDStatus = 0, ubEraseStatus = 0, ubWriteStatus = 0, ubReadStatus = 0, ubInitStatus = 0;

/* Private function prototypes -----------------------------------------------*/
static void     NAND_SetHint(void);
static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLenght, uint32_t Offset);
static uint8_t Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t BufferLength);
static NAND_AddressTypeDef NAND_GetAddress (uint32_t Address);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  NAND Demo
  * @param  None
  * @retval None
  */
void NAND_demo (void)
{ 
  /* NAND IDs structure */
  static NAND_IDTypeDef pNAND_ID = {0};

  NAND_SetHint();

  /*##-1- Configure the NAND device ###########################################*/
  /* NAND device configuration */ 
  if(BSP_NAND_Init() != NAND_OK)
  {
    ubInitStatus++; 
  }
  
  /*##-2- Read & check the NAND device IDs ####################################*/
  /* Read the NAND memory ID */
  BSP_NAND_Read_ID(&pNAND_ID);
  
  /* Test the NAND ID correctness */
  if(pNAND_ID.Maker_Id != (uint8_t)0x20)
  {
    ubIDStatus++;
  }
  if((pNAND_ID.Device_Id != (uint8_t)0x36) && (pNAND_ID.Device_Id != (uint8_t)0x76))
  {
    ubIDStatus++;
  }
    
  /*##-3- Erase NAND memory ###################################################*/ 
  if(BSP_NAND_Erase_Block(NAND_GetAddress(WRITE_READ_ADDR)) != NAND_OK)
  {
    ubEraseStatus++; 
  }
  
  /*##-4- NAND memory read/write access  ######################################*/   
  /* Fill the buffer to write */
  Fill_Buffer(nand_aTxBuffer, BUFFER_SIZE, 0xD201);   
  
  /* Write data to the NAND memory */
  if(BSP_NAND_WriteData(NAND_GetAddress(WRITE_READ_ADDR), nand_aTxBuffer, NB_PAGE) != NAND_OK)
  {
    ubWriteStatus++; 
  }
  
  /* Read back data from the NAND memory */
  if(BSP_NAND_ReadData(NAND_GetAddress(WRITE_READ_ADDR), nand_aRxBuffer, NB_PAGE) != NAND_OK)
  {
    ubReadStatus++; 
  }

  /*##-5- Checking data integrity ############################################*/  
  if(ubInitStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"NAND Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"NAND Test Aborted.", LEFT_MODE);
  }
  else
  {
    char desc[50];
    if (sprintf(desc,"NAND Init: OK. MAKER=0x%02x, DEVICE=0x%02x", pNAND_ID.Maker_Id, pNAND_ID.Device_Id) > 50)
    {
      Error_Handler();
    }
    /*sprintf(desc,"NAND Init: OK. MAKER=%d, DEVICE=%d %d %d", pNAND_ID.Maker_Id, pNAND_ID.Device_Id, pNAND_ID.Third_Id, pNAND_ID.Fourth_Id);*/
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)desc, LEFT_MODE);
  }  
  if(ubEraseStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"NAND ERASE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"NAND Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"NAND ERASE : OK.   ", LEFT_MODE);
  }
  if(ubWriteStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"NAND WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"NAND Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"NAND WRITE : OK.     ", LEFT_MODE);
  }
  if(ubReadStatus != 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"NAND READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"NAND Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"NAND READ :  OK.    ", LEFT_MODE);
  }    
  if(Buffercmp(nand_aRxBuffer, nand_aTxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"NAND COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t*)"NAND Test Aborted.", LEFT_MODE);
  }
  else
  {    
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"NAND Test : OK.     ", LEFT_MODE);
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
  * @brief  Display NAND Demo Hint
  * @param  None
  * @retval None
  */
static void NAND_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"NAND", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on NAND", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
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
  for (index = 0; index < (uwBufferLenght/2); index++ )
  {
    pBuffer[index] = index + uwOffset;
  }
  /* Put in global buffer same values */
  for (index = (uwBufferLenght/2); index < uwBufferLenght; index++ )
  {
    pBuffer[index] = uwOffset - index;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  uwBufferLenght: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t uwBufferLenght)
{
  uint32_t counter = 0;
  while(uwBufferLenght--)
  {
    if(*pBuffer != *pBuffer1)
    {
      return 1;
    }
    
    pBuffer++;
    pBuffer1++;
    counter++;
  }

  return 0;  
}

/**
  * @brief  Translate logical address into a phy one.
  * @param  Address
  * @retval Status
  */
static NAND_AddressTypeDef NAND_GetAddress (uint32_t Address)
{
  NAND_AddressTypeDef Address_t;

  Address_t.Page  = (Address % (NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE))) / (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE);
  Address_t.Block = (Address % (NAND_PLANE_SIZE * NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE))) / (NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE));
  Address_t.Plane = Address / (NAND_PLANE_SIZE * NAND_BLOCK_SIZE * (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE));

  return Address_t;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
