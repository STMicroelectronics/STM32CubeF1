/**
  @page ADC_MultimodeDualInterleaved ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_MultimodeDualInterleaved/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_MultimodeDualInterleaved example.
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

How to use several ADC peripherals in multimode and interleaved mode.
This example is based on the STM32F1xx ADC LL API.
The peripheral initialization is done using LL unitary service functions 
for optimization purposes (performance and size).

Description of multimode interleaved:
ADC master instance synchronizes and manages ADC slave instance.
Multimode interleaved combines ADC instances to convert
the same channel and increase the overall ADC conversion rate.
This example configures the ADC to perform conversions at the 
maximum ADC conversion rate possible (with a sampling time  
corresponding to ADC resolution 12 bits).


Example configuration:
ADC1 and ADC2 instances (master and slave) are configured in continuous conversion mode,
from SW trigger (in order to reach more simply the maximum conversion rate possible,
but trigger from timer is also possible).
Both ADC instances are configured to convert a single channel (1 channel from a GPIO),
this channel is common to ADC1 and ADC2.
DMA is configured to transfer conversion data in an array, in circular mode.
Note: One DMA channel is used (the one of ADC master) to transfer data from
      both ADC instances (master and slave).
      Usage of two DMA channels (one for ADC master, one for ADC slave) is also
      possible: this is the recommended configuration in case of high ADC
      conversions rates and applications using other DMA channels intensively.

Example execution:
From the first press on push button, the ADC converts the selected channel
continuously.
DMA transfers conversion data to the array, transfer complete interruption occurs.
Results array is updated indefinitely (DMA in circular mode).
LED2 is turned on when the DMA transfer is completed (results array full)
and turned off at next DMA half-transfer (result array first half updated).

Note: ADC is converting at high frequency, therefore LED2 is toggling at high frequency
      and will appear as dimming.
      Use an oscilloscope probe on LED2 to watch DMA transfers frequency.

Note: A file is provide with this example to detail the ADC conversions timings.
      Refer to file "ADC MultimodeDualInterleaved timing simulation STM32F1.xls".

For debug: variables to monitor with debugger watch window:
 - "aADCxADCyMultimodeDualConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]": ADC multimode dual conversion data: ADC master and ADC slave conversion data are concatenated in a registers of 32 bits.
 - "aADCxMultimodeDualMasterConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]": For the purpose of this example, dispatch multimode dual conversion data into array corresponding to ADC master conversion data.
 - "aADCyMultimodeDualSlaveConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]": For the purpose of this example, dispatch multimode dual conversion data into array corresponding to ADC slave conversion data.
 - "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions

Connection needed:
Use an external power supply, adjust supply voltage and connect it to analog input pin (cf pin below).

Other peripherals used:
  1 GPIO for push button
  1 GPIO for LED2
  1 GPIO for analog input: PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  DMA

@par Directory contents 

  - ADC/ADC_MultimodeDualInterleaved/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - ADC/ADC_MultimodeDualInterleaved/Inc/main.h                  Header for main.c module
  - ADC/ADC_MultimodeDualInterleaved/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_MultimodeDualInterleaved/Src/stm32f1xx_it.c          Interrupt handlers
  - ADC/ADC_MultimodeDualInterleaved/Src/main.c                  Main program
  - ADC/ADC_MultimodeDualInterleaved/Src/system_stm32f1xx.c      STM32F1xx system source file


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
