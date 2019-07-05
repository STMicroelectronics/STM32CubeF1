/**
  @page HID_Standalone USB Device Humain Interface (HID) example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Device/HID_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB HID example.
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

Use of the USB device application based on the Human Interface (HID).

This is a typical example on how to use the STM32F105x/107x USB OTG Device peripheral where the STM32 MCU is
enumerated as a HID device using the native PC Host HID driver to which the STM3210C-EVAL
board is connected, in order to emulate the Mouse directions using Joystick buttons mounted on the 
STM3210C-EVAL board.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 72 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL.

This example supports remote wakeup (which is the ability of a USB device to bring a suspended bus back
to the active condition), and the Key button is used as the remote wakeup source. 

By default, in Windows powered PC the Power Management feature of USB mouse devices is turned off.
This setting is different from classic PS/2 computer functionality. Therefore, to enable the Wake from 
standby option, user must manually turn on the Power Management feature for the USB mouse.

To manually enable the wake from standby option for the USB mouse, proceed as follows: 
 - Start "Device Manager",
 - Select "Mice and other pointing devices",
 - Select the "HID-compliant mouse" device (make sure that PID &VID are equal to 0x5710 & 0x0483 respectively) 
 - Right click and select "Properties", 
 - Select "Power Management" tab,
 - Finally click to select "Allow this device to wake the computer" check box.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".

@par Directory contents

  - USB_Device/HID_Standalone/Src/main.c                  Main program
  - USB_Device/HID_Standalone/Src/system_stm32f1xx.c      STM32F1xx system clock configuration file
  - USB_Device/HID_Standalone/Src/stm32f1xx_it.c          Interrupt handlers
  - USB_Device/HID_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_Standalone/Inc/main.h                  Main program header file
  - USB_Device/HID_Standalone/Inc/stm32f1xx_it.h          Interrupt handlers header file
  - USB_Device/HID_Standalone/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_Standalone/Inc/usbd_desc.h             USB device HID descriptor header file  

	
@par Hardware and Software environment

  - This example runs on STM32F107xx/STM32F105xx devices.
    
  - This example has been tested with STMicroelectronics STM3210C-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM3210C-EVAL RevC Set-up
    - Connect the STM3210C-EVAL board to the PC through 'USB Type micro A-Male 
      to A-Male' cable
              

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
  