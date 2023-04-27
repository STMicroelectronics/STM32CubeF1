/**
  @page ADC_ContinuousConversion_TriggerSW_Init ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_ContinuousConversion_TriggerSW_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_ContinuousConversion_TriggerSW_Init example.
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

How to use an ADC peripheral to perform continuous ADC conversions on a 
channel, from a software start.
This example is based on the STM32F1xx ADC LL API.
The peripheral initialization is done using LL initialization function
to demonstrate LL init usage. 

Example configuration:
ADC is configured to convert a single channel, in continuous conversion mode,
from SW trigger.

Example execution:
From the first press on User Button, the ADC converts the selected channel
continuously.
After the first trigger (software start in this example), following conversions
are launched successively automatically, indefinitely.
Software polls for the first conversion completion
and stores it into a variable, LED2 is turned on.
Main program reads frequently ADC conversion data 
(without waiting for end of each conversion: software reads data 
when main program execution pointer is available and can let 
some ADC conversions data unread and overwritten by newer data)
and stores it into the same variable.

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values

Connection needed:
Use an external power supply, adjust supply voltage and connect it to analog input pin (cf pin below).

Other peripherals used:
  1 GPIO for User Button
  1 GPIO for LED2
  1 GPIO for analog input: PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)

@par Directory contents 

  - ADC/ADC_ContinuousConversion_TriggerSW_Init/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - ADC/ADC_ContinuousConversion_TriggerSW_Init/Inc/main.h                  Header for main.c module
  - ADC/ADC_ContinuousConversion_TriggerSW_Init/Inc/stm32_assert.h          Template file to include assert_failed function.
  - ADC/ADC_ContinuousConversion_TriggerSW_Init/Src/stm32f1xx_it.c          Interrupt handlers
  - ADC/ADC_ContinuousConversion_TriggerSW_Init/Src/main.c                  Main program
  - ADC/ADC_ContinuousConversion_TriggerSW_Init/Src/system_stm32f1xx.c      STM32F1xx system source file


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
