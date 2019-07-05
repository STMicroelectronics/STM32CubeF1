/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Calendar/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F1xx RTC LL API to configure
  *          Time and Date.
  *          Peripheral initialization done using LL unitary services functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_LL_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Oscillator time-out values */
#define LSI_TIMEOUT_VALUE          ((uint32_t)2)     /* 2 ms */
#define LSE_TIMEOUT_VALUE          ((uint32_t)5000)  /* 5 s */
#define HSE_TIMEOUT_VALUE          ((uint32_t)4)     /* 4 ms */
#define RTC_TIMEOUT_VALUE          ((uint32_t)1000)  /* 1 s */

/* Defines related to Clock configuration */
/* Uncomment to enable the adequate Clock Source */
/* #define RTC_CLOCK_SOURCE_LSI */
#define RTC_CLOCK_SOURCE_LSE
/* #define RTC_CLOCK_SOURCE_HSE_DIV128 */

#ifdef RTC_CLOCK_SOURCE_LSI
/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=40kHz RC */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x9C3F)
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 1Hz with LSEFreq=32768Hz */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7FFF)
#endif

#ifdef RTC_CLOCK_SOURCE_HSE_DIV128
/* ck_apre=(HSEFreq/128)/(ASYNC prediv + 1) = 1Hz with HSEFreq=8MHz */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0xF423)
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Time Structure definition */
struct time_t
{
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
};
struct time_t RTC_TimeStruct;

struct date_t
{
  uint8_t month;
  uint8_t day;
  uint8_t year;
};
struct date_t RTC_DateStruct;

uint8_t EndOfMonth[12]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/* Buffers used for displaying Time and Date */
uint32_t timeCounter = 0;
uint8_t dateUpdate = 0;
uint8_t timeUpdate = 0;
uint8_t aShowTime[13] = {0};
uint8_t aShowDate[13] = {0};


#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_RTC_Clock(void);
void     Configure_RTC(void);
void     Configure_RTC_Calendar(void);
void     RTC_DATE_structUpdate(void);
void     RTC_TIME_structUpadate(void);
void     RTC_DATE_Config(uint8_t ,uint8_t , uint8_t);
void     RTC_TIME_Config(uint8_t ,uint8_t , uint8_t);
uint32_t WaitForSynchro_RTC(void);
void     Show_RTC_Calendar(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Blinking(uint32_t Period);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();
  
  /* Initialize LED1 */
  LED_Init();
  
  /*##-1-Configure the RTC peripheral #######################################*/
  Configure_RTC_Clock();
  
  /*##-2-Configure the RTC peripheral #######################################*/
  Configure_RTC();
  
  /* Configure RTC Calendar */
  Configure_RTC_Calendar();
  
  /* Turn-on LED1 to indicate that calendar has been well configured */
  LED_On();
  
  /* Infinite loop */
  while (1)
  {
    /*##-3- Display the updated Time and Date ################################*/
    Show_RTC_Calendar();
  }
}

/**
  * @brief  Configure RTC clock.
  * @param  None
  * @retval None
  */
void Configure_RTC_Clock(void)
{
  /*##-1- Enables the PWR Clock and Enables access to the backup domain #######*/
  /* To change the source clock of the RTC feature (LSE, LSI,HSE_DIV128), you have to:
     - Enable the power clock
     - Enable write access to configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain
     - Configure the needed RTC clock source */

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_BKP);
  
  LL_PWR_EnableBkUpAccess();

  /*##-2- Configure LSE/LSI as RTC clock source ###############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
  /* Enable LSE only if disabled.*/
  if (LL_RCC_LSE_IsReady() != 1)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSE_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_LSE_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag()) 
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }  
#endif /* USE_TIMEOUT */
    }
  }
  if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
  }
  
#elif defined(RTC_CLOCK_SOURCE_LSI)
  /* Enable LSI */
  LL_RCC_LSI_Enable();
#if (USE_TIMEOUT == 1)
  Timeout = LSI_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RCC_LSI_IsReady() != 1)
  {
#if (USE_TIMEOUT == 1)
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      Timeout --;
    }
    if (Timeout == 0)
    {
      /* LSI activation error */
      LED_Blinking(LED_BLINK_ERROR);
    }  
