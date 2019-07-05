/**
  ******************************************************************************
  * @file    stm3210e_eval_serialflash.c
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file provides a set of functions needed to manage the SPI M25Pxx
  *          FLASH memory mounted on STM3210E_EVAL board. 
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32 hardware resources (SPI and 
  *          GPIO) are defined in stm3210e_eval.h file, and the initialization is 
  *          performed in FLASH_SPI_IO_Init() function declared in stm3210e_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          FLASH_SPI_IO_Init() function.
  *            
  *   +---------------------------------------------------------------------+
  *   |                        Pin assignment                               |
  *   +---------------------------------------+---------------+-------------+
  *   |  STM32 SPI Pins                       |   BSP_FLASH   |    Pin      |
  *   +---------------------------------------+---------------+-------------+
  *   | BSP_SERIAL_FLASH_CS_PIN               | ChipSelect(/S)|    1        |
  *   | BSP_SERIAL_FLASH_SPI_MISO_PIN / MISO  |   DataOut(Q)  |    2        |
  *   |                                       |   VCC         |    3 (3.3 V)|
  *   |                                       |   GND         |    4 (0 V)  |
  *   | BSP_SERIAL_FLASH_SPI_MOSI_PIN / MOSI  |   DataIn(D)   |    5        |
  *   | BSP_SERIAL_FLASH_SPI_SCK_PIN / SCLK   |   Clock(C)    |    6        |
  *   |                                       |    VCC        |    7 (3.3 V)|
  *   |                                       |    VCC        |    8 (3.3 V)|
  *   +---------------------------------------+---------------+-------------+
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#include "stm3210e_eval_serialflash.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM3210E_EVAL
  * @{
  */ 
  
/** @defgroup STM3210E_EVAL_SERIAL_FLASH STM3210E EVAL Serial FLASH
  * @brief    This file includes the M25Pxxx SPI FLASH driver of 
  *           STM3210E-EVAL board.
  * @{
  */
  


/** @defgroup STM3210E_EVAL_SERIAL_FLASH_Exported_Functions STM3210E EVAL SERIAL FLASH Exported Functions
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the Serial FLASH device.
  * @retval FLASH_OK (0x00) if operation is correctly performed, else 
  *         return FLASH_ERROR (0x01).
  */
