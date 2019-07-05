/**
  @page DAC_GenerateWaveform_TriggerHW DAC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/DAC/DAC_GenerateWaveform_TriggerHW/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DAC_GenerateWaveform_TriggerHW example.
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

How to use the DAC peripheral to generate a voltage waveform from a digital data
stream transfered by DMA. This example is based on the STM32F1xx DAC
LL API. The peripheral initialization uses LL unitary service
functions for optimization purposes (performance and size).

Example configuration:
One DAC channel (DAC1 channel1) is configured to connect DAC channel output on GPIO pin
to get the samples from DMA transfer and to get conversion trigger from timer.

Other peripherals configured in this example:
one DMA channel and one timer are configured to provide samples to be generated
by DAC peripheral:
sinus waveform, frequency 1kHz, amplitude Vdda.
(these settings can be customized by changing the defined values for the waveform generation parameters in main.c file).

Example execution:
From the main program execution, LED1 is toggling quickly while waiting for
key push button press.
DMA and timer are configured and activated to transfer data and trig DAC conversion.
Then, the DAC is configured and activated: waveform is generated on DAC output
indefinitely.
DAC channel output value is provided by DMA transfer, a new value is loaded
at each trigger from trimer.
Finally, LED1 is turned-on.

Connection needed:
None. 
Oscilloscope for monitoring DAC channel output (cf pin below).
Other peripheral used:
  1 GPIO for key push button
  1 GPIO for DAC channel output PA.04 (STM3210E_EVAL board: morpho connector CN11 pin 57)
  For waveform voltage generation: 1 DMA channel, 1 timer.


@par Directory contents 

  - DAC/DAC_GenerateWaveform_TriggerHW/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - DAC/DAC_GenerateWaveform_TriggerHW/Inc/main.h                  Header for main.c module
  - DAC/DAC_GenerateWaveform_TriggerHW/Inc/stm32_assert.h          Template file to include assert_failed function
  - DAC/DAC_GenerateWaveform_TriggerHW/Src/stm32f1xx_it.c          Interrupt handlers
  - DAC/DAC_GenerateWaveform_TriggerHW/Src/main.c                  Main program
  - DAC/DAC_GenerateWaveform_TriggerHW/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xG devices.
    
  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
