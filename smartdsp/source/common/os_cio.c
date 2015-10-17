/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/17 14:31:54 $
 $Id: os_cio.c,v 1.23 2014/03/17 14:31:54 b33105 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_cio.c,v $
 $Revision: 1.23 $
******************************************************************************/

/******************************************************************************
 
 @File          os_cio.c

 @Description   OS CIO layer runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_cio_lld_.h"
#include "os_cio_.h"
#include "os_rm_.h"



/*****************************************************************************/
uint8_t * osCioChannelBufferGet(cio_channel_t *cio_ch, uint16_t size)
{
    uint8_t *ret_val;
 
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);
 
    osHwiSwiftDisable();
 
    if (size > cio_ch->available_size)
    {
        /* No more available space */
        osHwiSwiftEnable();
        return NULL;
    }
 
    if ((cio_ch->data_next + size) >  cio_ch->data_end)
    {
    /*
        The requested space is bigger than available before wrap.
        There are two options here:
        1. Wrap is impossible, because there is not enough space at
           start of the buffer too.
        2. Wrap is possible, but it creates unused space at end of the buffer,
           if so borrow_right should be set to that size.
    */
        if ((cio_ch->data_next < cio_ch->data_current_free) ||          /* 1 */
            (cio_ch->data_base + size > cio_ch->data_current_free) ||   /* 2 */
            ((cio_ch->channel_type & CIO_WRITE) && (cio_ch->data_current != cio_ch->data_next)))
        {
        /*
        1.
 
    data_base   data_next               data_current_free   data_end
        |          |                            |               |
        V          V                            V               V
        [_______________________________________________________]
 
 
        2.

    data_base   data_current_free           data_next        data_end
        |          |                            |               |
        V          V                            V               V
        [_______________________________________________________]
         ___________________
                size
 
        */
            osHwiSwiftEnable();
            return NULL;
        }
 
        cio_ch->borrow_right = (uint16_t)(cio_ch->data_end - (cio_ch->data_next));
        cio_ch->data_current = cio_ch->data_base;
        cio_ch->data_next = cio_ch->data_base + size;
        cio_ch->available_size -= (size + cio_ch->borrow_right);
 
        osHwiSwiftEnable();
        return cio_ch->data_current;
    }
 
    if ((cio_ch->data_next < cio_ch->data_current_free) &&
        ((cio_ch->data_next + size) > cio_ch->data_current_free))
    {
        /*
 
    data_base   data_next               data_current_free   data_end
        |          |                            |               |
        V          V                            V               V
        [_______________________________________________________]
                    _________________________________
                            size
 
        */
        osHwiSwiftEnable();
        return NULL;
    }
 
    ret_val = cio_ch->data_next;
    cio_ch->data_next += size;
    cio_ch->available_size -= size;
 
    osHwiSwiftEnable();
    return ret_val;
}


/*****************************************************************************/
uint16_t osCioChannelTxBufferPut(cio_channel_t *cio_ch)
{
    cio_interface_t *cio_interf;
    uint16_t num_tx;
 
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->channel_type & CIO_WRITE);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);
 
    cio_interf = (cio_interface_t *)cio_ch->cio_interf;

    osHwiSwiftDisable();

    /* Calling LLD transmit function and updating data_current for next
    transmit operation. LLD transmit returns a number of characters which was
    accepted for transmit. */
    num_tx = cio_interf->channel_tx(cio_ch->lld_ch,
                               cio_ch->data_current,
                            (uint16_t)(cio_ch->data_next - cio_ch->data_current));

    cio_ch->data_current += num_tx;
 
    osHwiSwiftEnable();
 
    return num_tx;
}


/*****************************************************************************/
void cioChannelTxCb(cio_channel_t *cio_ch, uint16_t size)
{
    cio_interface_t *cio_interf;
    uint16_t        size_to_end;
 
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->channel_type & CIO_WRITE);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);

    cio_interf = (cio_interface_t *)cio_ch->cio_interf;
    size_to_end = (uint16_t)(cio_ch->data_end - cio_ch->data_current_free);
 
    osHwiSwiftDisable();
 
    if ((cio_ch->available_size + size) > cio_ch->max_size)
    {
#ifdef CIO_STATISTICS
        cio_ch->errors += size;
#endif
        osHwiSwiftEnable();
        return;
    }
 
#ifdef CIO_STATISTICS
    cio_ch->processed_bytes += size;
