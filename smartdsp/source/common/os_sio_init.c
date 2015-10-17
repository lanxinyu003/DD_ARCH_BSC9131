/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_sio_init.c,v 1.24 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_sio_init.c,v $
 $Revision: 1.24 $
******************************************************************************/

/******************************************************************************
 
 @File          os_bio_init.c

 @Description   OS BIO layer initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_rm_.h"
#include "os_sio_lld_.h"
#include "os_sio_.h"


sio_interface_t *g_sio_list;
uint16_t        g_total_sio_devices;



/*****************************************************************************/
os_status osSioInitialize(uint16_t total_sio_devices)
{
    g_total_sio_devices = total_sio_devices;
 
    g_sio_list = (sio_interface_t *)
        osMalloc((sizeof(sio_interface_t) * g_total_sio_devices), OS_MEM_LOCAL);
 
    if (g_sio_list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }
 
    return OS_SUCCESS;
}


/*****************************************************************************/
sio_dev_handle sioRegister(char *device_name, sio_register_params_t *register_params)
{
    int i, j;

    OS_ASSERT_COMPILER_COND(g_total_sio_devices > 0);
 
    for (i=0; i < g_total_sio_devices; i++)
    {
        if (g_sio_list[i].status == OS_RESOURCE_UNUSED)
        {
            for (j=0; j < SIO_NAME_SIZE; j++)
            {
                g_sio_list[i].name[j] = device_name[j];
            }
            g_sio_list[i].status = OS_RESOURCE_ACQUIRED;
            g_sio_list[i].device_open = register_params->device_open;
            g_sio_list[i].channel_open = register_params->channel_open;
            g_sio_list[i].channel_close = register_params->channel_close;
            g_sio_list[i].device_ctrl = register_params->device_ctrl;
            g_sio_list[i].channel_ctrl = register_params->channel_ctrl;
            g_sio_list[i].device_handle = register_params->lld_handle;

            return &g_sio_list[i];
        }
    }
    return NULL;
}


/*****************************************************************************/
sio_dev_handle osSioDeviceOpen(char                     *device_name,
                               sio_dev_open_params_t    *dev_open_params)
{
    os_status   status;
    int         i, j;
    void        *lld_params = (dev_open_params ? dev_open_params->lld_params : NULL);
 
    OS_ASSERT_COMPILER_COND(g_total_sio_devices > 0);
 
    for (i=0; i < g_total_sio_devices; i++)
    {
        if (g_sio_list[i].status == OS_RESOURCE_ACQUIRED)
        {
            bool name = TRUE;
 
            for (j=0; j < SIO_NAME_SIZE; j++)
            {
                if (g_sio_list[i].name[j] != device_name[j])
                {
                    name = FALSE;
                    break;
                }
            }
            if (name)
            {
 
                OS_ASSERT_COND(g_sio_list[i].device_open != NULL);
 
                status = g_sio_list[i].device_open(g_sio_list[i].device_handle,
                                                   lld_params);
                if (status != OS_SUCCESS)
                    return NULL;
 
                if (dev_open_params != NULL)
                {
                    g_sio_list[i].rx_callback           = dev_open_params->rx_callback;
                    g_sio_list[i].tx_callback           = dev_open_params->tx_callback;
                    g_sio_list[i].rx_callback_parameter = dev_open_params->rx_callback_parameter;
                    g_sio_list[i].tx_callback_parameter = dev_open_params->tx_callback_parameter;
                    g_sio_list[i].error_callback        = dev_open_params->error_callback;
                }
 
                g_sio_list[i].rx_channels = NULL;
                g_sio_list[i].tx_channels = NULL;

                g_sio_list[i].status = OS_RESOURCE_USED;
                return &g_sio_list[i];
            }
        }
    }
 
    return NULL;
}


