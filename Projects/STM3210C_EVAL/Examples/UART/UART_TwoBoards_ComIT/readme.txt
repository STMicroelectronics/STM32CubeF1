/**
  @page UART_TwoBoards_ComIT UART Two Boards Communication IT example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    UART/UART_TwoBoards_ComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART Two Boards Communication IT example.
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

UART transmission (transmit/receive) in Interrupt mode 
between two boards.

Board: STM3210C-EVAL RevC (embeds a STM32F107xC device)
Tx Pin: PD.05
Rx Pin: PD.06
   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |USART         |                     |         USART|          |
  |          |              |                     |              |          |
  |          |           TX |_____________________| RX           |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |           RX |_____________________| TX           |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                         |                     |                         |
  |_STM32_Board 1___________|                     |_STM32_Board 2___________|



Two identical boards are connected as shown on the picture above.
Board 1: transmitting then receiving board
Board 2: receiving then transmitting board
The user presses the Key push-button on board 1.
Then, board 1 sends in interrupt mode a message to board 2 that sends it back to 
board 1 in interrupt mode as well.
Finally, board 1 and 2 compare the received message to that sent.
If the messages are the same, the test passes.


WARNING: as both boards do not behave the same way, "TRANSMITTER_BOARD" compilation
switch is defined in /Src/main.c and must be enabled
at compilation time before loading the executable in the board that first transmits
then receives.
The receiving then transmitting board needs to be loaded with an executable
software obtained with TRANSMITTER_BOARD disabled. 

STM3210C-EVAL RevC board LEDs are used to monitor the transfer status:
- While board 1 is waiting for the user to press the Key push-button, its LED1 is
  blinking rapidly (100 ms period).
- While board 2 is waiting for the message from board 1, its LED1 is emitting
  a couple of flashes every half-second.
- When the test passes, LED3 and LED1 on both boards are both turned on:
 * LED1 is ON when the transmission process is complete.
 * LED3 is ON when the reception process is complete.
 If LED3 and LED1 are not both turned on, the test has failed. 
- If there is an initialization or transfer error, LED4 is turned on.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.


The UART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - UART/UART_TwoBoards_ComIT/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - UART/UART_TwoBoards_ComIT/Inc/stm32f1xx_it.h          IT interrupt handlers header file
  - UART/UART_TwoBoards_ComIT/Inc/main.h                  Header for main.c module  
  - UART/UART_TwoBoards_ComIT/Src/stm32f1xx_it.c          IT interrupt handlers
  - UART/UART_TwoBoards_ComIT/Src/main.c                  Main program
  - UART/UART_TwoBoards_ComIT/Src/stm32f1xx_hal_msp.c     HAL MSP module
  - UART/UART_TwoBoards_ComIT/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment 

  - This example runs on STM32F107xC devices.    
  - This example has been tested with two STM3210C-EVAL RevC boards embedding
    a STM32F107xC device and can be easily tailored to any other supported device 
    and development board.

  - STM3210C-EVAL RevC set-up
    - Connect a wire between 1st board PD.05 pin (Uart Tx) and 2nd board PD.06 pin (Uart Rx)
    - Connect a wire between 1st board PD.06 pin (Uart Rx) and 2nd board PD.05 pin (Uart Tx)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
