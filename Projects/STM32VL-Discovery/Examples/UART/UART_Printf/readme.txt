/**
  @page UART_Printf UART Printf example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    UART/UART_Printf/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART Hyperterminal example.
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

Re-routing of the C library printf function to the UART.
The UART outputs a message on the HyperTerminal.

Board: STM32VL-Discovery
Tx Pin: PA.09
Rx Pin: PA.10
   _________________________ 
  |           ______________|                       _______________
  |          |USART         |                      | HyperTerminal |
  |          |              |                      |               |
  |          |           TX |______________________|RX             |
  |          |              |                      |               |
  |          |              |     RS232 Cable      |               |             
  |          |              |                      |               |
  |          |           RX |______________________|TX             |          
  |          |              |                      |               |           
  |          |______________|                      |_______________|          
  |                         |                       
  |                         |                    
  |                         |                      
  |                         |                      
  |_STM32_Board_____________|                      

The USART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.


@par Directory contents 

  - UART/UART_Printf/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - UART/UART_Printf/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - UART/UART_Printf/Inc/main.h                  Header for main.c module  
  - UART/UART_Printf/Src/stm32f1xx_it.c          Interrupt handlers
  - UART/UART_Printf/Src/main.c                  Main program
  - UART/UART_Printf/Src/stm32f1xx_hal_msp.c     HAL MSP module
  - UART/UART_Printf/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F100xB devices.
    
  - This example has been tested with STM32VL-Discovery board and can be
    easily tailored to any other supported device and development board.    

  - STM32_VL_DISCOVERY Set-up
    - If you want to display data on the HyperTerminal, please connect USART1 TX (PA9)
      to RX pin of PC serial port (or USB to UART adapter). 
      USART1 RX (PA10) could be connected similarly to TX pin of PC serial port.

  - Hyperterminal configuration:
    - Word Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - flow control: None 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
