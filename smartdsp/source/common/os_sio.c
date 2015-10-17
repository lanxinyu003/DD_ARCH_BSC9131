/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_sio.c,v 1.27 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_sio.c,v $
 $Revision: 1.27 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sio.c

 @Description   OS SIO layer runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_rm_.h"
#include "os_sio_lld_.h"
#include "os_sio_.h"


/*****************************************************************************/
os_status osSioDeviceCtrl(sio_dev_handle sio_handle, uint32_t command, void *param)
{
    sio_interface_t *sio_interf = (sio_interface_t *)sio_handle;

    OS_ASSERT_COND(sio_interf != NULL);
    OS_ASSERT_COND(sio_interf->status == OS_RESOURCE_USED);
    OS_ASSERT_COND(sio_interf->device_ctrl != NULL);
 
    return sio_interf->device_ctrl(sio_interf->device_handle, command, param);
}


/*****************************************************************************/
os_status osSioChannelCtrl(sio_channel_t *sio_ch, uint32_t command, void *param)
{
    sio_interface_t *sio_interf;

    OS_ASSERT_COND(sio_ch != NULL);
    OS_ASSERT_COND(sio_ch->status >= OS_RESOURCE_USED);

    sio_interf = (sio_interface_t *)sio_ch->sio_interf;
    OS_ASSERT_COND(sio_interf != NULL);
 
    if ((command & SIO_COMMAND_WRITE) && (sio_ch->mode & SIO_READ))
    {
        RETURN_ERROR(OS_ERR_COMMAND_INVALID);
    }
 
    if ((command == SIO_CHANNEL_TX_ENABLE) || (command == SIO_CHANNEL_RX_ENABLE))
    {
        sio_ch->status = OS_RESOURCE_ACTIVE;
    }
    else if ((command == SIO_CHANNEL_TX_DISABLE) || (command == SIO_CHANNEL_RX_DISABLE))
    {
        sio_ch->status = OS_RESOURCE_USED;
    }

    OS_ASSERT_COND(sio_interf->channel_ctrl != NULL);
 
    return sio_interf->channel_ctrl(sio_ch->lld_channel, command, param);
}


/*****************************************************************************/
uint8_t * osSioBufferGet(sio_channel_t *sio_ch, uint16_t *length)
{
    uint8_t *rc = NULL;
    sio_interface_t *sio_interf;
 
    OS_ASSERT_COND(sio_ch != NULL);
    OS_ASSERT_COND(sio_ch->status == OS_RESOURCE_ACTIVE);
    sio_interf = (sio_interface_t *)sio_ch->sio_interf;

    osHwiSwiftDisable();

    if (*sio_ch->last_driver_buffer == sio_ch->application_buffer)
    {
 
        sio_ch->application_buffer += 1;

        if (sio_ch->application_buffer == sio_ch->num_of_buffers)
        {
            sio_ch->application_buffer = 0;
        }
 
        /*if (sio_interf->error_callback)
        {
            sio_interf->error_callback(sio_ch->callback_parameter);
        }*/
    }
 
    if (sio_ch->application_buffer != *sio_ch->first_driver_buffer)
    {
        *length = (uint16_t)sio_ch->buffer_data_size;
        rc = ((sio_ch->channel_buffers_base) +
                (sio_ch->buffer_size * sio_ch->application_buffer));
    }
 
    osHwiSwiftEnable();

    return rc;
}


/*****************************************************************************/
os_status osSioBufferPut(sio_channel_t *sio_ch)
{
    os_status status = OS_FAIL;

    OS_ASSERT_COND(sio_ch != NULL);
    OS_ASSERT_COND(sio_ch->status == OS_RESOURCE_ACTIVE);

    osHwiSwiftDisable();
 
    if (sio_ch->application_buffer != *sio_ch->first_driver_buffer)
    {
        sio_ch->application_buffer = (uint16_t)(sio_ch->application_buffer + 1);
        if (sio_ch->application_buffer  == sio_ch->num_of_buffers)
        {
            sio_ch->application_buffer = 0;
        }
        status = OS_SUCCESS;
    }
 
    osHwiSwiftEnable();


    return status;
}


