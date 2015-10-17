/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/09/16 11:43:46 $
 $Id: os_bio_init.c,v 1.36 2014/09/16 11:43:46 b33105 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_bio_init.c,v $
 $Revision: 1.36 $
******************************************************************************/

/******************************************************************************
 
 @File          os_bio_init.c

 @Description   OS BIO layer initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_bio_lld_.h"
#include "os_bio_.h"
#include "os_rm_.h"


bio_interface_t *g_bio_list;
uint16_t        g_total_bio_devices;


/*****************************************************************************/
os_status osBioInitialize(uint16_t total_bio_devices)
{
    g_total_bio_devices = total_bio_devices;
 
    g_bio_list = (bio_interface_t *)
        osMalloc((sizeof(bio_interface_t) * g_total_bio_devices), OS_MEM_LOCAL);
 
    if (g_bio_list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }
 
    return OS_SUCCESS;
}


/*****************************************************************************/
bio_dev_handle bioRegister(char *device_name, bio_register_params_t *register_params)
{
    os_status   status;
    int         i, j;
 
    OS_ASSERT_COMPILER_COND(g_total_bio_devices > 0);
 
    for (i=0; i < g_total_bio_devices; i++)
    {
        if (g_bio_list[i].status == OS_RESOURCE_UNUSED)
        {
            for (j=0; j < BIO_NAME_SIZE; j++)
            {
                g_bio_list[i].name[j] = device_name[j];
            }
            g_bio_list[i].status = OS_RESOURCE_ACQUIRED;
            g_bio_list[i].device_open = register_params->device_open;
            g_bio_list[i].channel_open = register_params->channel_open;
            g_bio_list[i].channel_close = register_params->channel_close;
            g_bio_list[i].channel_tx = register_params->channel_tx;
            g_bio_list[i].device_ctrl = register_params->device_ctrl;
            g_bio_list[i].channel_ctrl = register_params->channel_ctrl;
            g_bio_list[i].tx_queue_size = register_params->tx_queue_size;
            g_bio_list[i].rx_queue_size = register_params->rx_queue_size;
            g_bio_list[i].rx_buffers_queue_size = register_params->rx_buffers_queue_size;
            g_bio_list[i].device_handle = register_params->lld_handle;
            g_bio_list[i].internal_buffers_pool = register_params->internal_buffers_pool;
#if BIO_STATISTICS == ON
            g_bio_list[i].bio_stat.is_promisc = register_params->is_promisc;
            g_bio_list[i].bio_stat.interface_speed = register_params->interface_speed;
#endif
            if (register_params->use_common_pool)
            {
                g_bio_list[i].buffers_pool = NULL;
 
                if (register_params->rx_common_buffers_queue_size > 0)
                {
                    /* Create the common buffers queue */
                    status = osQueueFind(&(g_bio_list[i].common_queue), FALSE);
                    if (status != OS_SUCCESS)
                        return NULL;
 
                    status = osQueueCreate(g_bio_list[i].common_queue,
                                           register_params->rx_common_buffers_queue_size);
                    if (status != OS_SUCCESS)
                        return NULL;
                }
                else
                {
                    g_bio_list[i].common_queue = NULL;
                }
            }
            else
            {
                /* This is our indication that a common pool is not allowed */
                g_bio_list[i].buffers_pool = (os_mem_part_t *)0xFFFFFFFF;
            }
 
            return &g_bio_list[i];
        }
    }
 
    return NULL;
}


/*****************************************************************************/
bio_channel_t * bioGlobalChannelGet(bio_dev_handle bio_handle)
{
    bio_interface_t *bio_interf = (bio_interface_t *)bio_handle;
 
    if (bio_interf->buffers_pool)
    {
        uint32_t addr;
 
        /* This is not a full channel structure - we will use only the
           buffers_pool and buffers_queue fields */
        addr = ((uint32_t)&(bio_interf->buffers_pool) -
                (uint32_t)&(((bio_channel_t *) 0)->buffers_pool));
 
        return (bio_channel_t *)addr;
    }
 
    return NULL;
}


/*****************************************************************************/

