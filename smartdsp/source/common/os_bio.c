/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_bio.c,v 1.45 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_bio.c,v $
 $Revision: 1.45 $
******************************************************************************/

/******************************************************************************
 
 @File          os_bio.c

 @Description   OS BIO layer runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_bio_lld_.h"
#include "os_bio_.h"
#include "os_queue_.h"
#include "os_rm_.h"
#if (BIO_STATISTICS == ON)
#include "os_frame.h"
#endif

/*****************************************************************************/
os_status osBioChannelTx(bio_channel_t *bio_ch, void *frame)
{
    os_status       status;
    bio_interface_t *bio_interf;
 
    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_WRITE);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);
 
    osHwiSwiftDisable();
 
    bio_interf = (bio_interface_t *)bio_ch->bio_interf;
    OS_ASSERT_COND(bio_interf != NULL);
    OS_ASSERT_COND(bio_interf->channel_tx != NULL);
 
 
    /* Call LLD transmit function */
    status = bio_interf->channel_tx(bio_ch->lld_ch, frame);
    if (status == OS_SUCCESS)
    {
        /* Insert frame into confirmation queue */
 
        /* The confirmation queue must be at least the size of the maximum number
           of frames in the Tx BD ring - this is the LLD's responsibility in
           registration, so when the LLD transmit function is successful,
           osQueueUnsafeEnqueue() must not fail. */
        status = osQueueUnsafeEnqueue(bio_ch->frames_queue, (uint32_t)frame);
#if (BIO_STATISTICS == ON)
        {
            uint32_t len;

            len = (uint32_t)osFrameLengthGet((os_frame_t*)frame);
            bio_interf->bio_stat.tx_frames++;
            bio_interf->bio_stat.tx_bytes+=len;
        }
#endif
        OS_ASSERT_COND(status == OS_SUCCESS);
    }
    else if (status == OS_SUCESS_LLD_TX)
    {
        /* LLD has it's own serialization mechanism.             */
        /* transmitted frames confirmation is handled by LLD     */
        /* no need to enqueue frame in BIO                       */
        status = OS_SUCCESS;
    }
#if (BIO_STATISTICS == ON)
    else
        bio_interf->bio_stat.tx_errors++;
#endif
 
    osHwiSwiftEnable();
 
    return status;
}


/*****************************************************************************/
void * osBioChannelRx(bio_channel_t *bio_ch)
{
    uint32_t rx_frame;

    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_READ);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);

    osHwiSwiftDisable();
 
    osQueueUnsafeDequeue(bio_ch->frames_queue, &rx_frame);
 
    osHwiSwiftEnable();

    /* rx_frame will be NULL if osQueueUnsafeDequeue failed */
    return (void *)rx_frame;
}

/*****************************************************************************/
void bioChannelTxCb(bio_channel_t *bio_ch, uint16_t num_of_frames, uint32_t error_status)
{
    os_status   status;
    uint32_t    conf_frame;

    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_WRITE);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);

    osHwiSwiftDisable();

    if (bio_ch->callback)
    {
        while (num_of_frames--)
        {
#if (BIO_STATISTICS == ON)
            {
                bio_interface_t *bio_interf;
                bio_interf = (bio_interface_t *)bio_ch->bio_interf;

                if(error_status)
                    bio_interf->bio_stat.tx_errors++;
            }
#endif
            status = osQueueUnsafeDequeue(bio_ch->frames_queue, &conf_frame);

            if (status == OS_SUCCESS)
            {
                /* Call application's transmit callback */
                bio_ch->callback(bio_ch->cb_parameter, conf_frame, error_status);
            }
            else
                REPORT_EVENT(OS_EVENT_BIO_DEQUEUE);
        }
    }
    else
    {
        while (num_of_frames--)
        {
#if (BIO_STATISTICS == ON)
            {
                bio_interface_t *bio_interf;
                bio_interf = (bio_interface_t *)bio_ch->bio_interf;
                if(error_status)
                    bio_interf->bio_stat.tx_errors++;
            }
#endif
            status = osQueueUnsafeDequeue(bio_ch->frames_queue, &conf_frame);

            if (status == OS_SUCCESS)
            {
                /* Delete confirmed frame */
                osFrameRelease((void *)conf_frame);
            }
            else
                REPORT_EVENT(OS_EVENT_BIO_DEQUEUE);
        }
    }

    osHwiSwiftEnable();
}


/*****************************************************************************/

