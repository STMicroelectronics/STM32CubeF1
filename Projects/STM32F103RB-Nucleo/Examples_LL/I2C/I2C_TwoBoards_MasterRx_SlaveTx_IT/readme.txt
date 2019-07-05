/**
  @page I2C_TwoBoards_MasterRx_SlaveTx_IT I2C example (IT Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_TwoBoards_MasterRx_SlaveTx_IT I2C example (IT Mode).
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
  
How to handle the reception of one data byte from an I2C slave device 
by an I2C master device. Both devices operate in interrupt mode. The peripheral 
is initialized with LL unitary service functions to optimize for performance 
and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using two STM32F103RB-Nucleo.

Boards: STM32F103RB-Nucleo (embeds a STM32F103RBT6 device)
SCL Pin: PB.6 (CN10, pin 17)
SDA Pin: PB.7 (CN7, pin 21)
A pull-up resistor must be connected on one I2C SDA pin
A pull-up resistor must be connected on one I2C SCL pin


   ______BOARD SLAVE_____    _____VDD _____     _____BOARD MASTER_____
  |        ______________|    |         |      |______________        |
  |       |I2C1          |    |         |      |          I2C1|       |
  |       |              |    R  (4.7K) R      |              |       |
  |       |          SCL |____|_________|______| SCL          |       |
  |       |              |              |      |              |       |
  |       |              |              |      |              |       |
  |       |          SDA |______________|______| SDA          |       |
  |       |______________|                     |______________|       |
  |         __           |                     |             __       |
  |        |__|          |                     |            |__|      |
  |        USER       GND|_____________________|GND         USER      |
  |___STM32F1xx_Nucleo___|                     |___STM32F1xx_Nucleo___|

The project is splitted in two parts the Master Board and the Slave Board
- Master Board
  I2C1 Peripheral is configured in Master mode with EXTI (Fast Mode @400kHz).
And GPIO associated to User push-button is linked with EXTI. 
- Slave Board
  I2C1 Peripheral is configured in Slave mode with EXTI (Fast Mode @400kHz, Own address 7-bit enabled).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:
- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

LED2 blinks quickly on both BOARD to wait for user-button press. 

Example execution:
To perform this example, two steps are necessary to prevent a wrong start/stop detection due to the initialization of the oposite GPIO board.

So at first step, press the User push-button on BOARD SLAVE to activate Slave peripheral and then prepare acknowledge for Slave address reception.
At second step, press the User push-button on BOARD MASTER to activate Slave peripheral and then to initiate a read request by Master.
This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C1 of BOARD SLAVE, an ADDR interrupt occurs.
I2C1 Slave IRQ Handler routine is then checking Address Match Code and direction Write (mean read direction for Master).
This will allow Slave to enter in transmitter mode and then send a byte when TXE interrupt occurs.
When byte is received on I2C1 of BOARD MASTER, an RXNE interrupt occurs.
When DR register is read, Master generate a NACK and a STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C1 Slave IRQ handler routine by a clear of NACK flag.
This NACK event initiate the end of transmission on Slave side

LED2 turn OFF on BOARD SLAVE when pressing the User push-button.

LED2 is On :
- Slave side if transfer sequence is completed.
- Master side if data is well received.

In case of errors, LED2 is blinking.


@par Directory contents 

  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Inc/main.h                  Header for main.c module
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Src/stm32f1xx_it.c          Interrupt handlers
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

  - STM32F103RB-Nucleo Set-up
    - Connect GPIOs connected to Board Slave I2C1 SCL/SDA (PB.6 and PB.7)
    to respectively Board Master SCL and SDA pins of I2C1 (PB.6 and PB.7).
      - I2C1_SCL  PB.6 (CN10, pin 17) : connected to I2C1_SCL PB.6 (CN10, pin 17) 
      - I2C1_SDA  PB.7 (CN7, pin 21) : connected to I2C1_SDA PB.7 (CN7, pin 21)
    - Connect Master board GND to Slave Board GND
    - Check that a pull-up resistor (4.7K) is connected on one I2C SDA pin
    - Check that a pull-up resistor (4.7K) is connected on one I2C SCL pin
  - Launch the program. Press User push-button on Slave BOARD then on Master BOARD
      to initiate a read request by Master then Slave send a byte.

  - User can easily change the number of data to transfer by modifying the initialization
      of SLAVE_BYTE_TO_SEND.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory (The user can choose between Master 
   and Slave target through "#define SLAVE_BOARD" in the "main.h" file)
    o Comment "#define SLAVE_BOARD" and load the project in Master Board
    o Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
