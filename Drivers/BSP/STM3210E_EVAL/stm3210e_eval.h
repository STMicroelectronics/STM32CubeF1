/**
  ******************************************************************************
  * @file    stm3210e_eval.h
  * @author  MCD Application Team
  * @version V7.0.0
  * @date    14-April-2017
  * @brief   This file contains definitions for STM3210E_EVAL's LEDs, 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3210E_EVAL_H
#define __STM3210E_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM3210E_EVAL
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM3210E_EVAL_Common
  * @{
  */ 

/** @defgroup STM3210E_EVAL_Exported_Types STM3210E EVAL Exported Types
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
  BUTTON_SEL    = 3,
  BUTTON_LEFT   = 4,
  BUTTON_RIGHT  = 5,
  BUTTON_DOWN   = 6,
  BUTTON_UP     = 7,

} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1,
  BUTTON_MODE_EVT  = 2

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

/** @defgroup STM3210E_EVAL_Exported_Constants STM3210E EVAL Exported Constants
  * @{
  */ 

/** 
  * @brief  Define for STM3210E_EVAL board  
  */ 
#if !defined (USE_STM3210E_EVAL)
 #define USE_STM3210E_EVAL
#endif
  
/** @defgroup STM3210E_EVAL_LED STM3210E EVAL LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_PIN_6             /* PF.06*/
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()
  
#define LED2_PIN                         GPIO_PIN_7             /* PF.07*/
#define LED2_GPIO_PORT                   GPIOF
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()


#define LED3_PIN                         GPIO_PIN_8            /* PF.08*/
#define LED3_GPIO_PORT                   GPIOF
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()


#define LED4_PIN                         GPIO_PIN_9            /* PF.09*/
#define LED4_GPIO_PORT                   GPIOF
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__LED__) do { if ((__LED__) == LED1) LED1_GPIO_CLK_ENABLE(); else\
                                           if ((__LED__) == LED2) LED2_GPIO_CLK_ENABLE(); else \
                                           if ((__LED__) == LED3) LED3_GPIO_CLK_ENABLE(); else\
                                           if ((__LED__) == LED4) LED4_GPIO_CLK_ENABLE();} while(0)

#define LEDx_GPIO_CLK_DISABLE(__LED__)   (((__LED__) == LED1) ? LED1_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED2) ? LED2_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED3) ? LED3_GPIO_CLK_DISABLE() :\
                                          ((__LED__) == LED4) ? LED4_GPIO_CLK_DISABLE() : 0 )

/**
  * @}
  */
  
/** @defgroup STM3210E_EVAL_BUTTON  STM3210E EVAL BUTTON
  * @{
  */  
#define JOYn                             5
#define BUTTONn                          3 + JOYn

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
#define KEY_BUTTON_PIN                      GPIO_PIN_8             /* PG.08*/
#define KEY_BUTTON_GPIO_PORT                GPIOG
#define KEY_BUTTON_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOG_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn                EXTI9_5_IRQn

/**
 * @brief Wake-up push-button
 */
#define WAKEUP_BUTTON_PIN                   GPIO_PIN_0             /* PA.00*/
#define WAKEUP_BUTTON_GPIO_PORT             GPIOA
#define WAKEUP_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define WAKEUP_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define WAKEUP_BUTTON_EXTI_IRQn             EXTI0_IRQn

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_JOY_PIN                       GPIO_PIN_13             /* PG.13*/
#define RIGHT_JOY_GPIO_PORT                 GPIOG
#define RIGHT_JOY_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOG_CLK_ENABLE()
#define RIGHT_JOY_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOG_CLK_DISABLE()
#define RIGHT_JOY_EXTI_IRQn                 EXTI15_10_IRQn

/**
 * @brief Joystick Left push-button
 */    
#define LEFT_JOY_PIN                        GPIO_PIN_14             /* PG.14*/
#define LEFT_JOY_GPIO_PORT                  GPIOG
#define LEFT_JOY_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOG_CLK_ENABLE()
#define LEFT_JOY_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOG_CLK_DISABLE()
#define LEFT_JOY_EXTI_IRQn                  EXTI15_10_IRQn

/**
 * @brief Joystick Up push-button
 */
#define UP_JOY_PIN                          GPIO_PIN_15             /* PG.15*/
#define UP_JOY_GPIO_PORT                    GPIOG
#define UP_JOY_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOG_CLK_ENABLE()
#define UP_JOY_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOG_CLK_DISABLE()
#define UP_JOY_EXTI_IRQn                    EXTI15_10_IRQn

/**
 * @brief Joystick Down push-button
 */   
#define DOWN_JOY_PIN                        GPIO_PIN_3             /* PD.03*/
#define DOWN_JOY_GPIO_PORT                  GPIOD
#define DOWN_JOY_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOD_CLK_ENABLE()
#define DOWN_JOY_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOD_CLK_DISABLE()
#define DOWN_JOY_EXTI_IRQn                  EXTI3_IRQn

/**
 * @brief Joystick Sel push-button
 */  
