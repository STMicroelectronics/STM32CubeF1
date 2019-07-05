/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to perform conversions in multimode dual-mode
  *          interleaved.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup ADC_DualModeInterleaved
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Application general parameters */
#define VDD_APPLI                      ((uint32_t) 3300)    /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_8BITS                    ((uint32_t)  255)    /* Max digital value for a full range of 8 bits */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max digital value for a full range of 12 bits */

/* ADC parameters */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)  256)    /* Size of array containing ADC converted values */

#if defined(ADC_TRIGGER_FROM_TIMER)
/* Timer for ADC trigger parameters */
#define TIMER_FREQUENCY                ((uint32_t) 1000)    /* Timer frequency (unit: Hz). With a timer 16 bits and time base freq min 1Hz, range is min=1Hz, max=32kHz. */
#define TIMER_FREQUENCY_RANGE_MIN      ((uint32_t)    1)    /* Timer minimum frequency used to calculate frequency range (unit: Hz). With a timer 16 bits, maximum frequency will be 32000 times this value. */
#define TIMER_PRESCALER_MAX_VALUE      (0xFFFF-1)           /* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */
#endif /* ADC_TRIGGER_FROM_TIMER */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
/* Timer for DAC trigger parameters */
#define TIMER_FOR_WAVEFORM_TEST_FREQUENCY                ((uint32_t)  500)    /* Timer for DAC trigger to send each sample of the waveform: Timer frequency (unit: Hz). With a timer 16 bits and time base freq min 1Hz, range is min=1Hz, max=32kHz. */
#define TIMER_FOR_WAVEFORM_TEST_FREQUENCY_RANGE_MIN      ((uint32_t)    1)    /* Timer for DAC trigger to send each sample of the waveform: Timer minimum frequency used to calculate frequency range (unit: Hz). With timer 16 bits, maximum frequency possible will be 32000 times this value. */
#define TIMER_FOR_WAVEFORM_TEST_PRESCALER_MAX_VALUE      (0xFFFF-1)           /* Timer prescaler maximum value (0xFFFF for a timer 16 bits) */

/* Waveform voltage generation for test parameters */
#define WAVEFORM_TEST_SAMPLES_NUMBER                     ((uint32_t)    5)   /* Size of array of DAC waveform samples */
#define WAVEFORM_TEST_PERIOD_US                          ((WAVEFORM_TEST_SAMPLES_NUMBER * 1000000) / TIMER_FOR_WAVEFORM_TEST_FREQUENCY_HZ)   /* Waveform voltage generation for test period (unit: us) */
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


/* Private macro -------------------------------------------------------------*/

/**
  * @brief  Computation of ADC master conversion result 
  *         from ADC dual mode conversion result (ADC master and ADC slave
  *         results concatenated on data register of ADC master).
  * @param  DATA: ADC dual mode conversion result
  * @retval None
  */
#define COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(DATA)                 \
  ((DATA) & 0x0000FFFF)

/**
  * @brief  Computation of ADC slave conversion result 
  *         from ADC dual mode conversion result (ADC master and ADC slave
  *         results concatenated on data register of ADC master).
  * @param  DATA: ADC dual mode conversion result
  * @retval None
  */
#define COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(DATA)                  \
  ((DATA) >> 16)

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
/**
  * @brief  Computation of digital value on range 8 bits from voltage value
  *         (unit: mV).
  *         Calculation depends on settings: digital resolution and power
  *         supply of analog voltage Vdda.
  * @param DATA: Voltage value (unit: mV)
  * @retval None
  */
#define COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(DATA)                             \
  ((DATA) * RANGE_8BITS / VDD_APPLI)
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


