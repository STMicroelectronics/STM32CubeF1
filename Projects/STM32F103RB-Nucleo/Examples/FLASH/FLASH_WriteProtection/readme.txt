/**
  @page FLASH_WriteProtection FLASH write protection
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    FLASH/FLASH_WriteProtection/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH write protection example.
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

How to configure and use the FLASH HAL API to enable and disable the write 
protection of the internal Flash memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 64 MHz.

  - If WRITE_PROTECTION_ENABLE is selected, the write protection will be enabled
    for the defined sectors: FLASH_PAGE_TO_BE_PROTECTED.
    To load the new option byte values, a system Reset is necessary, for this, the
    function HAL_FLASH_OB_Launch() is used.
    
  - If WRITE_PROTECTION_DISABLE is selected, the write protection will be enabled
    for the defined sectors: FLASH_PAGE_TO_BE_PROTECTED.
    To load the new option byte values, a system Reset is necessary, for this, the
    function HAL_FLASH_OB_Launch() is used.
    
  - Then an erase operation is done by filling the erase init structure giving the 
    starting erase page and the number of pages to erase. At this stage, all these 
    pages will be erased one by one separately.

    @note: if problem occurs on a page, erase will be stopped and faulty page will 
    be returned to user (through variable 'PageError').

    Once this operation is finished, word programming operation will be performed 
    in the Flash memory. The written data is then read back and checked.

STM32F103RB-Nucleo board's LED can be used to monitor the transfer status:
 - LED2 is ON when there are no errors detected after programming
    => should be the case when WRITE_PROTECTION_DISABLE flag is enabled
       (or when both switches are disabled)
 - LED2 is blinking with a 1s period when there are errors detected after programming
    => should be the case when WRITE_PROTECTION_ENABLE flag is enabled
 - LED2 is blinking (100ms on, 2s off) when there is an issue during erase, OB erase, program or OB program procedure

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FLASH/FLASH_WriteProtection/Inc/stm32f1xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_WriteProtection/Inc/stm32f1xx_it.h              Header for stm32f1xx_it.c
  - FLASH/FLASH_WriteProtection/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_WriteProtection/Src/stm32f1xx_it.c              Interrupt handlers
  - FLASH/FLASH_WriteProtection/Src/main.c                      Main program
  - FLASH/FLASH_WriteProtection/Src/system_stm32f1xx.c          STM32F1xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
