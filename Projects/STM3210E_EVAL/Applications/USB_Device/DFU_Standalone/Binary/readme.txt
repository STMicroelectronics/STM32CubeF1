/**
  @page Binary Description of the binary template
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Device/DFU_Standalone/Binary/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the binary template example.
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

This directory contains a binary template (in DFU format) to be loaded into Flash memory using Device 
Firmware Upgrade example. This file was converted to the DFU format using the "DFU File Manager Tool"
included in the "DfuSe" PC software install.
For more details on how to convert a .bin file to DFU format please refer to the UM0412 user manual 
"Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension" available from the
STMicroelectronics microcontroller website www.st.com.
   
This binary is a simple LED chaser. The 4 LEDs lights one by one for a period of 100 ms and the cycle 
repeats giving the running light appearance.
The system Timer (Systick) is used for to generate the delay.
The offset address of this binary is 0x08005000 which matches the definition in DFU application
"USBD_DFU_APP_DEFAULT_ADD".


@par Hardware and Software environment

  - This example runs on STM32F103xG devices.
    
  - This example has been tested with STMicroelectronics STM3210E-EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.  

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
