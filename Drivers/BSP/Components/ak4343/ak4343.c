/**
  ******************************************************************************
  * @file    ak4343.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    11-April-2016
  * @brief   This file provides the AK4343 Audio Codec driver.   
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
#include "ak4343.h"

/* Private variables ---------------------------------------------------------*/

/* Local variable to determine whether the audio Codec is driven by:          */
/* - I2S master clock: value different of "0",                                */
/*   equivalent to I2S_MCLKOUTPUT_ENABLE                                      */
/* - its own internal PLL from I2S_CK: value equal to "0",                    */
/*   equivalent to I2S_MCLKOUTPUT_DISABLE                                     */
uint8_t ak4343_i2s_mclk_output = 1;     /* Default setting: master clock required */


/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup Components
  * @{
  */ 

/** @addtogroup ak4343
  * @brief     This file provides a set of functions needed to drive the 
  *            CS43l22 audio codec.
  * @{
  */

/** @defgroup AK4343_Private_Types
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup AK4343_Private_Defines
  * @{
  */
/* Uncomment this line to enable verifying data sent to codec after each write 
   operation (for debug purpose) */
#if !defined (VERIFY_WRITTENDATA)  
/* #define VERIFY_WRITTENDATA */
#endif /* VERIFY_WRITTENDATA */
/**
  * @}
  */ 

/** @defgroup AK4343_Private_Macros
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup AK4343_Private_Variables
  * @{
  */

/* Audio codec driver structure initialization */  
AUDIO_DrvTypeDef ak4343_drv = 
{
  ak4343_Init,
  0,                      /* On some other codec audio devices: DeInit */
  0,                      /* On some other codec audio devices: ReadID */
  ak4343_Play,
  ak4343_Pause,
  ak4343_Resume,
  ak4343_Stop,  
  0,                      /* On some other codec audio devices: SetFrequency */
  ak4343_SetVolume,
  ak4343_SetMute,  
  ak4343_SetOutputMode,
  0,                      /* On some other codec audio devices: Reset */
};

static uint8_t Is_ak4343_Stop = 1;

/**
  * @}
  */ 

/** @defgroup AK4343_Function_Prototypes
  * @{
  */
static uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
/**
  * @}
  */ 

/** @defgroup AK4343_Private_Functions
  * @{
  */ 

/**
  * @brief Initializes the audio codec and the control interface.
  *        Note: I2S standard is fixed to standard Philips.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
  uint32_t counter = 0;
  uint32_t Standard = 0;
  uint32_t PLLMode =0;
  
  /* Initialize the Control interface of the Audio Codec */
  AUDIO_IO_Init();

  /* I2S standard fixed to standard Philips */
  /* (Corresponding to I2S init parameter standard = I2S_STANDARD_PHILIPS) */
  Standard = AK4343_I2S_STANDARD_PHILIPS;
  
  /* PLL Slave SD/WS reference mode ----------------------*/
  if (ak4343_i2s_mclk_output == 0) /* I2S_MCLKOUTPUT_DISABLE */
  {
    /* set the PLLMode variable */
    PLLMode = 0x1;
  
    /* Set I2S standard */
    counter += CODEC_IO_Write(DeviceAddr, 0x04, (Standard | 0x20));
    /* MCKI input frequency = 256.Fs */
    counter += CODEC_IO_Write(DeviceAddr, 0x05, 0x03);
    /* VCOM Power up (PMVCM bit)*/
    counter += CODEC_IO_Write(DeviceAddr, 0x00, 0x40);
    /* Enable PLL*/
    counter += CODEC_IO_Write(DeviceAddr, 0x01, 0x01);
  }
  /* Ext Slave mode with no PLL --------------------------*/
  else
  {
    /* Reset the PLL mode variable */
    PLLMode = 0;
    /* Set I2S standard */
    counter += CODEC_IO_Write(DeviceAddr, 0x04, Standard);
    /* MCKI input frequency = 256.Fs */
    counter += CODEC_IO_Write(DeviceAddr, 0x05, 0x00);
    /* VCOM Power up (PMVCM bit)*/
    counter += CODEC_IO_Write(DeviceAddr, 0x00, 0x40);
  }
  
  
    /* Left Channel Digital Volume control */
    counter += CODEC_IO_Write(DeviceAddr, 0x0A, VOLUME_CONVERT(Volume));  
    /* Right Channel Digital Volume control */
    counter += CODEC_IO_Write(DeviceAddr, 0x0D, VOLUME_CONVERT(Volume));
  
  /* Extra Configuration (of the ALC) */
  counter += CODEC_IO_Write(DeviceAddr, 0x06, 0x3C );
  counter += CODEC_IO_Write(DeviceAddr, 0x08, 0xE1 );
  counter += CODEC_IO_Write(DeviceAddr, 0x0B, 0x00 );
  counter += CODEC_IO_Write(DeviceAddr, 0x07, 0x20 );
  counter += CODEC_IO_Write(DeviceAddr, 0x09, 0xC1 );
  counter += CODEC_IO_Write(DeviceAddr, 0x0C, 0xC1 );
  
  /* Uncomment these lines and set the correct filters values to use the    */
  /* codec digital filters (for more details refer to the codec datasheet)  */
    /* Filter 1 programming as High Pass filter (Fc=500Hz, Fs=8KHz, K=20, A=1, B=1) */
    /*
    counter += CODEC_IO_Write(DeviceAddr, 0x1C, 0x01);
    counter += CODEC_IO_Write(DeviceAddr, 0x1D, 0x80);
    counter += CODEC_IO_Write(DeviceAddr, 0x1E, 0xA0);
    counter += CODEC_IO_Write(DeviceAddr, 0x1F, 0x0B);
    */
    /* Filter 3 programming as Low Pass filter (Fc=20KHz, Fs=8KHz, K=40, A=1, B=1) */
    /*
    counter += CODEC_IO_Write(DeviceAddr, 0x1C, 0x01);
    counter += CODEC_IO_Write(DeviceAddr, 0x1D, 0x00);
    counter += CODEC_IO_Write(DeviceAddr, 0x1E, 0x01);
    counter += CODEC_IO_Write(DeviceAddr, 0x1F, 0x01);
    */
  
    /* Equilizer programming BP filter (Fc1=20Hz, Fc2=2.5KHz, Fs=44.1KHz, K=40, A=?, B=?, C=?) */
    /*
    counter += CODEC_IO_Write(DeviceAddr, 0x16, 0x00);
    counter += CODEC_IO_Write(DeviceAddr, 0x17, 0x75);
    counter += CODEC_IO_Write(DeviceAddr, 0x18, 0x00);
    counter += CODEC_IO_Write(DeviceAddr, 0x19, 0x01);
    counter += CODEC_IO_Write(DeviceAddr, 0x1A, 0x00);
    counter += CODEC_IO_Write(DeviceAddr, 0x1B, 0x51);
    */
  
  
  
  /* HEADPHONE codec configuration */
  if ((OutputDevice & OUTPUT_DEVICE_HEADPHONE) != 0)
  {
    /* MCKI is 256.Fs with no PLL */
    counter += CODEC_IO_Write(DeviceAddr, 0x05, 0x00 );
    /* Switch control from DAC to Headphone */
    counter += CODEC_IO_Write(DeviceAddr, 0x0F, 0x09 );
    /* Bass Boost and Demphasis enable */
    counter += CODEC_IO_Write(DeviceAddr, 0x0E, 0x18 );
    /* Power up MIN and DAC (PMMIN and PMDAC bits) */
    counter += CODEC_IO_Write(DeviceAddr, 0x00, 0x74);
    /* Enable Slave mode and Left/Right HP lines*/
    counter += CODEC_IO_Write(DeviceAddr, 0x01, (0x30 | PLLMode));
    /* Exit HP mute mode */
    counter += CODEC_IO_Write(DeviceAddr, 0x01, (0x70 | PLLMode));
  }
  
  /* SPEAKER codec configuration */
  if ((OutputDevice & OUTPUT_DEVICE_SPEAKER) != 0)
  {
    /* ReSelect the MCKI frequency (FS0-1 bits): 256.Fs */
    counter += CODEC_IO_Write(DeviceAddr, 0x05, 0x02 );
    /* Set up the path "DAC->Speaker-Amp" with no power save (DACS and SPPSN bits) */ 
    counter += CODEC_IO_Write(DeviceAddr, 0x02, 0x20 );
    /* Speaker Gain (SPKG0-1 bits): Gain=+10.65dB(ALC off)/+12.65(ALC on) */
    counter += CODEC_IO_Write(DeviceAddr, 0x03, 0x10);
    /* Power up Speaker and DAC (PMSPK and PMDAC bits) */
    counter += CODEC_IO_Write(DeviceAddr, 0x00, 0x54);
    /* Set up the path "DAC -> Speaker-Amp" with no power save (SPPSN bit) */ 
    counter += CODEC_IO_Write(DeviceAddr, 0x02, 0xA0 /*0xA1*/);   
  }
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief Start the audio Codec play feature.
  * @note For this codec no Play options are required.
  * @param DeviceAddr: Device address on communication Bus.   
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size)
{
  uint32_t counter = 0;
  
  if(Is_ak4343_Stop == 1)
  {
    /* Enable Output device */  
    counter += ak4343_SetMute(DeviceAddr, AUDIO_MUTE_OFF);
    
    Is_ak4343_Stop = 0;
  }
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief Pauses playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_Pause(uint16_t DeviceAddr)
{
  uint32_t counter = 0;
 
  /* Pause the audio file playing */
  /* Mute the output first */
  counter += ak4343_SetMute(DeviceAddr, AUDIO_MUTE_ON);
    
  return counter;
}

/**
  * @brief Resumes playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_Resume(uint16_t DeviceAddr)
{
  uint32_t counter = 0;
  
  /* Unmute the output */
  counter += ak4343_SetMute(DeviceAddr, AUDIO_MUTE_OFF);
  
  return counter;
}

