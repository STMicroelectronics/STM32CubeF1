/**
  ******************************************************************************
  * @file    RTC/RTC_LowPower_STANDBY/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F1xx RTC HAL API to enter
  *          and exit the Standby mode using RTC.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup RTC_LowPower_STANDBY
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void RTC_AlarmConfig(void);
static uint8_t            RTC_IsLeapYear(uint16_t nYear);
static void               RTC_DateUpdate(RTC_DateTypeDef* pDate, uint32_t DayElapsed);
static uint8_t            RTC_WeekDayNum(uint32_t nYear, uint8_t nMonth, uint8_t nDay);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  RTC_DateTypeDef  sdatestructure = {0};

  /* STM32F1xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 24 MHz */
  SystemClock_Config();

  /* Configure LED4 */
  BSP_LED_Init(LED4);

  /*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Asynch Prediv  = Automatic calculation of prediv for 1 sec timebase
  */
  RtcHandle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;

  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Check and handle if the system was resumed from StandBy mode */ 
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    /* Clear Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 

    /* Configure LED_GREEN & LED_BLUE */
    BSP_LED_Init(LED_GREEN);
    BSP_LED_Init(LED_BLUE);

    /* Get the date available in RTC. */
    if(HAL_RTC_GetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler(); 
    } 

    /* Check that 1 day elapsed after wake-up */
    /* As HAL_RTC_Init initializes date to 1st of January 2000, check if date is
       equal to 2nd of January 2000 */
    if ((sdatestructure.Date != 2) ||(sdatestructure.Year != 0x00) || \
        (sdatestructure.Month != RTC_MONTH_JANUARY))
    {
      /* Wrong calculation day. Set LED_BLUE*/
      BSP_LED_On(LED_BLUE);
    }
    else
    {
      /* 1 day elapsed. Set LED_GREEN */
      BSP_LED_On(LED_GREEN);

      /* Read date saved in backup registers before entering in standby mode */
      sdatestructure.Month = HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1);
      sdatestructure.Date  = HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR2);
      sdatestructure.Year  = HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR3);

      /* Update date in RTC with */
      RTC_DateUpdate(&sdatestructure, 1);
      
      /* Check that date is well set to 1st of November 2014*/
      if ((sdatestructure.Date == 1) && (sdatestructure.Month == 11) \
        && (sdatestructure.Year == 14))
      {
        /* Date update OK. Set LED_BLUE */
        BSP_LED_On(LED_BLUE);
      }
    }
  }
  else
  {
    /*##-2- Configure Alarm ####################################################*/
    /* Configure RTC Alarm */
    RTC_AlarmConfig();
    
    /*## 3- Save Date in backup registers before entering in standby mode ######*/
    if(HAL_RTC_GetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler(); 
    }
    
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, sdatestructure.Month);
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR2, sdatestructure.Date);
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR3, sdatestructure.Year);
    
    /* Clear all related wakeup flags */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    
    /* Enter the Standby mode */
    HAL_PWR_EnterSTANDBYMode();
  }
  
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 24000000
  *            HCLK(Hz)                       = 24000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 2
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV2;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_AlarmConfig(void)
{
  RTC_DateTypeDef  sdatestructure_set = {0};
  RTC_TimeTypeDef  stimestructure = {0};
  RTC_AlarmTypeDef salarmstructure = {{0}, 0};
 
  /*##-1- Configure the Date #################################################*/
  /* Set Date: October 31th 2014 */
  sdatestructure_set.Year = 0x14;
  sdatestructure_set.Month = RTC_MONTH_OCTOBER;
  sdatestructure_set.Date = 0x31;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure_set,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 23:59:55 */
  stimestructure.Hours = 0x23;
  stimestructure.Minutes = 0x59;
  stimestructure.Seconds = 0x55;
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }  

  /*##-3- Configure the RTC Alarm peripheral #################################*/
  /* Set Alarm to 00:00:10 
     RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
  salarmstructure.Alarm = RTC_ALARM_A;
  salarmstructure.AlarmTime.Hours = 0x00;
  salarmstructure.AlarmTime.Minutes = 0x00;
  salarmstructure.AlarmTime.Seconds = 0x10;
  
  if(HAL_RTC_SetAlarm_IT(&RtcHandle,&salarmstructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

/**
  * @brief  Updates date when time is 23:59:59.
  * @param  pDate  pointer to a RTC_DateTypeDef structure.
  * @param  DayElapsed: Number of days elapsed from last date update
  * @retval None
  */
