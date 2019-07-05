/**
  ******************************************************************************
  * @file    hx8347d.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    24-November-2014
  * @brief   This file includes the LCD driver for HX8347D LCD.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include "hx8347d.h"

/** @addtogroup BSP
* @{
*/ 

/** @addtogroup Components
* @{
*/ 

/** @addtogroup HX8347D
* @brief     This file provides a set of functions needed to drive the
*            HX8347D LCD.
* @{
*/

/** @defgroup HX8347D_Private_TypesDefinitions
* @{
*/ 

/**
* @}
*/ 

/** @defgroup HX8347D_Private_Defines
* @{
*/

/**
* @}
*/ 

/** @defgroup HX8347D_Private_Macros
* @{
*/

/**
* @}
*/  

/** @defgroup HX8347D_Private_Variables
* @{
*/ 
LCD_DrvTypeDef   hx8347d_drv = 
{
  hx8347d_Init,
  hx8347d_ReadID,
  hx8347d_DisplayOn,
  hx8347d_DisplayOff,
  hx8347d_SetCursor,
  hx8347d_WritePixel,
  hx8347d_ReadPixel,
  hx8347d_SetDisplayWindow,
  hx8347d_DrawHLine,
  hx8347d_DrawVLine,
  hx8347d_GetLcdPixelWidth,
  hx8347d_GetLcdPixelHeight,
  hx8347d_DrawBitmap,  
};

static uint8_t Is_hx8347d_Initialized = 0;
static uint16_t ArrayRGB[320] = {0};


/**
* @}
*/ 

/** @defgroup HX8347D_Private_FunctionPrototypes
* @{
*/

/**
* @}
*/ 

/** @defgroup HX8347D_Private_Functions
* @{
*/   

