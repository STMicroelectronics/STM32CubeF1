/**
  @page ADC_GroupsRegularInjected ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_GroupsRegularInjected/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_GroupsRegularInjected example.
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

How to use an ADC peripheral with both ADC groups (regular and injected) 
in their intended use cases.
This example is based on the STM32F1xx ADC LL API.
The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Description of ADC group regular and group injected intended use case:
 - group regular for a high number of conversions and continuous
   data stream, with DMA transfer capability.
 - group injected for punctual conversions inserted between
   conversions of group regular.


Example configuration:
ADC is configured to use the 2 groups:
 - ADC group regular:
   ADC is configured to convert a single channel (1 channel from a GPIO),
   in single conversion mode, from HW trigger: timer peripheral .
 - ADC group injected:
   ADC is configured to convert a single channel (1 internal channel: VrefInt),
   in single conversion mode, from SW trigger.
Other peripherals are configured to be used with ADC group regular:
DMA is configured to transfer conversion data in an array (DMA transfer can be used
only with ADC group regular).
A timer is configured in time base and to generate TRGO events (trig from
timer can be used with both ADC groups regular and injected).

Example execution:
From the main program execution, the ADC group regular converts the selected channel
at each trig from timer.
DMA transfers conversion data to the array, DMA transfer complete interruption occurs.
Results array is updated indefinitely (DMA in circular mode).

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "uhADCxGrpInjectedConvertedData": ADC group injected conversion data
 - "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions
 - "ubAdcGrpInjectedUnitaryConvStatus": status of ADC group injected unitary conversion

From the first press on User Button, the ADC group injected converts the selected channel
from a SW conversion start command.
ADC group injected conversion complete interruption occurs, SW transfers
conversion data to a variable, LED2 is turned on.

Connection needed:
Use an external power supply, adjust supply voltage and connect it to analog input pin (cf pin below).

Other peripherals used:
  1 GPIO for User Button
  1 GPIO for LED2
  1 GPIO for analog input: PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  DMA
  Timer

@par Directory contents 

  - ADC/ADC_GroupsRegularInjected/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - ADC/ADC_GroupsRegularInjected/Inc/main.h                  Header for main.c module
  - ADC/ADC_GroupsRegularInjected/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_GroupsRegularInjected/Src/stm32f1xx_it.c          Interrupt handlers
  - ADC/ADC_GroupsRegularInjected/Src/main.c                  Main program
  - ADC/ADC_GroupsRegularInjected/Src/system_stm32f1xx.c      STM32F1xx system source file


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
