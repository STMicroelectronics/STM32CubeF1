/**
  ******************************************************************************
  * @file    Examples_MIX/ADC/ADC_SingleConversion_TriggerSW_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a ADC peripheral to perform
  *          a single  ADC conversion of a channel, at each software start.
  *          Example using programming model: interrupt 
  *          (for programming models polling or DMA transfer, refer to
  *          other examples).
  *          This example is based on the STM32F1xx ADC HAL & LL API
  *          (LL API is used for performance improvement).
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

/** @addtogroup STM32F1xx_MIX_HAL_Examples
  * @{
  */

/** @addtogroup ADC_SingleConversion_TriggerSW_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       ((uint32_t)3300)

/* Definitions of data related to this example */
  /* Init variable out of expected ADC conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

  /* Full-scale digital value with a resolution of 12 bits (voltage range     */
  /* determined by analog voltage references Vref+ and Vref-,                 */
  /* refer to reference manual).                                              */
  #define DIGITAL_SCALE_12BITS             (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Peripherals handlers declaration */
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

#if defined(WAVEFORM_GENERATION)
/* DAC handler declaration */
DAC_HandleTypeDef    DacHandle;  /* DAC used for waveform voltage generation for test */
#endif /* WAVEFORM_GENERATION */

/* Variables for ADC conversion data */
__IO uint16_t uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group regular conversion data */

/* Variables for ADC conversion data computation to physical values */
__IO uint16_t uhADCxConvertedData_Voltage_mVolt = 0;  /* Value of voltage calculated from ADC conversion data (unit: mV) */

/* Variable to report status of ADC group regular unitary conversion          */
/*  0: ADC group regular unitary conversion is not completed                  */
/*  1: ADC group regular unitary conversion is completed                      */
/*  2: ADC group regular unitary conversion has not been started yet          */
/*     (initial state)                                                        */
__IO uint8_t ubAdcGrpRegularUnitaryConvStatus = 2; /* Variable set into ADC interruption callback */

/* Variable to manage push button on board: interface between ExtLine interruption and main program */
__IO   uint8_t ubUserButtonClickEvent = RESET;  /* Event detection: Set after User Button interrupt */

/* Private function prototypes -----------------------------------------------*/
static     void SystemClock_Config(void);
static     void Error_Handler(void);
static     void Configure_ADC(void);
#if defined(WAVEFORM_GENERATION)
static     void Generate_waveform_SW_update_Config(void);
static     void Generate_waveform_SW_update(void);
#endif /* WAVEFORM_GENERATION */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F103xG HAL library initialization:
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
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED1);
  
  /* Configure Key push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  /* Configure ADC */
  /* Note: This function configures the ADC but does not enable it.           */
  /*       To enable it (ADC activation and conversion start), use            */
  /*       function "HAL_ADC_Start_xxx()".                                    */
  /*       This is intended to optimize power consumption:                    */
  /*       1. ADC configuration can be done once at the beginning             */
  /*          (ADC disabled, minimal power consumption)                       */
  /*       2. ADC enable (higher power consumption) can be done just before   */
  /*          ADC conversions needed.                                         */
  /*          Then, possible to perform successive "Activate_ADC()",          */
  /*          "Deactivate_ADC()", ..., without having to set again            */
  /*          ADC configuration.                                              */
  Configure_ADC();
  
  /* Run the ADC calibration */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }
  
  /* Start ADC group regular conversion with IT                             */
  /* Note: Perform initial ADC conversion start using driver HAL,           */
  /*       then following ADC conversion start using driver LL.             */
  /*       (mandatory to use driver LL after the first call of              */
  /*       ADC IRQ handler, implemented with driver LL).                    */
  if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
  {
    /* ADC conversion start error */
    Error_Handler();
  }
  
#if defined(WAVEFORM_GENERATION)
  /* Configure the DAC peripheral and generate a constant voltage of Vdda/2.  */
  Generate_waveform_SW_update_Config();
