    /**
  @page FSMC_NOR_CodeExecute FSMC NOR memory basic functionalities use example

  @verbatim
  ******************* (C) COPYRIGHT 2016 STMicroelectronics ********************
  * @file    FSMC/FSMC_NOR_CodeExecute/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FSMC NOR example.
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

How to build an application to be loaded into the NOR memory mounted on board
and then execute it from internal Flash memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

In order to make the program work, you must do the following:

1. Program the NOR memory with the example provided in the "binary" directory
@note You can use ST-LINK v3.4.6 tool and select NOR Flash memory in the external loader and
      then flash the binary file at @0x64000000.

2. Program the internal Flash with the code that will jump to the NOR memory to execute 
   the loaded example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

 - FSMC/NOR_CodeExecute/binary: Contains a set of sources files that build the 
                                application to be loaded into the NOR memory 
                                mounted on STM3210E-EVAL RevD board.
 - FSMC/FSMC_NOR_CodeExecute/Inc/stm32f1xx_hal_conf.h        HAL Configuration file
 - FSMC/FSMC_NOR_CodeExecute/Inc/main.h                      Header for main.c module 
 - FSMC/FSMC_NOR_CodeExecute/Inc/stm32f1xx_it.h              Header for stm32f1xx_it.c
 - FSMC/FSMC_NOR_CodeExecute/Src/main.c                      Main program
 - FSMC/FSMC_NOR_CodeExecute/Src/stm32f1xx_hal_msp.c         HAL MSP module  
 - FSMC/FSMC_NOR_CodeExecute/Src/stm32f1xx_it.c              Interrupt handlers
 - FSMC/FSMC_NOR_CodeExecute/Src/system_stm32f1xx.c          STM32F1xx system clock configuration file 
    

@par Hardware and Software environment 

  - This example runs on STM32F1xx devices.
    
  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