static void RTC_DateUpdate(RTC_DateTypeDef* pDate, uint32_t DayElapsed)
{
  uint32_t year = 0, month = 0, day = 0;
  uint32_t loop = 0;

  /* Get the current year*/
  year  = pDate->Year;

  /* Get the current month and day */
  month = pDate->Month;
  day   = pDate->Date;

  for (loop = 0; loop < DayElapsed; loop++)
  {
    if((month == 1) || (month == 3) || (month == 5) || (month == 7) || \
       (month == 8) || (month == 10) || (month == 12))
    {
      if(day < 31)
      {
        day++;
      }
      /* Date structure member: day = 31 */
      else
      {
        if(month != 12)
        {
          month++;
          day = 1;
        }
        /* Date structure member: day = 31 & month =12 */
        else
        {
          month = 1;
          day = 1;
          year++;
        }
      }
    }
    else if((month == 4) || (month == 6) || (month == 9) || (month == 11))
    {
      if(day < 30)
      {
        day++;
      }
      /* Date structure member: day = 30 */
      else
      {
        month++;
        day = 1;
      }
    }
    else if(month == 2)
    {
      if(day < 28)
      {
        day++;
      }
      else if(day == 28)
      {
        /* Leap year */
        if(RTC_IsLeapYear(year))
        {
          day++;
        }
        else
        {
          month++;
          day = 1;
        }
      }
      else if(day == 29)
      {
        month++;
        day = 1;
      }
    }
  }

  /* Update year */
  pDate->Year = year;

  /* Update day and month */
  pDate->Month = month;
  pDate->Date = day;

  /* Update day of the week */
  pDate->WeekDay = RTC_WeekDayNum(year, month, day);
}

/**
  * @brief  Check whether the passed year is Leap or not.
  * @param  nYear  year to check
  * @retval 1: leap year
  *         0: not leap year
  */
static uint8_t RTC_IsLeapYear(uint16_t nYear)
{
  if((nYear % 4) != 0) 
  {
    return 0;
  }
  
  if((nYear % 100) != 0) 
  {
    return 1;
  }
  
  if((nYear % 400) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Determines the week number, the day number and the week day number.
  * @param  nYear   year to check
  * @param  nMonth  Month to check
  * @param  nDay    Day to check
  * @note   Day is calculated with hypothesis that year > 2000
  * @retval Value which can take one of the following parameters:
  *         @arg RTC_WEEKDAY_MONDAY
  *         @arg RTC_WEEKDAY_TUESDAY
  *         @arg RTC_WEEKDAY_WEDNESDAY
  *         @arg RTC_WEEKDAY_THURSDAY
  *         @arg RTC_WEEKDAY_FRIDAY
  *         @arg RTC_WEEKDAY_SATURDAY
  *         @arg RTC_WEEKDAY_SUNDAY
  */
static uint8_t RTC_WeekDayNum(uint32_t nYear, uint8_t nMonth, uint8_t nDay)
{
  uint32_t year = 0, weekday = 0;

  year = 2000 + nYear;
  
  if(nMonth < 3)
  {
    /*D = { [(23 x month)/9] + day + 4 + year + [(year-1)/4] - [(year-1)/100] + [(year-1)/400] } mod 7*/
    weekday = (((23 * nMonth)/9) + nDay + 4 + year + ((year-1)/4) - ((year-1)/100) + ((year-1)/400)) % 7;
  }
  else
  {
    /*D = { [(23 x month)/9] + day + 4 + year + [year/4] - [year/100] + [year/400] - 2 } mod 7*/
    weekday = (((23 * nMonth)/9) + nDay + 4 + year + (year/4) - (year/100) + (year/400) - 2 ) % 7; 
  }

  return (uint8_t)weekday;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    /* Toggle the LED4 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(100);
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

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
