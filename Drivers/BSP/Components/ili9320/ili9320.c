/**
  ******************************************************************************
  * @file    ili9320.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    02-December-2014
  * @brief   This file includes the LCD driver for ILI9320 LCD.
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
#include "ili9320.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup ili9320
  * @brief     This file provides a set of functions needed to drive the 
  *            ILI9320 LCD.
  * @{
  */

/** @defgroup ILI9320_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup ILI9320_Private_Defines
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup ILI9320_Private_Macros
  * @{
  */
     
/**
  * @}
  */  

/** @defgroup ILI9320_Private_Variables
  * @{
  */ 
LCD_DrvTypeDef   ili9320_drv = 
{
  ili9320_Init,
  ili9320_ReadID,
  ili9320_DisplayOn,
  ili9320_DisplayOff,
  ili9320_SetCursor,
  ili9320_WritePixel,
  ili9320_ReadPixel,
  ili9320_SetDisplayWindow,
  ili9320_DrawHLine,
  ili9320_DrawVLine,
  ili9320_GetLcdPixelWidth,
  ili9320_GetLcdPixelHeight,
  ili9320_DrawBitmap,
  ili9320_DrawRGBImage,  
};

static uint8_t Is_ili9320_Initialized = 0;
static uint16_t ArrayRGB[320] = {0};

/**
  * @}
  */ 
  
/** @defgroup ILI9320_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup ILI9320_Private_Functions
  * @{
  */   

/**
  * @brief  Initialise the ILI9320 LCD Component.
  * @param  None
  * @retval None
  */
void ili9320_Init(void)
{  
  if(Is_ili9320_Initialized == 0)
  {
    Is_ili9320_Initialized = 1;
    /* Initialise ILI9320 low level bus layer --------------------------------*/
    LCD_IO_Init();
    
    /* Start Initial Sequence ------------------------------------------------*/
    ili9320_WriteReg(LCD_REG_229,0x8000); /* Set the internal vcore voltage */
    ili9320_WriteReg(LCD_REG_0,  0x0001); /* Start internal OSC. */
    ili9320_WriteReg(LCD_REG_1,  0x0100); /* set SS and SM bit */
    ili9320_WriteReg(LCD_REG_2,  0x0700); /* set 1 line inversion */
    ili9320_WriteReg(LCD_REG_3,  0x1030); /* set GRAM write direction and BGR=1. */
    ili9320_WriteReg(LCD_REG_4,  0x0000); /* Resize register */
    ili9320_WriteReg(LCD_REG_8,  0x0202); /* set the back porch and front porch */
    ili9320_WriteReg(LCD_REG_9,  0x0000); /* set non-display area refresh cycle ISC[3:0] */
    ili9320_WriteReg(LCD_REG_10, 0x0000); /* FMARK function */
    ili9320_WriteReg(LCD_REG_12, 0x0000); /* RGB interface setting */
    ili9320_WriteReg(LCD_REG_13, 0x0000); /* Frame marker Position */
    ili9320_WriteReg(LCD_REG_15, 0x0000); /* RGB interface polarity */

    /* Power On sequence -----------------------------------------------------*/
    ili9320_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    ili9320_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
    ili9320_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
    ili9320_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude */

    ili9320_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    ili9320_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */

    ili9320_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */

    ili9320_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
    ili9320_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */

    ili9320_WriteReg(LCD_REG_32, 0x0000); /* GRAM horizontal Address */
    ili9320_WriteReg(LCD_REG_33, 0x0000); /* GRAM Vertical Address */

    /* Adjust the Gamma Curve ------------------------------------------------*/
    ili9320_WriteReg(LCD_REG_48, 0x0007);
    ili9320_WriteReg(LCD_REG_49, 0x0007);
    ili9320_WriteReg(LCD_REG_50, 0x0007);
    ili9320_WriteReg(LCD_REG_53, 0x0007);
    ili9320_WriteReg(LCD_REG_54, 0x0007);
    ili9320_WriteReg(LCD_REG_55, 0x0700);
    ili9320_WriteReg(LCD_REG_56, 0x0700);
    ili9320_WriteReg(LCD_REG_57, 0x0700);
    ili9320_WriteReg(LCD_REG_60, 0x0700);
    ili9320_WriteReg(LCD_REG_61, 0x1F00);
  
    /* Set GRAM area ---------------------------------------------------------*/
    ili9320_WriteReg(LCD_REG_80, 0x0000); /* Horizontal GRAM Start Address */
    ili9320_WriteReg(LCD_REG_81, 0x00EF); /* Horizontal GRAM End Address */
    ili9320_WriteReg(LCD_REG_82, 0x0000); /* Vertical GRAM Start Address */
    ili9320_WriteReg(LCD_REG_83, 0x013F); /* Vertical GRAM End Address */
    ili9320_WriteReg(LCD_REG_96,  0x2700); /* Gate Scan Line */
    ili9320_WriteReg(LCD_REG_97,  0x0001); /* NDL,VLE, REV */
    ili9320_WriteReg(LCD_REG_106, 0x0000); /* set scrolling line */

    /* Partial Display Control -----------------------------------------------*/
    ili9320_WriteReg(LCD_REG_128, 0x0000);
    ili9320_WriteReg(LCD_REG_129, 0x0000);
    ili9320_WriteReg(LCD_REG_130, 0x0000);
    ili9320_WriteReg(LCD_REG_131, 0x0000);
    ili9320_WriteReg(LCD_REG_132, 0x0000);
    ili9320_WriteReg(LCD_REG_133, 0x0000);

    /* Panel Control ---------------------------------------------------------*/
    ili9320_WriteReg(LCD_REG_144, 0x0010);
    ili9320_WriteReg(LCD_REG_146, 0x0000);
    ili9320_WriteReg(LCD_REG_147, 0x0003);
    ili9320_WriteReg(LCD_REG_149, 0x0110);
    ili9320_WriteReg(LCD_REG_151, 0x0000);
    ili9320_WriteReg(LCD_REG_152, 0x0000);

    /* Set GRAM write direction and BGR = 1 */
    /* I/D=01 (Horizontal : increment, Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    ili9320_WriteReg(LCD_REG_3, 0x1018);

    ili9320_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */    
  }
  
  /* Set the Cursor */ 
  ili9320_SetCursor(0, 0);
    
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void ili9320_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  ili9320_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  ili9320_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  ili9320_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  ili9320_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude*/

  ili9320_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  ili9320_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */

  ili9320_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */

  ili9320_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
  ili9320_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
 
  /* Display On */
  ili9320_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ili9320_DisplayOff(void)
{
  /* Power Off sequence ---------------------------------------------------------*/
  ili9320_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  ili9320_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  ili9320_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  ili9320_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude*/
  
  ili9320_WriteReg(LCD_REG_41, 0x0000); /* VCM[4:0] for VCOMH */
  
  /* Display Off */
  ili9320_WriteReg(LCD_REG_7, 0x0); 
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t ili9320_GetLcdPixelWidth(void)
{
 return (uint16_t)320;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t ili9320_GetLcdPixelHeight(void)
{
 return (uint16_t)240;
}

/**
  * @brief  Get the ILI9320 ID.
  * @param  None
  * @retval The ILI9320 ID 
  */
uint16_t ili9320_ReadID(void)
{
  if(Is_ili9320_Initialized == 0)
  {
    ili9320_Init();  
  }
  return (ili9320_ReadReg(0x00));
}

/**
  * @brief  Set Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void ili9320_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  ili9320_WriteReg(LCD_REG_32, Ypos);
  ili9320_WriteReg(LCD_REG_33, (ILI9320_LCD_PIXEL_WIDTH - 1 - Xpos));      
}

/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  * @retval None
  */
void ili9320_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos);
  
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
uint16_t ili9320_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
  
  /* Dummy read */
  LCD_IO_ReadData(0x00);
  
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(0x00));
}

