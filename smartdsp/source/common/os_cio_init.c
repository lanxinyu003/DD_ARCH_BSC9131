/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_cio_init.c,v 1.17 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_cio_init.c,v $
 $Revision: 1.17 $
******************************************************************************/

/******************************************************************************
 
 @File          os_cio_init.c

 @Description   OS CIO layer Initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_cio_lld_.h"
#include "os_cio_.h"
#include "os_rm_.h"


cio_interface_t *g_cio_list;
uint16_t        g_total_cio_devices;


/*****************************************************************************/
os_status osCioInitialize(uint16_t total_cio_devices)
{
    g_total_cio_devices = total_cio_devices;
 
    g_cio_list = (cio_interface_t *)
        osMalloc((sizeof(cio_interface_t) * g_total_cio_devices), OS_MEM_LOCAL);
 
    if (g_cio_list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }
 
    return OS_SUCCESS;
}


/*****************************************************************************/
void * cioRegister(char *device_name, cio_register_params_t *register_params)
{
    int i, j;
 
    OS_ASSERT_COMPILER_COND(g_total_cio_devices > 0);
 
    for (i=0; i < g_total_cio_devices; i++)
    {
        if (g_cio_list[i].status == OS_RESOURCE_UNUSED)
        {
            for (j=0; j < CIO_NAME_SIZE; j++)
            {
                g_cio_list[i].name[j] = device_name[j];
            }
            g_cio_list[i].status = OS_RESOURCE_ACQUIRED;
            g_cio_list[i].device_open = register_params->device_open;
            g_cio_list[i].channel_open = register_params->channel_open;
            g_cio_list[i].channel_close = register_params->channel_close;
            g_cio_list[i].channel_tx = register_params->channel_tx;
            g_cio_list[i].device_ctrl = register_params->device_ctrl;
            g_cio_list[i].channel_ctrl = register_params->channel_ctrl;
            g_cio_list[i].device_handle = register_params->lld_handle;
 
            return &g_cio_list[i];
        }
    }
 
    return NULL;
}


/*****************************************************************************/
cio_dev_handle osCioDeviceOpen(char *device_name, void *lld_params)
{
    int i, j;
 
    OS_ASSERT_COMPILER_COND(g_total_cio_devices > 0);

    for (i=0; i < g_total_cio_devices; i++)
    {
        if (g_cio_list[i].status == OS_RESOURCE_ACQUIRED)
        {
            bool name = TRUE;
 
            for (j=0; j < CIO_NAME_SIZE; j++)
            {
                if (g_cio_list[i].name[j] != device_name[j])
                {
                    name = FALSE;
                    break;
                }
            }
            if (name)
            {
                g_cio_list[i].status = OS_RESOURCE_USED;
 
                if (g_cio_list[i].device_open(g_cio_list[i].device_handle, lld_params) == OS_SUCCESS)
                {
                    return &g_cio_list[i];
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
os_status osCioChannelOpen(cio_dev_handle       cio_handle,
                           cio_channel_t        *cio_ch,
                           int                  mode,
                           cio_ch_open_params_t *ch_open_params)
{
    cio_interface_t     *cio_interf = (cio_interface_t *)cio_handle;
    cio_lld_ch_open_t   lld_ch_open;

    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_interf != NULL);
    OS_ASSERT_COND(cio_interf->status == OS_RESOURCE_USED);
    OS_ASSERT_COND(ch_open_params != NULL);
    OS_ASSERT_COND(cio_ch->status < OS_RESOURCE_USED);
    OS_ASSERT_COND(!((mode & CIO_WRITE) && (mode & CIO_READ)));

    cio_ch->status = OS_RESOURCE_USED;
    cio_ch->cio_interf = cio_handle;
    cio_ch->channel_type = (uint8_t)mode;
    cio_ch->data_base = ch_open_params->data_base;
    cio_ch->data_current = ch_open_params->data_base;
    cio_ch->data_current_free = ch_open_params->data_base;
    cio_ch->available_size = ch_open_params->data_size;
    cio_ch->data_end = ch_open_params->data_base + ch_open_params->data_size;
    cio_ch->data_next = ch_open_params->data_base;
    cio_ch->borrow_right = 0;
    cio_ch->max_size = ch_open_params->data_size;
    cio_ch->cb_tx_rx = ch_open_params->cb_tx_rx;
    cio_ch->cb_parameter = ch_open_params->cb_parameter;
 
    lld_ch_open.serializer = cio_ch;
    lld_ch_open.channel_num = ch_open_params->channel_num;
    lld_ch_open.lld_params = ch_open_params->lld_params;
    lld_ch_open.data_base = cio_ch->data_base;
    lld_ch_open.data_size = ch_open_params->data_size;
 
    cio_ch->lld_ch = cio_interf->channel_open(cio_interf->device_handle,
                                             mode,
                                             &lld_ch_open);
    if (cio_ch->lld_ch == NULL)
    {
        RETURN_ERROR(OS_FAIL);
    }
 
    return OS_SUCCESS;
 
    /*
    At the beginning data_current_free and data_next point to data_base
 
    data_base                                               data_end
        |                                                       |
        V                                                       V
        [_______________________________________________________]
        |
    data_current_free
    data_next

    */
}


/*****************************************************************************/
os_status osCioChannelClose(cio_channel_t *cio_ch)
{
    os_status       status;
    cio_interface_t *cio_interf;
 
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);
 
    cio_interf = (cio_interface_t *)cio_ch->cio_interf;
 
    OS_ASSERT_COND(cio_interf != NULL);
    OS_ASSERT_COND(cio_interf->channel_close != NULL);
 
    status = cio_interf->channel_close(cio_ch->lld_ch, cio_ch->channel_type);

    if (status == OS_SUCCESS)
    {
        cio_ch->status = OS_RESOURCE_UNUSED;
    }
 
    return status;
}