#define SEL_JOY_PIN                         GPIO_PIN_7             /* PG.07*/
#define SEL_JOY_GPIO_PORT                   GPIOG
#define SEL_JOY_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOG_CLK_ENABLE()
#define SEL_JOY_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOG_CLK_DISABLE()
#define SEL_JOY_EXTI_IRQn                   EXTI9_5_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__BUTTON__) do { if ((__BUTTON__) == BUTTON_TAMPER) TAMPER_BUTTON_GPIO_CLK_ENABLE(); else\
                                                 if ((__BUTTON__) == BUTTON_KEY) KEY_BUTTON_GPIO_CLK_ENABLE(); else\
                                                 if ((__BUTTON__) == BUTTON_WAKEUP) WAKEUP_BUTTON_GPIO_CLK_ENABLE(); else\
                                                 if ((__BUTTON__) == BUTTON_SEL) SEL_JOY_GPIO_CLK_ENABLE(); else\
                                                 if ((__BUTTON__) == BUTTON_LEFT) LEFT_JOY_GPIO_CLK_ENABLE(); else\
                                                 if ((__BUTTON__) == BUTTON_RIGHT) RIGHT_JOY_GPIO_CLK_ENABLE(); else\
                                                 if ((__BUTTON__) == BUTTON_DOWN) DOWN_JOY_GPIO_CLK_ENABLE(); else\
                                                 if ((__BUTTON__) == BUTTON_UP) UP_JOY_GPIO_CLK_ENABLE();} while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__BUTTON__)    (((__BUTTON__) == BUTTON_TAMPER) ? TAMPER_BUTTON_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_KEY) ? KEY_BUTTON_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_WAKEUP) ? WAKEUP_BUTTON_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_SEL) ? SEL_JOY_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_LEFT) ? LEFT_JOY_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_RIGHT) ? RIGHT_JOY_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_DOWN) ? DOWN_JOY_GPIO_CLK_DISABLE()  :\
                                                 ((__BUTTON__) == BUTTON_UP) ? UP_JOY_GPIO_CLK_DISABLE() : 0 )

#define JOYx_GPIO_CLK_ENABLE(__JOY__)    do { if ((__JOY__) == JOY_SEL) SEL_JOY_GPIO_CLK_ENABLE(); else\
                                              if ((__JOY__) == JOY_LEFT) LEFT_JOY_GPIO_CLK_ENABLE(); else\
                                              if ((__JOY__) == JOY_RIGHT) RIGHT_JOY_GPIO_CLK_ENABLE(); else\
                                              if ((__JOY__) == JOY_DOWN) DOWN_JOY_GPIO_CLK_ENABLE(); else\
                                              if ((__JOY__) == JOY_UP) UP_JOY_GPIO_CLK_ENABLE();} while(0)

#define JOYx_GPIO_CLK_DISABLE(__JOY__)   (((__JOY__) == JOY_SEL) ? SEL_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_LEFT) ? LEFT_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_RIGHT) ? RIGHT_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_DOWN) ? DOWN_JOY_GPIO_CLK_DISABLE() :\
                                          ((__JOY__) == JOY_UP) ? UP_JOY_GPIO_CLK_DISABLE() : 0 )

/**
  * @}
  */ 

/** @addtogroup STM3210E_EVAL_COM
  * @{
  */
#define COMn                             2

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK_ENABLE()           __HAL_RCC_USART1_CLK_ENABLE()
#define EVAL_COM1_CLK_DISABLE()          __HAL_RCC_USART1_CLK_DISABLE()

#define EVAL_COM1_TX_PIN                 GPIO_PIN_9             /* PA.09*/
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_COM1_TX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()

#define EVAL_COM1_RX_PIN                 GPIO_PIN_10             /* PA.10*/
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_COM1_RX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()

#define EVAL_COM1_IRQn                   USART1_IRQn

/**
 * @brief Definition for COM port2, connected to USART2
 */ 
#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK_ENABLE()           __HAL_RCC_USART2_CLK_ENABLE()
#define EVAL_COM2_CLK_DISABLE()          __HAL_RCC_USART2_CLK_DISABLE()

#define EVAL_COM2_TX_PIN                 GPIO_PIN_2             /* PA.02*/
#define EVAL_COM2_TX_GPIO_PORT           GPIOA
#define EVAL_COM2_TX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_COM2_TX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()

#define EVAL_COM2_RX_PIN                 GPIO_PIN_3             /* PA.03*/
#define EVAL_COM2_RX_GPIO_PORT           GPIOA
#define EVAL_COM2_RX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_COM2_RX_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()

#define EVAL_COM2_IRQn                   USART2_IRQn

#define COMx_CLK_ENABLE(__INDEX__)              do { if((__INDEX__) == COM1) EVAL_COM1_CLK_ENABLE(); else\
                                                     if((__INDEX__) == COM2) EVAL_COM2_CLK_ENABLE();} while(0)

#define COMx_CLK_DISABLE(__INDEX__)             (((__INDEX__) == COM1) ? EVAL_COM1_CLK_DISABLE() :\
                                                 ((__INDEX__) == COM2) ? EVAL_COM2_CLK_DISABLE() : 0)