/**
  * @brief  Writes to the selected LCD register.
  * @param  LCDReg:      address of the selected register.
  * @param  LCDRegValue: value to write to the selected register.
  * @retval None
  */
void ili9320_WriteReg(uint8_t LCDReg, uint16_t LCDRegValue)
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
uint16_t ili9320_ReadReg(uint8_t LCDReg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_IO_WriteReg(LCDReg);
  
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(0x00));
}

/**
  * @brief  Sets a display window
  * @param  Xpos:   specifies the X bottom left position.
  * @param  Ypos:   specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width:  display window width.
  * @retval None
  */
void ili9320_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Horizontal GRAM Start Address */
  ili9320_WriteReg(LCD_REG_80, (Ypos));
  /* Horizontal GRAM End Address */
  ili9320_WriteReg(LCD_REG_81, (Ypos + Height - 1));
  
  /* Vertical GRAM Start Address */
  ili9320_WriteReg(LCD_REG_82, ILI9320_LCD_PIXEL_WIDTH - Xpos - Width);
  /* Vertical GRAM End Address */
  ili9320_WriteReg(LCD_REG_83, ILI9320_LCD_PIXEL_WIDTH - Xpos - 1);  
}

/**
  * @brief  Draw vertical line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void ili9320_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;
  
  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos); 
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Sent a complete line */
  for(counter = 0; counter < Length; counter++)
  {
    ArrayRGB[counter] = RGBCode;
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
void ili9320_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;

  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  ili9320_WriteReg(LCD_REG_3, 0x1010);
  
  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Fill a complete vertical line */
  for(counter = 0; counter < Length; counter++)
  {
    ArrayRGB[counter] = RGBCode;
  }
  
  LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);
  
  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  ili9320_WriteReg(LCD_REG_3, 0x1018);  
}

/**
  * @brief  Displays a bitmap picture..
  * @param  BmpAddress: Bmp picture address.
  * @param  Xpos:  Bmp X position in the LCD
  * @param  Ypos:  Bmp Y position in the LCD    
  * @retval None
  */
void ili9320_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
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
  /* Set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : decrement, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  ili9320_WriteReg(LCD_REG_3, 0x1008);

  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos);  
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
 
  LCD_IO_WriteMultipleData((uint8_t*)pbmp, size*2);

  /* Set GRAM write direction and BGR = 1 */
  /* I/D = 01 (Horizontal : increment, Vertical : decrement) */
  /* AM = 1 (address is updated in vertical writing direction) */
  ili9320_WriteReg(LCD_REG_3, 0x1018);
}

/**
  * @brief  Displays picture..
  * @param  pdata: picture address.
  * @param  Xpos:  Image X position in the LCD
  * @param  Ypos:  Image Y position in the LCD
  * @param  Xsize: Image X size in the LCD
  * @param  Ysize: Image Y size in the LCD
  * @retval None
  */
void ili9320_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  uint32_t size = 0;

  size = (Xsize * Ysize);

  /* Set Cursor */
  ili9320_SetCursor(Xpos, Ypos);  
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
 
  LCD_IO_WriteMultipleData((uint8_t*)pdata, size*2);
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
