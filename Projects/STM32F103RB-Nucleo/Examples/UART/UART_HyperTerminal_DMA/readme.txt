/**
  @page UART_HyperTerminal_DMA UART Hyperterminal DMA Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    UART/UART_HyperTerminal_DMA/readme.txt 
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

UART transmission (transmit/receive) in DMA mode 
between a board and an HyperTerminal PC application.

Board: STM32F103RB-Nucleo
Tx Pin: PA.09 (Pin 21 in CN10)
Rx Pin: PA.10 (Pin 33 in CN10)
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


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz for STM32F1xx Devices.

The UART peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit()function which core is implementing
the configuration of the needed UART resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change UART configuration.

The UART/Hyperterminal communication is then initiated.
The HAL_UART_Receive_DMA() and the HAL_UART_Transmit_DMA() functions allow respectively 
the reception of Data from Hyperterminal and the transmission of a predefined data 
buffer.

The Asynchronous communication aspect of the UART is clearly highlighted as the  
data buffers transmission/reception to/from Hyperterminal are done simultaneously.

For this example the TxBuffer is predefined and the RxBuffer size is limited to 
10 data by the mean of the RXBUFFERSIZE define in the main.c file.

In a first step the received data will be stored in the RxBuffer buffer and the 
TxBuffer buffer content will be displayed in the Hyperterminal interface.
In a second step the received data in the RxBuffer buffer will be sent back to 
Hyperterminal and displayed.
The end of this two steps are monitored through the HAL_UART_GetState() function
result.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED2 turns ON if transmission/reception is complete and OK.
 - LED2 turns OFF when there is an error in transmission/reception process (HAL_UART_ErrorCallback is called).
 - LED2 toggles when there another error is detected.  

The UART is configured as follows:
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

  - UART/UART_HyperTerminal_DMA/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - UART/UART_HyperTerminal_DMA/Inc/stm32f1xx_it.h          DMA interrupt handlers header file
  - UART/UART_HyperTerminal_DMA/Inc/main.h                  Header for main.c module  
  - UART/UART_HyperTerminal_DMA/Src/stm32f1xx_it.c          DMA interrupt handlers
  - UART/UART_HyperTerminal_DMA/Src/main.c                  Main program
  - UART/UART_HyperTerminal_DMA/Src/stm32f1xx_hal_msp.c     HAL MSP module
  - UART/UART_HyperTerminal_DMA/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.    

  - SSTM32F103RB_Nucleo Set-up
    - Connect USART1 TX (PA9 - D8 on CN5) to RX pin of PC serial port (or USB to UART adapter)
      and USART1 RX (PA10 - D2 on CN9) to TX pin of PC serial port.

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
