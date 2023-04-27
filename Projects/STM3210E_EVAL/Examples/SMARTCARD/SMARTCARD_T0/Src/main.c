/**
  ******************************************************************************
  * @file    SMARTCARD_T0/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F1xx SMARTCARD HAL API to communicate
  *          with card compatible with T0 protocol.
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

#if defined(HAL_UART_MODULE_ENABLED)
#include <stdio.h>
#endif /* HAL_UART_MODULE_ENABLED */


/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */


/** @addtogroup SMARTCARD_T0
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if defined(HAL_UART_MODULE_ENABLED)
#define UART_TIMEOUT_VALUE   1000
#endif /* HAL_UART_MODULE_ENABLED */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Directories & Files ID */
const uint8_t MasterRoot[2] = {0x3F, 0x00};
const uint8_t GSMDir[2] = {0x7F, 0x20};
const uint8_t ICCID[2] = {0x2F, 0xE2};
const uint8_t IMSI[2] = {0x6F, 0x07};
const uint8_t CHV1[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};

/* APDU Transport Structures */
SC_ADPU_Commands SC_ADPU;
SC_ADPU_Response SC_Response;

__IO uint32_t CardInserted = 0;
uint32_t CHV1Status = 0, i = 0;
__IO uint8_t ICCID_Content[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__IO uint8_t IMSI_Content[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


#if defined(HAL_UART_MODULE_ENABLED)
UART_HandleTypeDef   huart;
#endif /* HAL_UART_MODULE_ENABLED */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
#if defined(HAL_UART_MODULE_ENABLED)
void       UART_Config(void);
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif /* HAL_UART_MODULE_ENABLED */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F103xG HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
#if defined(HAL_UART_MODULE_ENABLED)
  UART_Config();
#endif /* HAL_UART_MODULE_ENABLED */

  SC_State SCState = SC_POWER_OFF;

  /* Configure Smartcard Interface GPIO pins */
  SC_IOConfig();

/*-------------------------------- Idle task ---------------------------------*/
  while(1)
  {
#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- Waiting for card insertion -- (LED1 blinking) \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Loop while no Smartcard is detected */
    while(CardInserted == 0)
    {
      BSP_LED_Toggle(LED1);
      HAL_Delay(200);
    }
    BSP_LED_On(LED1);

    /* Start SC Demo ---------------------------------------------------------*/
#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- Card inserted -- (All LEDs On) \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Wait A2R --------------------------------------------------------------*/
    SCState = SC_POWER_ON;

    SC_ADPU.Header.CLA = 0x00;
    SC_ADPU.Header.INS = SC_GET_A2R;
    SC_ADPU.Header.P1 = 0x00;
    SC_ADPU.Header.P2 = 0x00;
    SC_ADPU.Body.LC = 0x00;

    while(SCState != SC_ACTIVE_ON_T0)
    {
      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- Card powered ON : ATR received --  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Apply the Procedure Type Selection (PTS) */
    SC_PTSConfig();

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- PTS procedure performed --  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Inserts delay(400ms) for Smartcard clock resynchronisation */
    HAL_Delay(400);

    /* Select MF -------------------------------------------------------------*/
    SC_ADPU.Header.CLA = SC_CLA_GSM11;
    SC_ADPU.Header.INS = SC_SELECT_FILE;
    SC_ADPU.Header.P1 = 0x00;
    SC_ADPU.Header.P2 = 0x00;
    SC_ADPU.Body.LC = 0x02;

    for(i = 0; i < SC_ADPU.Body.LC; i++)
    {
      SC_ADPU.Body.Data[i] = MasterRoot[i];
    }
    while(i < LC_MAX)
    {
      SC_ADPU.Body.Data[i++] = 0;
    }
    SC_ADPU.Body.LE = 0;

    SC_Handler(&SCState, &SC_ADPU, &SC_Response);

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- ### SELECT MF ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Get Response on MF ----------------------------------------------------*/
    if(SC_Response.SW1 == SC_DF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_GET_RESPONCE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;
      SC_ADPU.Body.LE = SC_Response.SW2;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- ### GET RESPONSE ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Select ICCID ----------------------------------------------------------*/
    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
      /* Check if the CHV1 is enabled */
      if((SC_Response.Data[13] & 0x80) == 0x00)
      {
        CHV1Status = 0x01;
#if defined(HAL_UART_MODULE_ENABLED)
        printf("-- CHV1 is enabled --  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */
      }
      else
      {
#if defined(HAL_UART_MODULE_ENABLED)
        printf("-- CHV1 is disabled --  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */
      }

      /* Send APDU Command for ICCID selection */
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_SELECT_FILE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x02;

      for(i = 0; i < SC_ADPU.Body.LC; i++)
      {
        SC_ADPU.Body.Data[i] = ICCID[i];
      }
      while(i < LC_MAX)
      {
        SC_ADPU.Body.Data[i++] = 0;
      }
      SC_ADPU.Body.LE = 0;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);

#if defined(HAL_UART_MODULE_ENABLED)
      printf("-- ### SELECT EF ICCID ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    }

    /* Read Binary in ICCID --------------------------------------------------*/
    if(SC_Response.SW1 == SC_EF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_READ_BINARY;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;

      SC_ADPU.Body.LE = 10;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- ### READ BINARY (ICCID) ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Select GSMDir ---------------------------------------------------------*/
    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
      /* Copy the ICCID File content into ICCID_Content buffer */
      for(i = 0; i < SC_ADPU.Body.LE; i++)
      {
        ICCID_Content[i] =  SC_Response.Data[i];
      }
#if defined(HAL_UART_MODULE_ENABLED)
      printf("--     ICCID Value =  ");
      for(i = 0; i < SC_ADPU.Body.LE; i++)
      {
        printf("%x  ",ICCID_Content[i]);
      }
      printf("\n\r");
#endif /* HAL_UART_MODULE_ENABLED */

      /* Send APDU Command for GSMDir selection */
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_SELECT_FILE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x02;

      for(i = 0; i < SC_ADPU.Body.LC; i++)
      {
        SC_ADPU.Body.Data[i] = GSMDir[i];
      }
      while(i < LC_MAX)
      {
        SC_ADPU.Body.Data[i++] = 0;
      }
      SC_ADPU.Body.LE = 0;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);

#if defined(HAL_UART_MODULE_ENABLED)
      printf("-- ### SELECT DF GSM ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */
    }

    /* Select IMSI -----------------------------------------------------------*/
    if(SC_Response.SW1 == SC_DF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_SELECT_FILE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x02;

      for(i = 0; i < SC_ADPU.Body.LC; i++)
      {
        SC_ADPU.Body.Data[i] = IMSI[i];
      }
      while(i < LC_MAX)
      {
        SC_ADPU.Body.Data[i++] = 0;
      }
      SC_ADPU.Body.LE = 0;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);

#if defined(HAL_UART_MODULE_ENABLED)
      printf("-- ### SELECT EF IMSI ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */
    }

    /* Get Response on IMSI File ---------------------------------------------*/
    if(SC_Response.SW1 == SC_EF_SELECTED)
    {
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_GET_RESPONCE;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;
      SC_ADPU.Body.LE = SC_Response.SW2;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }

    /* Read Binary in IMSI ---------------------------------------------------*/
    if(CHV1Status == 0x00)
    {
      if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
      {
        /* Enable CHV1 (PIN1) ------------------------------------------------*/
        /* CHV1 enabling should be done using proper value of used card CHV1 */
        /* Please fill corresponding value in CHV1[array] . Current value is set to "00000000". */

        SC_ADPU.Header.CLA = SC_CLA_GSM11;
        SC_ADPU.Header.INS = SC_ENABLE;
        SC_ADPU.Header.P1 = 0x00;
        SC_ADPU.Header.P2 = 0x01;
        SC_ADPU.Body.LC = 0x08;

        for(i = 0; i < SC_ADPU.Body.LC; i++)
        {
          SC_ADPU.Body.Data[i] = CHV1[i];
        }
        while(i < LC_MAX)
        {
          SC_ADPU.Body.Data[i++] = 0;
        }
        SC_ADPU.Body.LE = 0;

        SC_Handler(&SCState, &SC_ADPU, &SC_Response);

#if defined(HAL_UART_MODULE_ENABLED)
       printf("-- ### CHV1 ENABLE ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */
      }
    }
    else
    {
      if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
      {
        /* Verify CHV1 (PIN1) ------------------------------------------------*/
        /* CHV1 enabling should be done using proper value of used card CHV1 */
        /* Please fill corresponding value in CHV1[array] . Current value is set to "00000000". */
        SC_ADPU.Header.CLA = SC_CLA_GSM11;
        SC_ADPU.Header.INS = SC_VERIFY;
        SC_ADPU.Header.P1 = 0x00;
        SC_ADPU.Header.P2 = 0x01;
        SC_ADPU.Body.LC = 0x08;

        for(i = 0; i < SC_ADPU.Body.LC; i++)
        {
          SC_ADPU.Body.Data[i] = CHV1[i];
        }
        while(i < LC_MAX)
        {
          SC_ADPU.Body.Data[i++] = 0;
        }
        SC_ADPU.Body.LE = 0;

        SC_Handler(&SCState, &SC_ADPU, &SC_Response);
#if defined(HAL_UART_MODULE_ENABLED)
        printf("-- ### CHV1 VERIFY ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */
      }
    }
    /* Read Binary in IMSI ---------------------------------------------------*/
    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
#if defined(HAL_UART_MODULE_ENABLED)
      printf("-- ### CHV1 Operation successfull ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */
      SC_ADPU.Header.CLA = SC_CLA_GSM11;
      SC_ADPU.Header.INS = SC_READ_BINARY;
      SC_ADPU.Header.P1 = 0x00;
      SC_ADPU.Header.P2 = 0x00;
      SC_ADPU.Body.LC = 0x00;

      SC_ADPU.Body.LE = 9;

      SC_Handler(&SCState, &SC_ADPU, &SC_Response);
    }
    else
    {
#if defined(HAL_UART_MODULE_ENABLED)
      printf("-- ### CHV1 Operation unsuccessfull : SW1/SW2=0x%4x ###  \n\r", ((SC_Response.SW1 << 8) | (SC_Response.SW2)));
#endif /* HAL_UART_MODULE_ENABLED */
    }

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- ### READ BINARY (IMSI) ###  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    if(((SC_Response.SW1 << 8) | (SC_Response.SW2)) == SC_OP_TERMINATED)
    {
      /* Copy the IMSI File content into IMSI_Content buffer */
      for(i = 0; i < SC_ADPU.Body.LE; i++)
      {
        IMSI_Content[i] =  SC_Response.Data[i];
      }
    }
#if defined(HAL_UART_MODULE_ENABLED)
    printf("--     IMSI Value =  ");
    for(i = 0; i < SC_ADPU.Body.LE; i++)
    {
      printf("%x  ",IMSI_Content[i]);
    }
    printf("\n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Disable the Smartcard interface */
    SCState = SC_POWER_OFF;
    SC_Handler(&SCState, &SC_ADPU, &SC_Response);

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- Card powered OFF --  \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

#if defined(HAL_UART_MODULE_ENABLED)
    printf("-- Waiting for card removal -- (LED1 blinking) \n\r");
#endif /* HAL_UART_MODULE_ENABLED */

    /* Loop while a Smartcard is detected */
    while(CardInserted == 1)
    {
      BSP_LED_Toggle(LED1);
      HAL_Delay(200);
    }
    BSP_LED_Off(LED1);
    BSP_LED_Off(LED2);
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED4);
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


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* Toggle LED3 */
  while (1)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(1000);
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == SC_OFF_PIN)
  {
    /* Toggle LED2..4 */
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);

    if (CardInserted == 0)
    {
      /* Smartcard detected */
      CardInserted = 1;

      /* Enable CMDVCC */
      SC_PowerCmd(ENABLE);

      /* Reset the card */
      SC_Reset(GPIO_PIN_RESET);
    }
    else
    {
      /* Smartcard removed */
      CardInserted = 0;

      /* Enable CMDVCC */
      SC_PowerCmd(DISABLE);
    }
  }
  else
  {
    /* Smartcard removed */
    CardInserted = 0;
  }
}

/**
  * @brief SMARTCARD error callbacks
  * @param hsc: usart handle
  * @retval None
  */
void HAL_SMARTCARD_ErrorCallback(SMARTCARD_HandleTypeDef *hsc)
{
  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_FE)
  {
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
    /* Resend the byte that failed to be received (by the Smartcard) correctly */
    SC_ParityErrorHandler();
  }

  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_PE)
  {
    /* Enable SC_USART RXNE Interrupt (until receiving the corrupted byte) */
    __HAL_SMARTCARD_ENABLE_IT(hsc, SMARTCARD_IT_RXNE);
    /* Flush the SC_USART DR register */
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
  }
  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_NE)
  {
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
  }

  if(HAL_SMARTCARD_GetError(hsc) & HAL_SMARTCARD_ERROR_ORE)
  {
    __HAL_SMARTCARD_FLUSH_DRREGISTER(hsc);
  }
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsc: usart handle
  * @retval None
  */
void HAL_SMARTCARD_RxCpltCallback(SMARTCARD_HandleTypeDef *hsc)
{
  /* Disable SC_USART RXNE Interrupt */
  __HAL_SMARTCARD_DISABLE_IT(hsc, SMARTCARD_IT_RXNE);
  hsc->Instance->DR;
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


#if defined(HAL_UART_MODULE_ENABLED)
/**
  * @brief  Configures the USARTx and associated pins.
  * @param  None
  * @retval None
  */
void UART_Config(void)
{
  /* UARTx configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  huart.Init.BaudRate   = 115200;
  huart.Init.Mode       = UART_MODE_TX_RX;
  huart.Init.Parity     = UART_PARITY_NONE;
  huart.Init.StopBits   = UART_STOPBITS_1;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  BSP_COM_Init(COM1, &huart);
}

/**
  * @brief  Retargets the C library printf function to the USART1.
  * @param  ch: character to send
  * @param  f: pointer to file (not used)
  * @retval The character transmitted
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, UART_TIMEOUT_VALUE);

  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART1.
  * @param  f: pointer to file (not used)
  * @retval The character received
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  /* We received the charracter on the handler of the USART1 */
  /* The handler must be initialise before */
  HAL_UART_Receive(&huart, (uint8_t *)&ch, 1, UART_TIMEOUT_VALUE);

  return ch;
}

#endif /* HAL_UART_MODULE_ENABLED */


/**
  * @}
  */

/**
  * @}
  */
