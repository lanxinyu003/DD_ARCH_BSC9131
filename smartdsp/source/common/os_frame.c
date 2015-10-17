/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/04/25 11:44:22 $
 $Id: os_frame.c,v 1.41 2013/04/25 11:44:22 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_frame.c,v $
 $Revision: 1.41 $
******************************************************************************/

/******************************************************************************
 
 @File          os_frame.c

 @Description   OS Frames runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"


/*****************************************************************************/
os_frame_t * osFrameGet(os_frames_pool_t *frames_pool, os_mem_part_t *data_mem_part)
{
    os_frame_t  *frame = NULL;
    uint8_t     i, max_index = 0;
 
    OS_ASSERT_COND(frames_pool != NULL && data_mem_part != NULL);
    OS_ASSERT_COND(frames_pool->init_frame_offset <= data_mem_part->buffer_offset);
    frame = (os_frame_t *)frames_pool->f_MemBlockGet(frames_pool->frames_part);
    OS_ASSERT_COND(frame != NULL);
 
    if (frame)
    {
        frame->data_part = data_mem_part;
        frame->frames_pool = frames_pool;
        frame->info = 0;
        frame->total = frames_pool->buffers_per_frame;
        frame->expansion = (uint8_t)(frames_pool->frame_expansion);
        frame->ref_count = 1;
 
        if (frame->total > 1)
        {
            max_index = (uint8_t)frame->total;
        }
 
        /* This loop will also reset the mbf structure (for multi-buffer frame). */
        for (i=0; i <= max_index; i++)
        {
            frame->ctrl.buffers[i].data_length = 0;
            frame->ctrl.buffers[i].offset = 0;
            frame->ctrl.buffers[i].data = NULL;
        }
 
        frame->ctrl.buffers[0].offset = 0;
    }
 
    return frame;
}


/*****************************************************************************/
void osFrameRelease(os_frame_t *frame)
{
    os_frames_pool_t *frames_pool;
    uint8_t i = 0, max_index = 0;
 
    OS_ASSERT_COND(frame != NULL);
 
    frames_pool = frame->frames_pool;
    OS_ASSERT_COND(frames_pool != NULL);
 
    if (frame->total > 1)
    {
        i = 1;
        max_index = (uint8_t)frame->total;
    }
 
    osSharedResourceLock(frame->data_part);
 
    if (--frame->ref_count)
    {
        osSharedResourceUnlock(frame->data_part);
        return;
    }
 
    for (; i <= max_index; i++)
    {
        if (frame->ctrl.buffers[i].data)
        {
            osMemBlockUnsafeFree(frame->data_part, frame->ctrl.buffers[i].data);
        }
    }
 
    osSharedResourceUnlock(frame->data_part);

    frames_pool->f_MemBlockFree(frames_pool->frames_part, (void *)frame);
}

/*****************************************************************************/
int32_t osFrameBufferOffsetGet(os_frame_t *frame, uint8_t index)
{
    uint8_t idx;
    OS_ASSERT_COND(frame != NULL);
    if (frame->total == 1)
    {
#ifdef FRAME_ERROR_CHECKING
        if (index!=0)
            return NULL;
#endif
        return ((frame)->ctrl.buffers[0].offset);
    }
    else
    {
#ifdef FRAME_ERROR_CHECKING
        if (index >= frame->ctrl.mbf.used)
            return NULL;
#endif
        idx = (frame->ctrl.mbf.first + index);
        if (idx >= frame->total)
            idx -= frame->total;
        idx++;
        return ((frame)->ctrl.buffers[idx].offset);
    }
}


