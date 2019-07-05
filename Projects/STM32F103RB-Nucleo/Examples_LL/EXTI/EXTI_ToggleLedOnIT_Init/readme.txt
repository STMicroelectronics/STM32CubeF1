/**
  @page EXTI_ToggleLedOnIT_Init EXTI example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/EXTI/EXTI_ToggleLedOnIT_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EXTI example.
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

@par Example Description

How to configure the EXTI and use GPIOs to toggle the user LEDs
available on the board when a user button is pressed. This example is 
based on the STM32F1xx LL API. The peripheral initialization uses 
LL initialization functions to demonstrate LL init usage.

In this example, one EXTI line (External lines 10 to 15) is configured to generate
an interrupt on each falling edge.

In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:
    - External lines 10 to 15 is connected to PC.13 pin
      - when falling edge is detected on External lines 10 to 15 by pressing User push-button, LED2 toggles

On STM32F103RB-Nucleo:
    - External lines 10 to 15 is connected to User push-button


@par Directory contents 

  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/main.h                  Header for main.c module
  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/stm32f1xx_it.c          Interrupt handlers
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/main.c                  Main program
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
