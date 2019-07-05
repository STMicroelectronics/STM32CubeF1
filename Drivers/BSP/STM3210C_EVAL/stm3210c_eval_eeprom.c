/**
  ******************************************************************************
  * @file    stm3210c_eval_eeprom.c
  * @author  MCD Application Team
  * @version V6.1.0
  * @date    14-April-2017
  * @brief   This file provides a set of functions needed to manage a M24C64 
  *          I2C EEPROM memory.
  *          
  *          =================================================================== 
  *          Notes:
  *           - This driver is intended for STM32F1xx families devices only.
  *           - The I2C EEPROM memory (M24CXX) is available directly
  *             on STM3210C EVAL board.
  *          ===================================================================
  *              
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32F10x hardware resources (I2C
  *          and GPIO) are defined in stm3210c_eval.h file,
  *          and the initialization is performed 
  *          in EEPROM_I2C_IO_Init() functions
  *          declared in stm3210c_eval.c file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          EEPROM_I2C_IO_Init() functions. 
  *        
  *          @note In this driver, basic read and write functions
  *          (BSP_EEPROM_ReadBuffer() and BSP_EEPROM_WriteBuffer())
  *          use Polling mode to perform the data transfer to/from EEPROM memories.
  *     +-----------------------------------------------------------------+
  *     |               Pin assignment for M24CXX EEPROM                 |
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32F1xx I2C Pins                   |   EEPROM  |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | EEPROM_I2C_SDA_PIN/ SDA               |   SDA     |    5        |
  *     | EEPROM_I2C_SCL_PIN/ SCL               |   SCL     |    6        |
  *     | .                                     |   VDD     |    7 (3.3V) |
  *     | .                                     |   GND     |    8 (0 V)  |
  *     +---------------------------------------+-----------+-------------+
  *
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
#include "stm3210c_eval_eeprom.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM3210C_EVAL
  * @{
  */
  
/** @defgroup STM3210C_EVAL_EEPROM STM3210C EVAL EEPROM
  * @brief      This file includes the I2C and SPI EEPROM driver
  *             of STM3210C-EVAL board.
  * @{
  */ 

/** @defgroup STM3210C_EVAL_EEPROM_Private_Variables STM3210C EVAL EEPROM Private Variables
  * @{
  */
__IO uint16_t  EEPROMAddress = 0;
__IO uint16_t  EEPROMPageSize = 0;
__IO uint16_t  EEPROMDataRead = 0;
__IO uint8_t   EEPROMDataWrite = 0;

static EEPROM_DrvTypeDef *EEPROM_SelectedDevice = 0;
/**
  * @}
  */ 


/** @defgroup STM3210C_EVAL_EEPROM_Private_Function_Prototypes STM3210C EVAL EEPROM Private Function Prototypes
  * @{
  */ 
static uint32_t EEPROM_I2C_Init(void);
static uint32_t EEPROM_I2C_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint32_t* NumByteToRead);
static uint32_t EEPROM_I2C_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint32_t* NumByteToWrite);
static uint32_t EEPROM_I2C_WaitEepromStandbyState(void);

/* EEPROM I2C driver typedef */
EEPROM_DrvTypeDef EEPROM_I2C_Drv =
{
  EEPROM_I2C_Init,
  EEPROM_I2C_ReadBuffer,
  EEPROM_I2C_WritePage
};

/**
  * @}
  */ 

/** @defgroup STM3210C_EVAL_EEPROM_Exported_Functions STM3210C EVAL EEPROM Exported Functions
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the EEPROM device selected.
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0)
  */
uint32_t BSP_EEPROM_Init(void)
{ 
  if(EEPROM_SelectedDevice->Init != 0)
  {
    return (EEPROM_SelectedDevice->Init());
  }
  else
  {
    return EEPROM_FAIL;
  }
}

/**
  * @brief  Select the EEPROM device to communicate.
  * @param  DeviceID: Specifies the EEPROM device to be selected. 
  *   This parameter can be one of following parameters:
  *     @arg BSP_EEPROM_M24C64_32
  *     @arg BSP_EEPROM_M24C08
  * 
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0)
  */
void BSP_EEPROM_SelectDevice(uint8_t DeviceID)
{
  switch(DeviceID)
  {
  case BSP_EEPROM_M24C64_32:
  case BSP_EEPROM_M24C08:
    EEPROM_SelectedDevice = &EEPROM_I2C_Drv;
    break;
    
  default:
    break;
  }
}

