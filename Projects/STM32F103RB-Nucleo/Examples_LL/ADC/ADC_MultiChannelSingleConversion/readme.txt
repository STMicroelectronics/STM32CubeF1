/**
  @page ADC_MultiChannelSingleConversion ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_MultiChannelSingleConversion/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_MultiChannelSingleConversion example.
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

How to use an ADC peripheral to convert several channels. ADC conversions are 
performed successively in a scan sequence.
This example is based on the STM32F1xx ADC LL API.
The peripheral initialization is done using LL unitary service functions 
for optimization purposes (performance and size).


Example configuration:
ADC is configured in single conversion mode, from SW trigger.
Sequencer of group regular (default group available on ADC of all STM32 devices)
is configured to convert 3 channels: 1 channel from GPIO, 2 internal channels:
temperature sensor and VrefInt.
DMA is configured to transfer conversion data in an array of size three (one array
address for conversion data of each channel), in circular mode to update
results array at each ADC sequence conversions.

Example execution:
At each press on User Button, the ADC executes the scan sequence: 
ADC performs conversion of the three selected channels successively,
in burst (discontinuous mode not enabled).
The 3 ADC conversions data are transferred by DMA into the results array
"aADCxConvertedData".
ADC conversion data of each sequence rank is at an address of the array:
 - aADCxConvertedData[0]: ADC channel set on rank1 (GPIO as analog input)
 - aADCxConvertedData[1]: ADC channel set on rank2 (VrefInt)
 - aADCxConvertedData[2]: ADC channel set on rank3 (Temperature sensor)
LED2 is turned on when ADC conversions and DMA transfer of the sequence
are completed.

ADC conversions raw data are computed to physical values
using LL ADC driver helper macro:
 - Value of voltage on GPIO pin (on which is mapped ADC channel) (unit: mV)
 - Value of internal voltage reference VrefInt (unit: mV)
 - Value of temperature (unit: degree Celcius)
 - Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda (unit: mV)

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions
 - "uhADCxConvertedData_VoltageGPIO_mVolt":        Value of voltage on GPIO pin (on which is mapped ADC channel) (unit: mV)
 - "uhADCxConvertedData_VrefInt_mVolt":            Value of internal voltage reference VrefInt (unit: mV)
 - "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature (unit: degree Celcius)

Connection needed:
Use an external power supply, adjust supply voltage and connect it to analog input pin (cf pin below).

Other peripherals used:
  1 GPIO for User Button
  1 GPIO for LED2
  1 GPIO for analog input: PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)
  DMA

@par Directory contents 

  - ADC/ADC_MultiChannelSingleConversion/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - ADC/ADC_MultiChannelSingleConversion/Inc/main.h                  Header for main.c module
  - ADC/ADC_MultiChannelSingleConversion/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_MultiChannelSingleConversion/Src/stm32f1xx_it.c          Interrupt handlers
  - ADC/ADC_MultiChannelSingleConversion/Src/main.c                  Main program
  - ADC/ADC_MultiChannelSingleConversion/Src/system_stm32f1xx.c      STM32F1xx system source file


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
