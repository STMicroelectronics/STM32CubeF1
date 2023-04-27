/**
  @page EXTI_ToggleLedOnIT EXTI example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/EXTI/EXTI_ToggleLedOnIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EXTI example.
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

How to configure the EXTI and use GPIOs to toggle the user LEDs 
available on the board when a user button is pressed. It is based on the
STM32F1xx LL API. The peripheral initialization uses LL unitary service
functions for optimization purposes (performance and size).

In this example, one EXTI line (External lines 10 to 15) is configured to generate
an interrupt on each falling edge.

In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:
    - External lines 10 to 15 is connected to PC.13 pin
      - when falling edge is detected on External lines 10 to 15 by pressing User push-button, LED2 toggles

On STM32F103RB-Nucleo:
    - External lines 10 to 15 is connected to User push-button


@par Directory contents 

  - EXTI/EXTI_ToggleLedOnIT/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - EXTI/EXTI_ToggleLedOnIT/Inc/main.h                  Header for main.c module
  - EXTI/EXTI_ToggleLedOnIT/Inc/stm32_assert.h          Template file to include assert_failed function
  - EXTI/EXTI_ToggleLedOnIT/Src/stm32f1xx_it.c          Interrupt handlers
  - EXTI/EXTI_ToggleLedOnIT/Src/main.c                  Main program
  - EXTI/EXTI_ToggleLedOnIT/Src/system_stm32f1xx.c      STM32F1xx system source file

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
