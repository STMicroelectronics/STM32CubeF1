/**
  @page IWDG_RefreshUntilUserEvent IWDG example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/IWDG/IWDG_RefreshUntilUserEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IWDG_RefreshUntilUserEvent example.
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

How to configure the IWDG peripheral to ensure periodical counter update and 
generate an MCU IWDG reset when a user button is pressed. The peripheral 
is initialized with LL unitary service functions to optimize 
for performance and size.

Example Configuration:
Configure the IWDG (prescaler, counter) and enable it.
Infinite refresh of the IWDG down-counter done in the main loop.
LED2 is blinking fast & continuously.

Example Execution:
When User Button is pressed, the down-counter automatic refresh mechanism is 
disable and thus, reset will occur. After a reset, when re-entering in the main, 
RCC IWDG Reset Flag will be checked and if we are back from a IWDG reset the LED2
will be switch ON.

Waiting a new user button pressed to re-activate the IWDG

@par Directory contents 

  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/main.h                  Header for main.c module
  - IWDG/IWDG_RefreshUntilUserEvent/Inc/stm32_assert.h          Template file to include assert_failed function
  - IWDG/IWDG_RefreshUntilUserEvent/Src/stm32f1xx_it.c          Interrupt handlers
  - IWDG/IWDG_RefreshUntilUserEvent/Src/main.c                  Main program
  - IWDG/IWDG_RefreshUntilUserEvent/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