/*****************************************************************************/
uint8_t * osFrameBufferGet(os_frame_t *frame, uint32_t *length, int8_t *index)
{
    uint8_t idx;
 
    OS_ASSERT_COND(frame != NULL);
    OS_ASSERT_COND(length != NULL);
 
    if (frame->total == 1)
    {
        if (index)
        {
#ifdef FRAME_ERROR_CHECKING
            /* for single-buffer frames, the index pointer can be NULL,
               but if it is not NULL, the index must be 0 */
            if (*index != 0)
                goto frame_bg_error;
#endif // FRAME_ERROR_CHECKING
            *index = FRAME_NONE;
        }
#ifdef FRAME_ERROR_CHECKING
        if (frame->ctrl.buffers[0].data == NULL)
            goto frame_bg_error;
#endif // FRAME_ERROR_CHECKING

        *length = (uint32_t)(frame->ctrl.buffers[0].data_length - frame->ctrl.buffers[0].offset);
        return (uint8_t *)(frame->ctrl.buffers[0].data + frame->ctrl.buffers[0].offset);
    }
 
    OS_ASSERT_COND(index != NULL);
 
#ifdef FRAME_ERROR_CHECKING
    if ((*index < 0) || (*index >= frame->ctrl.mbf.used))
        goto frame_bg_error;
#endif // FRAME_ERROR_CHECKING

    /* optimize: idx = (frame->ctrl.mbf.first + *index) % frame->total; */
    idx = (uint8_t)(frame->ctrl.mbf.first + *index);
    if (idx >= frame->total)
        idx -= frame->total;
 
    /* Set returned index to next buffer, or FRAME_NONE if no more */
    if (idx == frame->ctrl.mbf.last)
    {
        *index = FRAME_NONE;
    }
    else
    {
        (*index)++;
    }
 
    idx++;
#ifdef FRAME_ERROR_CHECKING
    if (frame->ctrl.buffers[idx].data == NULL)
        goto frame_bg_error;
#endif // FRAME_ERROR_CHECKING
 
    *length = (uint32_t)(frame->ctrl.buffers[idx].data_length - frame->ctrl.buffers[idx].offset);
    return (uint8_t *)(frame->ctrl.buffers[idx].data + frame->ctrl.buffers[idx].offset);

#ifdef FRAME_ERROR_CHECKING
frame_bg_error:
    *index = FRAME_NONE;
    return NULL;
#endif // FRAME_ERROR_CHECKING
}


/*****************************************************************************/
uint8_t * osFrameDataStart(os_frame_t *frame)
{
    OS_ASSERT_COND(frame != NULL);
 
    if (frame->total == 1)
    {
#ifdef FRAME_ERROR_CHECKING
        if (frame->ctrl.buffers[0].data == NULL)
        {
            return NULL;
        }
#endif // FRAME_ERROR_CHECKING
 
        return (uint8_t *)(frame->ctrl.buffers[0].data + frame->ctrl.buffers[0].offset);
    }
 
#ifdef FRAME_ERROR_CHECKING
    if (frame->ctrl.mbf.used == 0)
    {
        return NULL;
    }
#endif // FRAME_ERROR_CHECKING

    return (uint8_t *)(frame->ctrl.buffers[frame->ctrl.mbf.first + 1].data +
                       frame->ctrl.buffers[frame->ctrl.mbf.first + 1].offset);
}


/*****************************************************************************/
uint8_t * osFrameBufferSet(os_frame_t *frame, uint8_t *data, uint32_t length, int8_t index)
{
    uint8_t *prev_data;

    OS_ASSERT_COND(frame != NULL);
    OS_ASSERT_COND(data != NULL);
 
    if (frame->total > 1)
    {
#ifdef FRAME_ERROR_CHECKING
        if ((index < 0) || (index >= frame->ctrl.mbf.used))
        {
            return NULL;
        }
#endif // FRAME_ERROR_CHECKING

        /* optimize: index = (frame->ctrl.mbf.first + index) % frame->total; */
        index = (int8_t)(frame->ctrl.mbf.first + index);
        if (index >= frame->total)
            index -= frame->total;
 
        index++;

#ifdef FRAME_ERROR_CHECKING
        if (index < 0)
        {
            return NULL;
        }
#endif
        frame->ctrl.mbf.total_length -= frame->ctrl.buffers[index].data_length;
        frame->ctrl.mbf.total_length += length;
    }
#ifdef FRAME_ERROR_CHECKING
    else if (index != 0)
    {
        return NULL;
    }
#endif // FRAME_ERROR_CHECKING
 
    prev_data = frame->ctrl.buffers[index].data;
    frame->ctrl.buffers[index].data = data;
    frame->ctrl.buffers[index].data_length = length;
 
    return prev_data;
}


