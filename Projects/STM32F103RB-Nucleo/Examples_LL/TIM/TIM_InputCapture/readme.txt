/**
  @page TIM_InputCapture TIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/TIM/TIM_InputCapture/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_InputCapture example.
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

Use of the TIM peripheral to measure a periodic signal frequency 
provided either by an external signal generator or by 
another timer instance. This example is based on the STM32F1xx TIM 
LL API. The peripheral initialization uses LL unitary service functions 
for optimization purposes (performance and size).
  
TIM3_CH1 is configured in input capture mode. The TIM3CLK frequency is set to 
its maximum value (Prescaler is 1) in order to get the best possible resolution.
So the TIM3 counter clock is SystemCoreClock.

SystemCoreClock is set to 72 MHz for STM32F1xx Devices.

The "uwMeasuredFrequency" variable contains the measured signal frequency.
It is calculated within the capture/compare 1 interrupt service routine.

The minimum frequency value to measure is TIM3 counter clock / TIMx_CCR1 MAX
                                              = 72 MHz / 65535

Due to TIM3_IRQHandler processing time (around 3.50us), the maximum
frequency value to measure is around 300 kHz.

TIM2_CH1 is configured to generate a PWM signal.  User push-button can be used to
change the frequency of this signal from 2 kHz up to 20 kHz by steps of 2 kHz.
It is then possible to run this example without a signal generator by connecting
TIM2_CH1 to TIM3_CH1.

@par Directory contents 

  - TIM/TIM_InputCapture/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - TIM/TIM_InputCapture/Inc/main.h                  Header for main.c module
  - TIM/TIM_InputCapture/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_InputCapture/Src/stm32f1xx_it.c          Interrupt handlers
  - TIM/TIM_InputCapture/Src/main.c                  Main program
  - TIM/TIM_InputCapture/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

  - STM32F103RB-Nucleo Set-up
    - When no signal generator is used TIM2_CH1 can be connected to TIM3_CH1:
      - TIM3_CH1  PA.06: connected to pin 5 of CN5 connector  
      - TIM2_CH1  PA.00: connected to pin 1 of CN8 connector 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
