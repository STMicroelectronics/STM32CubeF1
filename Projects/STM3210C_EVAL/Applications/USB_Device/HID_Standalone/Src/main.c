/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/Src/main.c 
  * @author  MCD Application Team
  * @brief   USB device HID application main file.
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
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"


/** @addtogroup STM32F1xx_HAL_Validation
  * @{
  */

/** @addtogroup STANDARD_CHECK
  * @{
  */

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
USBD_HandleTypeDef USBD_Device;
extern PCD_HandleTypeDef hpcd;
__IO uint8_t JOYInitState = 0;

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. 
   */
  HAL_Init();

  /* Initialize LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Initialize Joystick */
  if (BSP_JOY_Init(JOY_MODE_GPIO) == HAL_OK)
  {
    JOYInitState = 1;
  }

  /* Configure Key push-button for remote wakeup */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);

  /* Start Device Process */
  USBD_Start(&USBD_Device);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 25000000
  *            HSE PREDIV1                    = 5
  *            HSE PREDIV2                    = 5
  *            PLL2MUL                        = 8
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = { 0 };
  RCC_OscInitTypeDef oscinitstruct = { 0 };
  RCC_PeriphCLKInitTypeDef rccperiphclkinit = { 0 };

  /* Configure PLLs ------------------------------------------------------ */
  /* PLL2 configuration: PLL2CLK = (HSE / HSEPrediv2Value) * PLL2MUL = (25 / 5) 
   * 8 = 40 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8
   * MHz */
  /* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
  oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
  oscinitstruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;

  oscinitstruct.PLL.PLLState = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL2.PLL2State = RCC_PLL2_ON;
  oscinitstruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
  oscinitstruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
  HAL_RCC_OscConfig(&oscinitstruct);

  /* USB clock selection */
  rccperiphclkinit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  rccperiphclkinit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV3;
  HAL_RCCEx_PeriphCLKConfig(&rccperiphclkinit);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
   * clocks dividers */
  clkinitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2);
}

/**
  * @brief  Toggle LEDs to shows user input state.
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
  static uint32_t ticks;

  if (ticks++ == 100)
  {
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
    ticks = 0;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
   * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file, 
   * line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
