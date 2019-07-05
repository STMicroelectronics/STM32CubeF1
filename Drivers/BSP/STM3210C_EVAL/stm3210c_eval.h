/**
  ******************************************************************************
  * @file    stm3210c_eval.h
  * @author  MCD Application Team
  * @version V6.1.0
  * @date    14-April-2017
  * @brief   This file contains definitions for STM3210C_EVAL's LEDs, 
  *          push-buttons and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM3210C_EVAL
  * @{
  */ 

/** @addtogroup STM3210C_EVAL_COMMON
  * @{
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3210C_EVAL_H
#define __STM3210C_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#ifdef HAL_I2C_MODULE_ENABLED
#include "stm3210c_eval_io.h"
#endif /* HAL_I2C_MODULE_ENABLED */

/** @defgroup STM3210C_EVAL_Exported_Types STM3210C EVAL Exported Types
  * @{
  */

/**
 * @brief LED Types Definition
 */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,

  LED_GREEN  = LED1,
  LED_ORANGE = LED2,
  LED_RED    = LED3,
  LED_BLUE   = LED4

} Led_TypeDef;

/**
 * @brief BUTTON Types Definition
 */
typedef enum 
{
  BUTTON_WAKEUP = 0,
  BUTTON_TAMPER = 1,
  BUTTON_KEY    = 2,

} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

/**
 * @brief JOYSTICK Types Definition
 */
typedef enum 
{ 
  JOY_SEL   = 0,
  JOY_LEFT  = 1,
  JOY_RIGHT = 2,
  JOY_DOWN  = 3,
  JOY_UP    = 4,
  JOY_NONE  = 5

}JOYState_TypeDef;

typedef enum 
{ 
  JOY_MODE_GPIO = 0,
  JOY_MODE_EXTI = 1

}JOYMode_TypeDef;

/**
 * @brief COM Types Definition
 */
typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;
/**
  * @}
  */ 

/** @defgroup STM3210C_EVAL_Exported_Constants STM3210C EVAL Exported Constants
  * @{
  */ 

/** 
  * @brief  Define for STM3210C_EVAL board  
  */ 
#if !defined (USE_STM3210C_EVAL)
 #define USE_STM3210C_EVAL
#endif
  
/** @addtogroup STM3210C_EVAL_LED STM3210C EVAL LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_PIN_7             /* PD.07*/
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()
  
#define LED2_PIN                         GPIO_PIN_13            /* PD.13*/
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()


#define LED3_PIN                         GPIO_PIN_3            /* PD.03*/
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()


#define LED4_PIN                         GPIO_PIN_4            /* PD.04*/
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__LED__)    do { if ((__LED__) == LED1) LED1_GPIO_CLK_ENABLE(); else \
                                              if ((__LED__) == LED2) LED2_GPIO_CLK_ENABLE(); else \
                                              if ((__LED__) == LED3) LED3_GPIO_CLK_ENABLE(); else \
                                              if ((__LED__) == LED4) LED4_GPIO_CLK_ENABLE();} while(0)

#define LEDx_GPIO_CLK_DISABLE(__LED__)   (((__LED__) == LED1) ? LED1_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED2) ? LED2_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED3) ? LED3_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED4) ? LED4_GPIO_CLK_DISABLE() : 0 )

/**
  * @}
  */
  
/** @addtogroup STM3210C_EVAL_BUTTON STM3210C EVAL BUTTON
  * @{
  */  
#define BUTTONn                          3

/**
 * @brief Tamper push-button
 */
#define TAMPER_BUTTON_PIN                   GPIO_PIN_13             /* PC.13*/
#define TAMPER_BUTTON_GPIO_PORT             GPIOC
#define TAMPER_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define TAMPER_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define TAMPER_BUTTON_EXTI_IRQn             EXTI15_10_IRQn

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PIN                      GPIO_PIN_9             /* PB.09*/
#define KEY_BUTTON_GPIO_PORT                GPIOB
#define KEY_BUTTON_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn                EXTI9_5_IRQn

