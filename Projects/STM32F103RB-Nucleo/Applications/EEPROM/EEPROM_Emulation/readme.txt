    /**
  @page FSMC_NOR_CodeExecute FSMC NOR memory basic functionalities use example

  @verbatim
  ******************* (C) COPYRIGHT 2016 STMicroelectronics ********************
  * @file    EEPROM_Emulation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EEPROM_Emulation application.
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

@par Application Description 
Please refer to AN2594 for further details regarding this application.

@note Take care, to work well, memory shall be erase before using this application.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 


 - EEPROM_Emulation/Inc/stm32f1xx_hal_conf.h        HAL Configuration file
 - EEPROM_Emulation/Inc/eeprom.h                      Header for main.c module 
 - EEPROM_Emulation/Inc/stm32f1xx_it.h              Header for stm32f1xx_it.c
 - EEPROM_Emulation/Src/main.c                      Main program
 - EEPROM_Emulation/Src/eeprom.c                      EEPROM program
 - EEPROM_Emulation/Src/stm32f1xx_it.c              Interrupt handlers
 - EEPROM_Emulation/Src/system_stm32f1xx.c          STM32F1xx system clock configuration file 
    

@par Hardware and Software environment 

  - This example runs on STM32F1xx devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