/*****************************************************************************/
void sioDeviceCall(void *sio_dev, uint32_t mode)
{
    sio_channel_t   **sio_ch_tmp;
    sio_interface_t *sio_interf = (sio_interface_t *)sio_dev;
 
    OS_ASSERT_COND(sio_interf != NULL);
    OS_ASSERT_COND(sio_interf->status == OS_RESOURCE_USED);
 
    if (mode & SIO_READ)
    {
        if(sio_interf->rx_callback_parameter)
        {
            sio_interf->rx_callback(sio_interf->rx_callback_parameter);
        }
        else
        {
            sio_ch_tmp =  &(sio_interf->rx_channels);
 
            while(*sio_ch_tmp)
            {
                if ((*sio_ch_tmp)->status == OS_RESOURCE_ACTIVE)
                {
                    sioChannelCall(*sio_ch_tmp);
                }
                sio_ch_tmp = &((*sio_ch_tmp)->next);
            }

        }
    }
 
    if (mode & SIO_WRITE)
    {
        if(sio_interf->tx_callback_parameter)
        {
            sio_interf->tx_callback(sio_interf->tx_callback_parameter);
        }
        else
        {
            sio_ch_tmp =  &(sio_interf->tx_channels);

            while(*sio_ch_tmp)
            {
                if ((*sio_ch_tmp)->status == OS_RESOURCE_ACTIVE)
                {
                    sioChannelCall(*sio_ch_tmp);
                }
                sio_ch_tmp = &((*sio_ch_tmp)->next);
            }
 
        }
    }
}


/*****************************************************************************/
void sioChannelCall(sio_channel_t *sio_ch)
{
    struct sio_interface_t *sio_interf;

    OS_ASSERT_COND(sio_ch != NULL);
    OS_ASSERT_COND(sio_ch->status == OS_RESOURCE_ACTIVE);
 
    sio_interf = (struct sio_interface_t *)sio_ch->sio_interf;
 
    /* Call data callback, if parameter is not NULL. */
    if (sio_ch->mode & SIO_READ)
    {
        if(sio_ch->callback_parameter)
        {
            sio_interf->rx_callback(sio_ch->callback_parameter);
        }
    }
    else
    {
        if(sio_ch->callback_parameter)
        {
            sio_interf->tx_callback(sio_ch->callback_parameter);
        }
    }
}


/*****************************************************************************/
void sioInsertChannel(sio_channel_t **sio_ch_list,
                             sio_channel_t *sio_ch)
{
    osHwiSwiftDisable();

    while (*sio_ch_list)
    {
        sio_ch_list = &((*sio_ch_list)->next);
 
        /* Checks for multiple opens of the same channel. */
        OS_ASSERT_COND(*sio_ch_list != sio_ch);
    }
 
    *sio_ch_list = sio_ch;

    osHwiSwiftEnable();
}


/*****************************************************************************/
os_status sioRemoveChannel(sio_channel_t *sio_ch)
{
    os_status       status = OS_FAIL;
    sio_channel_t   **sio_ch_tmp;
    sio_interface_t *sio_interf;

    sio_interf = (sio_interface_t *)sio_ch->sio_interf;

    osHwiSwiftDisable();
 
    if (sio_ch->mode & SIO_READ)
    {
        sio_ch_tmp = &(sio_interf->rx_channels);
    }
    else
    {
        sio_ch_tmp = &(sio_interf->tx_channels);
    }
 
    if(*sio_ch_tmp == sio_ch)
    {
        *sio_ch_tmp = (*sio_ch_tmp)->next;
        status = OS_SUCCESS;
    }
    else
    {
        while(*sio_ch_tmp)
        {
            if((*sio_ch_tmp)->next == sio_ch)
            {
                (*sio_ch_tmp)->next = ((*sio_ch_tmp)->next)->next;
                status = OS_SUCCESS;
                break;
            }
 
            sio_ch_tmp = &(*sio_ch_tmp)->next;
        }
    }

    osHwiSwiftEnable();
 
    return status;
}