#endif /* WAVEFORM_GENERATION */
  
  
  /*## Enable peripherals ####################################################*/
  
  /* Note: ADC is enabled afterwards when starting ADC conversion using       */
  /*       function "HAL_ADC_Start_xxx()".                                    */
  
  
  /* Infinite loop */
  while (1)
  {
    /* Wait for event on push button to perform following actions */
    while ((ubUserButtonClickEvent) == RESET)
    {
    }
    /* Reset variable for next loop iteration */
    ubUserButtonClickEvent = RESET;

#if defined(WAVEFORM_GENERATION)
    /* Modifies modifies the voltage level, to generate a waveform circular,  */
    /* shape of ramp: Voltage is increasing at each press on push button,     */
    /* from 0 to maximum range (Vdda) in 5 steps, then starting back from 0V. */
    /* Voltage is updated incrementally at each call of this function.        */
    Generate_waveform_SW_update();
#endif /* WAVEFORM_GENERATION */
    
    /* Turn LED off before performing a new ADC conversion start */
    BSP_LED_Off(LED1);
    
    /* Reset status variable of ADC group regular unitary conversion before   */
    /* performing a new ADC group regular conversion start.                   */
    if (ubAdcGrpRegularUnitaryConvStatus != 0)
    {
      ubAdcGrpRegularUnitaryConvStatus = 0;
    }
    
    /* Init variable containing ADC conversion data */
    uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE;
    
    /*## Start ADC conversions ###############################################*/
    /* ########## Starting from this point HAL API must not be used ########## */

      /* Start ADC group regular conversion */
      /* Note: Hardware constraint (refer to description of the functions         */
      /*       below):                                                            */
      /*       On this STM32 serie, setting of these features are not             */
      /*       conditioned to ADC state.                                          */
      /*       However, in order to be compliant with other STM32 series          */
      /*       and to show the best practice usages, ADC state is checked.        */
      /*       Software can be optimized by removing some of these checks, if     */
      /*       they are not relevant considering previous settings and actions    */
      /*       in user application.                                               */
      if (LL_ADC_IsEnabled(ADCx) == 1)
      {
        LL_ADC_REG_StartConversionSWStart(ADCx);
      }
      else
      {
        /* Error: ADC conversion start could not be performed */
        Error_Handler();
      }
    
    /* Note: Variable "ubUserButtonClickEvent" is set into push button        */
    /*       IRQ handler, refer to function "HAL_GPIO_EXTI_Callback()".       */
    
    /* Note: ADC conversions data are stored into variable                    */
    /*       "uhADCxConvertedData".                                           */
    /*       (for debug: see variable content into watch window).             */
    
  }
}

/**
  * @brief  Configure ADC (ADC instance: ADCx) and GPIO used by ADC channels.
  * @note   Using HAL driver, configuration of GPIO used by ADC channels,
  *         NVIC and clock source at top level (RCC)
  *         are not implemented into this function,
  *         must be implemented into function "HAL_ADC_MspInit()".
  * @param  None
  * @retval None
  */
