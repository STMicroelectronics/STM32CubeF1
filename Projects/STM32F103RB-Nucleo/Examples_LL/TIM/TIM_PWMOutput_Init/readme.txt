/**
  @page TIM_PWMOutput_Init TIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/TIM/TIM_PWMOutput_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_PWMOutput_Init example.
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

Use of a timer peripheral to generate a 
PWM output signal and update the PWM duty cycle. This example is based on the 
STM32F1xx TIM LL API. The peripheral initialization uses 
LL initialization function to demonstrate LL init.

In this example TIM2 input clock TIM2CLK is set to APB1 clock (PCLK1),
since APB1 pre-scaler is equal to 1.
   TIM2CLK = PCLK1                  
   PCLK1 = HCLK                     
   => TIM2CLK = SystemCoreClock (72 MHz)

To set the TIM2 counter clock frequency to 10 KHz, the pre-scaler (PSC) is
calculated as follows:
PSC = (TIM2CLK / TIM2 counter clock) - 1
PSC = (SystemCoreClock /10 KHz) - 1

SystemCoreClock is set to 72 MHz for STM32F1xx Devices.

Auto-reload (ARR) is calculated to get a time base period of 10 ms,
meaning a time base frequency of 100 Hz.
ARR = (TIM2 counter clock / time base frequency) - 1
ARR = (TIM2 counter clock / 100) - 1

Initially, the capture/compare register (CCR1) of the output channel is set to
half the auto-reload value meaning a initial duty cycle of 50%.
Generally speaking this duty cycle is calculated as follows:
Duty cycle = (CCR1 / ARR) * 100

The timer output channel is mapped on the pin PA.00. You can connect it to
an oscilliscope to monitor the waveform.

User push-button can be used to change the duty cycle from 0% up to 100% by
steps of 10%. Duty cycle is periodically measured. It can be observed through
the debugger by watching the variable uwMeasuredDutyCycle.

Initially the output channel is configured in output compare toggle mode.
@note If the dutycycle is 0% so there is no PWM signal. When pushing 
successively the user button, the dutycycle value keep increasing.

@par Directory contents 

  - TIM/TIM_PWMOutput_Init/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput_Init/Inc/main.h                  Header for main.c module
  - TIM/TIM_PWMOutput_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_PWMOutput_Init/Src/stm32f1xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput_Init/Src/main.c                  Main program
  - TIM/TIM_PWMOutput_Init/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

  - Connect PA.00 (TIM2 Channel1) to an oscilloscope.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