/**
  * @brief Stops audio Codec playing. It powers down the codec.
  * @param DeviceAddr: Device address on communication Bus. 
  * @param CodecPdwnMode: selects the  power down mode.
  *          - CODEC_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration 
  *                           (user should re-Initialize the codec in order to 
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_Stop(uint16_t DeviceAddr, uint32_t CodecPdwnMode)
{
  uint32_t counter = 0;
  
  /* Mute the output first */
  counter += ak4343_SetMute(DeviceAddr, AUDIO_MUTE_ON);
  
  /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
  counter += CODEC_IO_Write(DeviceAddr, 0x02, 0x9F);
  
  Is_ak4343_Stop = 1;
  return counter;    
}

/**
  * @brief Sets higher or lower the codec volume level.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param Volume: a byte value from 0 to 255 (refer to codec registers 
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_SetVolume(uint16_t DeviceAddr, uint8_t Volume)
{
  uint32_t counter = 0;
  uint8_t convertedvol = VOLUME_CONVERT(Volume);
  
  /* Left Channel Digital Volume control */
  counter += CODEC_IO_Write(DeviceAddr, 0x0A, convertedvol);
  
  /* Right Channel Digital Volume control */
  counter += CODEC_IO_Write(DeviceAddr, 0x0D, convertedvol);
    
  return counter;
}