/**
  * @brief  Reads a block of data from the EEPROM device selected.
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to start reading from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to 
  *         be read from the EEPROM.
  * 
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0) or the timeout user callback.
  */
uint32_t BSP_EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint32_t* NumByteToRead)
{
  if(EEPROM_SelectedDevice->ReadBuffer != 0)
  {
    return (EEPROM_SelectedDevice->ReadBuffer(pBuffer, ReadAddr, NumByteToRead));
  }
  else
  {
    return EEPROM_FAIL;
  }
}

/**
  * @brief  Writes buffer of data to the EEPROM device selected.
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0) or the timeout user callback.
  */
uint32_t BSP_EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint32_t NumByteToWrite)
{
  uint16_t numofpage = 0, numofsingle = 0, count = 0;
  uint16_t addr = 0;
  uint32_t dataindex = 0;
  uint32_t status = EEPROM_OK;

  addr = WriteAddr % EEPROMPageSize;
  count = EEPROMPageSize - addr;
  numofpage =  NumByteToWrite / EEPROMPageSize;
  numofsingle = NumByteToWrite % EEPROMPageSize;
 
  if(EEPROM_SelectedDevice->WritePage == 0)
  {
    return EEPROM_FAIL;
  }
  
  /*!< If WriteAddr is EEPROM_PAGESIZE aligned  */
  if(addr == 0) 
  {
    /*!< If NumByteToWrite < EEPROM_PAGESIZE */
    if(numofpage == 0) 
    {
      /* Store the number of data to be written */
      dataindex = numofsingle;
      /* Start writing data */
      status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
      if (status != EEPROM_OK)
      {
        return status;
      }
    }
    /*!< If NumByteToWrite > EEPROM_PAGESIZE */
    else  
    {
      while(numofpage--)
      {
        /* Store the number of data to be written */
        dataindex = EEPROMPageSize;        
        status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
        
        WriteAddr +=  EEPROMPageSize;
        pBuffer += EEPROMPageSize;
      }

      if(numofsingle!=0)
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;          
        status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
          }
        }
    }
  /*!< If WriteAddr is not EEPROM_PAGESIZE aligned  */
  else 
  {
    /*!< If NumByteToWrite < EEPROM_PAGESIZE */
    if(numofpage== 0) 
    {
      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        dataindex = count;        
        /*!< Write the data contained in same page */
        status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
        
        /* Store the number of data to be written */
        dataindex = (NumByteToWrite - count);          
        /*!< Write the remaining data in the following page */
        status = EEPROM_SelectedDevice->WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
      }      
      else      
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;         
        status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
        }
      }     
    /*!< If NumByteToWrite > EEPROM_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      numofpage =  NumByteToWrite / EEPROMPageSize;
      numofsingle = NumByteToWrite % EEPROMPageSize;
      
      if(count != 0)
      {  
        /* Store the number of data to be written */
        dataindex = count;         
        status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(numofpage--)
      {
        /* Store the number of data to be written */
        dataindex = EEPROMPageSize;          
        status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
        WriteAddr +=  EEPROMPageSize;
        pBuffer += EEPROMPageSize;  
      }
      if(numofsingle != 0)
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;           
        status = EEPROM_SelectedDevice->WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
        if (status != EEPROM_OK)
        {
          return status;
        }
      }
    }
  }  
  
  /* If all operations OK, return EEPROM_OK (0) */
  return EEPROM_OK;
}

/**
  * @brief  Basic management of the timeout situation.
  * @retval None.
  */
__weak void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
}
/**
  * @}
  */

/** @addtogroup STM3210C_EVAL_EEPROM_Private_Function_Prototypes
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @note There are 2 different versions of M24CXX (08 or 32 or 64).
  *             Then try to connect on 1st one (EEPROM_I2C_ADDRESS_A01) 
  *             and if problem, check the 2nd one (EEPROM_I2C_ADDRESS_A02)
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0)
  */