/*****************************************************************************/
int8_t osFrameBufferAppend(os_frame_t *frame, uint8_t *data, uint32_t length)
{
    uint8_t index;
 
    OS_ASSERT_COND(frame != NULL);
    OS_ASSERT_COND(data != NULL);
 
    if (frame->total == 1)
    {
#ifdef FRAME_ERROR_CHECKING
        if (frame->ctrl.buffers[0].data != NULL)
        {
            return FRAME_NONE;
        }
#endif // FRAME_ERROR_CHECKING
        frame->ctrl.buffers[0].data = data;
        frame->ctrl.buffers[0].data_length = length;
 
        return 1;
    }

#ifdef FRAME_ERROR_CHECKING
    if (frame->ctrl.mbf.used == frame->total)
    {
        /* All buffers are used */
        return FRAME_NONE;
    }
#endif // FRAME_ERROR_CHECKING

    if (frame->ctrl.mbf.used == 0)
    {
        /* No buffers are used */
        index = frame->ctrl.mbf.first;
    }
    else
    {
        /* optimize: index = (frame->ctrl.mbf.last + 1) % frame->total; */
        index = (uint8_t)(frame->ctrl.mbf.last + 1);
        if (index >= frame->total)
            index -= frame->total;
        frame->ctrl.mbf.last = index;
    }
 
    index++;

    frame->ctrl.buffers[index].data_length = length;
    frame->ctrl.buffers[index].data = data;
    frame->ctrl.mbf.total_length += length;
    frame->ctrl.mbf.used++;
 
    /* Return the number of buffers in the frame */
    return (int8_t)(frame->ctrl.mbf.used);
}


/*****************************************************************************/
int8_t osFrameBufferPrepend(os_frame_t *frame, uint8_t *data, uint32_t length)
{
    uint8_t index;

    OS_ASSERT_COND(frame != NULL);
    OS_ASSERT_COND(data != NULL);
 
    if (frame->total == 1)
    {
#ifdef FRAME_ERROR_CHECKING
        if (frame->ctrl.buffers[0].data != NULL)
        {
            return FRAME_NONE;
        }
#endif // FRAME_ERROR_CHECKING
        frame->ctrl.buffers[0].data = data;
        frame->ctrl.buffers[0].data_length = length;
 
        return 1;
    }
 
#ifdef FRAME_ERROR_CHECKING
    if (frame->ctrl.mbf.used == frame->total)
    {
        /* All buffers are used */
        return FRAME_NONE;
    }
#endif // FRAME_ERROR_CHECKING

    if (frame->ctrl.mbf.used == 0)
    {
        /* No buffers are used */
        index = frame->ctrl.mbf.first;
    }
    else
    {
        index = (uint8_t)((frame->ctrl.mbf.first ? (frame->ctrl.mbf.first - 1) : (frame->total - 1)));
        frame->ctrl.mbf.first = index;
    }
 
    index++;

    frame->ctrl.buffers[index].data_length = length;
    frame->ctrl.buffers[index].data = data;
    frame->ctrl.mbf.total_length += length;
    frame->ctrl.mbf.used++;
 
    /* Return the number of buffers in the frame */
    return (int8_t)(frame->ctrl.mbf.used);
}


/*****************************************************************************/
uint8_t * osFrameTailBufferRemove(os_frame_t *frame)
{
    uint8_t *data;
    uint8_t index;

    OS_ASSERT_COND(frame != NULL);
    index = (uint8_t)(frame->ctrl.mbf.last + 1);

    if (frame->total == 1)
    {
        data = frame->ctrl.buffers[0].data;
        frame->ctrl.buffers[0].data = NULL;
        frame->ctrl.buffers[0].data_length = 0;
        return data;
    }
 
#ifdef FRAME_ERROR_CHECKING
    if (frame->ctrl.mbf.used == 0)
    {
        return NULL;
    }
#endif // FRAME_ERROR_CHECKING

    data = frame->ctrl.buffers[index].data;
    frame->ctrl.mbf.total_length -= frame->ctrl.buffers[index].data_length;
    frame->ctrl.mbf.total_offset -= frame->ctrl.buffers[index].offset;
    frame->ctrl.buffers[index].data_length = 0;
    frame->ctrl.buffers[index].data = NULL;
    frame->ctrl.mbf.last =
        (uint8_t)(frame->ctrl.mbf.last ? (frame->ctrl.mbf.last - 1) : (frame->total - 1));
    frame->ctrl.mbf.used--;
 
    return data;
}


