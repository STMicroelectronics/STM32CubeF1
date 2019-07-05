/**
  @page ADC_AnalogWatchdog ADC conversion example with analog watchdog, using 
   related peripherals (GPIO, DMA, Timer), voltage input from DAC, user control
   by push button and LED

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

How to use the ADC peripheral to perform conversions with an analog watchdog 
and out-of-window interrupts enabled.

One compilation switch is available to select ADC configuration continuous mode
and external trigger (located in main.h):
 - "ADC_TRIGGER_FROM_TIMER" defined: ADC is operating in not continuous mode
   and conversions are trigger by external trigger: timer.
 - "ADC_TRIGGER_FROM_TIMER" not defined: ADC is operating in continuous mode
   and first conversion is trigger by software trigger.

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
 - Timer peripheral is used to trigger ADC conversions.
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
 - Regular conversion are triggered by external event (timer at 1kHz).
 - Continuous mode is disabled to yield only 1 conversion at each conversion trigger.
 - Analog watchdog 1 is enabled, minimum and maximum thresholds are respectively set 
   to 1/8 and 5/8 of full range scale (between 0.41V and 2.06V with full range of 3.3V).
  
ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - DMA and ADC are configured to operate continuously, in circular mode.
   When DMA transfer half-buffer and full buffer lengths are reached, callbacks 
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.

Board settings:
 - ADC is configured to convert ADC_CHANNEL_4 (pin PA.04).
 - Apply an analog voltage on pin PA.04 and click on push button to update the ADC
   analog watchdog result on STM32F103RB-Nucleo board LED.
   Note: This example, on some other STM32 boards, is using DAC to generate an analog voltage to ADC input.
         On STM32F103RB-Nucleo, the device has no DAC available, therefore analog signal must be supplied externally.


STM32F103RB-Nucleo board LED is be used to monitor the program execution status:
 - Normal operation: LED2 is turned-on/off in function of ADC conversion
   result.
    - Turned-off if voltage into of AWD window
    - Turned-on if voltage is out of AWD window
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_AnalogWatchdog/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - ADC/ADC_AnalogWatchdog/Inc/stm32f1xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/Inc/main.h                  Header for main.c module  
  - ADC/ADC_AnalogWatchdog/Src/stm32f1xx_it.c          HAL interrupt handlers
  - ADC/ADC_AnalogWatchdog/Src/main.c                  Main program
  - ADC/ADC_AnalogWatchdog/Src/stm32f1xx_hal_msp.c     HAL MSP file
  - ADC/ADC_AnalogWatchdog/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F1xx devices.

  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