/*****************************************************************************/
os_status osSioChannelOpen(sio_dev_handle       sio_handle,
                           sio_channel_t        *sio_ch,
                           int                  mode,
                           sio_ch_open_params_t *ch_open_params)
{
    os_status           status = OS_SUCCESS;
    sio_interface_t     *sio_interf = (sio_interface_t *)sio_handle;
    sio_lld_ch_open_t   lld_ch_open;
    sio_channel_t       **sio_ch_tmp;
 
    OS_ASSERT_COND(sio_ch != NULL);
    OS_ASSERT_COND(sio_interf != NULL);
    OS_ASSERT_COND(ch_open_params != NULL);

    lld_ch_open.data_base              = ch_open_params->channel_buffers_base;
    lld_ch_open.channel_num            = ch_open_params->channel_num;
    lld_ch_open.buffer_data_size       = ch_open_params->buffer_data_size;
    lld_ch_open.buffer_size            = ch_open_params->buffer_size;

    lld_ch_open.application_buffer     = &sio_ch->application_buffer;
    lld_ch_open.first_driver_buffer    = &sio_ch->first_driver_buffer;
    lld_ch_open.last_driver_buffer     = &sio_ch->last_driver_buffer;
    lld_ch_open.lld_channel            = &sio_ch->lld_channel;

    lld_ch_open.num_of_channel_buffers = ch_open_params->num_of_channel_buffers;
    lld_ch_open.lld_params             = ch_open_params->lld_params;
    lld_ch_open.num_of_channel_buffers = ch_open_params->num_of_buffers;
    lld_ch_open.sio_dev                = sio_interf;
    lld_ch_open.sio_channel            = sio_ch;

    sio_ch->lld_channel                = sio_interf->channel_open(sio_interf->device_handle,
                                                                  mode,
                                                                  &lld_ch_open);

    sio_ch->mode = (uint8_t)mode;
 
    if (sio_ch->lld_channel == NULL)
    {
        status = OS_FAIL;
    }
    else
    {
        if (mode & SIO_ACTIVE)
            sio_ch->status               = OS_RESOURCE_ACTIVE;
        else
            sio_ch->status               = OS_RESOURCE_USED;
 
        sio_ch->sio_interf               = sio_interf;
        sio_ch->callback_parameter       = ch_open_params->callback_parameter;
        sio_ch->num_of_buffers           = ch_open_params->num_of_buffers;
        sio_ch->buffer_size              = ch_open_params->buffer_size;
        sio_ch->buffer_data_size         = ch_open_params->buffer_data_size;
        sio_ch->channel_buffers_base     = ch_open_params->channel_buffers_base;
        sio_ch->next                     = NULL;

        if (mode & SIO_READ)
        {
            OS_ASSERT_COND(!(mode & SIO_WRITE));

            sio_ch->application_buffer  = *sio_ch->first_driver_buffer;
            sio_ch_tmp = &sio_interf->rx_channels;
        }
        else
        {
            sio_ch->application_buffer  = (uint16_t)(*sio_ch->last_driver_buffer + 1);
            if (sio_ch->application_buffer == sio_ch->num_of_buffers)
            {
                sio_ch->application_buffer = 0;
            }
 
            sio_ch_tmp = &sio_interf->tx_channels;
        }
 
        sioInsertChannel(sio_ch_tmp, sio_ch);
 
    }
 
    return status;
}


/*****************************************************************************/
os_status osSioChannelClose(sio_channel_t *sio_ch)
{
    os_status       status = OS_SUCCESS;
    sio_interface_t *sio_interf;

    OS_ASSERT_COND(sio_ch != NULL);
    OS_ASSERT_COND(sio_ch->status >= OS_RESOURCE_USED);
 
    sio_interf = (sio_interface_t *)sio_ch->sio_interf;
 
    OS_ASSERT_COND(sio_interf != NULL);
    OS_ASSERT_COND(sio_interf->channel_close != NULL);

    status = sioRemoveChannel(sio_ch);

    if (status == OS_SUCCESS)
    {
        status = sio_interf->channel_close(sio_ch->lld_channel,
                                           sio_ch->mode);

        if (status == OS_SUCCESS)
        {
            sio_ch->status = OS_RESOURCE_UNUSED;
        }
    }
 
    return status;
}


