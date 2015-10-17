/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/09/23 07:23:01 $
 $Id: os_cop_init.c,v 1.22 2013/09/23 07:23:01 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_cop_init.c,v $
 $Revision: 1.22 $
******************************************************************************/

/******************************************************************************
 
 @File          os_cop_init.c

 @Description   OS COP layer initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_cop_lld_.h"
#include "os_cop_.h"
#include "os_rm_.h"

cop_interface_t         *g_cop_list;
uint16_t                g_total_cop_devices;

/*****************************************************************************/
os_status osCopInitialize(uint16_t total_cop_devices)
{
    g_total_cop_devices = total_cop_devices;

    g_cop_list = (cop_interface_t *)
    osMalloc((sizeof(cop_interface_t) * g_total_cop_devices), OS_MEM_LOCAL);

    if (g_cop_list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }

    return OS_SUCCESS;
}


/*****************************************************************************/
cop_dev_handle copRegister(char *device_name, cop_register_params_t *register_params)
{
    int         i, j;
 
    OS_ASSERT_COMPILER_COND(g_total_cop_devices > 0);
 
    for (i=0; i < g_total_cop_devices; i++)
    {
        if (g_cop_list[i].status == OS_RESOURCE_UNUSED)
        {
            for (j=0; j < COP_NAME_SIZE; j++)
            {
                g_cop_list[i].name[j] = device_name[j];
            }
            g_cop_list[i].status = OS_RESOURCE_ACQUIRED;
            g_cop_list[i].device_open = register_params->device_open;
            g_cop_list[i].channel_open = register_params->channel_open;
            g_cop_list[i].channel_close = register_params->channel_close;
            g_cop_list[i].channel_dispatch = register_params->channel_dispatch;
            g_cop_list[i].device_ctrl = register_params->device_ctrl;
            g_cop_list[i].channel_ctrl = register_params->channel_ctrl;
            g_cop_list[i].max_job_queue_size = register_params->max_job_queue_size;
            g_cop_list[i].device_handle = register_params->lld_handle;
            g_cop_list[i].dispatch_callback = NULL;
            g_cop_list[i].error_callback = NULL;
            g_cop_list[i].device_close = register_params->device_close;
            return &g_cop_list[i];
        }
    }

    return NULL;
}