/*****************************************************************************/
uint8_t * osFrameHeadBufferRemove(os_frame_t *frame)
{
    uint8_t *data;
    uint8_t index;

    OS_ASSERT_COND(frame != NULL);
    index = (uint8_t)(frame->ctrl.mbf.first + 1);

    if (frame->total == 1)
    {
        data = frame->ctrl.buffers[0].data;
        frame->ctrl.buffers[0].data = NULL;
        frame->ctrl.buffers[0].data_length = 0;
        return data;
    }

#ifdef FRAME_ERROR_CHECKING
    if (frame->ctrl.mbf.used == 0)
    {
        return NULL;
    }
#endif // FRAME_ERROR_CHECKING

    data = frame->ctrl.buffers[index].data;
    frame->ctrl.mbf.total_length -= frame->ctrl.buffers[index].data_length;
    frame->ctrl.mbf.total_offset -= frame->ctrl.buffers[index].offset;
    frame->ctrl.buffers[index].data_length = 0;
    frame->ctrl.buffers[index].data = NULL;
 
    /* optimize: frame->ctrl.mbf.first = (frame->ctrl.mbf.first + 1) % frame->total; */
    frame->ctrl.mbf.first++;
    if (frame->ctrl.mbf.first >= frame->total)
        frame->ctrl.mbf.first -= frame->total;
    frame->ctrl.mbf.used--;
 
    return data;
}


/*****************************************************************************/
uint8_t osFrameUsedBuffers(os_frame_t *frame)
{
    OS_ASSERT_COND(frame != NULL);

    if (frame->total == 1)
    {
        return (uint8_t)(frame->ctrl.buffers[0].data ? 1 : 0);
    }
    else
    {
        return frame->ctrl.mbf.used;
    }
}

/*****************************************************************************/
os_status osFrameTailRemove(os_frame_t* frame, uint32_t length)
{
    uint32_t frame_length = (uint32_t)osFrameLengthGet(frame);
    int i;
 
    if(frame->total == 1)
    {
        if(frame_length > length)
        {
            osFrameLengthSet(frame, frame_length - length);
            frame->ctrl.buffers[0].data_length =  (uint32_t)(frame_length - length + (frame)->ctrl.buffers[0].offset);
        }
        else
        {
            uint8_t* buffer = osFrameTailBufferRemove(frame);
            osSharedResourceLock(frame->data_part);
            osMemBlockUnsafeFree(frame->data_part, buffer);
            osSharedResourceUnlock(frame->data_part);
        }
    }
    else
    {
        for(i = frame->ctrl.mbf.last + 1; i >= frame->ctrl.mbf.first + 1; i--)
        {
            if(frame->ctrl.buffers[i].data_length > length)
            {
                osFrameLengthSet(frame, frame_length - length);
                frame->ctrl.buffers[i].data_length =  (uint32_t)(frame_length - length + (frame)->ctrl.buffers[i].offset);
                break;
            }
            else /* current buffer length <= length, so we must remove this buffer */
            {
                uint8_t* buffer;
                frame_length -= frame->ctrl.buffers[i].data_length;
                length -= frame->ctrl.buffers[i].data_length;
                buffer = osFrameTailBufferRemove(frame);
                osSharedResourceLock(frame->data_part);
                osMemBlockUnsafeFree(frame->data_part, buffer);
                osSharedResourceUnlock(frame->data_part);
            }
 
            if(length == 0)
                break;
        }
 
    }
    return OS_SUCCESS;
}

/*****************************************************************************/

