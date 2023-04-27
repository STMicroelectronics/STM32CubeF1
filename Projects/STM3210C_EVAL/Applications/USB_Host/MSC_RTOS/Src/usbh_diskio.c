/**
  ******************************************************************************
  * @file    USB_Host/MSC_RTOS/Src/usbh_diskio.c
  * @author  MCD Application Team
  * @brief   USB diskio interface
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "ffconf.h"
#include "diskio.h"
#include "usbh_msc.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
extern USBH_HandleTypeDef hUSBHost;

/* Private function prototypes ----------------------------------------------- */
DWORD get_fattime(void);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Initializes a Disk
  * @param  pdrv: Physical drive number
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize(BYTE pdrv)
{
  return RES_OK;
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_status(BYTE pdrv)
{
  DRESULT res = RES_ERROR;

  if (USBH_MSC_UnitIsReady(&hUSBHost, pdrv))
  {
    res = RES_OK;
  }
  else
  {
    res = RES_ERROR;
  }

  return res;
}

/**
  * @brief  Reads Sector
  * @param  pdrv: Physical drive number
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read
  * @retval DRESULT: Operation result
  */
DRESULT disk_read(BYTE pdrv, BYTE * buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef status = USBH_OK;
  DWORD scratch[_MAX_SS / 4];

  if ((DWORD) buff & 3)         /* DMA Alignment issue, do single up to aligned 
                                 * buffer */
  {
    while ((count--) && (status == USBH_OK))
    {
      status =
        USBH_MSC_Read(&hUSBHost, pdrv, sector + count, (uint8_t *) scratch, 1);
      if (status == USBH_OK)
      {
        memcpy(&buff[count * _MAX_SS], scratch, _MAX_SS);
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    status = USBH_MSC_Read(&hUSBHost, pdrv, sector, buff, count);
  }

  if (status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSBHost, pdrv, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}

/**
  * @brief  Writes Sector
  * @param  pdrv: Physical drive number
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE
DRESULT disk_write(BYTE pdrv, const BYTE * buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;
  USBH_StatusTypeDef status = USBH_OK;
  DWORD scratch[_MAX_SS / 4];

  if ((DWORD) buff & 3)         /* DMA Alignment issue, do single up to aligned 
                                 * buffer */
  {
    while (count--)
    {
      memcpy(scratch, &buff[count * _MAX_SS], _MAX_SS);

      status =
        USBH_MSC_Write(&hUSBHost, pdrv, sector + count, (BYTE *) scratch, 1);
      if (status == USBH_FAIL)
      {
        break;
      }
    }
  }
  else
  {
    status = USBH_MSC_Write(&hUSBHost, pdrv, sector, (BYTE *) buff, count);
  }

  if (status == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&hUSBHost, pdrv, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = RES_WRPRT;
      break;

    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}
#endif

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
  DRESULT res = RES_OK;
  MSC_LUNTypeDef info;

  switch (cmd)
  {
    /* Make sure that no pending write process */
  case CTRL_SYNC:
    res = RES_OK;
    break;

    /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT:
    if (USBH_MSC_GetLUNInfo(&hUSBHost, pdrv, &info) == USBH_OK)
    {
      *(DWORD *) buff = info.capacity.block_nbr;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

  case GET_SECTOR_SIZE:        /* Get R/W sector size (WORD) */
    if (USBH_MSC_GetLUNInfo(&hUSBHost, pdrv, &info) == USBH_OK)
    {
      *(DWORD *) buff = info.capacity.block_size;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

    /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE:

    if (USBH_MSC_GetLUNInfo(&hUSBHost, pdrv, &info) == USBH_OK)
    {
      *(DWORD *) buff = info.capacity.block_size;
      res = RES_OK;
    }
    else
    {
      res = RES_ERROR;
    }
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}
#endif

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  return 0;
}