/* Private variables ---------------------------------------------------------*/
/* Peripherals handler declaration */
ADC_HandleTypeDef    AdcHandle_master;
ADC_HandleTypeDef    AdcHandle_slave;
TIM_HandleTypeDef    TimHandle;

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
DAC_HandleTypeDef    DacHandle;  /* DAC used for waveform voltage generation for test */
TIM_HandleTypeDef    TimForWaveformTestHandle;  /* TIM used for waveform voltage generation for test */
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/* Variable containing ADC conversions results */
__IO uint32_t   aADCDualConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];    /* ADC dual mode interleaved conversion results (ADC master and ADC slave results concatenated on data register 32 bits of ADC master). */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];       /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
__IO uint16_t   aADCyConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];       /* For the purpose of this example, dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
uint8_t         ubDCDualConversionComplete = RESET;                         /* Set into ADC conversion complete callback */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
/* Waveform sent by DAC channel. With timer frequency 1kHz and size of 5 samples: waveform 200Hz */
const uint8_t Waveform_8bits[WAVEFORM_TEST_SAMPLES_NUMBER] = 
  {COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(             0),    /* Expected voltage: 0V,          corresponding digital values: to   0 on 8 bits and    0 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI *1/4),    /* Expected voltage: 1/4 of Vdda, corresponding digital values: to  63 on 8 bits and 1023 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI *2/4),    /* Expected voltage: 1/2 of Vdda, corresponding digital values: to 127 on 8 bits and 2047 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI *3/4),    /* Expected voltage: 3/4 of Vdda, corresponding digital values: to 191 on 8 bits and 3071 and 12 bits */
   COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(VDD_APPLI    )};    /* Expected voltage: Vdda,        corresponding digital values: to 255 on 8 bits and 4095 and 12 bits */
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void ADC_Config(void);
#if defined ADC_TRIGGER_FROM_TIMER
static void TIM_Config(void);
#endif
#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
static void WaveformVoltageGenerationForTest(void);
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F107xC HAL library initialization:
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
  
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();
  
  /*## Configure peripherals #################################################*/
  
  /* Initialize LEDs on board */
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_GREEN);


  /* Configure the ADCx and ADCy peripherals */
  ADC_Config();
  
  /* Run the ADC calibration */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle_master) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  if (HAL_ADCEx_Calibration_Start(&AdcHandle_slave) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

#if defined(ADC_TRIGGER_FROM_TIMER)
  /* Configure the TIM peripheral */
  TIM_Config();
#endif

  /*## Enable peripherals ####################################################*/
#if defined(ADC_TRIGGER_FROM_TIMER)
  /* Timer enable */
  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)
  {
    /* Counter Enable Error */
    Error_Handler();
  }
