/**
  @page ADC_SingleConversion_TriggerSW ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_SingleConversion_TriggerSW/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerSW example.
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

How to use an ADC peripheral to perform a single ADC conversion on a channel 
at each software start. This example uses the polling programming model (for 
interrupt or DMA programming models, please refer to other examples).
This example is based on the STM32F1xx ADC LL API.
The peripheral initialization is done using LL unitary service functions for 
optimization purposes (performance and size).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.

Example execution:
At each press on User Button, the ADC performs 1 conversion of the selected channel.
Software polls for conversion completion.
When conversion is completed, main program reads conversion data
from ADC data register and stores it into a variable, LED2 is turned on.

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

  - ADC/ADC_SingleConversion_TriggerSW/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerSW/Inc/main.h                  Header for main.c module
  - ADC/ADC_SingleConversion_TriggerSW/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_SingleConversion_TriggerSW/Src/stm32f1xx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerSW/Src/main.c                  Main program
  - ADC/ADC_SingleConversion_TriggerSW/Src/system_stm32f1xx.c      STM32F1xx system source file


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
