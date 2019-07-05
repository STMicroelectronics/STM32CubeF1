/**
  ******************************************************************************
  * @file    BSP/Src/temperature_sensor.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to manage an I2C Temperature Sensor
  *          =================================================================== 
  *          Notes:
  *           - This driver is intended for STM32F1xx families devices only. 
  *           - The I2C Temperature Sensor is available on the STM3210E-EVAL RevD board.
  *          ===================================================================
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

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus; 
typedef enum {NOT_READY = 0, READY = !NOT_READY} Status; 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TSENSOR_SetHint(void);
static void TSENSOR_Display_Temperature(uint16_t temperature);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM Demo
  * @param  None
  * @retval None
  */
void TSENSOR_demo (void)
{ 
  uint16_t TempValue = 0;
  uint8_t  TsensorReady = 0;
  uint8_t  RequestSample = 0;
  uint32_t Tick = 0;

  TSENSOR_SetHint();

  /* Initialize the I2C TSENSOR driver ----------------------------------------*/
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  if (BSP_TSENSOR_Init() != TSENSOR_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Temperature Sensor not detected", CENTER_MODE); 
  }
  else
  {
    TsensorReady = READY;
    RequestSample = SET;
  }

  while (1)
  {
    if((TsensorReady == READY) && (RequestSample == SET))
    {
      TempValue = BSP_TSENSOR_ReadTemp();
      TSENSOR_Display_Temperature(TempValue);
      Tick = HAL_GetTick();
      RequestSample = RESET;
    }
    else
    {
      /* Request a Temperature sampling each 1s <-> 1000 ms */
      if(HAL_GetTick() >= Tick + 1000)
      {
        RequestSample = SET;
      }
    }
    
    
    if(CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay(5);
  }
}

/**
  * @brief  Display TSENSOR Demo Hint
  * @param  None
  * @retval None
  */
static void TSENSOR_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"TEMPERATURE SENSOR", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"read a Temperature", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"Press key button to switch to next menu", CENTER_MODE); 
  
   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
 }

  /**
  * @brief  Display temperature
  * @param  temperature : temperature value
  * @retval None
  */
static void TSENSOR_Display_Temperature(uint16_t temperature)
{
  uint8_t TempCelsiusDisplay[20] = {0};

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Measured Temperature : ", CENTER_MODE);

  /* Change Font size to display Temperature Value */
  BSP_LCD_SetFont(&Font16);

  if (temperature & 0x8000)
  {
    /* Display negative temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    if (temperature & 0x0001)
    {
      sprintf ((char*)TempCelsiusDisplay, " -%d.5 C ", ((temperature & 0x7FFF)>>1));
    }
    else
    {
      sprintf ((char*)TempCelsiusDisplay, "  -%d C  ", ((temperature & 0x7FFF)>>1));
    }
  }
  else
  {
    /* Display positive temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    if (temperature & 0x0001)
    {
      sprintf ((char*)TempCelsiusDisplay, "  %d.5 C ", ((temperature & 0x7FFF)>>1));
    }
    else
    {
      sprintf ((char*)TempCelsiusDisplay, "  %d C   ", ((temperature & 0x7FFF)>>1));
    }
  }
  /* Display Temperature value on LCD */
  BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)TempCelsiusDisplay, CENTER_MODE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
}

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
