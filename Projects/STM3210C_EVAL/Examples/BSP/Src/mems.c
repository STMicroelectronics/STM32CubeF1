/**
  ******************************************************************************
  * @file    BSP/Src/mems.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use MEMS Accelerometer features.
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
#define  CIRCLE_RADIUS        30

#define CIRCLE_UP_X_POS (BSP_LCD_GetXSize()/2)
#define CIRCLE_UP_Y_POS (BSP_LCD_GetYSize()+CIRCLE_RADIUS +15 + 92)

#define CIRCLE_DOWN_X_POS CIRCLE_UP_X_POS
#define CIRCLE_DOWN_Y_POS (BSP_LCD_GetYSize()-CIRCLE_RADIUS-10)

#define CIRCLE_LEFT_X_POS (BSP_LCD_GetXSize()/5)
#define CIRCLE_LEFT_Y_POS (BSP_LCD_GetYSize()-(BSP_LCD_GetYSize()-92)/2)

#define CIRCLE_RIGHT_X_POS (4*(BSP_LCD_GetXSize()/5))
#define CIRCLE_RIGHT_Y_POS CIRCLE_LEFT_Y_POS


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Init af threahold to detect acceleration on MEMS */
/* Typical value: 
      - No  acceleration: X, Y inferior to 37 (positive or negative)
      - Max acceleration: X, Y around 200 (positive or negative) */
int16_t ThresholdHigh = 200;
int16_t ThresholdLow =  37;
/* Private function prototypes -----------------------------------------------*/
static void MEMS_SetHint(void);
static void ACCELERO_ReadAcc(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test ACCELERATOR MEMS Hardware.
  *   The main objective of this test is to check acceleration on 2 axis X and Y
  * @param None
  * @retval None
  */
void ACCELERO_MEMS_Test(void)
{
  MEMS_SetHint();

  /* Init Accelerometer Mems */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"MEMS cannot be initialized", CENTER_MODE); 
    return;
  }

  while (1)
  {
    ACCELERO_ReadAcc();
    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @brief  Display MEMS demo hint
  * @param  None
  * @retval None
  */
static void MEMS_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"MEMS", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows MEMS Features", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"move board around axis", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"to start test", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
 }

static void ACCELERO_ReadAcc(void)
{
  int16_t buffer[3] = {0};
  int16_t xval, yval = 0x00;
  
  /* Read Acceleration*/
  BSP_ACCELERO_GetXYZ(buffer);
  
 /* Update autoreload and capture compare registers value*/
  xval = buffer[0];
  yval = buffer[1];

  if(xval > yval)
  {
    if(xval > ThresholdHigh)
    { 
      /* LEFT */
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
      BSP_LCD_FillCircle(CIRCLE_LEFT_X_POS, CIRCLE_LEFT_Y_POS, CIRCLE_RADIUS);
      HAL_Delay(10);
    }
    else if(xval < ThresholdLow)
    { 
      HAL_Delay(10);
    }
    else
    { 
      /* UP */
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
      BSP_LCD_FillCircle(CIRCLE_UP_X_POS, CIRCLE_UP_Y_POS, CIRCLE_RADIUS);
      HAL_Delay(10);
    }
  }
  else
  {
    if(yval < ThresholdLow)
    {
      HAL_Delay(10);
    }
    else if(yval > ThresholdHigh)
    {
      /* RIGHT */
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
      BSP_LCD_FillCircle(CIRCLE_RIGHT_X_POS, CIRCLE_RIGHT_Y_POS, CIRCLE_RADIUS);
      HAL_Delay(10);
    } 
    else
    { 
      /* DOWN */
      BSP_LCD_SetTextColor(LCD_COLOR_RED); 
      BSP_LCD_FillCircle(CIRCLE_DOWN_X_POS, CIRCLE_DOWN_Y_POS, CIRCLE_RADIUS);
      HAL_Delay(10);
    }
  } 
  
  BSP_LED_Off(LED_ORANGE);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_BLUE);
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
