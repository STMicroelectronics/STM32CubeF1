/**
  @page Templates  Description of the Templates example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Templates/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Templates example.
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

This projects provides a reference template that can be used to build any firmware application.

This projects is configured for STM32F100xB devices using STM32CubeF1 HAL and running on 
STM32VL-Discovery board from STMicroelectronics. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - Templates/Src/main.c                 Main program
  - Templates/Src/system_stm32f1xx.c     STM32F1xx system clock configuration file
  - Templates/Src/stm32f1xx_it.c         Interrupt handlers 
  - Templates/Src/stm32f1xx_hal_msp.c    HAL MSP module
  - Templates/Inc/main.h                 Main program header file  
  - Templates/Inc/stm32f1xx_hal_conf.h   HAL Configuration file
  - Templates/Inc/stm32f1xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32F100xB devices.
    
  - This example has been tested with STMicroelectronics STM32VL-Discovery
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