void bioChannelRxCb(bio_channel_t *bio_ch, uint32_t frame_length)
{
    os_status   status;
    os_frame_t  *frame;

    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_READ);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);
 
    /* This function should be used when BIO builds the frame from the buffers queue */
    OS_ASSERT_COND(bio_ch->buffers_queue != NULL);

    /* Get an empty frame from the pool */
    frame = osFrameGet(bio_ch->frames_pool, bio_ch->buffers_pool);
    OS_ASSERT_COND(frame != NULL);

    /* Remove any pre-defined offset */
    frame->ctrl.buffers[0].offset = 0;
 
    osHwiSwiftDisable();
 

#if (BIO_STATISTICS == ON)
    {
        bio_interface_t *bio_interf;
        bio_interf = (bio_interface_t *)bio_ch->bio_interf;
        bio_interf->bio_stat.rx_frames++;
        bio_interf->bio_stat.rx_bytes+= frame_length;
    }
#endif

 
    /* Add buffers to new frame, according to frame_length */
    bio_ch->f_RxFrameBuild(bio_ch, frame, frame_length);
 
 
    /* Call application's receive callback */
    if (bio_ch->callback)
    {
        osHwiSwiftEnable();
        bio_ch->callback(bio_ch->cb_parameter, (uint32_t)frame, 0);
    }
    else
    {
        /* Insert received frame into the Rx queue */
         status = osQueueUnsafeEnqueue(bio_ch->frames_queue, (uint32_t)frame);
         OS_ASSERT_COND(status == OS_SUCCESS);
 
         osHwiSwiftEnable();

    }
}


/*****************************************************************************/
os_frame_t * bioChannelRxFrameNew(bio_channel_t *bio_ch)
{
    os_frame_t *frame;
 
    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_READ);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);

    /* This function should be used when the LLD builds the frame - no need
       for a buffers queue */
    OS_ASSERT_COND(bio_ch->buffers_queue == NULL);
 
    /* Get an empty frame from the pool */
    frame = osFrameGet(bio_ch->frames_pool, bio_ch->buffers_pool);
    OS_ASSERT_COND(frame != NULL);
 
    /* Remove any pre-defined offset */
    frame->ctrl.buffers[0].offset = 0;
 
    return frame;
}


/*****************************************************************************/
void bioChannelRxFrameCb(bio_channel_t *bio_ch, void *frame)
{
    os_status status;

    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_READ);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);

    /* This function should be used when the LLD builds the frame - no need
       for a buffers queue */
    OS_ASSERT_COND(bio_ch->buffers_queue == NULL);

 
    /* Call application's receive callback */
    if (bio_ch->callback)
    {
        bio_ch->callback(bio_ch->cb_parameter, (uint32_t)frame, 0);
    }
    else
    {
        osHwiSwiftDisable();

        /* Insert received frame into the Rx queue */
        status = osQueueUnsafeEnqueue(bio_ch->frames_queue, (uint32_t)frame);
        OS_ASSERT_COND(status == OS_SUCCESS);
 
        osHwiSwiftEnable();
 
    }
}


/*****************************************************************************/
uint8_t * bioChannelRxBufferGet(bio_channel_t *bio_ch, uint32_t *size)
{
    os_status   status;
    uint8_t     *buf;

    OS_ASSERT_COND(bio_ch != NULL);
    /* Do not check other channel parameters, because it may be the virtual
       global channel of the device. */
 
    buf = (uint8_t *)osMemBlockSyncGet(bio_ch->buffers_pool);
    OS_ASSERT_COND(buf != NULL);
 
    *size = (uint32_t)osMemBlockSize(bio_ch->buffers_pool);
 
    if (bio_ch->buffers_queue)
    {
        /* Queue the buffer, for building the frame later */
        osHwiSwiftDisable();
 
        status = osQueueUnsafeEnqueue(bio_ch->buffers_queue, (uint32_t)buf);
        OS_ASSERT_COND(status == OS_SUCCESS);
 
        osHwiSwiftEnable();
    }

    return buf;
}


/*****************************************************************************/
os_status osBioDeviceCtrl(bio_dev_handle bio_handle, uint32_t command, void *param)
{
    bio_interface_t *bio_interf = (bio_interface_t *)bio_handle;

    OS_ASSERT_COND(bio_interf != NULL);
    OS_ASSERT_COND(bio_interf->device_ctrl != NULL);
 
    if(BIO_LLD_COMMAND & command)
        return bio_interf->device_ctrl(bio_interf->device_handle, command, param);
 
    switch (command)
    {
#if BIO_STATISTICS == ON
        case BIO_DEVICE_STAT_GET:
            *(bio_interface_stat_t**)param = &bio_interf->bio_stat;
        return OS_SUCCESS;
#endif

    default:
        RETURN_ERROR(OS_ERR_COMMAND_UNSUPPORTED);

    }
}


