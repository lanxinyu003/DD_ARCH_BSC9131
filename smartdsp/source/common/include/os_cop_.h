/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_cop_.h,v 1.13 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_cop_.h,v $
 $Revision: 1.13 $
******************************************************************************/

/******************************************************************************
 
 @File          os_cop_.h

 @Description   OS COP internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_COP_H_
#define __OS_COP_H_


#define COP_NAME_SIZE   9

typedef struct cop_interface_t
{
    char                name[COP_NAME_SIZE];
    /**< COP device identification name. */
    uint32_t            status;
    /**< COP Device status. */
    os_status           (*device_open)(void *device_handle, void *lld_params);
    /**< Device-open function. */
    os_status           (*device_ctrl)(void *device_handle, uint32_t command, void *param);
    /**< Device-control function. */
    void*               (*channel_open)(void *device_handle, cop_lld_ch_open_t *ch_params);
    /**< Channel-open function. */
    os_status           (*channel_close)(void *ch_handle);
    /**< Channel-close function. */
    os_status           (*channel_dispatch)(void *ch_handle, void *jobs, int *num_jobs);
    /**< Channel-transmit function. */
    os_status           (*channel_ctrl)(void *ch_handle, uint32_t command, void *param);
    /**< Channel-control function. */
    os_cop_callback     dispatch_callback;
    /**< Application's Dispatch callback. */
    os_cop_error        error_callback;
    /**< Application's callback for underrun error. */
    void                *device_handle;
    /**< LLD Handle. */
    cop_interface_stat_t cop_stat;
    /**< COP interface statistics structure. */
    os_status           (*device_close)(void *device_handle);
    /**< Device-close function. */
    cop_channel_t*      open_channels;
    /**< COP device opened channels*/
    uint8_t             max_num_of_channels;
    /**< Max number of channels allowed on device. */
    uint16_t            max_job_queue_size;
    /**< Max size of Job queue. */
} cop_interface_t;



#endif // __OS_COP_H_