/*****************************************************************************/
cop_dev_handle  osCopDeviceOpen(char                    *device_name,
                                cop_dev_open_params_t   *dev_open_params)
{
    int i, j;
    uint8_t     numOfChannels = dev_open_params->max_num_of_channels;
 
      if ( numOfChannels == 0 || numOfChannels > MAX_NUM_OF_CHANNELS)
        numOfChannels = MAX_NUM_OF_CHANNELS;
    OS_ASSERT_COMPILER_COND(g_total_cop_devices > 0);

    for (i=0; i < g_total_cop_devices; i++)
    {
        if (g_cop_list[i].status == OS_RESOURCE_DELETED || g_cop_list[i].status == OS_RESOURCE_ACQUIRED)
        {
            bool name = TRUE;

            for (j=0; j < COP_NAME_SIZE && device_name[j]!='\0'; j++)
            {
                if (g_cop_list[i].name[j] != device_name[j])
                {
                    name = FALSE;
                    break;
                }
            }
            if (name)
            {
                g_cop_list[i].open_channels = (cop_channel_t*)
                                              osMalloc(sizeof(cop_channel_t) * numOfChannels, OS_MEM_LOCAL);
                if (g_cop_list[i].open_channels == NULL)
                    return NULL;
                g_cop_list[i].dispatch_callback = dev_open_params->dispatch_callback;
                g_cop_list[i].error_callback = dev_open_params->error_callback;
                if (g_cop_list[i].device_open(g_cop_list[i].device_handle, dev_open_params->lld_params) == OS_SUCCESS)
                {
                    g_cop_list[i].max_num_of_channels = numOfChannels;
                    g_cop_list[i].status = OS_RESOURCE_USED;
#if COP_STATISTICS == ON
                    g_cop_list[i].cop_stat.dispatched_jobs = 0;
                    g_cop_list[i].cop_stat.reaped_jobs = 0;
                    g_cop_list[i].cop_stat.failed_jobs = 0;
#endif
                    return &g_cop_list[i];
                }
                else
                {
                    return NULL;
                }
            }
        }
    }

    return NULL;
}
/*****************************************************************************/
os_status  osCopDeviceClose(cop_dev_handle         deviceHandle)
{
    cop_interface_t *copDev = (cop_interface_t*)deviceHandle;
    int i;
 
    OS_ASSERT_COMPILER_COND(deviceHandle != NULL);
    OS_ASSERT_COND(copDev->status > OS_RESOURCE_DELETED)
    OS_ASSERT_COND(copDev->device_close != NULL);

    if (copDev->status < OS_RESOURCE_ACQUIRED)
        return OS_ERR_ALREADY_FREE;
 
    for (i = 0 ; i < copDev->max_num_of_channels; i++)
    {
        if (copDev->open_channels[i] == NULL)
                continue;
        osCopChannelClose(copDev->open_channels[i]);
        copDev->open_channels[i] = NULL;
    }
    copDev->dispatch_callback = NULL;
    copDev->error_callback = NULL;

    copDev->device_close(copDev->device_handle);
 
    osFree(copDev->open_channels);
 
    copDev->open_channels = NULL;
    copDev->max_num_of_channels = NULL;
 
    copDev->status = OS_RESOURCE_DELETED;

    return OS_SUCCESS;
}
/*****************************************************************************/
static os_status copChannelOpen(cop_dev_handle       cop_handle,
                                cop_channel_t        *cop_ch,
                                cop_ch_open_params_t *ch_open_params,
                                int                  mode,
                                os_queue_handle      queue)
{
    cop_interface_t     *cop_dev = (cop_interface_t *)cop_handle;
    cop_lld_ch_open_t   lld_ch_params;
    cop_channel_        *ch, *tmpCH;
    int i;


    OS_ASSERT_COND(cop_dev != NULL);
    OS_ASSERT_COND(cop_dev->status == OS_RESOURCE_USED);

    OS_ASSERT_COND(cop_ch != NULL);
 
    OS_ASSERT_COND(ch_open_params != NULL);
    OS_ASSERT_COND(cop_dev->max_job_queue_size >= ch_open_params->num_jobs);
    if (!osMemTypeIsValid(ch_open_params->heap))
    {
        OS_ASSERT;
        ch_open_params->heap = OS_MEM_LOCAL;
    }
 
    ch = osMalloc(sizeof(cop_channel_), ch_open_params->heap);
    if (ch == NULL)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }
 
    (cop_channel_ *)*cop_ch = ch;

 
    for (i = 0; i < cop_dev->max_num_of_channels; i++)
    {
        tmpCH = (cop_channel_t)cop_dev->open_channels[i];
        if (tmpCH == NULL || tmpCH->status < OS_RESOURCE_ACQUIRED)
        {
            cop_dev->open_channels[i] = ch;
            break;
        }
    }
    if(i >= cop_dev->max_num_of_channels)
    {
    	OS_ASSERT;
        return OS_ERR_FULL;
    }
    ch->status = OS_RESOURCE_USED;
    ch->cop_dev = cop_handle;
    ch->flags = (uint8_t)mode;
    ch->job_queue = queue;
    ch->cb_job = NULL;
    ch->dispatch_callback = cop_dev->dispatch_callback;
 
    // Open the channel on the LLD
    lld_ch_params.channel_num = ch_open_params->channel_num;
    lld_ch_params.number_of_jobs = ch_open_params->num_jobs;
    lld_ch_params.cop_ch = ch;
    lld_ch_params.lld_params = ch_open_params->lld_params;

    ch->lld_channel = cop_dev->channel_open(cop_dev->device_handle, &lld_ch_params);
    if (ch->lld_channel == NULL)
    {
        OS_ASSERT;
        RETURN_ERROR(OS_FAIL);
    }
    return OS_SUCCESS;
}

