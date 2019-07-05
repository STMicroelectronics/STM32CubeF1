/**
  @page USART_Communication_Rx_IT_Continuous USART Receiver example (IT Mode, Continous reception)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/USART/USART_Communication_Rx_IT_Continuous/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USART_Communication_Rx_IT_Continuous example.
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

Configuration of GPIO and USART peripherals to continuously receive 
characters from an HyperTerminal (PC) in Asynchronous mode using an interrupt. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
GPIO associated to User push-button is linked with EXTI. 

Example execution:
After startup from reset and system configuration, LED2 is blinking quickly.
On press on push button, USART RX Not Empty interrupt is enabled.
When character is received on USART Rx line, a RXNE interrupt occurs. 
Received data is read from USART DR register and stored in user buffer.
A double buffer is available for received data, allowing continuous reception.
User could process data received in Buffer A, while buffer B is used for reception. 
When buffer B is full, it could be handled by user, while buffer A becomes active buffer for next reception, and so on.
Each time a reception buffer is full, user data process callback is called.
Data processing consists in echoing data buffer content on PC Com port, and making LED2 toggle.
In case of errors, LED2 is blinking.

Virtual Com port feature of STLINK could be used for UART communication between board and PC.

@par Directory contents 

  - USART/USART_Communication_Rx_IT_Continuous/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Rx_IT_Continuous/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Rx_IT_Continuous/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Rx_IT_Continuous/Src/stm32f1xx_it.c          Interrupt handlers
  - USART/USART_Communication_Rx_IT_Continuous/Src/main.c                  Main program
  - USART/USART_Communication_Rx_IT_Continuous/Src/system_stm32f1xx.c      STM32F1xx system source file


@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

  - STM32F103RB-Nucleo Set-up
    In order to select use of Virtual Com port feature of STLINK for connection between STM32F103RB-Nucleo and PC,
    User has to set USE_VCP_CONNECTION define to 1 in main.h file.
    If so, please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled 
    on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)

    If VCP is not used (USE_VCP_CONNECTION define set to 0 in main.h file), Connect USART1 TX/RX 
    to respectively RX and TX pins of PC UART (could be done through a USB to UART adapter) :
    - Connect STM32 MCU board USART1 TX pin (GPIO PA.09 connected to pin 21 of CN10 connector)
      to PC COM port RX signal
    - Connect STM32 MCU board USART1 RX pin (GPIO PA.10 connected to pin 33 of CN10 connector)
      to PC COM port TX signal
    - Connect STM32 MCU board GND to PC COM port GND signal

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).
    Port should be selected according to USE_VCP_CONNECTION value.

  - Launch the program. Enter characters on PC communication SW side.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
