/**
  @page CDC_Standalone USB Host Communication Class (CDC) example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Host/CDC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Host CDC example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim
  
@par Example Description

Use of the USB host application based on the CDC class.

This is a typical example on how to use the STM32F10x USB OTG Host peripheral to operate with an USB 
CDC device application based on the two CDC transfer directions with a dynamic serial configuration: 

 - Transmission: 
   The user can select in the Host board, using the menu, a file among the ones
   available on the SD and send it to the Device board. The content of the file could be visualized 
   through the Hyperterminal (the link configuration is imposed initially by the device and could be 
   checked using the configuration menu). Data to be transmitted is stored in CDC_TX_Buffer buffer.

 - Reception: 
   The data entered by the user using the Hyperterminal in ASCII format are transferred by the device
   board to the Host board and displayed on its LCD screen. The CDC_RX_Buffer is the buffer used for 
   data reception.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 72 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL.

When the application is started, the connected USB CDC device is detected in CDC mode and gets 
initialized. The STM32 MCU behaves as a CDC Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial no and product name information and displays it on the LCD screen.

A menu is displayed and the user can select any operation from the menu using the Joystick buttons:
 - "Send Data" operation starts the Data Transmission.
 - "Receive Data" operation starts the Data Reception.
 - "Configuration" operation defines the desired Host CDC configuration (Baudrate,Parity, DataBit and StopBit)
   The baudrate comes with a default value of 115,2 kbps (BAUDRATE = 115200).
 - "Re-Enumerate" operation performs a new Enumeration of the device.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720  
"STM32Cube USB Host library".

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.


@par Directory contents

  - USB_Host/CDC_Standalone/Src/main.c                  Main program
  - USB_Host/CDC_Standalone/Src/system_stm32f1xx.c      STM32F10x system clock configuration file
  - USB_Host/CDC_Standalone/Src/stm32f1xx_it.c          Interrupt handlers
  - USB_Host/CDC_Standalone/Src/menu.c                  CDC State Machine
  - USB_Host/CDC_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/CDC_Standalone/Src/explorer.c              Explore the uSD content
  - USB_Host/CDC_Standalone/Src/cdc_configuration.c     CDC settings State Machine
  - USB_Host/CDC_Standalone/Src/cdc_receive.c           CDC Receive State Machine
  - USB_Host/CDC_Standalone/Src/cdc_send.c              CDC Send State Machine
  - USB_Host/CDC_Standalone/Inc/main.h                  Main program header file
  - USB_Host/CDC_Standalone/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - USB_Host/CDC_Standalone/Inc/lcd_log_conf.h          LCD log configuration file
  - USB_Host/CDC_Standalone/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - USB_Host/CDC_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/CDC_Standalone/Inc/ffconf.h                FatFs Module Configuration file
  

@par Hardware and Software environment

  - This example runs on STM32F107xx devices.
    
  - This example has been tested with STMicroelectronics STM3210C-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM3210C-EVAL RevC Set-up
    - Insert a microSD card into the STM3210C-EVAL uSD slot (CN16)
    - Plug the CDC device into the STM3210C-EVAL board through 'USB micro A-Male 
      to B-Male' cable to the connector CN2.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
