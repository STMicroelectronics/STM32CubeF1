    /**
  @page FSMC_SRAM FSMC SRAM basic functionalities use example

  @verbatim
  ******************* (C) COPYRIGHT 2016 STMicroelectronics ********************
  * @file    FSMC/FSMC_SRAM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FSMC SRAM example.
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

How to configure the FSMC controller to access the SRAM memory.

The SRAM is IS61WV51216BLL.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

The goal of this example is to explain the different steps to configure the FMC 
and make the SRAM device ready for access, without using the MSP layer.          

In this example, the SRAM device is configured and initialized explicitly 
following all initialization sequence steps. After initializing the device, user 
can perform read/write operations on it. A data buffer is written to the SRAM 
memory, then read back and checked to verify its correctness.

The user can chose his own configuration by commenting/uncommenting the defines for  
undesired/desired configurations , for example, to change the data memory access width, 
uncomment the define for the configuration "SRAM_MEMORY_WIDTH" in "main.h".

If the data is read correctly from SRAM, the LED1 is ON, otherwise the LED2 is ON.
In case of HAL initialization issue, LED3 will be ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

 - FSMC/FSMC_SRAM/Inc/stm32f1xx_hal_conf.h        HAL Configuration file
 - FSMC/FSMC_SRAM/Inc/main.h                      Header for main.c module 
 - FSMC/FSMC_SRAM/Inc/stm32f1xx_it.h              Header for stm32f1xx_it.c
 - FSMC/FSMC_SRAM/Src/main.c                      Main program
 - FSMC/FSMC_SRAM/Src/stm32f1xx_hal_msp.c         HAL MSP module  
 - FSMC/FSMC_SRAM/Src/stm32f1xx_it.c              Interrupt handlers
 - FSMC/FSMC_SRAM/Src/system_stm32f1xx.c          STM32F1xx system clock configuration file 
    

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