/*****************************************************************************/
os_status osBioChannelCtrl(bio_channel_t *bio_ch, uint32_t command, void *param)
{
    bio_interface_t *bio_interf;

    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);

    bio_interf = (bio_interface_t *)bio_ch->bio_interf;
    OS_ASSERT_COND(bio_interf != NULL);

    if (!(command & BIO_COMMAND_READ) && (bio_ch->channel_type & BIO_READ))
    {
        RETURN_ERROR(OS_ERROR(OS_ERR_COMMAND_INVALID, OS_ERRMODULE_BIO));
    }

    OS_ASSERT_COND(bio_interf->channel_ctrl != NULL);

    return bio_interf->channel_ctrl(bio_ch->lld_ch, command, param);
}


/*****************************************************************************/
void bioMbFrameBuild(void *bio_ch_handle, os_frame_t *frame, uint32_t frame_length)
{
    bio_channel_t       *bio_ch = (bio_channel_t *)bio_ch_handle;
    os_status           status;
    uint32_t            buffer;
    uint32_t            buffer_size;
    volatile uint32_t   tmp_length = 0;
 
    buffer_size = (uint32_t)osMemBlockSize(bio_ch->buffers_pool);
 
    /* Add the received buffers to the frame */
    while (frame_length - tmp_length > buffer_size)
    {
        status = osQueueUnsafeDequeue(bio_ch->buffers_queue, &buffer);
        OS_ASSERT_COND(status == OS_SUCCESS);
 
        status = osFrameBufferAppend(frame, (uint8_t *)buffer, buffer_size);
        tmp_length += buffer_size;
    }
 
    status = osQueueUnsafeDequeue(bio_ch->buffers_queue, &buffer);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    status = osFrameBufferAppend(frame, (uint8_t *)buffer, (uint32_t)(frame_length - tmp_length));
}


/*****************************************************************************/
void bioSbFrameBuild(void *bio_ch_handle, os_frame_t *frame, uint32_t frame_length)
{
    bio_channel_t   *bio_ch = (bio_channel_t *)bio_ch_handle;
    os_status       status;
    uint32_t        buffer;

    /* Add the received buffer to the frame */
    status = osQueueUnsafeDequeue(bio_ch->buffers_queue, &buffer);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    /* This is faster than osFrameSingleBufferSet... */
    status = osFrameBufferAppend(frame, (uint8_t *)buffer, frame_length);
}


/*****************************************************************************/
void bioChannelRxError(bio_channel_t *bio_ch, uint16_t amount, uint32_t error_status)
{
    uint32_t    tmp_buf;
    os_status   status;
 
    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_READ);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);
 
    if (bio_ch->buffers_queue)
    {
        osHwiSwiftDisable();

#if (BIO_STATISTICS == ON)
    {
        bio_interface_t *bio_interf;
        bio_interf = (bio_interface_t *)bio_ch->bio_interf;
        bio_interf->bio_stat.rx_errors++;
    }
#endif
 
        /* Dequeue and free the stated amount of buffers (no frame is built) */
        while (amount--)
        {
            status = osQueueUnsafeDequeue(bio_ch->buffers_queue, &tmp_buf);
            OS_ASSERT_COND(status == OS_SUCCESS);
 
            osMemBlockSyncFree(bio_ch->buffers_pool, (void *)tmp_buf);
        }
 
        osHwiSwiftEnable();
    }
 
    /* No need to call call back */
    if(error_status == BIO_REMOVE_BUFFER)
        return;

    /* Call application's receive callback */
    if (bio_ch->callback)
    {
        bio_ch->callback(bio_ch->cb_parameter, 0, error_status);
    }
}


/*****************************************************************************/
void bioChannelRxBufferFree(bio_channel_t *bio_ch, uint8_t *buffer)
{
    OS_ASSERT_COND(bio_ch != NULL);
    OS_ASSERT_COND(bio_ch->channel_type & BIO_READ);
    OS_ASSERT_COND(bio_ch->status >= OS_RESOURCE_USED);
 
    /* This function should be used when the LLD builds the frame - no need
       for a buffers queue */
    OS_ASSERT_COND(bio_ch->buffers_queue == NULL);
 
    osMemBlockSyncFree(bio_ch->buffers_pool, buffer);
}


