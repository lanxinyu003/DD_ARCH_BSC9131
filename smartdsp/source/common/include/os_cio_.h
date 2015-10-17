/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_cio_.h,v 1.8 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_cio_.h,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************
 
 @File          os_cio_.h

 @Description   OS CIO internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CIO_H_
#define __OS_CIO_H_


#define CIO_NAME_SIZE   4


typedef struct
{
    char        name[CIO_NAME_SIZE];
    uint32_t    status;
    void        *device_handle;
    os_status   (*device_open)(void *device_handle, void *lld_params);
    void *      (*channel_open)(void *device_handle, int mode, cio_lld_ch_open_t *lld_ch_params);
    os_status   (*channel_close)(void *ch_handle, int mode);
    uint16_t    (*channel_tx)(void *ch_handle, uint8_t *data, uint16_t size);
    os_status   (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    os_status   (*device_ctrl)(void *device_handle, uint32_t command, void *param);
 
} cio_interface_t;



#endif // __OS_CIO_H_
