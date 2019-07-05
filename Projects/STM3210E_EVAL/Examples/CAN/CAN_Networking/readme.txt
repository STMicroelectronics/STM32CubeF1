/**
  @page CAN_Networking CAN Networking example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CAN/CAN_Networking/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the CAN Networking example.
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

How to configure the CAN peripheral to send and receive CAN frames in 
normal mode.

The sent frames are used to control LEDs by pressing KEY Push Button.

The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

This program behaves as follows:
  - After reset, all LEDs are OFF on the N eval boards
  - By Pressing on KEY Push Button : LED1 turns ON and all other LEDs are OFF, on the N
    eval boards connected to the bus. 
  - Press on KEY Push Button again to send CAN Frame to command LEDn+1 ON, all other LEDs 
    are OFF on the N eval boards.
    
@note This example is tested with a bus of 3 units. The same program example is 
      loaded in all units to send and receive frames.
@note Any unit in the CAN bus may play the role of sender (by pressing KEY Push Button)
      or receiver.

  The CAN is configured as follows:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
    - ID Filter  = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte
    - Data: Led number that should be turned ON

@par Directory contents

  - CAN/CAN_Networking/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - CAN/CAN_Networking/Inc/stm32f1xx_it.h          DMA interrupt handlers header file
  - CAN/CAN_Networking/Inc/main.h                  Header for main.c module
  - CAN/CAN_Networking/Src/stm32f1xx_it.c          DMA interrupt handlers
  - CAN/CAN_Networking/Src/main.c                  Main program
  - CAN/CAN_Networking/Src/stm32f1xx_hal_msp.c     HAL MSP file
  - CAN/CAN_Networking/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment

  - This example runs on STM32F103xG devices.
  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board.

  - STM3210E-EVAL RevD Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PF.06, PF.07, PF.08
      and PF.09 pins
    - Use KEY Push Button connected to PG.08
    - Connect a male/male CAN cable between at least 2 EVAL CAN connectors (CN4)
    - You have to configure the jumpers as follows:

  @verbatim

       +-------------------------------+
       |   Jumper    |       CAN       |
       +-------------------------------+
       |   JP8       |       1-2       |
       |   JP6       |      fitted     |
       +-------------------------------+
  @endverbatim

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