uint8_t BSP_SERIAL_FLASH_Init(void)
{
  if(FLASH_SPI_IO_Init() != HAL_OK)
  {
    return FLASH_ERROR;
  }
  else
  {
    return FLASH_OK;
  }
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval FLASH_OK (0x00) if operation is correctly performed, else 
  *         return FLASH_ERROR (0x01).
  */
uint8_t BSP_SERIAL_FLASH_EraseSector(uint32_t SectorAddr)
{
  /*!< Sector Erase */
  /*!< Select the FLASH  and send "Write Enable" instruction */
  FLASH_SPI_IO_WriteEnable();
  
  /*!< Send Sector Erase instruction */
  FLASH_SPI_IO_WriteByte(FLASH_SPI_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  FLASH_SPI_IO_WriteByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  FLASH_SPI_IO_WriteByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  FLASH_SPI_IO_WriteByte(SectorAddr & 0xFF);

  /*!< Wait the end of Flash writing and Deselect the FLASH*/
  if(FLASH_SPI_IO_WaitForWriteEnd()!= HAL_OK)
  {
    return FLASH_ERROR;
  }
  else
  {
    return FLASH_OK;
  }
}

/**
  * @brief  Erases the entire FLASH.
  * @retval FLASH_OK (0x00) if operation is correctly performed, else 
  *         return FLASH_ERROR (0x01).
  */
uint8_t BSP_SERIAL_FLASH_EraseBulk(void)
{
  /*!< Bulk Erase */
  /*!< Select the FLASH  and send "Write Enable" instruction */
  FLASH_SPI_IO_WriteEnable();
  
  /*!< Send Bulk Erase instruction  */
  FLASH_SPI_IO_WriteByte(FLASH_SPI_CMD_BE);

  /*!< Wait the end of Flash writing and Deselect the FLASH*/
  if(FLASH_SPI_IO_WaitForWriteEnd()!= HAL_OK)
  {
    return FLASH_ERROR;
  }
  else
  {
    return FLASH_OK;
  }
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size (FLASH_SPI_PAGESIZE).
  * @param  pData: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  uwStartAddress: FLASH's internal address to write to.
  * @param  uwDataSize: number of bytes to write to the FLASH, must be equal
  *         or less than "FLASH_SPI_PAGESIZE" value.
  * @retval FLASH_OK (0x00) if operation is correctly performed, else 
  *         return FLASH_ERROR (0x01).
  */
uint8_t BSP_SERIAL_FLASH_WritePage(uint32_t uwStartAddress, uint8_t* pData, uint32_t uwDataSize)
{
  /*!< Select the FLASH  and send "Write Enable" instruction */
  FLASH_SPI_IO_WriteEnable();

  /*!< Send "Write to Memory " instruction */
  FLASH_SPI_IO_WriteByte(FLASH_SPI_CMD_WRITE);
  /*!< Send uwStartAddress high nibble address byte to write to */
  FLASH_SPI_IO_WriteByte((uwStartAddress & 0xFF0000) >> 16);
  /*!< Send uwStartAddress medium nibble address byte to write to */
  FLASH_SPI_IO_WriteByte((uwStartAddress & 0xFF00) >> 8);
  /*!< Send uwStartAddress low nibble address byte to write to */
  FLASH_SPI_IO_WriteByte(uwStartAddress & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (uwDataSize--)
  {
    /*!< Send the current byte */
    FLASH_SPI_IO_WriteByte(*pData);
    /*!< Point on the next byte to be written */
    pData++;
  }

  /*!< Wait the end of Flash writing */
  if (FLASH_SPI_IO_WaitForWriteEnd()!= HAL_OK)
  {
    return FLASH_ERROR;
  }
  else
  {
    return FLASH_OK;
  }
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pData: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  uwStartAddress: FLASH's internal address to write to.
  * @param  uwDataSize: number of bytes to write to the FLASH.
  * @retval FLASH_OK (0x00) if operation is correctly performed, else 
  *         return FLASH_ERROR (0x01).
  */
uint8_t BSP_SERIAL_FLASH_WriteData(uint32_t uwStartAddress, uint8_t* pData, uint32_t uwDataSize)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0, status = 0;

  Addr = uwStartAddress % FLASH_SPI_PAGESIZE;
  count = FLASH_SPI_PAGESIZE - Addr;
  NumOfPage =  uwDataSize / FLASH_SPI_PAGESIZE;
  NumOfSingle = uwDataSize % FLASH_SPI_PAGESIZE;

  if (Addr == 0) /*!< uwStartAddress is FLASH_SPI_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< uwDataSize < FLASH_SPI_PAGESIZE */
    {
      status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, uwDataSize);
    }
    else /*!< uwDataSize > FLASH_SPI_PAGESIZE */
    {
      while (NumOfPage--)
      {
        status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, FLASH_SPI_PAGESIZE);
        uwStartAddress +=  FLASH_SPI_PAGESIZE;
        pData += FLASH_SPI_PAGESIZE;
      }

      status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, NumOfSingle);
    }
  }
  else /*!< uwStartAddress is not FLASH_SPI_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< uwDataSize < FLASH_SPI_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (uwDataSize + uwStartAddress) > FLASH_SPI_PAGESIZE */
      {
        temp = NumOfSingle - count;

        status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, count);
        uwStartAddress +=  count;
        pData += count;

        status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, temp);
      }
      else
      {
        status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, uwDataSize);
      }
    }
    else /*!< uwDataSize > BSP_SERIAL_FLASH_PAGESIZE */
    {
      uwDataSize -= count;
      NumOfPage =  uwDataSize / FLASH_SPI_PAGESIZE;
      NumOfSingle = uwDataSize % FLASH_SPI_PAGESIZE;

      status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, count);
      uwStartAddress +=  count;
      pData += count;

      while (NumOfPage--)
      {
        status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, FLASH_SPI_PAGESIZE);
        uwStartAddress +=  FLASH_SPI_PAGESIZE;
        pData += FLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        status = BSP_SERIAL_FLASH_WritePage(uwStartAddress, pData, NumOfSingle);
      }
    }
  }
  if (status!= HAL_OK)
  {
    return FLASH_ERROR;
  }
  else
  {
    return FLASH_OK;
  }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pData: pointer to the buffer that receives the data read from the FLASH.
  * @param  uwStartAddress: FLASH's internal address to read from.
  * @param  uwDataSize: number of bytes to read from the FLASH.
  * @retval FLASH_OK (0x00) if operation is correctly performed, else 
  *         return FLASH_ERROR (0x01).
  */
uint8_t BSP_SERIAL_FLASH_ReadData(uint32_t uwStartAddress, uint8_t* pData, uint32_t uwDataSize)
{
  if(FLASH_SPI_IO_ReadData(uwStartAddress, pData, uwDataSize)!= HAL_OK)
  {
    return FLASH_ERROR;
  }
  else
  {
    return FLASH_OK;
  }
}


/**
  * @brief  Reads FLASH identification.
  * @retval FLASH identification
  */
uint32_t BSP_SERIAL_FLASH_ReadID(void)
{
  return(FLASH_SPI_IO_ReadID());
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
