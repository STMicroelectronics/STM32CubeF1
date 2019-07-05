/**
  @page CustomHID_Standalone USB Device Custom HID example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Device/CustomHID_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Custom HID example.
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

Use of the USB device application based on the Custom HID Class.

This is a typical example on how to use the STM32F1xx USB Device peripheral, where the STM32 MCU is 
enumerated as a HID compliant device using the native PC Host HID driver to which the STM3210C-EVAL board
is connected.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 72 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL.

@note The application needs to ensure that the SysTick time base is set to 1 millisecond
      to have correct HAL configuration.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

To test the application, user must proceed as follows:
 - Start the "USB HID Demonstrator" PC applet and connect STM32 USB to PC
 - The device should be detected and shown in the USB HID target box
 - Press Graphic View button
 - Select "use SET_FEATURE" or "use SET_REPORT" in order to use SET_REPORT request to send HID Report
   for LED control
 - Use the potentiometer of the STM3210C-EVAL board to transfer the result of the converted voltage
  (via the ADC) the to the PC host (these values are sent to the PC using the endpoint1 IN)   
 - Make sure that following report ID are configured: LED1 ID (0x1), LED2 ID(0x2), LED3 ID(0x3),
   LED4 ID(0x4), BUTTON1_ID(0x6) and Potentiometer_ID(0x7).
 - Select LEDs to switch on/off on the STM3210C-EVAL board: a SET_REPORT request will be sent

@par Directory contents

  - USB_Device/CustomHID_Standalone/Src/main.c                  Main program
  - USB_Device/CustomHID_Standalone/Src/system_stm32f1xx.c      STM32F1xx system clock configuration file
  - USB_Device/CustomHID_Standalone/Src/stm32f1xx_it.c          Interrupt handlers
  - USB_Device/CustomHID_Standalone/Src/stm32f1xx_hal_msp.c     HAL MSP module
  - USB_Device/CustomHID_Standalone/Src/usbd_customhid_if.c     Custom HID interface file  
  - USB_Device/CustomHID_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/CustomHID_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/CustomHID_Standalone/Inc/main.h                  Main program header file
  - USB_Device/CustomHID_Standalone/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - USB_Device/CustomHID_Standalone/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - USB_Device/CustomHID_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/CustomHID_Standalone/Inc/usbd_desc.h             USB device descriptor header file
  - USB_Device/CustomHID_Standalone/Inc/usbd_customhid_if.h     Custom HID interface header file  

@par Hardware and Software environment

  - This example runs on STM32F1xx devices.

  - This example has been tested with a STM3210C-EVAL board embedding
    a STM32F107x device and can be easily tailored to any other supported device
    and development board.

  - STM3210C-EVAL Set-up
    - Connect the STM3210C-EVAL board to the PC through 'USB Type micro A-Male
      to A-Male' cable.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 