/**
* @brief  Initialise the HX8347D LCD Component.
* @param  None
* @retval None
*/
void hx8347d_Init(void)
{  
  if(Is_hx8347d_Initialized == 0)
  {
    Is_hx8347d_Initialized = 1;
    /* Initialise HX8347D low level bus layer --------------------------------*/
    LCD_IO_Init();
    
    /* Driving ability setting */
    hx8347d_WriteReg(LCD_REG_234, 0x00);
    hx8347d_WriteReg(LCD_REG_235, 0x20);
    hx8347d_WriteReg(LCD_REG_236, 0x0C);
    hx8347d_WriteReg(LCD_REG_237, 0xC4);
    hx8347d_WriteReg(LCD_REG_232, 0x40);
    hx8347d_WriteReg(LCD_REG_233, 0x38);
    hx8347d_WriteReg(LCD_REG_241, 0x01);
    hx8347d_WriteReg(LCD_REG_242, 0x10);
    hx8347d_WriteReg(LCD_REG_39,  0xA3);
    
    /* Adjust the Gamma Curve */
    hx8347d_WriteReg(LCD_REG_64, 0x01);
    hx8347d_WriteReg(LCD_REG_65, 0x00);
    hx8347d_WriteReg(LCD_REG_66, 0x00);
    hx8347d_WriteReg(LCD_REG_67, 0x10);
    hx8347d_WriteReg(LCD_REG_68, 0x0E);
    hx8347d_WriteReg(LCD_REG_69, 0x24);
    hx8347d_WriteReg(LCD_REG_70, 0x04);
    hx8347d_WriteReg(LCD_REG_71, 0x50);
    hx8347d_WriteReg(LCD_REG_72, 0x02);
    hx8347d_WriteReg(LCD_REG_73, 0x13);
    hx8347d_WriteReg(LCD_REG_74, 0x19);
    hx8347d_WriteReg(LCD_REG_75, 0x19);
    hx8347d_WriteReg(LCD_REG_76, 0x16);
    hx8347d_WriteReg(LCD_REG_80, 0x1B);
    hx8347d_WriteReg(LCD_REG_81, 0x31);
    hx8347d_WriteReg(LCD_REG_82, 0x2F);
    hx8347d_WriteReg(LCD_REG_83, 0x3F);
    hx8347d_WriteReg(LCD_REG_84, 0x3F);
    hx8347d_WriteReg(LCD_REG_85, 0x3E);
    hx8347d_WriteReg(LCD_REG_86, 0x2F);
    hx8347d_WriteReg(LCD_REG_87, 0x7B);
    hx8347d_WriteReg(LCD_REG_88, 0x09);
    hx8347d_WriteReg(LCD_REG_89, 0x06);
    hx8347d_WriteReg(LCD_REG_90, 0x06);
    hx8347d_WriteReg(LCD_REG_91, 0x0C);
    hx8347d_WriteReg(LCD_REG_92, 0x1D);
    hx8347d_WriteReg(LCD_REG_93, 0xCC);
    
    /* Power voltage setting */
    hx8347d_WriteReg(LCD_REG_27, 0x1B);
    hx8347d_WriteReg(LCD_REG_26, 0x01);
    hx8347d_WriteReg(LCD_REG_36, 0x2F);
    hx8347d_WriteReg(LCD_REG_37, 0x57);
    /*****VCOM offset ****/
    hx8347d_WriteReg(LCD_REG_35, 0x86);
    
    /* Power on setting up flow */
    hx8347d_WriteReg(LCD_REG_24, 0x36); /* Display frame rate = 70Hz RADJ = '0110' */
    hx8347d_WriteReg(LCD_REG_25, 0x01); /* OSC_EN = 1 */
    hx8347d_WriteReg(LCD_REG_28, 0x06); /* AP[2:0] = 111 */
    hx8347d_WriteReg(LCD_REG_29, 0x06); /* AP[2:0] = 111 */
    hx8347d_WriteReg(LCD_REG_31,0x90); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
    hx8347d_WriteReg(LCD_REG_39, 1); /* REF = 1 */
    
    LCD_Delay(10);
    /* 262k/65k color selection */
    hx8347d_WriteReg(LCD_REG_23, 0x05); /* default 0x06 262k color,  0x05 65k color */
    /* SET PANEL */
    hx8347d_WriteReg(LCD_REG_54, 0x09); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 0, BGR_PANEL = 1 */
    
    /* Display ON flow */
    hx8347d_WriteReg(LCD_REG_40, 0x38); /* GON=1, DTE=1, D=10 */
    LCD_Delay(60);
    hx8347d_WriteReg(LCD_REG_40, 0x3C); /* GON=1, DTE=1, D=11 */
    
    /* Set GRAM Area - Partial Display Control */
    hx8347d_WriteReg(LCD_REG_1, 0x00); /* DP_STB = 0, DP_STB_S = 0, SCROLL = 0, */
    hx8347d_WriteReg(LCD_REG_2, 0x00); /* Column address start 2 */
    hx8347d_WriteReg(LCD_REG_3, 0x00); /* Column address start 1 */
    hx8347d_WriteReg(LCD_REG_4, 0x01); /* Column address end 2 */
    hx8347d_WriteReg(LCD_REG_5, 0x3F); /* Column address end 1 */
    hx8347d_WriteReg(LCD_REG_6, 0x00); /* Row address start 2 */
    hx8347d_WriteReg(LCD_REG_7, 0x00); /* Row address start 2 */
    hx8347d_WriteReg(LCD_REG_8, 0x00); /* Row address end 2 */
    hx8347d_WriteReg(LCD_REG_9, 0xEF); /* Row address end 1 */
    hx8347d_WriteReg(LCD_REG_22, 0xE0); /* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
  }
  /* Set the Cursor */ 
  hx8347d_SetCursor(0, 0);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
}

/**
* @brief  Enables the Display.
* @param  None
* @retval None
*/
void hx8347d_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  hx8347d_WriteReg(LCD_REG_24, 0x36); /* Display frame rate = 70Hz RADJ = '0110' */
  hx8347d_WriteReg(LCD_REG_25, 0x01); /* OSC_EN = 1 */
  hx8347d_WriteReg(LCD_REG_28, 0x06); /* AP[2:0] = 111 */
  hx8347d_WriteReg(LCD_REG_31,0x90); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
  LCD_Delay(10);
  /* 262k/65k color selection */
  hx8347d_WriteReg(LCD_REG_23, 0x05); /* default 0x06 262k color,  0x05 65k color */
  /* SET PANEL */
  hx8347d_WriteReg(LCD_REG_54, 0x09); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 0, BGR_PANEL = 1 */
  
  /* Display On */
  hx8347d_WriteReg(LCD_REG_40, 0x38);
  LCD_Delay(60);
  hx8347d_WriteReg(LCD_REG_40, 0x3C);
}

/**
* @brief  Disables the Display.
* @param  None
* @retval None
*/
void hx8347d_DisplayOff(void)
{
  /* Power Off sequence ---------------------------------------------------------*/
  hx8347d_WriteReg(LCD_REG_23, 0x0000); /* default 0x06 262k color,  0x05 65k color */
  hx8347d_WriteReg(LCD_REG_24, 0x0000); /* Display frame rate = 70Hz RADJ = '0110' */
  hx8347d_WriteReg(LCD_REG_25, 0x0000); /* OSC_EN = 1 */
  hx8347d_WriteReg(LCD_REG_28, 0x0000); /* AP[2:0] = 111 */
  hx8347d_WriteReg(LCD_REG_31, 0x0000); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
  hx8347d_WriteReg(LCD_REG_54, 0x0000); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 0, BGR_PANEL = 1 */
  
  /* Display Off */
  hx8347d_WriteReg(LCD_REG_40, 0x38);
  LCD_Delay(60);
  hx8347d_WriteReg(LCD_REG_40, 0x04);
}

/**
* @brief  Get the LCD pixel Width.
* @param  None
* @retval The Lcd Pixel Width
*/
uint16_t hx8347d_GetLcdPixelWidth(void)
{
  return (uint16_t)HX8347D_LCD_PIXEL_WIDTH;
}

/**
* @brief  Get the LCD pixel Height.
* @param  None
* @retval The Lcd Pixel Height
*/
uint16_t hx8347d_GetLcdPixelHeight(void)
{
  return (uint16_t)HX8347D_LCD_PIXEL_HEIGHT;
}