#endif

    if (size > size_to_end)
    {
        cio_ch->data_current_free =
            cio_ch->data_base + (size - size_to_end) + cio_ch->borrow_right;
        cio_ch->available_size += cio_ch->borrow_right;
        cio_ch->borrow_right = 0;
    }
    else
    {
        cio_ch->data_current_free += size;
    }
    cio_ch->available_size += size;

    /* Calling LLD transmit function and updating data_current for
       next transmit operation. LLD transmit returns a number of characters
       which was accepted for transmit. */
    if (cio_ch->data_current != cio_ch->data_next)
    {
        cio_ch->data_current +=
            cio_interf->channel_tx(cio_ch->lld_ch,
                                   cio_ch->data_current,
                                   (uint16_t)(cio_ch->data_next - cio_ch->data_current));
    }
 
    /* Calling application's transmit callback */
    if (cio_ch->cb_tx_rx)
    {
        cio_ch->cb_tx_rx(cio_ch->cb_parameter, size);
    }
 
    osHwiSwiftEnable();
}


/*****************************************************************************/
void cioChannelRxCb(cio_channel_t *cio_ch, uint16_t size)
{
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->channel_type & CIO_READ);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);

    osHwiSwiftDisable();
 
    /* Calling application's receive callback */
    if (cio_ch->cb_tx_rx)
    {
        cio_ch->cb_tx_rx(cio_ch->cb_parameter, size);
    }
 
    osHwiSwiftEnable();
}


/*****************************************************************************/
uint8_t * osCioChannelRxBufferGet(cio_channel_t *cio_ch, uint16_t *size)
{
    uint16_t size_to_end;
 
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->channel_type & CIO_READ);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);
 
    size_to_end = (uint16_t)(cio_ch->data_end - cio_ch->data_current_free);
 
    if (cio_ch->data_next > cio_ch->data_current_free)
    {
        *size = (uint16_t)(cio_ch->data_next - cio_ch->data_current_free);
    }
    else if ((cio_ch->data_next < cio_ch->data_current_free))
    {
        *size = (uint16_t)(size_to_end - cio_ch->borrow_right);
    }
    else if (cio_ch->available_size != 0)
    {
        *size = 0;
        return NULL;
    }
 
 
    if (cio_ch->borrow_right == size_to_end)
    {
        *size = (uint16_t)(cio_ch->data_next - cio_ch->data_base);
        return cio_ch->data_base;
    }
 
    return cio_ch->data_current_free;
}


/*****************************************************************************/
void osCioChannelRxBufferFree(cio_channel_t *cio_ch, uint16_t size)
{
    uint16_t size_to_end;
 
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->channel_type & CIO_READ);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);

    size_to_end = (uint16_t)(cio_ch->data_end - cio_ch->data_current_free);
 
    osHwiSwiftDisable();
 
    if (size > size_to_end)
    {
        cio_ch->data_current_free = cio_ch->data_base + (size - size_to_end) + cio_ch->borrow_right;
        cio_ch->available_size += cio_ch->borrow_right;
        cio_ch->borrow_right = 0;
    }
    else
    {
        cio_ch->data_current_free += size;
    }
    cio_ch->available_size += size;
    if (cio_ch->available_size == cio_ch->max_size)
    {
        cio_ch->data_current = cio_ch->data_base;
        cio_ch->data_current_free = cio_ch->data_base;
        cio_ch->data_next = cio_ch->data_base;
        cio_ch->borrow_right = 0;
    }
 
    osHwiSwiftEnable();
}


/*****************************************************************************/
os_status osCioDeviceCtrl(cio_dev_handle cio_handle, uint32_t command, void *param)
{
    cio_interface_t *cio_interf = (cio_interface_t *)cio_handle;
 
    OS_ASSERT_COND(cio_interf != NULL);
 
    return cio_interf->device_ctrl(cio_interf->device_handle, command, param);
}


/*****************************************************************************/
os_status osCioChannelCtrl(cio_channel_t *cio_ch, uint32_t command, void *param)
{
    cio_interface_t *cio_interf;
 
    OS_ASSERT_COND(cio_ch != NULL);
    OS_ASSERT_COND(cio_ch->status >= OS_RESOURCE_USED);
 
    cio_interf = (cio_interface_t *)cio_ch->cio_interf;
    OS_ASSERT_COND(cio_interf != NULL);

    if ((command & CIO_COMMAND_WRITE) && (cio_ch->channel_type & CIO_READ))
    {
        RETURN_ERROR(OS_ERR_COMMAND_UNSUPPORTED);
    }
 
    return cio_interf->channel_ctrl(cio_ch->lld_ch, command, param);
}





