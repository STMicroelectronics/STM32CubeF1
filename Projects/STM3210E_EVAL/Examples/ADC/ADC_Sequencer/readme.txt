/**
  @page ADC_Sequencer ADC conversion example, using related peripherals (GPIO, DMA), voltage input from DAC, user control by push button and LED

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_Sequencer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

How to use the ADC peripheral with a sequencer to convert several channels. 
The channels converted are, in order, one external channel and two internal 
channels (VrefInt and temperature sensors).

Moreover, voltage and temperature are then computed.

One compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: For this example purpose, generates a
   waveform voltage on a spare DAC channel DAC_CHANNEL_1 (pin PA.04),
   so user has just to connect a wire between DAC channel output and ADC input to run this example.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example.


Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - DMA peripheral is used to transfer ADC conversions data.

ADC settings:
  Sequencer is enabled, and set to convert 3 ranks (3 channels) in discontinuous
  mode, one by one at each conversion trig.

ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - Each address of this array is containing the conversion data of 1 rank of the
   ADC sequencer.
 - When DMA transfer half-buffer and buffer length are reached, callbacks
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
 - When the ADC sequence is fully completed (3 ADC conversions), the
   voltage and temperature are computed and placed in variables:
   uhADCChannelToDAC_mVolt, uhVrefInt_mVolt, wTemperature_DegreeCelsius.


Board settings:
 - ADC is configured to convert ADC_CHANNEL_4 (pin PA.04).
 - The voltage input on ADC channel is provided from potentiometer RV2.
   Turning this potentiometer will make the voltage vary into full range: from 0 to Vdda (3.3V).
   ==> Therefore, there is no external connection needed to run this example.


STM3210E-EVAL RevD board's LEDs are be used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off in function of ADC conversion
   result.
    - Turned-off if sequencer has not yet converted all ranks
    - Turned-on if sequencer has converted all ranks
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_Sequencer/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - ADC/ADC_Sequencer/Inc/stm32f1xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_Sequencer/Inc/main.h                  Header for main.c module  
  - ADC/ADC_Sequencer/Src/stm32f1xx_it.c          DMA interrupt handlers
  - ADC/ADC_Sequencer/Src/main.c                  Main program
  - ADC/ADC_Sequencer/Src/stm32f1xx_hal_msp.c     HAL MSP file
  - ADC/ADC_Sequencer/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F1xx devices.

  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