static uint32_t EEPROM_I2C_Init(void)
{
  EEPROM_I2C_IO_Init();

  /*Select the EEPROM address for M24C32 or M24C64 and check if OK*/
  EEPROMAddress = EEPROM_ADDRESS_M24C64_32;
  EEPROMPageSize = EEPROM_PAGESIZE_M24C64_32;
  if (EEPROM_I2C_IO_IsDeviceReady(EEPROMAddress, EEPROM_MAX_TRIALS) != HAL_OK) 
  {
    EEPROMPageSize = EEPROM_PAGESIZE_M24C08;
    /*Select the EEPROM address for M24C08 (BLOCK0) and check if OK*/
    EEPROMAddress = EEPROM_ADDRESS_M24C08_BLOCK0;
    if (EEPROM_I2C_IO_IsDeviceReady(EEPROMAddress, EEPROM_MAX_TRIALS) != HAL_OK) 
    {
      /*Select the EEPROM address for M24C08 (BLOCK1) and check if OK*/
      EEPROMAddress = EEPROM_ADDRESS_M24C08_BLOCK1;
      if (EEPROM_I2C_IO_IsDeviceReady(EEPROMAddress, EEPROM_MAX_TRIALS) != HAL_OK) 
      {
        /*Select the EEPROM address for M24C08 (BLOCK2) and check if OK*/
        EEPROMAddress = EEPROM_ADDRESS_M24C08_BLOCK2;
        if (EEPROM_I2C_IO_IsDeviceReady(EEPROMAddress, EEPROM_MAX_TRIALS) != HAL_OK) 
        {
          /*Select the EEPROM address for M24C08 (BLOCK3) and check if OK*/
          EEPROMAddress = EEPROM_ADDRESS_M24C08_BLOCK3;
          if (EEPROM_I2C_IO_IsDeviceReady(EEPROMAddress, EEPROM_MAX_TRIALS) != HAL_OK) 
          {
            return EEPROM_FAIL;
          }
        }
      }
    }
  }

  return EEPROM_OK;
}

/**
  * @brief  Reads a block of data from the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to start reading from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to 
  *         be read from the EEPROM.
  * 
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0) or the timeout user callback.
  */
static uint32_t EEPROM_I2C_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint32_t* NumByteToRead)
{  
  uint32_t buffersize = *NumByteToRead;
  
  if (EEPROM_I2C_IO_ReadData(EEPROMAddress, ReadAddr, pBuffer, buffersize) != HAL_OK)
  {
    return EEPROM_FAIL;
  }
  
  /* If all operations OK, return EEPROM_OK (0) */
  return EEPROM_OK;
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  *
  * @note   The number of bytes (combined to write start address) must not 
  *         cross the EEPROM page boundary. This function can only write into
  *         the boundaries of an EEPROM page.
  *         This function doesn't check on boundaries condition (in this driver 
  *         the function BSP_EEPROM_WriteBuffer() which calls EEPROM_WritePage() is 
  *         responsible of checking on Page boundaries).
  * 
  * @param  pBuffer : pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : pointer to the variable holding number of bytes to 
  *         be written into the EEPROM. 
  * 
  *        @note The variable pointed by NumByteToWrite is reset to 0 when all the 
  *              data are written to the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * 
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0) or the timeout user callback.
  */
static uint32_t EEPROM_I2C_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint32_t* NumByteToWrite)
{ 
  uint32_t buffersize = *NumByteToWrite;

  if (EEPROM_I2C_IO_WriteData(EEPROMAddress, WriteAddr, pBuffer, buffersize) != HAL_OK)
  {
    return EEPROM_FAIL;
  }
  
  /* Wait for EEPROM Standby state */
  if (EEPROM_I2C_WaitEepromStandbyState() != EEPROM_OK) 
  {
    return EEPROM_FAIL;
  }
  
  return EEPROM_OK;
}

/**
  * @brief  Wait for EEPROM I2C Standby state.
  * 
  * @note  This function allows to wait and check that EEPROM has finished the 
  *        last operation. It is mostly used after Write operation: after receiving
  *        the buffer to be written, the EEPROM may need additional time to actually
  *        perform the write operation. During this time, it doesn't answer to
  *        I2C packets addressed to it. Once the write operation is complete
  *        the EEPROM responds to its address.
  * 
  * @retval EEPROM_OK (0) if operation is correctly performed, else return value 
  *         different from EEPROM_OK (0) or the timeout user callback.
  */
static uint32_t EEPROM_I2C_WaitEepromStandbyState(void)  
{
    /* Check if the maximum allowed number of trials has bee reached */
  if (EEPROM_I2C_IO_IsDeviceReady(EEPROMAddress, EEPROM_MAX_TRIALS) != HAL_OK)
  {
    /* If the maximum number of trials has been reached, exit the function */
    BSP_EEPROM_TIMEOUT_UserCallback();
    return EEPROM_TIMEOUT;
  }
  return EEPROM_OK;
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