os_status osFrameTailAppend(os_frame_t* frame, uint32_t length)
{
    uint32_t block_size = (uint32_t)GET_MAX_BUFFER_SIZE(frame);
    uint32_t free_length;
    uint8_t *data;
    int idx;

    /* frame should not be empty */
    OS_ASSERT_COND(frame->ctrl.buffers[0].data != NULL)

    /* single buffer case */
    if(frame->total == 1)
    {
        free_length = (uint32_t)(block_size - frame->ctrl.buffers[0].data_length);
        if(length <= free_length)
        {
            frame->ctrl.buffers[0].data_length += length;
            return OS_SUCCESS;
        }
 
        return OS_FAIL;
    }

    /* try last buffer */
    idx = frame->ctrl.mbf.last + 1;
    free_length = (uint32_t)(block_size - frame->ctrl.buffers[idx].data_length);
 
    if(free_length >= length)
    {
        frame->ctrl.buffers[idx].data_length += length;
        frame->ctrl.buffers[0].data_length += length;
 
        return OS_SUCCESS;
    }
    else
    {
        length -= free_length;
        frame->ctrl.buffers[idx].data_length += free_length;
        frame->ctrl.buffers[0].data_length += free_length;
    }
 
    /* add new buffers */
    while(frame->ctrl.mbf.used < frame->total)
    {
        free_length = (uint32_t)((length > block_size)?block_size:length);
        length -= free_length;
 
        data = osFrameBufferNew(frame);
        osFrameBufferAppend(frame, data, free_length);
 
        if(length == 0) return OS_SUCCESS;
    }
 
    return OS_FAIL;
}

/*****************************************************************************/
void osFrameDuplicate(os_frame_t *src_frame, os_frame_t *dst_frame)
{

    int i;
    unsigned char * restrict src_buffer;
    unsigned char * restrict dst_buffer;
    unsigned short buf_length;
    unsigned char index = 0;
 
    dst_frame->ref_count = src_frame->ref_count;
 
    if(src_frame->total == 1)
    {
        dst_buffer = osFrameBufferNew(dst_frame);
        dst_frame->ctrl.buffers[0].data_length = src_frame->ctrl.buffers[0].data_length;
        dst_frame->ctrl.buffers[0].offset = src_frame->ctrl.buffers[0].offset;
        dst_frame->ctrl.buffers[0].data = dst_buffer;
        buf_length = (unsigned short)(src_frame->ctrl.buffers[0].data_length - src_frame->ctrl.buffers[0].offset);
        src_buffer = src_frame->ctrl.buffers[0].data + src_frame->ctrl.buffers[0].offset;
        dst_buffer += dst_frame->ctrl.buffers[0].offset;
        for(i = 0; i < buf_length; i++)
            *dst_buffer++ = *src_buffer++;
    }
    else
 
    {
        *(uint32_t *)&dst_frame->ctrl.mbf.total_length = *(uint32_t *)&src_frame->ctrl.mbf.total_length;
        *(uint32_t *)&dst_frame->ctrl.mbf.total_length = *(uint32_t *)&src_frame->ctrl.mbf.used;
        for(index = 1; index <= dst_frame->ctrl.mbf.used; index++ )
        {
            int data_length;
            dst_buffer =  osFrameBufferNew(dst_frame);
            OS_ASSERT_COND(dst_buffer  != NULL);
            dst_frame->ctrl.buffers[index].data = dst_buffer;
            dst_frame->ctrl.buffers[index].data_length =
                src_frame->ctrl.buffers[index].data_length;
            dst_frame->ctrl.buffers[index].offset =
                src_frame->ctrl.buffers[index].offset;
            dst_buffer += dst_frame->ctrl.buffers[index].offset;
            src_buffer = src_frame->ctrl.buffers[index].data + dst_frame->ctrl.buffers[index].offset;
            data_length = (int) (dst_frame->ctrl.buffers[index].data_length - dst_frame->ctrl.buffers[index].offset);
            for(i = 0; i < data_length; i++)
                *dst_buffer++ = *src_buffer++;
        }
    }
}