void Configure_ADC(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  
  /*## Configuration of ADC ##################################################*/
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## common to several ADC, ADC instance, ADC group regular  ###############*/
  
  /* Set ADC instance of HAL ADC handle AdcHandle */
  AdcHandle.Instance = ADCx;
  
  /* Configuration of HAL ADC handle init structure:                          */
  /* parameters of scope ADC instance and ADC group regular.                  */
  /* Note: On this STM32 family, ADC group regular sequencer is               */
  /*       fully configurable: sequencer length and each rank                 */
  /*       affectation to a channel are configurable.                         */
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## ADC group injected and channels mapped on group injected ##############*/
  
  /* Note: ADC group injected not used and not configured in this example.    */
  /*       Refer to other ADC examples using this feature.                    */
  /* Note: Call of the functions below are commented because they are         */
  /*       useless in this example:                                           */
  /*       setting corresponding to default configuration from reset state.   */
  
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## channels mapped on group regular         ##############################*/
  
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: On this STM32 family, ADC group regular sequencer is               */
  /*       fully configurable: sequencer length and each rank                 */
  /*       affectation to a channel are configurable.                         */
  /* Note: Considering IT occurring after each ADC conversion                 */
  /*       (IT by ADC group regular end of unitary conversion),               */
  /*       select sampling time and ADC clock with sufficient                 */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADCx_CHANNELa;               /* ADC channel selection */
  sConfig.Rank         = ADC_REGULAR_RANK_1;          /* ADC group regular rank in which is mapped the selected ADC channel */
  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;  /* ADC channel sampling time */
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*## Configuration of ADC hierarchical scope: multimode ####################*/
  /* Note: ADC multimode not used and not configured in this example.         */
  /*       Refer to other ADC examples using this feature.                    */
  
  
  /*## Configuration of ADC transversal scope: analog watchdog ###############*/
  
  /* Note: ADC analog watchdog not used and not configured in this example.   */
  /*       Refer to other ADC examples using this feature.                    */
  
  
  /*## Configuration of ADC transversal scope: oversampling ##################*/
  
  /* Note: Feature not available on this STM32 family */ 
  
}

#if defined(WAVEFORM_GENERATION)
/**
  * @brief  For this example, generate a waveform voltage on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PA.04) and ADC channel (pin PA.04) to run this example.
  *         (this prevents the user from resorting to an external signal
  *         generator).
  *         This function configures the DAC and generates a constant voltage of Vdda/2.
  * @note   Voltage level can be modifying afterwards using function
  *         "Generate_waveform_SW_update()".
  * @param  None
  * @retval None
  */