/**
  * @brief Enables or disables the mute feature on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.   
  * @param Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
  uint32_t counter = 0;
  uint32_t tmp = 0;
  
  /* Read the current value of the config register number 0x0E */
  tmp = AUDIO_IO_Read(DeviceAddr, 0x0E);
  
  /* Set the Mute mode */
  if(Cmd == AUDIO_MUTE_ON)
  {
    counter += CODEC_IO_Write(DeviceAddr, 0x0E, (tmp | 0x20));
  }
  else /* AUDIO_MUTE_OFF Disable the Mute */
  {
    counter += CODEC_IO_Write(DeviceAddr, 0x0E, (tmp & 0xD1));
  }
  return counter;
}

/**
  * @brief Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @param DeviceAddr: Device address on communication Bus.
  * @param Output: specifies the audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_BOTH
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t ak4343_SetOutputMode(uint16_t DeviceAddr, uint8_t Output)
{
  uint32_t counter = 0; 
  uint32_t tmp_reg = 0;
  
  switch (Output) 
  {
    case OUTPUT_DEVICE_SPEAKER:       /* SPK always OFF & HP always ON */
      /* Turn-off headset */
      /* Disable Left/Right HP lines*/
      tmp_reg = AUDIO_IO_Read(DeviceAddr, 0x01);
      counter += CODEC_IO_Write(DeviceAddr, 0x01, tmp_reg &~ 0x70);   
      
      /* Turn-on speaker */
      /* Power up Speaker and DAC (PMSPK and PMDAC bits) */
      counter += CODEC_IO_Write(DeviceAddr, 0x00, 0x54);
      /* Set up the path "DAC -> Speaker-Amp" with no power save (SPPSN bit) */ 
      counter += CODEC_IO_Write(DeviceAddr, 0x02, 0xA0 /*0xA1*/);   

      break;
    
    case OUTPUT_DEVICE_HEADPHONE:         /* SPK always ON & HP always OFF */
      /* Turn-on headset */
      /* Power up MIN and DAC (PMMIN and PMDAC bits) */
      counter += CODEC_IO_Write(DeviceAddr, 0x00, 0x74);
      /* Enable Slave mode and Left/Right HP lines*/
      counter += CODEC_IO_Write(DeviceAddr, 0x01, (0x30 | 0x00));
      /* Exit HP mute mode */
      counter += CODEC_IO_Write(DeviceAddr, 0x01, (0x70 | 0x00));
      
      /* Turn-off speaker */
      /* Set up the path "DAC -> Speaker-Amp" with power save (SPPSN bit) */ 
      tmp_reg = AUDIO_IO_Read(DeviceAddr, 0x02);
      counter += CODEC_IO_Write(DeviceAddr, 0x02, tmp_reg &~ 0x80);   

      break;

    default:
    case OUTPUT_DEVICE_BOTH:            /* SPK always ON & HP always ON */
      /* Turn-on headset */
      /* Enable Slave mode and Left/Right HP lines*/
      tmp_reg = AUDIO_IO_Read(DeviceAddr, 0x01);
      counter += CODEC_IO_Write(DeviceAddr, 0x01, tmp_reg | 0x70); 
      
      /* Turn-on speaker */
      /* Set up the path "DAC -> Speaker-Amp" with no power save (SPPSN bit) */ 
      tmp_reg = AUDIO_IO_Read(DeviceAddr, 0x02);
      counter += CODEC_IO_Write(DeviceAddr, 0x02, tmp_reg | 0x80);  
      break;

  }  
  return counter;
}

/**
  * @brief Enables or disables the requirement of I2S master clock 
  *        (supplied by I2S master device)
  * @param I2S_MCLKOutput: I2S initialisation parameter "MCLKOutput" determines 
  *        whether the audio Codec is driven by:
  *         - I2S master clock: value different of "0",
  *           equivalent to I2S_MCLKOUTPUT_ENABLE
  *         - its own internal PLL from I2S_CK: value equal to "0",
  *           equivalent to I2S_MCLKOUTPUT_DISABLE
  * @retval None
  */
void ak4343_MCLKOutput(uint32_t I2S_MCLKOutput)
{
  /* Update private variable */
  ak4343_i2s_mclk_output = I2S_MCLKOutput;
  
}

/**
  * @brief  Writes/Read a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  Value: Data to be written
  * @retval None
  */
static uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  uint32_t result = 0;
  
  AUDIO_IO_Write(Addr, Reg, Value);
  
#ifdef VERIFY_WRITTENDATA
  /* Verify that the data has been correctly written */  
  result = (AUDIO_IO_Read(Addr, Reg) == Value)? 0:1;
#endif /* VERIFY_WRITTENDATA */
  
  return result;
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