uint16_t bioChannelRxBufferOffsetGet(bio_channel_t *bio_ch)
{
    OS_ASSERT_COND(bio_ch != NULL);
    /* Do not check other channel parameters, because it may be the virtual
       global channel of the device. */
 
    return osMemBlockOffset(bio_ch->buffers_pool);
}

/*****************************************************************************/
bio_dev_handle osBioDeviceOpen(char                     *device_name,
                               bio_dev_open_params_t    *dev_open_params)
{
    os_status   status;
    int         i, j;
    void        *lld_params = (dev_open_params ? dev_open_params->lld_params : NULL);
 
    OS_ASSERT_COMPILER_COND(g_total_bio_devices > 0);
 
    for (i=0; i < g_total_bio_devices; i++)
    {
        if (g_bio_list[i].status == OS_RESOURCE_ACQUIRED)
        {
            bool name = TRUE;
 
            for (j=0; j < BIO_NAME_SIZE; j++)
            {
                if (g_bio_list[i].name[j] != device_name[j])
                {
                    name = FALSE;
                    break;
                }
            }
            if (name)
            {
                if (dev_open_params && dev_open_params->common_pool)
                {
                    if (g_bio_list[i].buffers_pool == (os_mem_part_t *)0xFFFFFFFF)
                    {
                        /* The LLD does not expect a common pool. */
                        return NULL;
                    }
                    /* Set the common pool for this device */
                    g_bio_list[i].buffers_pool = dev_open_params->common_pool;
                }
                else
                {
                    /* If the LLD requested a common pool, it must be supplied */
                    if (g_bio_list[i].buffers_pool != (os_mem_part_t *)0xFFFFFFFF)
                    {
                        return NULL;
                    }
                }
 
                OS_ASSERT_COND(g_bio_list[i].device_open != NULL);
 
                status = g_bio_list[i].device_open(g_bio_list[i].device_handle,
                                                   lld_params);
                if (status != OS_SUCCESS)
                    return NULL;

                g_bio_list[i].status = OS_RESOURCE_USED;
                return &g_bio_list[i];
            }
        }
    }
 
    return NULL;
}


