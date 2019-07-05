/**
  ******************************************************************************
  * @file    ak4343.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    11-April-2016
  * @brief   This file contains all the functions prototypes for the ak4343.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AK4343_H
#define __AK4343_H

/* Includes ------------------------------------------------------------------*/
#include "../Common/audio.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Component
  * @{
  */ 
  
/** @addtogroup AK4343
  * @{
  */

/** @defgroup AK4343_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup AK4343_Exported_Constants
  * @{
  */ 

/******************************************************************************/
/***************************  Codec User defines ******************************/
/******************************************************************************/
/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER         0x01
#define OUTPUT_DEVICE_HEADPHONE       0x02
#define OUTPUT_DEVICE_BOTH            (OUTPUT_DEVICE_SPEAKER | OUTPUT_DEVICE_HEADPHONE)

/* Volume Levels values */
#define DEFAULT_VOLMIN                0x00
#define DEFAULT_VOLMAX                0xFF
#define DEFAULT_VOLSTEP               0x04

#define AUDIO_PAUSE                   0
#define AUDIO_RESUME                  1

/* Codec POWER DOWN modes */
#define CODEC_PDWN_HW                 1
#define CODEC_PDWN_SW                 2

/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0

/******************************************************************************/
/****************************** REGISTER MAPPING ******************************/
/******************************************************************************/
/** 
  * @brief  AK4343 registers definitions  
  */  
#define AK4343_MODE_CONTROL_1           0x04

#define AK4343_I2S_STANDARD_PHILIPS     0x03
#define AK4343_I2S_STANDARD_MSB         0x02
#define AK4343_I2S_STANDARD_LSB         0x01
    
/**
  * @}
  */ 

/** @defgroup AK4343_Exported_Macros
  * @{
  */
/* Conversion of volume from user scale [0:100] to audio codec AK4343 scale [255:0] */
#define VOLUME_CONVERT(Volume) (((Volume) > 100) ? 255 : ((uint8_t)(255 - ((255*(Volume))/100))))
/**
  * @}
  */ 

/** @defgroup AK4343_Exported_Functions
  * @{
  */
    
/*------------------------------------------------------------------------------
                           Audio Codec functions 
------------------------------------------------------------------------------*/
/* High Layer codec functions */
uint32_t ak4343_Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t I2S_MCLKOutput);

uint32_t ak4343_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size);
uint32_t ak4343_Pause(uint16_t DeviceAddr);
uint32_t ak4343_Resume(uint16_t DeviceAddr);
uint32_t ak4343_Stop(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t ak4343_SetVolume(uint16_t DeviceAddr, uint8_t Volume);
uint32_t ak4343_SetMute(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t ak4343_SetOutputMode(uint16_t DeviceAddr, uint8_t Output);

/* AUDIO IO functions */
void AUDIO_IO_Init(void);
void ak4343_MCLKOutput(uint32_t I2S_MCLKOutput);
void AUDIO_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t AUDIO_IO_Read(uint8_t Addr, uint8_t Reg);


/* Audio driver structure */
extern AUDIO_DrvTypeDef   ak4343_drv;

#endif /* __AK4343_H */

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
