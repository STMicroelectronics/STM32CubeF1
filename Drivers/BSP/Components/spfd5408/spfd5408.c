/**
  ******************************************************************************
  * @file    spfd5408.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    26-November-2014
  * @brief   This file includes the LCD driver for SPFD5408 LCD.
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
#include "spfd5408.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @defgroup SPFD5408
  * @brief     This file provides a set of functions needed to drive the 
  *            SPFD5408 LCD.
  * @{
  */

/** @defgroup SPFD5408_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup SPFD5408_Private_Defines
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup SPFD5408_Private_Macros
  * @{
  */
     
/**
  * @}
  */  

/** @defgroup SPFD5408_Private_Variables
  * @{
  */ 
LCD_DrvTypeDef   spfd5408_drv = 
{
  spfd5408_Init,
  spfd5408_ReadID,
  spfd5408_DisplayOn,
  spfd5408_DisplayOff,
  spfd5408_SetCursor,
  spfd5408_WritePixel,
  spfd5408_ReadPixel,
  spfd5408_SetDisplayWindow,
  spfd5408_DrawHLine,
  spfd5408_DrawVLine,
  spfd5408_GetLcdPixelWidth,
  spfd5408_GetLcdPixelHeight,
  spfd5408_DrawBitmap,  
};

static uint8_t Is_spfd5408_Initialized = 0;
static uint16_t ArrayRGB[320] = {0};

/**
  * @}
  */ 
  
/** @defgroup SPFD5408_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup SPFD5408_Private_Functions
  * @{
  */   

/**
  * @brief  Initialise the SPFD5408 LCD Component.
  * @param  None
  * @retval None
  */