/*****************************************************************************/
os_status osBioChannelOpen(bio_dev_handle       bio_handle,
                           bio_channel_t        *bio_ch,
                           int                  mode,
                           bio_ch_open_params_t *ch_open_params)
{
    bio_interface_t     *bio_interf = (bio_interface_t *)bio_handle;
    bio_lld_ch_open_t   lld_ch_open;
    os_status           status;
    uint16_t            queue_size;
 
    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_interf != NULL);
    OS_ASSERT_COND(bio_interf->status == OS_RESOURCE_USED);
    OS_ASSERT_COND(ch_open_params != NULL);
    OS_ASSERT_COND(bio_ch->status < OS_RESOURCE_USED);
 
    bio_ch->status = OS_RESOURCE_USED;
    bio_ch->bio_interf = bio_handle;
    bio_ch->channel_type = (uint8_t)mode;
    bio_ch->callback = ch_open_params->callback;
    bio_ch->cb_parameter = ch_open_params->cb_parameter;
    bio_ch->frames_pool = ch_open_params->frames_pool;
 
    lld_ch_open.serializer = bio_ch;
    lld_ch_open.channel_num = ch_open_params->channel_num;
    lld_ch_open.lld_params = ch_open_params->lld_params;

    if (mode & BIO_READ)
    {
        OS_ASSERT_COND(!(mode & BIO_WRITE));
        OS_ASSERT_COND(bio_interf->device_ctrl != NULL);
 
        /* This is a receive channel */
        if ((bio_ch->frames_pool)->buffers_per_frame == 1)
        {
            bio_ch->f_RxFrameBuild = bioSbFrameBuild;
            /* LLD should fit BIO */
            if (bio_interf->device_ctrl(bio_interf->device_handle, BIO_DEVICE_SUPPORT_MULTIBUFFER, 0) == OS_SUCCESS)
                RETURN_ERROR(OS_ERR_PARAM_INVALID);
        }
        else
        {
            bio_ch->f_RxFrameBuild = bioMbFrameBuild;
            if (bio_interf->device_ctrl(bio_interf->device_handle, BIO_DEVICE_SUPPORT_MULTIBUFFER, 0) == OS_FAIL)
                RETURN_ERROR(OS_ERR_PARAM_INVALID);

        }
 
        if (ch_open_params->buffers_pool)
        {
            /* The channel is set to work with the given buffers pool */
            bio_ch->buffers_pool = ch_open_params->buffers_pool;
            lld_ch_open.using_common_pool = FALSE;
 
            if (bio_interf->rx_buffers_queue_size > 0)
            {
                /* Create the channel's buffers queue */
                status = osQueueFind(&bio_ch->buffers_queue, FALSE);
                if (status != OS_SUCCESS)
                    return status;
 
                status = osQueueCreate(bio_ch->buffers_queue,
                                       bio_interf->rx_buffers_queue_size);
                if (status != OS_SUCCESS)
                    return status;
            }
            else
            {
                bio_ch->buffers_queue = NULL;
            }
        }
        else if (bio_interf->buffers_pool)
        {
            /* The channel is set to work with the common buffers pool */
            bio_ch->buffers_pool = bio_interf->buffers_pool;
            bio_ch->buffers_queue = bio_interf->common_queue;
            lld_ch_open.using_common_pool = TRUE;
        }
        else if (!bio_interf->internal_buffers_pool)
            /* A buffers pool must be assigned */
            RETURN_ERROR(OS_ERR_PARAM_INVALID);
 
        if (!bio_interf->buffers_pool)
            lld_ch_open.max_rx_buffer_size = (uint32_t)osMemBlockSize(bio_ch->buffers_pool);
 
        queue_size = bio_interf->rx_queue_size;
    }
    else
    {
        /* This is a transmit channel */
        queue_size = bio_interf->tx_queue_size;
        lld_ch_open.using_common_pool = FALSE;
        lld_ch_open.max_rx_buffer_size = 0;
    }
 
    /* channel might manage frames queue internally */
    if (queue_size != 0)
    {
        /* Create the channel's frames queue */
        status = osQueueFind(&bio_ch->frames_queue, FALSE);
        if (status != OS_SUCCESS)
            return status;
 
        status = osQueueCreate(bio_ch->frames_queue, queue_size);
        if (status != OS_SUCCESS)
            return status;
    }

    OS_ASSERT_COND(bio_interf->channel_open != NULL);
 
    bio_ch->lld_ch = bio_interf->channel_open(bio_interf->device_handle,
                                              mode,
                                              &lld_ch_open);
    if (bio_ch->lld_ch == NULL)
    {
        RETURN_ERROR(OS_FAIL);
    }
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osBioChannelClose(bio_channel_t *bio_ch)
{
	os_status       status = OS_SUCCESS;
	bio_interface_t *bio_interf;
	
	OS_ASSERT_COND(bio_ch != NULL);
	OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);
	
	bio_interf = (bio_interface_t *)bio_ch->bio_interf;
	
	OS_ASSERT_COND(bio_interf != NULL);
	
    if (bio_interf->channel_close != NULL)
    {
    	status = bio_interf->channel_close(bio_ch->lld_ch, bio_ch->channel_type);
		OS_ASSERT_COND(status == OS_SUCCESS);
    }

    if (bio_interf->tx_queue_size | bio_interf->rx_queue_size)
    {
        status = osQueueDelete(bio_ch->frames_queue);
        OS_ASSERT_COND(status == OS_SUCCESS);
    }
    if (bio_ch->channel_type & BIO_READ)
    {
        if (bio_interf->rx_buffers_queue_size > 0)
        {
            status = osQueueDelete(bio_ch->buffers_queue);
            OS_ASSERT_COND(status == OS_SUCCESS);
        }
    }
 
 
	if (status == OS_SUCCESS)
	{
		bio_ch->frames_pool  = NULL;
		bio_ch->cb_parameter = NULL;
		bio_ch->callback     = NULL;
		bio_ch->channel_type = NULL;
		bio_ch->bio_interf   = NULL;
		bio_ch->status       = OS_RESOURCE_DELETED;
	}
	
	return status;
}


