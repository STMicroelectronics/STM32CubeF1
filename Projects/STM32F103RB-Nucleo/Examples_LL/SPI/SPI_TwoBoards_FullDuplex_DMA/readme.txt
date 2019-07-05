/**
  @page SPI_TwoBoards_FullDuplex_DMA SPI example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/SPI/SPI_TwoBoards_FullDuplex_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_TwoBoards_FullDuplex_DMA example.
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

Data buffer transmission and receptionvia SPI using DMA mode. This example is 
based on the STM32F1xx SPI LL API. The peripheral initialization uses 
LL unitary service functions for optimization purposes (performance and size).

The communication is done with 2 boards through SPI.
   _________________________                        _________________________
  |       ___________ ______|                      |__________________       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |        CLK(PB13) |______________________|(PB13)CLK         |      |
  |      |                  |                      |                  |      |
  |      |        MISO(PB14)|______________________|(PB14)MISO        |      |
  |      |                  |                      |                  |      |
  |      |        MOSI(PB15)|______________________|(PB15)MOSI        |      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F1xx Master _______|                      |_STM32F1xx Slave ________|

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using DMA Transfer mode through the STM32F1xx SPI LL API.

The project is splitted in two parts the Master Board and the Slave Board:

- Master Board
  SPI2 Peripheral is configured in Master mode.
  DMA1_Channel4 and DMA1_Channel5 configured to transfer Data via SPI peripheral
  GPIO associated to User push-button is linked with EXTI. 
  
- Slave Board
  SPI2 Peripheral is configured in Slave mode.
  DMA1_Channel4 and DMA1_Channel5 configured to transfer Data via SPI peripheral

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

Example execution:
On BOARD MASTER, LED2 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD MASTER start a Full-Duplex communication through DMA.
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line) 
will be done at the same time. 
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED2 is On if data is well received.

In case of errors, LED2 is blinking Slowly (1s).

@note You need to perform a reset on Master board, then perform it on Slave board
      to have the correct behaviour of this example.


@par Directory contents 

  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/stm32f1xx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_DMA/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment

  - This example runs on STM32F103xB devices.

  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

  - STM32F103RB-Nucleo Set-up
    - Connect Master board PB13 to Slave Board PB13 (connected to pin 30 of CN10 connector)
    - Connect Master board PB14 to Slave Board PB14 (connected to pin 28 of CN10 connector)
    - Connect Master board PB15 to Slave Board PB15 (connected to pin 26 of CN10 connector)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
