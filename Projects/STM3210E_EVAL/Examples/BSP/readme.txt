/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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
  @endverbatim

@par Example Description 

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using key button. 

 ** Push the User button to start first Test.  
Blue Led (LD4) will blink between each test.Press Key push-button to start another test:

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** SD **
This example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

 ** EEPROM **
This example show how to read and write data in EEPROM I2C M24LR64 connected on STM3210E-EVAL RevD
   * The I2C RF EEPROM memory (M24LR64) is available through the connector CN12.

 ** TSENSOR **
This example show how to read a Temperature using the temperature sensor.

 ** AUDIO PLAY **
This example show how to play/pause/resume or change the volume (min 50% / max 100 %)
of an audio playback using Key or Joystick button.

      Plug a headphone to ear the sound  /!\ Take care of yours ears.
      Default volume is 60%.
      A press of JOY_SEL button will pause the audio file play (LED 1 green keep previous state
       and LED2 orange ON).
      Another press on JOY_SEL button will resumes audio file play (only LED 1 green blink at each replay)
      @Note: Copy file 'audio.bin' (available in Utilities\Media\AudioFile folder) directly in the flash 
      at @0x08016800 using ST-Link utility


 ** NOR **
This example shows how to erase, write and read data available on the NOR flash 
available on the board.

 ** SRAM **
This example shows how to erase, write and read data available on the external 
SRAM available on the board.

 ** NAND **
This example shows how to erase, write and read data available on the external 
NAND available on the board.

 ** FLASH **
This example shows how to erase, write and read data available on the external 
FLASH SPI available on the board.
 ** LCD LOG **
This example show how to use the LCD log features. 

@par Directory contents 

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32f1xx.c     STM32F1xx system clock configuration file
  - BSP/Src/stm32f1xx_it.c         Interrupt handlers 
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/sd.c                   SD features
  - BSP/Src/eeprom.c               EEPROM Read/Write features
  - BSP/Src/temperature_sensor.c   Temperature Sensor features
  - BSP/Src/audio_record.c         Audio Record features
  - BSP/Src/nor.c                  NOR features
  - BSP/Src/sram.c                 SRAM features
  - BSP/Src/nand.c                 NAND features
  - BSP/Src/flash.c                FLASH features
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32f1xx_conf.h       HAL Configuration file
  - BSP/Inc/stm32f1xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/stlogo.h               Image used for BSP example
        
@par Hardware and Software environment  

  - This example runs on STM32F103xG devices.
    
  - This example has been tested with STMicroelectronics STM3210E-EVAL RevD
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
