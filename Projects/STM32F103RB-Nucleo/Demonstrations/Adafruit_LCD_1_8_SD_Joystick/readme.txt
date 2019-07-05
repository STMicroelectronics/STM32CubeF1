/**
  @page Demo   Demo STM32F103RB-Nucleo
 
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32F103RB-Nucleo Demo
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
  @endverbatim

@par Demo Description

Demonstration firmware based on STM32Cube. This example helps you to discover
STM32 Cortex-M devices that are plugged onto your STM32 Nucleo board.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 64 MHz.
 
 
Below you find the sequence to discover the demonstration :

 - Check the availability of adafruit 1.8" TFT shield on top of STM32 Nucleo 
   board. This is done by reading the state of IO PB.00 pin (mapped to JoyStick 
   available on adafruit 1.8" TFT shield). If the state of PB.00 is high then 
   the adafruit 1.8" TFT shield is available.
    
   - Adafruit 1.8" TFT shield is not available:
     LED2 is toggling with a first frequency equal to ~1Hz.
     Pressing User button lets LED2 toggling with a second frequency equal to ~5Hz.
     Pressing User button again, changes LED2 toggling frequency to ~10Hz.
     This is done in an infinite loop.      

   - Adafruit 1.8" TFT shield is available:
     LED2 is turned ON, because it's sharing the same pin with the SPI CLK signal
     used to communicate with the Adafruit 1.8" TFT shield.
     A menu will be displayed on TFT. Follow instructions below:
     
     - Joystick DOWN to continue menu display
     - Choose the desired display mode: Press Joystick DOWN for automatic mode 
       or Joystick UP for manual mode.
       
       - Manual Mode selected:
         Images available on SD Card, are displayed by pressing Joystick RIGHT 
         to display next image or Joystick LEFT to display previous one. 
         Pressing long (~1s) Joystick SEL, switches display mode to automatic one.
       
       - Automatic Mode selected:
         Images available on SD Card are displayed sequentially in a forever loop.


It is worth noting that the application manages some errors occurred during the 
access to uSD card to parse bmp images:

 - If SD Card is not FAT formatted, a message will be displayed on TFT. 
   In this case, format the SD card and put into its root directory the .bmp 
   files available within the FW package under Utilities\Media\Images folder.

 - If the content of the SD card is other than a bitmap file, a message will 
   be displayed on TFT mentioning that it is not supported.
   In this case, put into the SD card's root directory the .bmp 
   files available within the FW package under Utilities\Media\Images folder which are respecting
   the following criteria:
   o Dimensions: 128x160
   o Width: 128 pixels
   o Height: 160 pixels
   o Bit depth: 16
   o Item type: BMP file
   o The name of the bmp image file must not exceed 11 characters (including
     .bmp extension).


@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, 
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

@par Directory contents
 
  - Adafruit_LCD_1_8_SD_Joystick/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - Adafruit_LCD_1_8_SD_Joystick/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - Adafruit_LCD_1_8_SD_Joystick/Inc/main.h                  Header for main.c module
  - Adafruit_LCD_1_8_SD_Joystick/Inc/fatfs_storage.h         Header for fatfs_storage.c module
  - Adafruit_LCD_1_8_SD_Joystick/Inc/ffconf.h                FAT file system module configuration file   
  - Adafruit_LCD_1_8_SD_Joystick/Src/stm32f1xx_it.c          Interrupt handlers
  - Adafruit_LCD_1_8_SD_Joystick/Src/main.c                  Main program
  - Adafruit_LCD_1_8_SD_Joystick/Src/fatfs_storage.c         Storage (FatFs) driver
  - Adafruit_LCD_1_8_SD_Joystick/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.      
    
  - A SDSC microSD card (capacity up to 4GB), must contain the .bmp files at root
    available under the FW package Utilities\Media\Images folder.

  - Adafruit 1.8" TFT shield must be connected on CN5,CN6, CN8 and CN9 Arduino connectors, 
    for more details please refer to UM1726.

For more details about the adafruit 1.8" TFT shield, please visit: 
http://www.adafruit.com/blog/2012/04/26/new-product-adafruit-1-8-18-bit-color-tft-shield-wmicrosd-and-joystick/

@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
