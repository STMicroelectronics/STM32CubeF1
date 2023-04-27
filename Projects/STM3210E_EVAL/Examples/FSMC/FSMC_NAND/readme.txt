    /**
  @page FSMC_NAND FSMC NAND memory basic functionalities use example

  @verbatim
  ******************* (C) COPYRIGHT 2016 STMicroelectronics ********************
  * @file    FSMC/FSMC_NAND/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FSMC NAND example.
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

How to configure the FSMC controller to access the NAND memory.

The NAND is NAND512W3A2CN6E.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

The goal of this example is to explain the different steps to configure the FSMC 
and make the NAND device ready for access, without using the BSP layer.          

In this example, the NAND device is configured and initialized explicitly 
following all initialization sequence steps. After initializing the device, user 
can perform erase/read NAND ID/read data/write data operations on it. 
A data buffer is written to the NAND memory, then read back and checked to verify 
its correctness.

If the data is read correctly from NAND, the LED1 is ON, otherwise the LED2 is ON.
In case of HAL initialization issue, LED3 will be ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

 - FSMC/FSMC_NAND/Inc/stm32f1xx_hal_conf.h        HAL Configuration file
 - FSMC/FSMC_NAND/Inc/main.h                      Header for main.c module 
 - FSMC/FSMC_NAND/Inc/stm32f1xx_it.h              Header for stm32f1xx_it.c
 - FSMC/FSMC_NAND/Src/main.c                      Main program
 - FSMC/FSMC_NAND/Src/stm32f1xx_hal_msp.c         HAL MSP module  
 - FSMC/FSMC_NAND/Src/stm32f1xx_it.c              Interrupt handlers
 - FSMC/FSMC_NAND/Src/system_stm32f1xx.c          STM32F1xx system clock configuration file 
    

@par Hardware and Software environment 

  - This example runs on STM32F1xx devices.
    
  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board. 
    @note make sure that the Jumper 7 (JP7) is in position 1<-->2. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
