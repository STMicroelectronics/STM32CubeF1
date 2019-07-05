/**
  @page I2C_OneBoard_Communication_IT_Init I2C example (IT Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_OneBoard_Communication_IT_Init/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_Communication_IT_Init I2C example (IT Mode).
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
by an I2C master device. Both devices operate in interrupt mode. The peripheral is initialized 
with LL initialization function to demonstrate LL init usage.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using only one STM32F103RB-Nucleo.

I2C2 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C1 Peripheral is configured in Master mode with EXTI (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI. 

LED2 blinks quickly to wait for user-button press. 

Example execution:
Press the User push-button to initiate a Start condition by Master.
This action will generate an I2C start condition on the I2C bus.
When the I2C start condition is sent on I2C1, a SB interrupt occurs.
I2C1 IRQ Handler routine is then sending the Slave address with a read bit condition.
When address Slave match code is received on I2C2, the Slave acknowledge the address.
When this acknowledge is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then preparing the enable of data acknowledge and interrupts for next statement.

When address Slave match code is received on I2C2, an ADDR interrupt occurs.
I2C2 IRQ Handler routine is then checking the slave direction at Write (mean read direction for Master).
This will allow Slave to enter in transmitter mode and then send a byte when TXE interrupt occurs.
When byte is received on I2C1, an RXNE interrupt occurs.
When DR register is read, Master disable all I2C1 interrupts and generate a Non-acknowledge then a STOP condition
to inform the Slave that the transfer is finished.
The Non-acknowledge condition generate a AF interrupt in Slave side treated in the I2C2 IRQ handler routine by a clear of NACK flag
and disable all I2C2 interrupts.

LED2 is On if data is well received.

In case of errors, LED2 is blinking.


@par Directory contents 

  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_Communication_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/stm32f1xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/main.c                  Main program
  - I2C/I2C_OneBoard_Communication_IT_Init/Src/system_stm32f1xx.c      STM32F1xx system source file

@par Hardware and Software environment

  - This example runs on STM32F103xB devices.
    
  - This example has been tested with STM32F103RB-Nucleo board and can be
    easily tailored to any other supported device and development board.

  - STM32F103RB-Nucleo Set-up
    - Connect GPIOs connected to I2C2 SCL/SDA (PB.10 and PB.11)
    to respectively SCL and SDA pins of I2C1 (PB.6 and PB.7).
      - I2C2_SCL  PB.10 (CN10, pin 25) : connected to I2C1_SCL PB.6 (CN10, pin 17) 
      - I2C2_SDA  PB.11 (CN10, pin 18) : connected to I2C1_SDA PB.7 (CN7, pin 21)
    - Check that a pull-up resistor (4.7K) is connected on one I2C SDA pin
    - Check that a pull-up resistor (4.7K) is connected on one I2C SCL pin
	  
  - Launch the program. Press User push-button to initiate a read request by Master 
      then Slave send a byte.
  - User can easily change the number of data to transfer by modifying
      the initialization of variables ubNbDataToTransmit and ubNbDataToReceive.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