/**
 * @brief Wake-up push-button
 */
#define WAKEUP_BUTTON_PIN                   GPIO_PIN_0             /* PA.00*/
#define WAKEUP_BUTTON_GPIO_PORT             GPIOA
#define WAKEUP_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define WAKEUP_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define WAKEUP_BUTTON_EXTI_IRQn             EXTI0_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__BUTTON__) do { if ((__BUTTON__) == BUTTON_TAMPER) TAMPER_BUTTON_GPIO_CLK_ENABLE()  ; else \
                                                 if ((__BUTTON__) == BUTTON_KEY) KEY_BUTTON_GPIO_CLK_ENABLE()  ; else \
                                                 if ((__BUTTON__) == BUTTON_WAKEUP) WAKEUP_BUTTON_GPIO_CLK_ENABLE();} while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__BUTTON__)    (((__BUTTON__) == BUTTON_TAMPER) TAMPER_BUTTON_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_KEY) KEY_BUTTON_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_WAKEUP) WAKEUP_BUTTON_GPIO_CLK_DISABLE()  : 0 )

/**
  * @brief IO Pins definition 
  */ 
/* Joystick */
#define JOY_SEL_PIN                  (IO2_PIN_7) /* IO_Expander_2 */
#define JOY_DOWN_PIN                 (IO2_PIN_6) /* IO_Expander_2 */
#define JOY_LEFT_PIN                 (IO2_PIN_5) /* IO_Expander_2 */
#define JOY_RIGHT_PIN                (IO2_PIN_4) /* IO_Expander_2 */
#define JOY_UP_PIN                   (IO2_PIN_3) /* IO_Expander_2 */
#define JOY_NONE_PIN                 JOY_ALL_PINS
#define JOY_ALL_PINS                 (JOY_SEL_PIN | JOY_DOWN_PIN | JOY_LEFT_PIN | JOY_RIGHT_PIN | JOY_UP_PIN)

/* MEMS */
#define MEMS_INT1_PIN                (IO1_PIN_3) /* IO_Expander_1 */ /* Input */
#define MEMS_INT2_PIN                (IO1_PIN_2) /* IO_Expander_1 */ /* Input */
#define MEMS_ALL_PINS                (MEMS_INT1_PIN | MEMS_INT2_PIN)

#define AUDIO_RESET_PIN              (IO2_PIN_2) /* IO_Expander_2 */ /* Output */
#define MII_INT_PIN                  (IO2_PIN_0) /* IO_Expander_2 */ /* Output */
#define VBAT_DIV_PIN                 (IO1_PIN_0) /* IO_Expander_1 */ /* Output */

/**
  * @}
  */ 

/** @addtogroup STM3210C_EVAL_COM STM3210C EVAL COM
  * @{
  */
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART2
 */ 
#define EVAL_COM1                        USART2
#define EVAL_COM1_CLK_ENABLE()           __HAL_RCC_USART2_CLK_ENABLE()
#define EVAL_COM1_CLK_DISABLE()          __HAL_RCC_USART2_CLK_DISABLE()

#define AFIOCOM1_CLK_ENABLE()            __HAL_RCC_AFIO_CLK_ENABLE()
#define AFIOCOM1_CLK_DISABLE()           __HAL_RCC_AFIO_CLK_DISABLE()

#define EVAL_COM1_TX_PIN                 GPIO_PIN_5             /* PD.05*/
#define EVAL_COM1_TX_GPIO_PORT           GPIOD
#define EVAL_COM1_TX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define EVAL_COM1_TX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOD_CLK_DISABLE()

#define EVAL_COM1_RX_PIN                 GPIO_PIN_6             /* PD.06*/
#define EVAL_COM1_RX_GPIO_PORT           GPIOD
#define EVAL_COM1_RX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define EVAL_COM1_RX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOD_CLK_DISABLE()

#define EVAL_COM1_IRQn                   USART2_IRQn

