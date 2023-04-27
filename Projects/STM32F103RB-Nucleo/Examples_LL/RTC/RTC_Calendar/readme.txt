/**
  @page RTC_Calendar RTC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    Examples_LL/RTC/RTC_Calendar/readme.txt 
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

Configuration of the LL API to set the RTC calendar. The peripheral initialization uses LL unitary service 
functions for optimization purposes (performance and size).

In this example, after start-up, SYSCLK is configured to the max frequency using the PLL with
HSE as clock source.

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

Configure_RTC_Calendar function is then called to initialize the 
time and the date.

LED1 is turned ON when the RTC configuration is done correctly.

The current time and date are updated and displayed on the debugger in aShowTime 
and aShowDate variables (watch or live watch).

- LED1 is toggling : This indicates that the system generates an error.

@par Directory contents 

  - RTC/RTC_Calendar/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - RTC/RTC_Calendar/Inc/main.h                  Header for main.c module
  - RTC/RTC_Calendar/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_Calendar/Src/stm32f1xx_it.c          Interrupt handlers
  - RTC/RTC_Calendar/Src/main.c                  Main program
  - RTC/RTC_Calendar/Src/system_stm32f1xx.c      STM32f1xx system source file


@par Hardware and Software environment

  - This example runs on STM32f103xx devices.
    
  - This example has been tested with NUCLEO-F103RB board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