#endif /* ADC_TRIGGER_FROM_TIMER */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
  /* Generate a periodic signal on a spare DAC channel */
  WaveformVoltageGenerationForTest();
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */
  
  /* Enable ADC slave */
  if (HAL_ADC_Start(&AdcHandle_slave) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*## Start ADC conversions #################################################*/
  
  /* Start ADCx and ADCy multimode conversion on regular group with transfer by DMA */
  if (HAL_ADCEx_MultiModeStart_DMA(&AdcHandle_master,
                                   (uint32_t *)aADCDualConvertedValues,
                                    ADCCONVERTEDVALUES_BUFFER_SIZE
                                  ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  
  /* Infinite loop */
  while (1)
  {
  
    /* Turn-on/off LED_GREEN in function of ADC conversion result */
    /*  - Turn-off if ADC conversions buffer is not complete */
    /*  - Turn-on if ADC conversions buffer is complete */

    /* ADC conversion buffer complete variable is updated into ADC conversions*/
    /* complete callback.                                                     */
    if (ubDCDualConversionComplete == RESET)
    {
      BSP_LED_Off(LED_GREEN);
    }
    else
    {
      BSP_LED_On(LED_GREEN);
    }
  
    /* For information: ADC conversion results are stored into array          */
    /* "aADCDualConvertedValues" (for debug: check into watch window)         */

    /* For the purpose of this example, dual conversion values are            */
    /* dispatched into 2 arrays corresponding to each ADC conversion values.  */
    /* (aADCxConvertedValues, aADCyConvertedValues)                           */
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
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Configure PLLs ------------------------------------------------------*/
  /* PLL2 configuration: PLL2CLK = (HSE / HSEPrediv2Value) * PLL2MUL = (25 / 5) * 8 = 40 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8 MHz */
  /* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */ 

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState              = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue        = RCC_HSE_PREDIV_DIV5;
  oscinitstruct.Prediv1Source         = RCC_PREDIV1_SOURCE_PLL2;
  oscinitstruct.PLL.PLLState          = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL            = RCC_PLL_MUL9;
  oscinitstruct.PLL2.PLL2State        = RCC_PLL2_ON;
  oscinitstruct.PLL2.PLL2MUL          = RCC_PLL2_MUL8;
  oscinitstruct.PLL2.HSEPrediv2Value  = RCC_HSE_PREDIV2_DIV5;
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
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}



/**
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  ADC_MultiModeTypeDef     MultiModeInit;

  /* Configuration of ADC (master) init structure: ADC parameters and regular group */
  AdcHandle_master.Instance = ADCx;
  
  AdcHandle_master.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle_master.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
#if defined ADC_TRIGGER_FROM_TIMER
  AdcHandle_master.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
#else
  AdcHandle_master.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */
#endif
  AdcHandle_master.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle_master.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle_master.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
#if defined ADC_TRIGGER_FROM_TIMER
  AdcHandle_master.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_Tx_TRGO;  /* Trig of conversion start done by external event */
#else
  AdcHandle_master.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
#endif
  
  if (HAL_ADC_Init(&AdcHandle_master) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }

  /* Configuration of ADC (slave) init structure: ADC parameters and regular group */
  AdcHandle_slave.Instance = ADCy;

  /* Same configuration as ADC master, with continuous mode and external      */
  /* trigger disabled since ADC master is triggering the ADC slave            */
  /* conversions                                                              */
  AdcHandle_slave.Init = AdcHandle_master.Init;
  AdcHandle_slave.Init.ExternalTrigConv      = ADC_SOFTWARE_START;

  if (HAL_ADC_Init(&AdcHandle_slave) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
  /* Configuration of channel on ADC (master) regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each number of                      */
  /*       "ADCCONVERTEDVALUES_BUFFER_SIZE" ADC conversions (IT by DMA end    */
  /*       of transfer), select sampling time and ADC clock with sufficient   */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel = ADCx_CHANNELa;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;


  if (HAL_ADC_ConfigChannel(&AdcHandle_master, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /* Configuration of channel on ADC (slave) regular group on sequencer rank 1 */
  /* Same channel as ADCx for dual mode interleaved: both ADC are converting  */
  /* the same channel.                                                        */
  sConfig.Channel = ADCx_CHANNELa;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle_slave, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /* Configuration of multimode */
  /* Multimode parameters settings and set ADCy (slave) under control of      */
  /* ADCx (master).                                                           */
  MultiModeInit.Mode = ADC_DUALMODE_INTERLFAST;
  if (HAL_ADCEx_MultiModeConfigChannel(&AdcHandle_master, &MultiModeInit) != HAL_OK)
  {
    /* Multimode Configuration Error */
    Error_Handler();
  }
  
}

#if defined(ADC_TRIGGER_FROM_TIMER)
/**
  * @brief  TIM configuration
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_MasterConfigTypeDef master_timer_config;
  RCC_ClkInitTypeDef clk_init_struct = {0};       /* Temporary variable to retrieve RCC clock configuration */
  uint32_t latency;                               /* Temporary variable to retrieve Flash Latency */
  
  uint32_t timer_clock_frequency = 0;             /* Timer clock frequency */
  uint32_t timer_prescaler = 0;                   /* Time base prescaler to have timebase aligned on minimum frequency possible */
  
  /* Configuration of timer as time base:                                     */ 
  /* Caution: Computation of frequency is done for a timer instance on APB1   */
  /*          (clocked by PCLK1)                                              */
  /* Timer frequency is configured modifying the following constants:         */
  /* - TIMER_FREQUENCY: timer frequency (unit: Hz).                           */
  /* - TIMER_FREQUENCY_RANGE_MIN: timer minimum frequency possible            */
  /*   (unit: Hz).                                                            */
  /* Note: Refer to comments at these literals definition for more details.   */

  /* Retrieve timer clock source frequency */
  HAL_RCC_GetClockConfig(&clk_init_struct, &latency);
  /* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
  /* clock source.                                                            */
  if (clk_init_struct.APB1CLKDivider == RCC_HCLK_DIV1)
  {
    timer_clock_frequency = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    timer_clock_frequency = HAL_RCC_GetPCLK1Freq() *2;
  }
  
  /* Timer prescaler calculation */
  /* (computation for timer 16 bits, additional + 1 to round the prescaler up) */
  timer_prescaler = (timer_clock_frequency / (TIMER_PRESCALER_MAX_VALUE * TIMER_FREQUENCY_RANGE_MIN)) +1;
  
  /* Set timer instance */
  TimHandle.Instance = TIMx;
  
  /* Configure timer parameters */
  TimHandle.Init.Period            = ((timer_clock_frequency / (timer_prescaler * TIMER_FREQUENCY)) - 1);
  TimHandle.Init.Prescaler         = (timer_prescaler - 1);
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0x0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Timer initialization Error */
    Error_Handler();
  }

  /* Timer TRGO selection */
  master_timer_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
  master_timer_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &master_timer_config) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }
  
}
#endif /* ADC_TRIGGER_FROM_TIMER */

#if defined(WAVEFORM_VOLTAGE_GENERATION_FOR_TEST)
/**
  * @brief  For this example purpose, generate a periodic signal on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PA.04) and ADC channel (pin PA.04) to run this example.
  *         (this avoid to user the need of an external signal generator)
  * @param  None
  * @retval None
  */
static void WaveformVoltageGenerationForTest(void)
{
  DAC_ChannelConfTypeDef sConfig;
  TIM_MasterConfigTypeDef master_timer_config;
  RCC_ClkInitTypeDef clk_init_struct = {0};       /* Temporary variable to retrieve RCC clock configuration */
  uint32_t latency;                               /* Temporary variable to retrieve Flash Latency */
  
  uint32_t timer_clock_frequency = 0;             /* Timer clock frequency */
  uint32_t timer_prescaler = 0;                   /* Time base prescaler to have timebase aligned on minimum frequency possible */
  
  /* Configuration of timer as time base:                                     */ 
  /* Caution: Computation of frequency is done for a timer instance on APB1   */
  /*          (clocked by PCLK1)                                              */
  /* - TIMER_FOR_WAVEFORM_TEST_FREQUENCY: timer frequency (unit: Hz).         */
  /* - TIMER_FOR_WAVEFORM_TEST_FREQUENCY_RANGE_MIN: time base minimum         */
  /*   frequency possible (unit: Hz).                                         */
  /* Note: Refer to comments at these literals definition for more details.   */
  
  /* Retrieve timer clock source frequency */
  HAL_RCC_GetClockConfig(&clk_init_struct, &latency);
  /* If APB1 prescaler is different of 1, timers have a factor x2 on their    */
  /* clock source.                                                            */
  if (clk_init_struct.APB1CLKDivider == RCC_HCLK_DIV1)
  {
    timer_clock_frequency = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    timer_clock_frequency = HAL_RCC_GetPCLK1Freq() *2;
  }
  
  /* Timer prescaler calculation */
  /* (computation for timer 16 bits, additional + 1 to round the prescaler up) */
  timer_prescaler = (timer_clock_frequency / (TIMER_FOR_WAVEFORM_TEST_PRESCALER_MAX_VALUE * TIMER_FOR_WAVEFORM_TEST_FREQUENCY_RANGE_MIN)) +1;
  
  /* Set timer instance */
  TimForWaveformTestHandle.Instance = TIM_test_signal_generation;
  
  /* Configure timer parameters */
  TimForWaveformTestHandle.Init.Period            = ((timer_clock_frequency / (timer_prescaler * TIMER_FOR_WAVEFORM_TEST_FREQUENCY)) - 1);
  TimForWaveformTestHandle.Init.Prescaler         = (timer_prescaler - 1);
  TimForWaveformTestHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  TimForWaveformTestHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimForWaveformTestHandle.Init.RepetitionCounter = 0x0;
  TimForWaveformTestHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
  if (HAL_TIM_Base_Init(&TimForWaveformTestHandle) != HAL_OK)
  {
    /* Timer initialization Error */
    Error_Handler();
  }

  /* Timer TRGO selection */
  master_timer_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
  master_timer_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&TimForWaveformTestHandle, &master_timer_config) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }
  
  
  /* Configuration of DACx peripheral */
  DacHandle.Instance = DACx;
  
  /* Initialize the DAC peripheral */
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Configuration of DAC channel */
  sConfig.DAC_Trigger = DACx_TRIGGER_Tx_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration error */
    Error_Handler();
  }
  
  
  /*## Enable peripherals ####################################################*/
  
  /* Timer counter enable */
  if (HAL_TIM_Base_Start(&TimForWaveformTestHandle) != HAL_OK)
  {
    /* Counter Enable Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel1 and associated DMA */
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNELa, (uint32_t *)Waveform_8bits, WAVEFORM_TEST_SAMPLES_NUMBER, DAC_ALIGN_8B_R) != HAL_OK)
  {
    /* Start DMA Error */
    Error_Handler();
  }
  
}
#endif /* WAVEFORM_VOLTAGE_GENERATION_FOR_TEST */

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
  uint32_t tmp_index = 0;
  
  /* For the purpose of this example, dispatch dual conversion values         */
  /* into 2 arrays corresponding to each ADC conversion values.               */
  for (tmp_index = (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index < ADCCONVERTEDVALUES_BUFFER_SIZE; tmp_index++)
  {
    aADCxConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(aADCDualConvertedValues[tmp_index]);
    aADCyConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(aADCDualConvertedValues[tmp_index]);
  }
  
  /* Set variable to report DMA transfer status to main program */
  ubDCDualConversionComplete = SET;
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  uint32_t tmp_index = 0;
  
  /* For the purpose of this example, dispatch dual conversion values         */
  /* into 2 arrays corresponding to each ADC conversion values.               */
  for (tmp_index = 0; tmp_index < (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index++)
  {
    aADCxConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(aADCDualConvertedValues[tmp_index]);
    aADCyConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(aADCDualConvertedValues[tmp_index]);
  }

  /* Reset variable to report DMA transfer status to main program */
  ubDCDualConversionComplete = RESET;
}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED_RED is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED_RED */
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(500);
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