#define COMx_CLK_ENABLE(__INDEX__)              do { if((__INDEX__) == COM1) EVAL_COM1_CLK_ENABLE();} while(0)
#define COMx_CLK_DISABLE(__INDEX__)             (((__INDEX__) == COM1) ? EVAL_COM1_CLK_DISABLE() : 0)

#define AFIOCOMx_CLK_ENABLE(__INDEX__)          do { if((__INDEX__) == COM1) AFIOCOM1_CLK_ENABLE();} while(0)
#define AFIOCOMx_CLK_DISABLE(__INDEX__)         (((__INDEX__) == COM1) ? AFIOCOM1_CLK_DISABLE() : 0)

#define AFIOCOMx_REMAP(__INDEX__)               (((__INDEX__) == COM1) ? (AFIO->MAPR |= (AFIO_MAPR_USART2_REMAP)) : 0)

#define COMx_TX_GPIO_CLK_ENABLE(__INDEX__)      do { if((__INDEX__) == COM1) EVAL_COM1_TX_GPIO_CLK_ENABLE();} while(0)
#define COMx_TX_GPIO_CLK_DISABLE(__INDEX__)     (((__INDEX__) == COM1) ? EVAL_COM1_TX_GPIO_CLK_DISABLE() : 0)

#define COMx_RX_GPIO_CLK_ENABLE(__INDEX__)      do { if((__INDEX__) == COM1) EVAL_COM1_RX_GPIO_CLK_ENABLE();} while(0)
#define COMx_RX_GPIO_CLK_DISABLE(__INDEX__)     (((__INDEX__) == COM1) ? EVAL_COM1_RX_GPIO_CLK_DISABLE() : 0)

/**
  * @}
  */ 

/** @addtogroup STM3210C_EVAL_BUS STM3210C EVAL BUS
  * @{
  */

/** 
  * @brief  IO Expander Interrupt line on EXTI  
  */ 
#define IOE_IT_PIN                       GPIO_PIN_14
#define IOE_IT_GPIO_PORT                 GPIOB
#define IOE_IT_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define IOE_IT_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()
#define IOE_IT_EXTI_IRQn                 EXTI15_10_IRQn       
#define IOE_IT_EXTI_IRQHANDLER           EXTI15_10_IRQHandler

/* Exported constant IO ------------------------------------------------------*/
#define IO1_I2C_ADDRESS                       0x82
#define IO2_I2C_ADDRESS                       0x88
#define TS_I2C_ADDRESS                        0x82

/*The Slave ADdress (SAD) associated to the LIS302DL is 001110xb. SDO pad can be used 
to modify less significant bit of the device address. If SDO pad is connected to voltage 
supply LSb is ‘1’ (address 0011101b) else if SDO pad is connected to ground LSb value is 
‘0’ (address 0011100b).*/
#define L1S302DL_I2C_ADDRESS                  0x38


/*##################### ACCELEROMETER ##########################*/
/* Read/Write command */
#define READWRITE_CMD                     ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD                  ((uint8_t)0x40)

/*##################### I2Cx ###################################*/
/* User can use this section to tailor I2Cx instance used and associated 
   resources */
/* Definition for I2Cx Pins */
#define EVAL_I2Cx_SCL_PIN                       GPIO_PIN_6        /* PB.06*/
#define EVAL_I2Cx_SCL_GPIO_PORT                 GPIOB
#define EVAL_I2Cx_SDA_PIN                       GPIO_PIN_7        /* PB.07*/
#define EVAL_I2Cx_SDA_GPIO_PORT                 GPIOB

/* Definition for I2Cx clock resources */
#define EVAL_I2Cx                               I2C1
#define EVAL_I2Cx_CLK_ENABLE()                  __HAL_RCC_I2C1_CLK_ENABLE()
#define EVAL_I2Cx_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define EVAL_I2Cx_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE() 

#define EVAL_I2Cx_FORCE_RESET()                 __HAL_RCC_I2C1_FORCE_RESET()
#define EVAL_I2Cx_RELEASE_RESET()               __HAL_RCC_I2C1_RELEASE_RESET()
    
