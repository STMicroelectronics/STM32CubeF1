/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Alarm/Src/main.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use STM32F1xx RTC LL API to configure
  *          an alarm.
  *          Peripheral initialization done using LL unitary services functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
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

/** @addtogroup STM32F1xx_LL_Examples
  * @{
  */

/** @addtogroup RTC_Alarm
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

/* Define used to indicate date/time updated */
#define RTC_BKP_DATE_TIME_UPDTATED ((uint32_t)0x32F1)
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
struct time_t RTC_AlarmStruct;

struct date_t
{
  uint8_t month;
  uint8_t day;
  uint8_t year;
};
struct date_t RTC_DateStruct;
uint8_t EndOfMonth[12]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint32_t TimeCounter = 0;

/* Buffers used for displaying Time and Date */
uint8_t aShowTime[50] = {0};
uint8_t aShowDate[50] = {0};

#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_RTC(void);
void     Configure_RTC_Alarm(void);
void     RTC_DATE_structUpdate(void);
void     RTC_TIME_StructUpadate(void);
void     Alarm_Callback(void);
void     RTC_DATE_Config(uint8_t ,uint8_t , uint8_t);
void     RTC_TIME_Config(uint8_t ,uint8_t , uint8_t);
void     RTC_ALARM_Config(uint8_t ,uint8_t , uint8_t);
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
  
  /*##-Configure the RTC peripheral #######################################*/
  Configure_RTC();
  
  /* Configure RTC Alarm */
  Configure_RTC_Alarm();
  
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
void Configure_RTC(void)
{
  /*##-1- Enables the PWR Clock and Enables access to the backup domain #######*/
  /* To change the source clock of the RTC feature (LSE, LSI), you have to:
     - Enable the power clock
     - Enable write access to configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain
     - Configure the needed RTC clock source */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  LL_PWR_EnableBkUpAccess();

  /*##-2- Configure LSE/LSI as RTC clock source ###############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
  /* Enable LSE only if disabled.*/
  if (LL_RCC_LSE_IsReady() == 0)
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
  
  /*##-3- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */ 
  LL_RCC_EnableRTC();
  
  /*##-4- Disable RTC registers write protection ##############################*/
  LL_RTC_DisableWriteProtection(RTC);

  /*##-5- Enter in initialization mode #######################################*/
  if (LL_RTC_EnterInitMode(RTC) != RTC_ERROR_NONE)   
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /*##-6- Configure RTC ######################################################*/
  /* Configure RTC prescaler */
  /* Set Asynch Prediv (value according to source clock) */
  LL_RTC_SetAsynchPrescaler(RTC, RTC_ASYNCH_PREDIV);
  
  /*##-7- Exit of initialization mode #######################################*/
  LL_RTC_ExitInitMode(RTC);

  /*##-8- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);
}

/**
  * @brief  Configure the current time and date.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @param  None
  * @retval None
  */
void Configure_RTC_Alarm(void)
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
  /* Set Date: 29 March 2017 */
  RTC_DATE_Config(29, 03, 17);
  
  /*##-4- Configure the Time ################################################*/
  /* Set Time: 11:59:55 */
  RTC_TIME_Config(11, 59, 55);
  
  /*##-5- Configure the RTC Alarm peripheral #################################*/
  /* Set Alarm to 12:00:25 
     RTC Alarm Generation: Alarm on Hours, Minutes and Seconds (ignore date/weekday)*/
  RTC_ALARM_Config(12, 00, 25);

  /* Clear the Alarm interrupt pending bit */
  LL_RTC_ClearFlag_ALR(RTC);
  
  /* Enable IT Alarm */
  LL_RTC_EnableIT_ALR(RTC);

  /* RTC Alarm Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);
  
  /*##-6- Configure the NVIC for RTC Alarm ###############################*/
  NVIC_SetPriority(RTC_Alarm_IRQn, 0x0F);
  NVIC_EnableIRQ(RTC_Alarm_IRQn);

  /*##-7- Exit of initialization mode #######################################*/
  if (LL_RTC_ExitInitMode(RTC) != RTC_ERROR_NONE)   
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_ERROR);
  }
   
  /*##-8- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);
}

/**
  * @brief  Configure RTC_DATE Structure
  * @param  fDate:  date
  *         fMonth: month
  *         fYear:  year
  * @retval None
  */
void RTC_DATE_Config(uint8_t fDate , uint8_t fMonth , uint8_t fYear)
{
  RTC_DateStruct.day   = fDate;
  RTC_DateStruct.month = fMonth;
  RTC_DateStruct.year  = fYear;
}
/**
  * @brief  Update RTC_Date Structure
  * @param  None
  * @retval None
  */
void RTC_DATE_structUpdate(void)
{
  /* Update DATE when Time is 23:59:59 */
  if (TimeCounter == 0x0001517FU)
  {
    if(RTC_DateStruct.day == EndOfMonth[RTC_DateStruct.month -1])
    {
      RTC_DateStruct.day = 1U;
      RTC_DateStruct.month += 1U;
    }
    else
    {
      RTC_DateStruct.day = RTC_DateStruct.day + 0x1U;
    }
  }
}
/**
  * @brief  Update RTC_TIME Structure
  * @param  None
  * @retval None
  */
void RTC_TIME_StructUpadate(void)
{
  TimeCounter = LL_RTC_TIME_Get(RTC);
  RTC_TimeStruct.hour = (TimeCounter/3600);
  RTC_TimeStruct.min  = (TimeCounter % 3600) / 60;
  RTC_TimeStruct.sec  = (TimeCounter % 3600) % 60;
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
  * @brief  Configure RTC_Alarm Structure
  * @param  fHour: Hour
  *         fMin:  Minut
  *         fSec:  seconds
  * @retval None
  */
void RTC_ALARM_Config(uint8_t fHour, uint8_t fMin, uint8_t fSec)
{
  RTC_AlarmStruct.hour = fHour;
  RTC_AlarmStruct.min  = fMin;
  RTC_AlarmStruct.sec  = fSec;

  LL_RTC_ALARM_Set(RTC,((RTC_AlarmStruct.hour * 3600) +
                        (RTC_AlarmStruct.min * 60) +
                        RTC_AlarmStruct.sec));
}

/**
  * @brief  Wait until the RTC registers are
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
  RTC_TIME_StructUpadate();
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
  * @brief  Alarm callback
  * @param  None
  * @retval None
  */
void Alarm_Callback(void)
{
  /* Turn LED1 on: Alarm generation */
  LED_On();
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