/**
* @brief  Get the HX8347D ID.
* @param  None
* @retval The HX8347D ID 
*/
uint16_t hx8347d_ReadID(void)
{
  if(Is_hx8347d_Initialized == 0)
  {
    hx8347d_Init();
    Is_hx8347d_Initialized = 1;    
  }
  return (hx8347d_ReadReg(0x00));
}

/**
* @brief  Set Cursor position.
* @param  Xpos: specifies the X position.
* @param  Ypos: specifies the Y position.
* @retval None
*/
void hx8347d_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  hx8347d_WriteReg(LCD_REG_6, 0x00);
  hx8347d_WriteReg(LCD_REG_7, Xpos);
  hx8347d_WriteReg(LCD_REG_2, Ypos >> 8);
  hx8347d_WriteReg(LCD_REG_3, Ypos & 0xFF);
}

/**
* @brief  Write pixel.   
* @param  Xpos: specifies the X position.
* @param  Ypos: specifies the Y position.
* @param  RGBCode: the RGB pixel color
* @retval None
*/
void hx8347d_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&RGBCode, 2);
}

/**
* @brief  Read pixel.
* @param  None
* @retval the RGB pixel color
*/
uint16_t hx8347d_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos);
  
  /* Dummy read */
  LCD_IO_ReadData(LCD_REG_34);
  
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(LCD_REG_34));
}

/**
* @brief  Writes to the selected LCD register.
* @param  LCDReg:      address of the selected register.
* @param  LCDRegValue: value to write to the selected register.
* @retval None
*/
void hx8347d_WriteReg(uint8_t LCDReg, uint16_t LCDRegValue)
{
  LCD_IO_WriteReg(LCDReg);
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&LCDRegValue, 2);
}

/**
* @brief  Reads the selected LCD Register.
* @param  LCDReg: address of the selected register.
* @retval LCD Register Value.
*/
uint16_t hx8347d_ReadReg(uint8_t LCDReg)
{
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(LCDReg));
}

/**
* @brief  Sets a display window
* @param  Xpos:   specifies the X bottom left position.
* @param  Ypos:   specifies the Y bottom left position.
* @param  Height: display window height.
* @param  Width:  display window width.
* @retval None
*/
void hx8347d_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Horizontal GRAM Start Address */
  hx8347d_WriteReg(LCD_REG_6, (Xpos) >> 8); /* SP */
  hx8347d_WriteReg(LCD_REG_7, (Xpos) & 0xFF); /* SP */
  
  /* Horizontal GRAM End Address */
  hx8347d_WriteReg(LCD_REG_8, (Xpos + Height - 1) >> 8); /* EP */
  hx8347d_WriteReg(LCD_REG_9, (Xpos + Height - 1) & 0xFF); /* EP */
  
  /* Vertical GRAM Start Address */
  hx8347d_WriteReg(LCD_REG_2, (Ypos) >> 8); /* SC */
  hx8347d_WriteReg(LCD_REG_3, (Ypos) & 0xFF); /* SC */
  
  /* Vertical GRAM End Address */
  hx8347d_WriteReg(LCD_REG_4, (Ypos + Width - 1) >> 8); /* EC */
  hx8347d_WriteReg(LCD_REG_5, (Ypos + Width - 1) & 0xFF); /* EC */
}

/**
* @brief  Draw vertical line.
* @param  RGBCode: Specifies the RGB color   
* @param  Xpos:     specifies the X position.
* @param  Ypos:     specifies the Y position.
* @param  Length:   specifies the Line length.  
* @retval None
*/
void hx8347d_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t i = 0;
  
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos); 
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
  
  
  /* Sent a complete line */
  for(i = 0; i < Length; i++)
  {
    ArrayRGB[i] = RGBCode;
  }  

  LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);
}

/**
* @brief  Draw vertical line.
* @param  RGBCode: Specifies the RGB color    
* @param  Xpos:     specifies the X position.
* @param  Ypos:     specifies the Y position.
* @param  Length:   specifies the Line length.  
* @retval None
*/
void hx8347d_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;
  
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos);
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
  
  /* Fill a complete vertical line */
  for(counter = 0; counter < Length; counter++)
  {
    ArrayRGB[counter] = RGBCode;
  }
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);
}

/**
* @brief  Displays a bitmap picture loaded in the internal Flash.
* @param  BmpAddress: Bmp picture address in the internal Flash.
* @retval None
*/
void hx8347d_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  
  /* Read bitmap size */
  size = *(volatile uint16_t *) (pbmp + 2);
  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
  size = (size - index)/2;
  pbmp += index;
  
  /* Set GRAM write direction and BGR = 0 */
  /* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
  hx8347d_WriteReg(LCD_REG_22, 0xA0);
  
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos);  
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
  
  LCD_IO_WriteMultipleData((uint8_t*)pbmp, size*2);
  
  /* Set GRAM write direction and BGR = 0 */
  /* Memory access control: MY = 1, MX = 1, MV = 1, ML = 0 */
  hx8347d_WriteReg(LCD_REG_22, 0xE0);
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