void spfd5408_Init(void)
{  
  if(Is_spfd5408_Initialized == 0)
  {
    Is_spfd5408_Initialized = 1;
    /* Initialise SPFD5408 low level bus layer --------------------------------*/
    LCD_IO_Init();
   
    /* Start Initial Sequence --------------------------------------------------*/
    spfd5408_WriteReg(LCD_REG_227, 0x3008); /* Set internal timing */
    spfd5408_WriteReg(LCD_REG_231, 0x0012); /* Set internal timing */
    spfd5408_WriteReg(LCD_REG_239, 0x1231); /* Set internal timing */
    spfd5408_WriteReg(LCD_REG_1, 0x0100);   /* Set SS and SM bit */
    spfd5408_WriteReg(LCD_REG_2, 0x0700);   /* Set 1 line inversion */
    spfd5408_WriteReg(LCD_REG_3, 0x1030);   /* Set GRAM write direction and BGR=1. */
    spfd5408_WriteReg(LCD_REG_4, 0x0000);   /* Resize register */
    spfd5408_WriteReg(LCD_REG_8, 0x0202);   /* Set the back porch and front porch */
    spfd5408_WriteReg(LCD_REG_9, 0x0000);   /* Set non-display area refresh cycle ISC[3:0] */
    spfd5408_WriteReg(LCD_REG_10, 0x0000);  /* FMARK function */
    spfd5408_WriteReg(LCD_REG_12, 0x0000);  /* RGB interface setting */
    spfd5408_WriteReg(LCD_REG_13, 0x0000);  /* Frame marker Position */
    spfd5408_WriteReg(LCD_REG_15, 0x0000);  /* RGB interface polarity */
    /* Power On sequence -------------------------------------------------------*/
    spfd5408_WriteReg(LCD_REG_16, 0x0000);  /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    spfd5408_WriteReg(LCD_REG_17, 0x0000);  /* DC1[2:0], DC0[2:0], VC[2:0] */
    spfd5408_WriteReg(LCD_REG_18, 0x0000);  /* VREG1OUT voltage */
    spfd5408_WriteReg(LCD_REG_19, 0x0000);  /* VDV[4:0] for VCOM amplitude */
    LCD_Delay(200);                /* Dis-charge capacitor power voltage (200ms) */  
    spfd5408_WriteReg(LCD_REG_17, 0x0007);  /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_Delay(50);                 /* Delay 50 ms */
    spfd5408_WriteReg(LCD_REG_16, 0x12B0);  /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_Delay(50);                  /* Delay 50 ms */
    spfd5408_WriteReg(LCD_REG_18, 0x01BD);  /* External reference voltage= Vci */
    LCD_Delay(50);                 /* Delay 50 ms */ 
    spfd5408_WriteReg(LCD_REG_19, 0x1400);       /* VDV[4:0] for VCOM amplitude */
    spfd5408_WriteReg(LCD_REG_41, 0x000E);  /* VCM[4:0] for VCOMH */
    LCD_Delay(50);                 /* Delay 50 ms */
    spfd5408_WriteReg(LCD_REG_32, 0x0000);  /* GRAM horizontal Address */
    spfd5408_WriteReg(LCD_REG_33, 0x013F);  /* GRAM Vertical Address */
    /* Adjust the Gamma Curve --------------------------------------------------*/
    spfd5408_WriteReg(LCD_REG_48, 0x0007);
    spfd5408_WriteReg(LCD_REG_49, 0x0302);
    spfd5408_WriteReg(LCD_REG_50, 0x0105);
    spfd5408_WriteReg(LCD_REG_53, 0x0206);
    spfd5408_WriteReg(LCD_REG_54, 0x0808);
    spfd5408_WriteReg(LCD_REG_55, 0x0206);
    spfd5408_WriteReg(LCD_REG_56, 0x0504);
    spfd5408_WriteReg(LCD_REG_57, 0x0007);
    spfd5408_WriteReg(LCD_REG_60, 0x0105);
    spfd5408_WriteReg(LCD_REG_61, 0x0808);
    /* Set GRAM area -----------------------------------------------------------*/
    spfd5408_WriteReg(LCD_REG_80, 0x0000);  /* Horizontal GRAM Start Address */
    spfd5408_WriteReg(LCD_REG_81, 0x00EF);  /* Horizontal GRAM End Address */
    spfd5408_WriteReg(LCD_REG_82, 0x0000);  /* Vertical GRAM Start Address */
    spfd5408_WriteReg(LCD_REG_83, 0x013F);  /* Vertical GRAM End Address */
    spfd5408_WriteReg(LCD_REG_96,  0xA700); /* Gate Scan Line */
    spfd5408_WriteReg(LCD_REG_97,  0x0001); /* NDL,VLE, REV */
    spfd5408_WriteReg(LCD_REG_106, 0x0000); /* Set scrolling line */
    /* Partial Display Control -------------------------------------------------*/
    spfd5408_WriteReg(LCD_REG_128, 0x0000);
    spfd5408_WriteReg(LCD_REG_129, 0x0000);
    spfd5408_WriteReg(LCD_REG_130, 0x0000);
    spfd5408_WriteReg(LCD_REG_131, 0x0000);
    spfd5408_WriteReg(LCD_REG_132, 0x0000);
    spfd5408_WriteReg(LCD_REG_133, 0x0000);
    /* Panel Control -----------------------------------------------------------*/
    spfd5408_WriteReg(LCD_REG_144, 0x0010);
    spfd5408_WriteReg(LCD_REG_146, 0x0000);
    spfd5408_WriteReg(LCD_REG_147, 0x0003);
    spfd5408_WriteReg(LCD_REG_149, 0x0110);
    spfd5408_WriteReg(LCD_REG_151, 0x0000);
    spfd5408_WriteReg(LCD_REG_152, 0x0000);
    /* Set GRAM write direction and BGR = 1
       I/D=01 (Horizontal : increment, Vertical : decrement)
       AM=1 (address is updated in vertical writing direction) */
    spfd5408_WriteReg(LCD_REG_3, 0x1018);
    spfd5408_WriteReg(LCD_REG_7, 0x0112);   /* 262K color and display ON */
  }

  /* Set the Cursor */ 
  spfd5408_SetCursor(0, 0);
    
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void spfd5408_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  spfd5408_WriteReg(LCD_REG_24, 0x36); /* Display frame rate = 70Hz RADJ = '0110' */
  spfd5408_WriteReg(LCD_REG_25, 0x01); /* OSC_EN = 1 */
  spfd5408_WriteReg(LCD_REG_28, 0x06); /* AP[2:0] = 111 */
  spfd5408_WriteReg(LCD_REG_31,0x90); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
  LCD_Delay(10);
  /* 262k/65k color selection */
  spfd5408_WriteReg(LCD_REG_23, 0x05); /* default 0x06 262k color,  0x05 65k color */
  /* SET PANEL */
  spfd5408_WriteReg(LCD_REG_54, 0x09); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 0, BGR_PANEL = 1 */
 
  /* Display On */
  spfd5408_WriteReg(LCD_REG_40, 0x38);
  LCD_Delay(60);
  spfd5408_WriteReg(LCD_REG_40, 0x3C);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void spfd5408_DisplayOff(void)
{
  /* Power Off sequence ---------------------------------------------------------*/
  spfd5408_WriteReg(LCD_REG_23, 0x0000); /* default 0x06 262k color,  0x05 65k color */
  spfd5408_WriteReg(LCD_REG_24, 0x0000); /* Display frame rate = 70Hz RADJ = '0110' */
  spfd5408_WriteReg(LCD_REG_25, 0x0000); /* OSC_EN = 1 */
  spfd5408_WriteReg(LCD_REG_28, 0x0000); /* AP[2:0] = 111 */
  spfd5408_WriteReg(LCD_REG_31, 0x0000); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
  spfd5408_WriteReg(LCD_REG_54, 0x0000); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 0, BGR_PANEL = 1 */

  /* Display Off */
  spfd5408_WriteReg(LCD_REG_40, 0x38);
  LCD_Delay(60);
  spfd5408_WriteReg(LCD_REG_40, 0x04);
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t spfd5408_GetLcdPixelWidth(void)
{
 return (uint16_t)SPFD5408_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t spfd5408_GetLcdPixelHeight(void)
{
 return (uint16_t)SPFD5408_LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Get the SPFD5408 ID.
  * @param  None
  * @retval The SPFD5408 ID 
  */
uint16_t spfd5408_ReadID(void)
{
  LCD_IO_Init();
  
  return (spfd5408_ReadReg(0x00));
}

/**
  * @brief  Set Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void spfd5408_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  spfd5408_WriteReg(LCD_REG_32, Xpos);
  spfd5408_WriteReg(LCD_REG_33, 319-Ypos);
}

/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  * @retval None
  */
void spfd5408_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  spfd5408_SetCursor(Xpos, Ypos);
  
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
uint16_t spfd5408_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  /* Set Cursor */
  spfd5408_SetCursor(Xpos, Ypos);
  
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
void spfd5408_WriteReg(uint8_t LCDReg, uint16_t LCDRegValue)
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
uint16_t spfd5408_ReadReg(uint8_t LCDReg)
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
void spfd5408_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Horizontal GRAM Start Address */
  spfd5408_WriteReg(LCD_REG_80, Xpos);
  /* Horizontal GRAM End Address */
  spfd5408_WriteReg(LCD_REG_81, Xpos + Height - 1);
  
  /* Vertical GRAM Start Address */
  spfd5408_WriteReg(LCD_REG_82, (SPFD5408_LCD_PIXEL_WIDTH - Ypos - Width));
  /* Vertical GRAM End Address */
  spfd5408_WriteReg(LCD_REG_83, (SPFD5408_LCD_PIXEL_WIDTH - Ypos - 1));  
}

/**
  * @brief  Draw vertical line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void spfd5408_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t i = 0;
  
  /* Set Cursor */
  spfd5408_SetCursor(Xpos, Ypos); 
  
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
void spfd5408_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;

  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  spfd5408_WriteReg(LCD_REG_3, 0x1010);
  
  /* Set Cursor */
  spfd5408_SetCursor(Xpos, Ypos);
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Fill a complete vertical line */
  for(counter = 0; counter < Length; counter++)
  {
    ArrayRGB[counter] = RGBCode;
  }
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);

  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  spfd5408_WriteReg(LCD_REG_3, 0x1018);  
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void spfd5408_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
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
  spfd5408_WriteReg(LCD_REG_3, 0x1008);

  /* Set Cursor */
  spfd5408_SetCursor(Xpos, Ypos);  

  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
   
  LCD_IO_WriteMultipleData((uint8_t*)pbmp, size*2);
 
  /* Set GRAM write direction and BGR = 1 */
  /* I/D = 01 (Horizontal : increment, Vertical : decrement) */
  /* AM = 1 (address is updated in vertical writing direction) */
  spfd5408_WriteReg(LCD_REG_3, 0x1018);
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
