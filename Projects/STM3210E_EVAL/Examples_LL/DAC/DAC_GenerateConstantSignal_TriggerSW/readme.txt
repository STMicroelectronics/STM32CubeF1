/**
  @page DAC_GenerateConstantSignal_TriggerSW DAC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/DAC/DAC_GenerateConstantSignal_TriggerSW/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DAC_GenerateConstantSignal_TriggerSW example.
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

How to use the DAC peripheral to generate a constant voltage signal. This
example is based on the STM32F1xx DAC LL API. The peripheral 
initialization uses LL unitary service functions for optimization purposes
(performance and size).

Example configuration:
One DAC channel (DAC1 channel1) is configured to connect DAC channel output on GPIO pin
to get the samples from SW (no DMA transfer) and to get conversion trigger from SW.

Example execution:
From the main program execution, LED1 is toggling quickly while waiting for
key push button press.
Then, the DAC is configured and activated: constant signal is generated on DAC output
indefinitely.
DAC channel output value is provided by SW, a new value is loaded
at each press on key push button: LED1 toggles and the signal starts from 0, increased by a quarter of Vdda and
finishing at Vdda voltage.
Finally, LED1 is turned-on.

Note: In case of noise on DAC channel, DAC output voltage may trig at a voltage level with an uncertainty of tens of mV.

Connection needed:
None. 
Oscilloscope for monitoring DAC channel output (cf pin below).
Other peripheral used:
  1 GPIO for key push button
  1 GPIO for DAC channel output PA.04 (STM3210E_EVAL board: morpho connector CN11 pin 57)


@par Directory contents 

  - DAC/DAC_GenerateConstantSignal_TriggerSW/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - DAC/DAC_GenerateConstantSignal_TriggerSW/Inc/main.h                  Header for main.c module
  - DAC/DAC_GenerateConstantSignal_TriggerSW/Inc/stm32_assert.h          Template file to include assert_failed function
  - DAC/DAC_GenerateConstantSignal_TriggerSW/Src/stm32f1xx_it.c          Interrupt handlers
  - DAC/DAC_GenerateConstantSignal_TriggerSW/Src/main.c                  Main program
  - DAC/DAC_GenerateConstantSignal_TriggerSW/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xG devices.
    
  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