#endif /* USE_TIMEOUT */
  }
  /* Reset backup domain only if LSI is not yet selected as RTC clock source */
  if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  }
#elif defined(RTC_CLOCK_SOURCE_HSE_DIV128)
  /* Enable HSE only if disabled.*/
  if (LL_RCC_HSE_IsReady() == 0)
  {
    LL_RCC_HSE_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = HSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_HSE_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag()) 
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* HSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }  
#endif /* USE_TIMEOUT */
    }
  }
  /* Reset backup domain only if LSI is not yet selected as RTC clock source */
  if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_HSE_DIV128)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_HSE_DIV128);
  }
#else
#error "configure clock for RTC"
#endif
}

/**
  * @brief  Configure RTC.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_RTC(void)
{
  /*##-1- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */ 
  LL_RCC_EnableRTC();
  
  /*##-2- Disable RTC registers write protection ##############################*/
  LL_RTC_DisableWriteProtection(RTC);

  /*##-3- Enter in initialization mode #######################################*/
  if (LL_RTC_EnterInitMode(RTC) != RTC_ERROR_NONE)   
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /*##-4- Configure RTC ######################################################*/
  /* Configure RTC prescaler */
  /* Set Asynch Prediv (value according to source clock) */
  LL_RTC_SetAsynchPrescaler(RTC, RTC_ASYNCH_PREDIV);

  /* RTC_Alarm Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);
  
  /*##-5- Configure the NVIC for RTC Alarm ###############################*/
  NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);
  
  /*##-6- Exit of initialization mode #######################################*/
  LL_RTC_ExitInitMode(RTC);

  /*##-7- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void Configure_RTC_Calendar(void)
{
  /*##-1- Disable RTC registers write protection ############################*/
  LL_RTC_DisableWriteProtection(RTC);

  /*##-2- Enter in initialization mode ######################################*/
  if (LL_RTC_EnterInitMode(RTC) != RTC_ERROR_NONE)   
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /*##-3- Configure the Date ################################################*/
  /* Note: __LL_RTC_CONVERT_BIN2BCD helper macro can be used if user wants to*/
  /*       provide directly the decimal value:                               */
  /*       LL_RTC_DATE_Config(RTC, ,                                         */
  /*                          __LL_RTC_CONVERT_BIN2BCD(31), (...))           */
  /* Set Date: 31 December 2017 */
  RTC_DATE_Config(31, 12, 17);
  
  /*##-4- Configure the Time ################################################*/
  /* Set Time: 29:59:55 */
  RTC_TIME_Config(23, 59, 55);

  /* Enable Second Interrupt */
  LL_RTC_EnableIT_SEC(RTC);

  /*##-5- Exit of initialization mode #######################################*/
  if (LL_RTC_ExitInitMode(RTC) != RTC_ERROR_NONE)   
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /*##-6- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);
}

/**
  * @brief  Configure RTC_DATE Structure
  * @param  fDate:  Date
  *         fMonth: Month
  *         fYear:  Year
  * @retval None
  */
void RTC_DATE_Config(uint8_t fDate , uint8_t fMonth , uint8_t fYear)
{
  RTC_DateStruct.day   = fDate;
  RTC_DateStruct.month = fMonth;
  RTC_DateStruct.year  = fYear;
}

/**
  * @brief  Configure RTC_TIME Structure
  * @param  fHour: Hour
  *         fMin:  Minut
  *         fSec:  seconds
  * @retval None
  */
void RTC_TIME_Config(uint8_t fHour, uint8_t fMin, uint8_t fSec)
{
  RTC_TimeStruct.hour = fHour;
  RTC_TimeStruct.min  = fMin;
  RTC_TimeStruct.sec  = fSec;

  LL_RTC_TIME_Set(RTC,((RTC_TimeStruct.hour * 3600) +
                       (RTC_TimeStruct.min * 60) +
                        RTC_TimeStruct.sec));
}

