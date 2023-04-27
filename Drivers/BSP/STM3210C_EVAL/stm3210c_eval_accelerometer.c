/**
  ******************************************************************************
  * @file    stm3210c_eval_accelerometer.c
  * @author  MCD Application Team
  * @version V6.1.0
  * @date    14-April-2017
  * @brief   This file provides a set of functions needed to manage the ACCELEROMETER
  *          MEMS accelerometer available on STM3210C_EVAL board.
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
#include "stm3210c_eval_accelerometer.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM3210C_EVAL
  * @{
  */ 

/** @defgroup STM3210C_EVAL_ACCELEROMETER STM3210C EVAL ACCELEROMETER
  * @brief  This file includes the motion sensor driver for ACCELEROMETER motion sensor 
  *         devices.
  * @{
  */
  
/** @defgroup STM3210C_EVAL_ACCELEROMETER_Private_Variables STM3210C EVAL ACCELEROMETER Private Variables
  * @{
  */ 
static ACCELERO_DrvTypeDef *AcceleroDrv;

/**
  * @}
  */


/** @defgroup STM3210C_EVAL_ACCELEROMETER_Exported_Functions STM3210C EVAL ACCELEROMETER Exported Functions
  * @{
  */

/**
  * @brief  Set ACCELEROMETER Initialization.
  */
uint8_t BSP_ACCELERO_Init(void)
{ 
  uint8_t ret = ACCELERO_ERROR;
  uint16_t ctrl = 0x0000;
  LIS302DL_InitTypeDef lis302dl_initstruct;
  LIS302DL_FilterConfigTypeDef lis302dl_filter={0,0,0};

  if(Lis302dlDrv.ReadID() == I_AM_LIS302DL)
  {
    /* Initialize the gyroscope driver structure */
    AcceleroDrv = &Lis302dlDrv;

    /* Set configuration of LIS302DL MEMS Accelerometer *********************/
    lis302dl_initstruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
    lis302dl_initstruct.Output_DataRate = LIS302DL_DATARATE_100;
    lis302dl_initstruct.Axes_Enable = LIS302DL_XYZ_ENABLE;
    lis302dl_initstruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
    lis302dl_initstruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
    
    /* Configure MEMS: data rate, power mode, full scale, self test and axes */
    ctrl = (uint16_t) (lis302dl_initstruct.Output_DataRate | lis302dl_initstruct.Power_Mode | \
                      lis302dl_initstruct.Full_Scale | lis302dl_initstruct.Self_Test | \
                      lis302dl_initstruct.Axes_Enable);
    
    /* Configure the accelerometer main parameters */
    AcceleroDrv->Init(ctrl);
    
    /* MEMS High Pass Filter configuration */
    lis302dl_filter.HighPassFilter_Data_Selection = LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER;
    lis302dl_filter.HighPassFilter_CutOff_Frequency = LIS302DL_HIGHPASSFILTER_LEVEL_1;
    lis302dl_filter.HighPassFilter_Interrupt = LIS302DL_HIGHPASSFILTERINTERRUPT_1_2;
    
    /* Configure MEMS high pass filter cut-off level, interrupt and data selection bits */                     
    ctrl = (uint8_t)(lis302dl_filter.HighPassFilter_Data_Selection | \
                      lis302dl_filter.HighPassFilter_CutOff_Frequency | \
                      lis302dl_filter.HighPassFilter_Interrupt);

    /* Configure the accelerometer LPF main parameters */
    AcceleroDrv->FilterConfig(ctrl);

    ret = ACCELERO_OK;
  }
  else
  {
    ret = ACCELERO_ERROR;
  }
  
  return ret;
}

/**
  * @brief  Read ID of Accelerometer component
  * @retval ID
  */
uint8_t BSP_ACCELERO_ReadID(void)
{
  uint8_t id = 0x00;

  if(AcceleroDrv->ReadID != NULL)
  {
    id = AcceleroDrv->ReadID();
  }  
  return id;
}

/**
  * @brief  Reboot memory content of ACCELEROMETER
  */
void BSP_ACCELERO_Reset(void)
{
  if(AcceleroDrv->Reset != NULL)
{
    AcceleroDrv->Reset();
  }
}

/**
  * @brief  Config Accelerometer click IT 
  */
void BSP_ACCELERO_Click_ITConfig(void)
{
  if(AcceleroDrv->ConfigIT!= NULL)
  {
    AcceleroDrv->ConfigIT();
  }
}


/**
  * @brief  Clear Accelerometer click IT 
  */
void BSP_ACCELERO_Click_ITClear(void)
{
  if(AcceleroDrv->ClearIT!= NULL)
  {
    AcceleroDrv->ClearIT();
  }
}

/**
  * @brief  Get XYZ acceleration
  * @param pDataXYZ: angular acceleration on X/Y/Z axis
  */
void BSP_ACCELERO_GetXYZ(int16_t *pDataXYZ)
{
  int16_t SwitchXY = 0;
  
  if(AcceleroDrv->GetXYZ!= NULL)
  {   
    AcceleroDrv->GetXYZ(pDataXYZ);
    
    /* Switch X and Y Axis in case of MEMS LIS302DL */
    if(AcceleroDrv == &Lis302dlDrv)
    { 
      SwitchXY  = pDataXYZ[0];
      pDataXYZ[0] = pDataXYZ[1];
      /* Invert Y Axis to be conpliant with LIS3DSH */
      pDataXYZ[1] = -SwitchXY;
    } 
  }
}


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
  

