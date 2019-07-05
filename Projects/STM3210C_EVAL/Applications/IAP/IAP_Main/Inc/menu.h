/**
  ******************************************************************************
  * @file    IAP_Main/Inc/menu.h 
  * @author  MCD Application Team
  * @brief   This file provides all the headers of the menu functions.
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENU_H
#define __MENU_H

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "ymodem.h"

/* Imported variables --------------------------------------------------------*/
extern uint8_t aFileName[FILE_NAME_LENGTH];

/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Main_Menu(void);

#endif  /* __MENU_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