/* Definition for I2Cx's NVIC */
#define EVAL_I2Cx_EV_IRQn                       I2C1_EV_IRQn
#define EVAL_I2Cx_EV_IRQHandler                 I2C1_EV_IRQHandler
#define EVAL_I2Cx_ER_IRQn                       I2C1_ER_IRQn
#define EVAL_I2Cx_ER_IRQHandler                 I2C1_ER_IRQHandler

/* I2C clock speed configuration (in Hz) */
#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                            400000
#endif /* I2C_SPEED */


/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define EVAL_I2Cx_TIMEOUT_MAX                   3000

/*##################### SPI3 ###################################*/
#define EVAL_SPIx                               SPI3
#define EVAL_SPIx_CLK_ENABLE()                  __HAL_RCC_SPI3_CLK_ENABLE()

#define EVAL_SPIx_SCK_GPIO_PORT                 GPIOC             /* PC.10*/
#define EVAL_SPIx_SCK_PIN                       GPIO_PIN_10
#define EVAL_SPIx_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define EVAL_SPIx_SCK_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOC_CLK_DISABLE()

#define EVAL_SPIx_MISO_MOSI_GPIO_PORT           GPIOC
#define EVAL_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define EVAL_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define EVAL_SPIx_MISO_PIN                      GPIO_PIN_11       /* PC.11*/
#define EVAL_SPIx_MOSI_PIN                      GPIO_PIN_12       /* PC.12*/
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define EVAL_SPIx_TIMEOUT_MAX                   1000

/**
  * @}
  */ 

/** @addtogroup STM3210C_EVAL_COMPONENT STM3210C EVAL COMPONENT
  * @{
  */

/*##################### LCD ###################################*/
/* Chip Select macro definition */
#define LCD_CS_LOW()       HAL_GPIO_WritePin(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()      HAL_GPIO_WritePin(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, GPIO_PIN_SET)

/** 
  * @brief  LCD Control Interface pins 
  */ 
#define LCD_NCS_PIN                             GPIO_PIN_2        /* PB.02*/
#define LCD_NCS_GPIO_PORT                       GPIOB
#define LCD_NCS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_NCS_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOB_CLK_DISABLE()

/*##################### SD ###################################*/
/* Chip Select macro definition */
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  SD Control Interface pins
  */
#define SD_CS_PIN                               GPIO_PIN_4        /* PA.04*/
#define SD_CS_GPIO_PORT                         GPIOA
#define SD_CS_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOA_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOA_CLK_DISABLE()

/**
  * @brief  SD Detect Interface pins
  */
#define SD_DETECT_PIN                           GPIO_PIN_0
#define SD_DETECT_GPIO_PORT                     GPIOE
#define SD_DETECT_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()
#define SD_DETECT_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOE_CLK_DISABLE()
#define SD_DETECT_EXTI_IRQn                     EXTI0_IRQn

/*##################### AUDIO ##########################*/
/**
  * @brief  AUDIO I2C Interface pins
  */
#define AUDIO_I2C_ADDRESS                     0x94
  
/**
  * @}
  */

/**
  * @}
  */ 



/** @addtogroup STM3210C_EVAL_Exported_Functions
  * @{
  */ 
uint32_t                BSP_GetVersion(void);
void                    BSP_LED_Init(Led_TypeDef Led);
void                    BSP_LED_On(Led_TypeDef Led);
void                    BSP_LED_Off(Led_TypeDef Led);
void                    BSP_LED_Toggle(Led_TypeDef Led);
void                    BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t                BSP_PB_GetState(Button_TypeDef Button);
#ifdef HAL_UART_MODULE_ENABLED
void                    BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef* huart);
#endif /* HAL_UART_MODULE_ENABLED */
#ifdef HAL_I2C_MODULE_ENABLED
uint8_t                 BSP_JOY_Init(JOYMode_TypeDef Joy_Mode);
JOYState_TypeDef        BSP_JOY_GetState(void);
#endif /* HAL_I2C_MODULE_ENABLED */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif
  
#endif /* __STM3210C_EVAL_H */

/**
  * @}
  */
  
/**
  * @}
  */
  
/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