static os_status copChannelClose(cop_channel_t cop_ch)
{
    cop_channel_    *ch = (cop_channel_ *)cop_ch;
    cop_interface_t *cop_dev = (cop_interface_t *)ch->cop_dev;
    os_status       status;

    OS_ASSERT_COND(cop_ch != NULL);
 
    cop_dev->channel_close(ch->lld_channel);
 
    ch->status = OS_RESOURCE_DELETED;
    ch->cop_dev = NULL;
    ch->flags = NULL;
    ch->job_queue = NULL;
    ch->cb_job = NULL;
    ch->dispatch_callback = NULL;
 
 
    status = osFree(ch);
 
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    return status;
}
/*****************************************************************************/
os_status   osCopChannelOpen(cop_dev_handle       cop_handle,
                             cop_channel_t        *cop_ch,
                             cop_ch_open_params_t *ch_open_params)
{
    os_status           status;
    os_queue_handle queue;
    const int mode = (COP_CH_REAP | COP_CH_DISPATCH);
 
    // Find and create a queue for dispatched jobs
    status = osQueueFind(&queue, FALSE);
    if (status != OS_SUCCESS)
    {
        OS_ASSERT;
        RETURN_ERROR(status);
    }
 
    status = osQueueCreate(queue, ch_open_params->num_jobs);
    if (status != OS_SUCCESS)
    {
        OS_ASSERT;
        RETURN_ERROR(status);
    }
 
    return copChannelOpen(cop_handle, cop_ch, ch_open_params, mode, queue);
 
}
/*****************************************************************************/
os_status   osCopChannelClose(cop_channel_t cop_ch)
{
    os_status           status;
    bool                shared_ch;
    os_queue_handle     queue;
    cop_channel_ *ch = (cop_channel_ *)cop_ch;
 
    OS_ASSERT_COND(cop_ch != NULL);
 
    if (ch->status < OS_RESOURCE_ACQUIRED)
        return OS_ERR_ALREADY_FREE;
 
    queue = ch->job_queue;
 
    shared_ch = (COP_CH_REAP | COP_CH_DISPATCH) != (ch->flags & (COP_CH_REAP | COP_CH_DISPATCH));
    status = copChannelClose(cop_ch);
 
    if (status != OS_SUCCESS)
    {
        OS_ASSERT;
        RETURN_ERROR(status);
    }
 
    if (!shared_ch && osQueueShared(queue) == OS_QUEUE_PRIVATE)
        status = osQueueDeleteUnsafe(queue);
 
    return status;
}
/*****************************************************************************/
os_status   osCopSharedChannelOpen(cop_dev_handle       cop_handle,
                                   cop_channel_t        *cop_ch,
                                   cop_ch_open_params_t *ch_open_params,
                                   int                  mode,
                                   os_queue_handle      queue
                                   )
{
    os_status           status;
    uint32_t            queue_size;
    os_queue_shared     queue_shared;
    bool                shared_ch = (mode != (COP_CH_REAP | COP_CH_DISPATCH));

    status = osQueueSize(queue, &queue_size);
    if (status != OS_SUCCESS)
    {
        OS_ASSERT;
        RETURN_ERROR(status);
    }
    if (queue_size < ch_open_params->num_jobs)
    {
        OS_ASSERT;
        return OS_ERR_COP_QUEUE_SIZE_MISMATCH;
    }
 
    /* A channel that can't reap and dispatch MUST have a shared queue */
    queue_shared = osQueueShared(queue);
    if (shared_ch && (queue_shared != OS_QUEUE_SHARED))
    {
        OS_ASSERT;
        RETURN_ERROR(OS_ERR_Q_UNAVAILABLE);
    }

    return copChannelOpen(cop_handle, cop_ch, ch_open_params, mode, queue);

}


