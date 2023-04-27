/**
  @page RTC_Alarm RTC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/RTC/RTC_Alarm/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

Configuration of the RTC LL API to configure and generate an alarm using the RTC peripheral. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

In this example, after start-up, SYSCLK is configured to the max frequency using the PLL with
HSI as clock source.

The RTC peripheral configuration is ensured by the Configure_RTC() function 
(configure of the needed RTC resources according to the used hardware CLOCK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

@note LSE oscillator clock is used as RTC clock source by default.
      The user can use also LSI or HSE_DIV128 as RTC clock source. The user uncomment the adequate 
      line on the main.c file.
      @code
         /* #define RTC_CLOCK_SOURCE_LSI */
         #define RTC_CLOCK_SOURCE_LSE
         /* #define RTC_CLOCK_SOURCE_HSE_DIV128 */
      @endcode
      LSI oscillator clock is delivered by a clock frequency around 40 kHz RC(between 30 kHz and 60 kHz).
      LSE (when available on board) is delivered by a 32.768 kHz crystal.
      HSE clock divided by 128 (on this Board HSE_DIV128 = 62.500 kHz).
      
@note The frequency dispersion of the Low Speed Internal RC (LSI) oscillator can be calibrated to
      have accurate RTC time base with an acceptable accuracy. 
      For more details, refer to LSI calibration procedure in the Reference Manual.


Configure_RTC_Alarm function is then called to initialize the 
time, date and alarm.

In this example, the Time is set to 11:59:55 and the Alarm must be generated after 
30 seconds on 12:00:25.

The current time and date are updated and displayed on the debugger in aShowTime 
and aShowDate variables (watch or live watch).

- LED1 is turned ON when the RTC Alarm is generated correctly.
- LED1 is toggling : This indicates that the system generates an error.

@par Directory contents 

  - RTC/RTC_Alarm/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - RTC/RTC_Alarm/Inc/main.h                  Header for main.c module
  - RTC/RTC_Alarm/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_Alarm/Src/stm32f1xx_it.c          Interrupt handlers
  - RTC/RTC_Alarm/Src/main.c                  Main program
  - RTC/RTC_Alarm/Src/system_stm32f1xx.c      STM32f1xx system source file


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
