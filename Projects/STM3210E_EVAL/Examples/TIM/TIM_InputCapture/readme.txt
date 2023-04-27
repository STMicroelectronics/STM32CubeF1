/**
  @page TIM_InputCapture Input Capture example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_InputCapture/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_InputCapture example.
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

Use of the TIM peripheral to measure an external signal frequency.

  The TIM2CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 0,
  so the TIM2 counter clock is SystemCoreClock (Hz).
  SystemCoreClock is set to 72 MHz for STM32F103xG Devices.

  TIM2 is configured in Input Capture Mode: the external signal is connected to 
  TIM2 Channel2 used as input pin.
  To measure the frequency we use the TIM2 CC2 interrupt request, so in the 
  TIM2_IRQHandler routine, the frequency of the external signal is computed.

  The "uwFrequency" variable contains the external signal frequency:
  uwFrequency = TIM2 counter clock / uwDiffCapture (Hz),
  where "uwDiffCapture" is the difference between two consecutive TIM2 captures.


  The minimum frequency value to measure is TIM2 counter clock / CCR MAX
                                              = 72 MHz / 65535

  Due to TIM2_IRQHandler processing time (around 3.50us), the maximum
  frequency value to measure is around 300kHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_InputCapture/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - TIM/TIM_InputCapture/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - TIM/TIM_InputCapture/Inc/main.h                  Header for main.c module  
  - TIM/TIM_InputCapture/Src/stm32f1xx_it.c          Interrupt handlers
  - TIM/TIM_InputCapture/Src/main.c                  Main program
  - TIM/TIM_InputCapture/Src/stm32f1xx_hal_msp.c     HAL MSP file
  - TIM/TIM_InputCapture/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xG devices.
  - In this example, the clock is set to 72 MHz.
    
  - This example has been tested with STMicroelectronics STM3210E-EVAL RevD
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM3210E-EVAL RevD Set-up
    - Connect the external signal to measure to the TIM2 CH2 pin (PA.01).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