static void Generate_waveform_SW_update_Config(void)
{
  static DAC_ChannelConfTypeDef sConfig;

  /*## Configure peripherals #################################################*/
  /* Configuration of DACx peripheral */
  DacHandle.Instance = DACx;

  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* DAC initialization error */
    Error_Handler();
  }

  /* Configuration of DAC channel */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration error */
    Error_Handler();
  }
  
  /*## Enable peripherals ####################################################*/
  
  /* Set DAC Channel data register: channel corresponding to ADC channel ADCx_CHANNELa */
  /* Set DAC output to 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V */
  if (HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa, DAC_ALIGN_12B_R, DIGITAL_SCALE_12BITS/2) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel: channel corresponding to ADC channel ADCx_CHANNELa */
  if (HAL_DAC_Start(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

}

/**
  * @brief  For this example, generate a waveform voltage on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PA.04) and ADC channel (pin PA.04) to run this example.
  *         (this prevents the user from resorting to an external signal
  *         generator).
  *         This function modifies the voltage level, to generate a
  *         waveform circular, shape of ramp: Voltage is increasing at each 
  *         press on push button, from 0 to maximum range (Vdda) in 5 steps,
  *         then starting back from 0V.
  *         Voltage is updated incrementally at each call of this function.
  * @note   Preliminarily, DAC must be configured once using
  *         function "Generate_waveform_SW_update_Config()".
  * @param  None
  * @retval None
  */
static void Generate_waveform_SW_update(void)
{
  static uint8_t ub_dac_steps_count = 0;      /* Count number of clicks: Incremented after User Button interrupt */
  
  /* Set DAC voltage on channel corresponding to ADCx_CHANNELa              */
  /* in function of user button clicks count.                                   */
  /* Set DAC output on 5 voltage levels, successively to:                       */
  /*  - minimum of full range (0 <=> ground 0V)                                 */
  /*  - 1/4 of full range (4095 <=> Vdda=3.3V): 1023 <=> 0.825V                 */
  /*  - 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V                  */
  /*  - 3/4 of full range (4095 <=> Vdda=3.3V): 3071 <=> 2.475V                 */
  /*  - maximum of full range (4095 <=> Vdda=3.3V)                              */
  if (HAL_DAC_SetValue(&DacHandle,
                       DACx_CHANNEL_TO_ADCx_CHANNELa,
                       DAC_ALIGN_12B_R,
                       ((DIGITAL_SCALE_12BITS * ub_dac_steps_count) / 4)
                      ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  /* Wait for voltage settling time */
  HAL_Delay(1);
  
  /* Manage ub_dac_steps_count to increment it in 4 steps and circularly.   */
  if (ub_dac_steps_count < 4)
  {
    ub_dac_steps_count++;
  }
  else
  {
    ub_dac_steps_count = 0;
  }

}
#endif /* WAVEFORM_GENERATION */

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
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
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
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
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


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == KEY_BUTTON_PIN)
 {
   /* Set variable to report push button event to main program */
   ubUserButtonClickEvent = SET;
 }
}

/* Note: Lines of code commented below correspond to the example using        */
/*       HAL driver only.                                                     */
/*       This example demonstrating a mix of HAL and LL drivers has replaced  */
/*       these lines using LL driver.                                         */
// /**
//   * @brief  Conversion complete callback in non blocking mode
//   * @param  AdcHandle : ADC handle
//   * @note   This function is executed when the ADC group regular 
//   *         sequencer has converted one rank of the sequence.
//   *         Therefore, this function is executed as many times as number
//   *         of ranks in the sequence.
//   * @note   This example shows a simple way to report end of conversion
//   *         and get conversion result. You can add your own implementation.
//   * @retval None
//   */
// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
// {
//   /* Retrieve ADC conversion data */
//   uhADCxConvertedData = HAL_ADC_GetValue(AdcHandle);
//
//   /* Computation of ADC conversions raw data to physical values               */
//   /* using LL ADC driver helper macro.                                        */
//   uhADCxConvertedData_Voltage_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxConvertedData, LL_ADC_RESOLUTION_12B);
//
//   /* Update status variable of ADC unitary conversion */
//   ubAdcGrpRegularUnitaryConvStatus = 1;
//
//   /* Set LED depending on ADC unitary conversion status */
//   /* - Turn-on if ADC group regular unitary conversion is completed */
//   /* - Turn-off if ADC group regular unitary conversion is not completed */
//   BSP_LED_On(LED1);
// }
//
// /**
//   * @brief  ADC error callback in non blocking mode
//   *        (ADC conversion with interruption or transfer by DMA)
//   * @param  hadc: ADC handle
//   * @retval None
//   */
// void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
// {
//   /* In case of ADC error, call main error handler */
//   Error_Handler();
// }

/**
  * @brief  ADC group regular end of unitary conversion interruption callback
  * @note   This function is executed when the ADC group regular 
  *         sequencer has converted one rank of the sequence.
  *         Therefore, this function is executed as many times as number
  *         of ranks in the sequence.
  * @retval None
  */
void AdcGrpRegularUnitaryConvComplete_Callback()
{
  /* Retrieve ADC conversion data */
  /* (data maximum amplitude corresponds to ADC resolution: 12 bits) */
  uhADCxConvertedData = LL_ADC_REG_ReadConversionData12(ADCx);
  
  /* Computation of ADC conversions raw data to physical values               */
  /* using LL ADC driver helper macro.                                        */
  uhADCxConvertedData_Voltage_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxConvertedData, LL_ADC_RESOLUTION_12B);
  
  /* Update status variable of ADC unitary conversion */
  ubAdcGrpRegularUnitaryConvStatus = 1;
  
  /* Set LED depending on ADC unitary conversion status */
  /* - Turn-on if ADC group regular unitary conversion is completed */
  /* - Turn-off if ADC group regular unitary conversion is not completed */
  BSP_LED_On(LED1);
  
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED3 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