/**
  * @brief  Update RTC_Date Structure
  * @param  None
  * @retval None
  */
void RTC_DATE_structUpdate(void)
{
  /* Update DATE when Time is 23:59:59 */
  if ((timeCounter == 0x0001517FU) & (dateUpdate != 0U))
  {
    dateUpdate = 0;
    if(RTC_DateStruct.day == EndOfMonth[RTC_DateStruct.month - 1U])
    {
      RTC_DateStruct.day = 1U;
      if(RTC_DateStruct.month == 12U)
      {
        RTC_DateStruct.month = 1U;
        RTC_DateStruct.year += 1U;
      }
      else
      {
        RTC_DateStruct.month += 1U;
      }
    }
    else
    {
      RTC_DateStruct.day = RTC_DateStruct.day + 1U;
    }
  }
}

/**
  * @brief  Configure RTC_TIME Structure
  * @param  None
  * @retval None
  */
void RTC_TIME_structUpadate(void)
{
  if(timeUpdate != 0)
  {
    timeUpdate = 0;
    RTC_TimeStruct.hour = (timeCounter/3600);
    RTC_TimeStruct.min  = (timeCounter % 3600) / 60;
    RTC_TimeStruct.sec  = (timeCounter % 3600) % 60;
  }  
}

/**
  * @brief  Wait until the RTC Time and Date registers (RTC_TR and RTC_DR) are
  *         synchronized with RTC APB clock.
  * @param  None
  * @retval RTC_ERROR_NONE if no error (RTC_ERROR_TIMEOUT will occur if RTC is 
  *         not synchronized)
  */
uint32_t WaitForSynchro_RTC(void)
{
  /* Clear RSF flag */
  LL_RTC_ClearFlag_RS(RTC);

#if (USE_TIMEOUT == 1)
    Timeout = RTC_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */

  /* Wait the registers to be synchronised */
  while(LL_RTC_IsActiveFlag_RS(RTC) != 1)
  {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
    {
        Timeout --;
    }
      if (Timeout == 0)
    {
      return RTC_ERROR_TIMEOUT;
    }  
#endif /* USE_TIMEOUT */
  }
  return RTC_ERROR_NONE;
}

/**
  * @brief  Display the current time and date.
  * @param  None
  * @retval None
  */
void Show_RTC_Calendar(void)
{
  RTC_TIME_structUpadate();
  RTC_DATE_structUpdate();
  
  /* Note: need to convert in decimal value in using __LL_RTC_CONVERT_BCD2BIN helper macro */
  /* Display time Format : hh:mm:ss */
  sprintf((char*)aShowTime,"%.2d:%.2d:%.2d", RTC_TimeStruct.hour, 
          RTC_TimeStruct.min, 
          RTC_TimeStruct.sec);
  /* Display date Format : mm-dd-yy */
  sprintf((char*)aShowDate,"%.2d-%.2d-%.2d", RTC_DateStruct.day,
          RTC_DateStruct.month,
          (2000 + RTC_DateStruct.year));
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);  
    LL_mDelay(Period);
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
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}
/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Calendar callback
  * @param  None
  * @retval None
  */
void Calendar_Callback(void)
{
  timeCounter = LL_RTC_TIME_Get(RTC);
  timeUpdate = 1;
  /* Reset RTC Counter when Time is 23:59:59 */
  if (timeCounter == 0x0001517FU)
  {
    dateUpdate = 1;
    LL_RTC_DisableWriteProtection(RTC);
    if (LL_RTC_EnterInitMode(RTC) != RTC_ERROR_NONE)   
    {
      /* Initialization Error */
      LED_Blinking(LED_BLINK_ERROR);
    }
    LL_RTC_TIME_Set(RTC,0x0U);
    /* Wait until last write operation on RTC registers has finished */
    WaitForSynchro_RTC();
    if (LL_RTC_ExitInitMode(RTC) != RTC_ERROR_NONE)   
    {
      /* Initialization Error */
      LED_Blinking(LED_BLINK_ERROR);
    }
    LL_RTC_EnableWriteProtection(RTC);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
