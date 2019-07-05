/**
  @page USART_SyncCommunication_FullDuplex_DMA USART Synchronous Transmitter/Receiver example (DMA mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/USART/USART_SyncCommunication_FullDuplex_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USART_SyncCommunication_FullDuplex_DMA example.
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

Configuration of GPIO, USART, DMA and SPI peripherals to transmit 
bytes between a USART and an SPI (in slave mode) in DMA mode. This example is based on the STM32F1xx USART LL API. The peripheral 
initialization uses LL unitary service functions for optimization purposes (performance and size).

This example works with only one STM32F103RB-Nucleo.

USART1 Peripheral is acts as Master.
SPI1 Peripheral is configured in Slave mode.
GPIO associated to User push-button is linked with EXTI. 

Example execution:

LED2 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD start a Full-Duplex communication through DMA.
On USART1 side, Clock will be generated on SCK line, Transmission done on TX, reception on RX.
On SPI1 side, reception is done through the MOSI Line, transmission on MISO line.

At end of transmission, both received buffers are compared to expected ones.
In case of both transfers successfully completed, LED2 is turned on.
In case of errors, LED2 is blinking Slowly (1s).

@par Directory contents 

  - USART/USART_SyncCommunication_FullDuplex_DMA/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - USART/USART_SyncCommunication_FullDuplex_DMA/Inc/main.h                  Header for main.c module
  - USART/USART_SyncCommunication_FullDuplex_DMA/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_SyncCommunication_FullDuplex_DMA/Src/stm32f1xx_it.c          Interrupt handlers
  - USART/USART_SyncCommunication_FullDuplex_DMA/Src/main.c                  Main program
  - USART/USART_SyncCommunication_FullDuplex_DMA/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment

  - This example runs on STM32F103xB devices.

  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

  - STM32F103RB-Nucleo Set-up
    - Connect USART1 SCK PA.08 to Slave SPI1 SCK PB.03
    - Connect USART1 TX PA.09 to Slave SPI1 MOSI PB.05
    - Connect USART1 RX PA.10 to Slave SPI1 MISO PB.04

Board connector:
  PA.08 :   connected to pin 23 of CN10 connector
  PA.09 :   connected to pin 21 of CN10 connector
  PA.10 :   connected to pin 33 of CN10 connector
  PB.03 :   connected to pin 31 of CN10 connector
  PB.05 :   connected to pin 29 of CN10 connector
  PB.04 :   connected to pin 27 of CN10 connector

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Launch the program. Press on User push button on board to initiate data transfers.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