#define COMx_TX_GPIO_CLK_ENABLE(__INDEX__)      do { if((__INDEX__) == COM1) EVAL_COM1_TX_GPIO_CLK_ENABLE(); else\
                                                     if((__INDEX__) == COM2) EVAL_COM2_TX_GPIO_CLK_ENABLE();} while(0)

#define COMx_TX_GPIO_CLK_DISABLE(__INDEX__)     (((__INDEX__) == COM1) ? EVAL_COM1_TX_GPIO_CLK_DISABLE() :\
                                                 ((__INDEX__) == COM2) ? EVAL_COM2_TX_GPIO_CLK_DISABLE() : 0)

#define COMx_RX_GPIO_CLK_ENABLE(__INDEX__)      do { if((__INDEX__) == COM1) EVAL_COM1_RX_GPIO_CLK_ENABLE(); else\
                                                     if((__INDEX__) == COM2) EVAL_COM2_RX_GPIO_CLK_ENABLE();} while(0)

#define COMx_RX_GPIO_CLK_DISABLE(__INDEX__)     (((__INDEX__) == COM1) ? EVAL_COM1_RX_GPIO_CLK_DISABLE() :\
                                                 ((__INDEX__) == COM2) ? EVAL_COM2_RX_GPIO_CLK_DISABLE() : 0)

/**
  * @}
  */ 

/** @defgroup STM3210E_EVAL_BUS STM3210E EVAL BUS
  * @{
  */

/* Exported constant IO ------------------------------------------------------*/
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

/* I2C clock speed configuration (in Hz) 
   WARNING: 
   Make sure that this define is not already declared in other files (ie. 
   stm3210e_eval.h file). It can be used in parallel by other modules. */
#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                            100000
#endif /* I2C_SPEED */


/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define EVAL_I2Cx_TIMEOUT_MAX                   3000

/*##################### SPIx ###################################*/
#define EVAL_SPIx                               SPI1
#define EVAL_SPIx_CLK_ENABLE()                  __HAL_RCC_SPI1_CLK_ENABLE()

#define EVAL_SPIx_SCK_GPIO_PORT                 GPIOA             /* PA.05*/
#define EVAL_SPIx_SCK_PIN                       GPIO_PIN_5
#define EVAL_SPIx_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_SPIx_SCK_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

#define EVAL_SPIx_MISO_MOSI_GPIO_PORT           GPIOA
#define EVAL_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define EVAL_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define EVAL_SPIx_MISO_PIN                      GPIO_PIN_6       /* PA.06*/
#define EVAL_SPIx_MOSI_PIN                      GPIO_PIN_7       /* PA.07*/
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define EVAL_SPIx_TIMEOUT_MAX                   1000

/**
  * @}
  */ 

/** @defgroup STM3210E_EVAL_COMPONENT STM3210E EVAL COMPONENT
  * @{
  */

/*##################### FLASH SPI ###################################*/
/**
  * @brief  M25P FLASH SPI Chip Select macro definition 
  */
#define FLASH_SPI_CS_LOW()       HAL_GPIO_WritePin(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_RESET)
#define FLASH_SPI_CS_HIGH()      HAL_GPIO_WritePin(FLASH_SPI_CS_GPIO_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  M25P FLASH SPI Control Interface pins
  */
#define FLASH_SPI_CS_PIN                           GPIO_PIN_2        /* PB.02*/
#define FLASH_SPI_CS_GPIO_PORT                     GPIOB
#define FLASH_SPI_CS_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define FLASH_SPI_CS_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  M25P FLASH SPI supported commands
  */  
#define FLASH_SPI_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define FLASH_SPI_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define FLASH_SPI_CMD_WREN           0x06  /*!< Write enable instruction */
#define FLASH_SPI_CMD_READ           0x03  /*!< Read from Memory instruction */
#define FLASH_SPI_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define FLASH_SPI_CMD_RDID           0x9F  /*!< Read identification */
#define FLASH_SPI_CMD_SE             0xD8  /*!< Sector Erase instruction */
#define FLASH_SPI_CMD_BE             0xC7  /*!< Bulk Erase instruction */

#define FLASH_SPI_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define FLASH_SPI_DUMMY_BYTE         0xA5
#define FLASH_SPI_PAGESIZE           0x100

#define FLASH_SPI_M25P128_ID         0x202018
#define FLASH_SPI_M25P64_ID          0x202017


/*##################### AUDIO ##########################*/
/**
  * @brief  AUDIO I2C Interface pins
  */
#define AUDIO_I2C_ADDRESS                     0x27
  
  /* Audio Reset Pin definition */
#define AUDIO_RESET_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_RESET_PIN                       GPIO_PIN_11
#define AUDIO_RESET_GPIO                      GPIOG

/**
  * @}
  */

/**
  * @}
  */ 



/** @addtogroup STM3210E_EVAL_Exported_Functions
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
uint8_t                 BSP_JOY_Init(JOYMode_TypeDef Joy_Mode);
JOYState_TypeDef        BSP_JOY_GetState(void);

/**
  * @}
  */


/**
  * @}
  */
  
/**
  * @}
  */
  
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif
  
#endif /* __STM3210E_EVAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

