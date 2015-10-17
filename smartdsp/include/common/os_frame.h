/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/06/04 08:21:20 $
 $Id: os_frame.h,v 1.57 2013/06/04 08:21:20 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_frame.h,v $
 $Revision: 1.57 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_frame.h

 @Description   OS definitions for frames and buffers.

 @Cautions      Many functions in this file are implemented as static inline
                functions if the file is pre-processed with the define:
                FRAME_SINGLE_BUFFER_ONLY. Use this option ONLY if you know the
                application to require only one buffer per frame in order to
                save cycles. Setting this differently in different parts of
                the application may lead to unexpected behavior.
 
*//***************************************************************************/

#ifndef __OS_FRAME_H
#define __OS_FRAME_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         frame_id Frames API

 @Description   SmartDSP OS implementation of data frames.
 
                Frames and frame pools are used (primarily) with the BIO module
                to perform I/O operations through frame/cell interfaces.
 
 @{
*//***************************************************************************/

#define FRAME_NONE      (-1)      /**< Error code for some frame functions. */


/**************************************************************************//**
 @Description   Frames pool.
 
                The frames pool is required for dynamic allocation and release
                of frames. The application should not create static instances
                of this structure, but use osFramePoolCreate() to get a pointer
                to an initialized frames pool.
 
                The frames pool creates a bank of frames with identical
                attributes (mainly, number of buffers per frame). These frames
                should be accessed only through osFrameGet() and osFrameFree().
 
 @Cautions      The application must not modify the contents of this structure.
 
*//***************************************************************************/
typedef struct
{
    os_mem_part_t   *frames_part;       /**< Pointer to a memory partition. */
    uint8_t         buffers_per_frame;  /**< Total number of buffers per frame. */
    uint8_t         frame_expansion;    /**< The expansion size of each frame. */
    uint32_t        init_frame_offset;  /**< Number of bytes reserved for LLD upon frame creation. */
    void *          (*f_MemBlockGet)(os_mem_part_t *mem_part);
    /**< Frame allocation function pointer. */
    void            (*f_MemBlockFree)(os_mem_part_t *mem_part, void *addr);
    /**< Frame deletion function pointer. */
} os_frames_pool_t;


/**************************************************************************//**
 @Description   Data frame structure.
 
                The frame structure is used for holding frame-oriented data
                (an alternative notion may be "cell" or "packet"). A frame may
                contain a variable number of buffers for holding the data, but
                this number must be set before using the frame (when the frames
                pool is created).
 
                The frame structure is much faster and efficient when using
                single-buffer frames. But, you can create frame pools with
                different number of buffers. If you are using ONLY single-buffer
                frames in your application, it is recommended to define
                FRAME_SINGLE_BUFFER_ONLY before including %smartdsp_os.h. This
                would let your application use a faster implementation of the
                frames functions. There is no error checking in this case, so it
                is recommended to define it only after the application is stable.
 
                A frame can contain a 32-bits value that represents the user's
                information (usually an index or a pointer to some other user's
                information structure). You can access this information using
                osFrameInfoSet() and osFrameInfoGet().
 
                In addition, the frame structure can be expanded to any size,
                in case additional information (besides the buffers) is required.
                The expansion size is defined when the frames pool is created.
                You can call osFrameExpansionGet() to get a pointer to the
                expanded area, and osFrameExpansionSize() to get the size of the
                expanded area.
 
                Because of the frame structure's dynamic nature, it should be
                accessed only through a frames pool.
 
 @Cautions      The size of the frame structure is dynamic, and depends on the
                number of data buffers in the frame and the expansion size.
                You must not use a straight-forward array of frames, and not
                even a single frame instance. Instead, you should create a
                frames pool for allocating and deleting frames.

                NOTE: The frame structure is extended according to the maximum number
                of buffers in the frame and the expansion area of the frame. Do not
                use sizeof(os_frame_t) in your calculations!
 
*//***************************************************************************/
typedef struct
{
    union
    {
        os_mem_part_t   *data_part;             /**< Memory partition for data buffer.   */
        void            *lld_channel_handle;    /**< LLD handler for buffers management. */
    };

    os_frames_pool_t    *frames_pool;           /**< Backward reference to frame  pool */

    uint32_t            info;                   /**< Owner's custom information. */
    uint16_t            total;                  /**< Total number of buffers in frame. */
    uint8_t             expansion;              /**< Size (in bytes) of the optional frame's expansion. */
    uint8_t             ref_count;              /**< Reference counter.                                 */
    union
    {
        struct
        {
            uint32_t    total_length;           /**< Total length of data, in bytes (includes data and offset). */
            int32_t     total_offset;           /**< Total length of reserved header size. */
            uint8_t     used;                   /**< Number of used buffers in frame. */
            uint8_t     first;                  /**< Index of first buffer in frame. */
            uint8_t     last;                   /**< Index of last buffer in frame. */
            uint8_t     reserved;
        } mbf;                                  /**< Multi-buffer frame management. */
 
        struct
        {
            uint32_t    data_length;            /**< Length of data block, in bytes. */
            int32_t     offset;                 /**< Reserved header size. */
                                                /**< When negative, space is taken from init_frame_offset */
            uint8_t     *data;                  /**< Pointer to the data block. */
        } buffers[1];                           /**< Dynamic array of data buffers. */
    } ctrl;
} os_frame_t;


/**************************************************************************//**
 @Function      GET_MAX_BUFFER_SIZE

 @Description   Returns the maximal capacity of the frame's buffers
 
 @Param[in]     frame -  Pointer to the frame

 @Return        Size of frame's buffers (in bytes)
 
*//***************************************************************************/
#define GET_MAX_BUFFER_SIZE(frame) (frame->data_part->block_size)


/**************************************************************************//**
 @Function      osFramePoolCreate

 @Description   Creates a pool of frames with the given attributes.
 
                This function creates a bank of frames with identical attributes
                (number of buffers per frame, and expansion size). The frames
                must be accessed only through osFrameGet() and osFrameFree().
 
 @Param[in]     total_frames      -  Required number of frames in the pool.
 @Param[in]     buffers_per_frame -  Maximum number of buffers per frame.
 @Param[in]     frame_expansion   -  Expansion size for the frames (in bytes).
 @Param[in]     frame_offset      -  Reserved header size for the frames (in bytes).
 @Param[in]     mem_type          -  Memory type (local, shared, etc.) for
                                     the pool (NOT the frames' data buffers).

 @Return        Pointer to the initialized frames pool; NULL if not enough
                memory is available.
 
*//***************************************************************************/
os_frames_pool_t *  osFramePoolCreate(uint16_t      total_frames,
                                      uint8_t       buffers_per_frame,
                                      uint16_t      frame_expansion,
                                      uint32_t      frame_offset,
                                      os_mem_type   mem_type);


/**************************************************************************//**
 @Function      osFrameGet

 @Description   Allocates an empty frame from a frames pool.
 
 @Param[in]     frames_pool   -  Pointer to an initialized frames pool.
 @Param[in]     data_mem_part -  Pointer to the buffers pool for the
                                 frame (an initialized memory partition).

 @Return        Pointer to the allocated frame; NULL if no frame is available.
 
*//***************************************************************************/
#ifndef FRAME_SINGLE_BUFFER_ONLY
os_frame_t *    osFrameGet(os_frames_pool_t *frames_pool,
                           os_mem_part_t    *data_mem_part);
#else
INLINE os_frame_t * osFrameGet(os_frames_pool_t *frames_pool,
                               os_mem_part_t    *data_mem_part)
{
    os_frame_t *frame;
 
    OS_ASSERT_COND(frames_pool != NULL && data_mem_part != NULL);
    OS_ASSERT_COND(frames_pool->init_frame_offset <= data_mem_part->buffer_offset);

    frame = (os_frame_t *)frames_pool->f_MemBlockGet(frames_pool->frames_part);
    if (frame)
    {
        frame->data_part = data_mem_part;
        frame->frames_pool = frames_pool;
        frame->total = frames_pool->buffers_per_frame;
        frame->expansion = frames_pool->frame_expansion;
        frame->ctrl.buffers[0].offset = 0;
        frame->ctrl.buffers[0].data_length = 0;
        frame->ctrl.buffers[0].data = NULL;
        frame->info = 0;
        frame->ref_count = 1;
    }
    return frame;
}
#endif // FRAME_SINGLE_BUFFER_ONLY


/**************************************************************************//**
 @Function      osFrameRelease

 @Description   Releases a frame back to the frames pool.
 
                This function will decrement the frame's reference counter.
                If the counter reaches 0, the frame and all valid buffers
                of the frame are released.
 
 @Param[in]     frame -  Pointer to the frame to delete.
 
*//***************************************************************************/
#ifndef FRAME_SINGLE_BUFFER_ONLY
void    osFrameRelease(os_frame_t *frame);
#else
INLINE void osFrameRelease(os_frame_t *frame)
{
    os_frames_pool_t *frames_pool = frame->frames_pool;

    osHwiSwiftDisable();
    if (--frame->ref_count)
    {
        osHwiSwiftEnable();
        return;
    }
    osHwiSwiftEnable();
 
    if (frame->ctrl.buffers[0].data)
        osMemBlockSyncFree(frame->data_part, frame->ctrl.buffers[0].data);
    frames_pool->f_MemBlockFree(frames_pool->frames_part, (void *)frame);
}


#endif // FRAME_SINGLE_BUFFER_ONLY



/**************************************************************************//**
 @Function      osFrameBufferNew

 @Description   Allocates a new data buffer from the frame's buffers pool.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        Pointer to the allocated buffer; NULL if no buffer is available.
 
*//***************************************************************************/
#define osFrameBufferNew(frame) \
    ((uint8_t *)osMemBlockSyncGet((frame)->data_part))


/**************************************************************************//**
 @Function      osFrameBufferFree

 @Description   Releases a frame's buffer back to the frame's buffers pool.
 
 @Param[in]     frame -  Pointer to the frame.
 @Param[in]     data  -  The data buffer to delete.
 
*//***************************************************************************/
#define osFrameBufferFree(frame, data) \
    osMemBlockSyncFree((frame)->data_part, (void *)(data))


/**************************************************************************//**
 @Function      osFrameInfoGet

 @Description   Gets the user's information from the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The user's information (uint32_t).
 
*//***************************************************************************/
#define osFrameInfoGet(frame)       ((frame)->info)


/**************************************************************************//**
 @Function      osFrameInfoSet

 @Description   Sets the user's information to the frame.
 
 @Param[in]     frame      -  Pointer to the frame.
 @Param[in]     owner_info -  User's information (uint32_t).
 
*//***************************************************************************/
#define osFrameInfoSet(frame, owner_info) \
    { (frame)->info = (uint32_t)(owner_info); }

#ifndef B4860
/**************************************************************************//**
 @Function      osFrameSecInfoGet

 @Description   Gets the security information from the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The security information information (os_sec_info_t*).
 
 @Cautions      The security information shares space with the user information
 
*//***************************************************************************/
#define osFrameSecInfoGet(frame)       ((os_sec_info_t*)(frame)->info)


/**************************************************************************//**
 @Function      osFrameSecInfoSet

 @Description   Sets the security information to the frame.
 
 @Param[in]     frame    -  Pointer to the frame.
 @Param[in]     sec_info -  security information (uint32_t).
 
 @Cautions      The security information shares space with the user information
 
*//***************************************************************************/
#define osFrameSecInfoSet(frame, sec_info) \
    { (frame)->info = (uint32_t)(sec_info); }
#endif //B4860

/**************************************************************************//**
 @Function      osFrameRefCntInc

 @Description   Increment the frame's reference counter.
 
 @Param[in]     frame -  Pointer to the frame.
 
*//***************************************************************************/

#define osFrameRefCntInc(frame) \
    {  frame->ref_count++;}

/**************************************************************************//**
 @Function      osFrameRefCntDec

 @Description   Decrement the frame's reference counter.
 
 @Param[in]     frame -  Pointer to the frame.
 
*//***************************************************************************/
#define osFrameRefCntDec(frame) \
    { frame->ref_count--;}

/**************************************************************************//**
 @Function      osFrameRefCntGet

 @Description   Gets the frame's reference counter.
 
 @Param[in]     frame -  Pointer to the frame.
 
*//***************************************************************************/
#define osFrameRefCntGet(frame) \
    ( frame->ref_count )

/**************************************************************************//**
 @Function      osFrameRefCntSet

 @Description   Gets the frame's reference counter.
 
 @Param[in]     frame -  Pointer to the frame
 @Param[in]     count -  New reference count value (higher than 0)
 
 @Cautions      Setting the reference count directly is not the recommended
                way; you should use osFrameRefCntInc() or osFrameRefCntDec()
                in the general use-case
 
*//***************************************************************************/
#define osFrameRefCntSet(frame, count) \
    { OS_ASSERT_COND((count) > 0); frame->ref_count = (count); }

/**************************************************************************//**
 @Function      osFrameDataPartGet

 @Description   Gets memory partition of frame's buffers
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        Pointer to the memory partition of frame's buffers
 
*//***************************************************************************/
#define osFrameDataPartGet(frame) ((os_mem_part_t*)frame->data_part)
 

/**************************************************************************//**
 @Function      osFrameLengthGet

 @Description   Gets the total length of the frame's data.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The total length of the frame's data (in bytes).
 
*//***************************************************************************/
#define osFrameLengthGet(frame) \
    ((frame)->ctrl.buffers[0].data_length - (frame)->ctrl.buffers[0].offset)


/**************************************************************************//**
 @Function      osFrameLengthSet

 @Description   Sets the total length of the frame's data.
 
 @Param[in]     frame -  Pointer to the frame.
 @Param[in]     length -  The total length of the frame's data (uint32_t).
 
 @Cautions      Refrain from using this with multi-buffer frames, because it
                does not perform checks on the number of buffers after the
                change.
 
*//***************************************************************************/
#define osFrameLengthSet(frame, length) \
    { (frame)->ctrl.buffers[0].data_length = (uint32_t)((length) + (frame)->ctrl.buffers[0].offset); }


/**************************************************************************//**
 @Function      osFrameBufferOffsetGet

 @Description   Gets the size of the frame's reserved header space (offset) in the
                specified buffer. When working with single-buffer frames the
                reserved header space (offset) of the first and only buffer
                will be  returned.
 
 @Param[in]     frame -  Pointer to the frame.
 @Param[in]     index -  The index of the buffer for the requested offset.

 @Return        The size of the frame's reserved header (in bytes).

*//***************************************************************************/
#ifndef FRAME_SINGLE_BUFFER_ONLY
int32_t osFrameBufferOffsetGet(os_frame_t *frame, uint8_t index);
#else
INLINE int32_t osFrameBufferOffsetGet(os_frame_t *frame, uint8_t index)
{
    VAR_UNUSED(index);
    OS_ASSERT_COND(index == 0);
    return ((frame)->ctrl.buffers[0].offset);
}
#endif

/**************************************************************************//**
 @Function      osFrameOffsetGet

 @Description   Gets the size of the frame's first buffer reserved header space

 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The size of the frame's first buffer reserved header (in bytes).
 
*//***************************************************************************/
#define osFrameOffsetGet(frame)     osFrameBufferOffsetGet((frame), 0)


/**************************************************************************//**
 @Function      osFrameBufferOffsetSet

 @Description   Sets the size of the frame's specified buffer reserved header space

 @Param[in]     frame         -  Pointer to the frame.
 @Param[in]     index         -  Index of the buffer which should be modified.
 @Param[in]     header_length -  The length (in bytes) to set as a header (offset).
 
 @Return        A pointer to the new data position in the specified buffer
 
*//***************************************************************************/
INLINE uint8_t * osFrameBufferOffsetSet(os_frame_t *frame, int8_t index, int32_t header_length)
{
#ifdef FRAME_SINGLE_BUFFER_ONLY
    VAR_UNUSED(index);
    OS_ASSERT_COND(index == 0);
    frame->ctrl.buffers[0].offset = header_length;
    OS_ASSERT_COND(frame->ctrl.buffers[0].offset <= frame->ctrl.buffers[0].data_length);
    return (uint8_t *)(frame->ctrl.buffers[0].data + frame->ctrl.buffers[0].offset);
#else
    int idx = (uint8_t)(frame->ctrl.mbf.first + index);

    if (frame->total == 1)
        idx = 0;
    else
    {
        if (idx >= frame->total)
            idx -= frame->total;
        idx++;
    }
    OS_ASSERT_COND(frame->ctrl.buffers[idx].offset <= frame->ctrl.buffers[idx].data_length);
    frame->ctrl.mbf.total_offset = (int32_t)(frame->ctrl.mbf.total_offset
                                 - frame->ctrl.buffers[idx].offset
                                 + header_length);
    frame->ctrl.buffers[idx].offset = (int32_t)header_length;
    return (uint8_t *)(frame->ctrl.buffers[idx].data + frame->ctrl.buffers[idx].offset);
#endif // FRAME_SINGLE_BUFFER_ONLY
}


/**************************************************************************//**
 @Function      osFrameOffsetBuffer

 @Description   Offsets a given buffer with the frame's first offset (reserved header
                size). You can use this macro to get the correct position to
                fill a new buffer (returned by osFrameBufferNew()) before adding
                it as the first (or only) buffer in the frame. Note the example
                below:
 
                @code
                uint8_t *buffer = osFrameBufferNew(frame);
                uint8_t *data1 = osFrameOffsetBuffer(frame, buffer);
                uint8_t *data2 = (uint8_t *)(buffer + osFrameOffsetGet(frame));
                OS_ASSERT_COND(data1 == data2);
                @endcode
 
 @Param[in]     frame -  Pointer to the frame.
 @Param[in]     buffer -  Pointer to a buffer.
 
 @Return        A pointer to the data position in the buffer (after the offset).
 
*//***************************************************************************/
#ifdef FRAME_SINGLE_BUFFER_ONLY
#define osFrameOffsetBuffer(frame, buffer) \
    ((uint8_t *)((buffer) + (frame)->ctrl.buffers[0].offset))
#else
#define osFrameOffsetBuffer(frame, buffer) \
    (uint8_t *)((buffer) + ((frame)->total == 1) ? (frame)->ctrl.buffers[0].offset : \
                                                   (frame)->ctrl.buffers[(frame)->ctrl.mbf.first+1].offset))
#endif // FRAME_SINGLE_BUFFER_ONLY



/**************************************************************************//**
 @Function      osFrameUsedBuffers

 @Description   Gets the number of used (valid) buffers in the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The number of used (valid) buffers in the frame.
 
*//***************************************************************************/
uint8_t   osFrameUsedBuffers(os_frame_t *frame);


/**************************************************************************//**
 @Function      osFrameTotalBuffers

 @Description   Gets the total number of buffers (used or unused) in the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The total number of buffers in the frame.
 
*//***************************************************************************/
#define osFrameTotalBuffers(frame)  ((frame)->total)


/**************************************************************************//**
 @Function      osFrameExpansionGet

 @Description   Gets a pointer to the expansion area of the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        A pointer to the expansion area of the frame.
 
 @Cautions      This function does not verify that the frame contains an
                expansion area - it is the responsibility of the user. You
                can check the expansion size with osFrameExpansionSize().
 
*//***************************************************************************/
#ifndef FRAME_SINGLE_BUFFER_ONLY
#define osFrameExpansionGet(frame) \
    ((void *)&((frame)->ctrl.buffers[((frame)->total == 1) ? 1 : ((frame)->total + 1)]))
#else
#define osFrameExpansionGet(frame) \
    ((void *)&((frame)->ctrl.buffers[1]))
#endif // FRAME_SINGLE_BUFFER_ONLY


/**************************************************************************//**
 @Function      osFrameExpansionSize

 @Description   Gets the size of the expansion area of the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The size (in bytes) of the expansion area of the frame.
 
*//***************************************************************************/
#define osFrameExpansionSize(frame) ((frame)->expansion)


/**************************************************************************//**
 @Function      osFrameBufferGet

 @Description   Gets a data buffer from the frame, according to the given index.
 
 @Param[in]     frame  -  Pointer to the frame.
 @Param[out]    length -  Receives the length of the buffer (in bytes).
 @Param[out]    index  -  Pointer to the index of the requested buffer.
                          When the function returns, the pointer will receive
                          the index of the next available buffer (FRAME_NONE
                          if there are no more valid buffers).
 
 @Return        The requested buffer; NULL if index is not valid.
 
 @Cautions      Do not free the buffer that is returned by this function. You
                can only free buffers that are returned by osFrameBufferSet(),
                osFrameHeadBufferRemove() or osFrameTailBufferRemove().
 
*//***************************************************************************/
#ifndef FRAME_SINGLE_BUFFER_ONLY
uint8_t * osFrameBufferGet(os_frame_t *frame, uint32_t *length, int8_t *index);
#else
INLINE  uint8_t * osFrameBufferGet(os_frame_t *frame, uint32_t *length, int8_t *index)
{
    OS_ASSERT_COND(frame->ctrl.buffers[0].data != NULL);
    if (index)
        *index = FRAME_NONE;
    *length = (uint32_t)(frame->ctrl.buffers[0].data_length - frame->ctrl.buffers[0].offset);
    return (uint8_t *)(frame->ctrl.buffers[0].data + frame->ctrl.buffers[0].offset);
}
#endif // FRAME_SINGLE_BUFFER_ONLY


/**************************************************************************//**
 @Function      osFrameSingleBufferGet

 @Description   Macro for calling osFrameBufferGet() on a single-buffer frame.

 @Param[in]     frame  -  Pointer to the frame.
 @Param[out]    length -  Receives the length of the buffer (in bytes).

 @Return        The requested buffer; NULL if not valid.
 
*//***************************************************************************/
#define osFrameSingleBufferGet(frame, length) \
            osFrameBufferGet((frame), (length), NULL)


/**************************************************************************//**
 @Function      osFrameDataStart

 @Description   Returns a pointer to the first data byte (in the first buffer).
 
                The first data buffer may contain a reserved header space
                (offset) before the actual data. To reserve a header size, you
                should specify the frame offset when creating the frames pool.
                You can then push and pop headers to that reserved size using
                osFrameHeaderPush() and osFrameHeaderPop().
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        A pointer to the first data byte in the first buffer.
 
*//***************************************************************************/
#ifndef FRAME_SINGLE_BUFFER_ONLY
uint8_t * osFrameDataStart(os_frame_t *frame);
#else
#define osFrameDataStart(frame) \
            ((uint8_t *)((frame)->ctrl.buffers[0].data + (frame)->ctrl.buffers[0].offset))
#endif // FRAME_SINGLE_BUFFER_ONLY


/**************************************************************************//**
 @Function      osFrameBufferSet

 @Description   Replaces an existing data buffer at the specified index.
 
                For single-buffer frames, this function can be used even if the
                frame is empty. For multi-buffer frames, you should use the
                osFrameBufferAppend() to add the first buffer to the frame.
 
                If the index already contains a data buffer, the old buffer
                will be returned to the user.
 
 @Param[in]     frame  -  Pointer to the frame.
 @Param[in]     data   -  The data buffer to set.
 @Param[in]     length -  The length of the data buffer (in bytes).
 @Param[in]     index  -  The buffer's index in the frame.
 
 @Return        The previous data buffer at the given index. If this is a valid
                buffer (not NULL), it must be freed using osFrameBufferFree().
 
 @Cautions      This function does not modify the offset, if you want to
                modify the offset call OsFrameOffsetSet() after calling this
                function.
 
*//***************************************************************************/
#ifndef FRAME_SINGLE_BUFFER_ONLY
uint8_t * osFrameBufferSet(os_frame_t *frame, uint8_t *data, uint32_t length, int8_t index);
#else
INLINE uint8_t * osFrameBufferSet(os_frame_t *frame, uint8_t *data, uint32_t length, int8_t index)
{
    uint8_t *prev_data = frame->ctrl.buffers[index].data;
    frame->ctrl.buffers[index].data = data;
    frame->ctrl.buffers[index].data_length = length;
    return prev_data;
}
#endif // FRAME_SINGLE_BUFFER_ONLY


/**************************************************************************//**
 @Function      osFrameSingleBufferSet

 @Description   Macro for calling osFrameBufferSet() on a single-buffer frame.

 @Param[in]     frame  -  Pointer to the frame.
 @Param[in]     data   -  The data buffer to set.
 @Param[in]     length -  The length of the data buffer (in bytes).
 
 @Return        The previous data buffer. If this is a valid buffer (not NULL),
                it must be freed using osFrameBufferFree().
 
*//***************************************************************************/
#define osFrameSingleBufferSet(frame, data, length) \
            osFrameBufferSet((frame), (data), (length), 0)


/**************************************************************************//**
 @Function      osFrameBufferAppend

 @Description   Adds a new data buffer after the last buffer of the frame.
 
 @Param[in]     frame  -  Pointer to the frame.
 @Param[in]     data   -  The data buffer.
 @Param[in]     length -  The length of the data (in bytes).
 
 @Return        Number of used buffers in the frame after the operation;
                FRAME_NONE if the buffer cannot be added.
 
*//***************************************************************************/
#ifdef FRAME_SINGLE_BUFFER_ONLY
INLINE int8_t    osFrameBufferAppend(os_frame_t *frame, uint8_t *data, uint32_t length)
{
    osFrameBufferSet((frame), (data), (length), 0);
    return 1;
}
#else
int8_t    osFrameBufferAppend(os_frame_t *frame, uint8_t *data, uint32_t length);
#endif

/**************************************************************************//**
 @Function      osFrameBufferPrepend

 @Description   Adds a new data buffer before the first buffer of the frame.
 
 @Param[in]     frame  -  Pointer to the frame.
 @Param[in]     data   -  The data buffer.
 @Param[in]     length -  The length of the data (in bytes).
 
 @Return        Number of used buffers in the frame after the operation;
                FRAME_NONE if the buffer cannot be added.
 
*//***************************************************************************/
int8_t    osFrameBufferPrepend(os_frame_t *frame, uint8_t *data, uint32_t length);


/**************************************************************************//**
 @Function      osFrameHeadBufferRemove

 @Description   Removes the first data buffer from the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The first data buffer in the frame; NULL if no buffers exist.
                This buffer must be freed using osFrameBufferFree().
 
*//***************************************************************************/
uint8_t * osFrameHeadBufferRemove(os_frame_t *frame);


/**************************************************************************//**
 @Function      osFrameTailBufferRemove

 @Description   Removes the last data buffer from the frame.
 
 @Param[in]     frame -  Pointer to the frame.
 
 @Return        The last data buffer in the frame; NULL if no buffers exist.
                This buffer must be freed using osFrameBufferFree().
 
*//***************************************************************************/
uint8_t * osFrameTailBufferRemove(os_frame_t *frame);


/**************************************************************************//**
 @Function      osFrameHeaderPush

 @Description   Adds a given size to the frame header in the first buffer.
 
                To reserve a header size (in the first buffer only), you should
                specify the frame offset when creating the frames pool. You can
                push headers to that reserved size, but you cannot exceed it.
 
 @Param[in]     frame         -  Pointer to the frame.
 @Param[in]     header_length -  The length (in bytes) to add as a header.
 
 @Return        A pointer to the new position of the first data byte.
 
*//***************************************************************************/
INLINE uint8_t * osFrameHeaderPush(os_frame_t *frame, uint32_t header_length)
{
#ifdef FRAME_SINGLE_BUFFER_ONLY
    OS_ASSERT_COND(frame->ctrl.buffers[0].offset + frame->frames_pool->init_frame_offset >= header_length);
    frame->ctrl.buffers[0].offset -= header_length;
    return osFrameDataStart(frame);
#else
    int index = frame->ctrl.mbf.first + 1;
    if (frame->total == 1)
        index = 0;
    OS_ASSERT_COND(frame->ctrl.buffers[index].offset + frame->frames_pool->init_frame_offset >= header_length);
    frame->ctrl.mbf.total_offset -= header_length;
    if (frame->total != 1)
        frame->ctrl.buffers[index].offset -= header_length;
    return osFrameDataStart(frame);
#endif
}

/**************************************************************************//**
 @Function      osFrameHeaderPop

 @Description   Subtracts a given size from the frame header in the first buffer.
 
                To reserve a header size (in the first buffer only), you should
                specify the frame offset when creating the frames pool. You can
                pop headers from that first buffer, but you must not exceed the
                length of the buffer.
 
 @Param[in]     frame         -  Pointer to the frame.
 @Param[in]     header_length -  The length (in bytes) to remove from the header.
 
 @Return        A pointer to the new position of the first data byte.
 
*//***************************************************************************/
INLINE uint8_t * osFrameHeaderPop(os_frame_t *frame, uint32_t header_length)
{
#ifdef FRAME_SINGLE_BUFFER_ONLY
    frame->ctrl.buffers[0].offset += header_length;
    OS_ASSERT_COND(frame->ctrl.buffers[0].offset <= frame->ctrl.buffers[0].data_length);
    return osFrameDataStart(frame);
#else
    int index;
    if (frame->total == 1)
        index = 0;
    else
    {
        index = frame->ctrl.mbf.first + 1;
        frame->ctrl.buffers[index].offset += header_length;
    }
    frame->ctrl.mbf.total_offset += header_length;
    OS_ASSERT_COND(frame->ctrl.buffers[index].offset <= frame->ctrl.buffers[index].data_length);
    return osFrameDataStart(frame);
#endif
}


/**************************************************************************//**
 @Function      osFrameBufferSetWithOffset

 @Description   Replaces the (only) data buffer and sets its data offset.
 
                This function is available for single-buffer frames only.
                If the frame already contains a data buffer, the old buffer
                will be returned to the user.
 
 @Param[in]     frame  -  Pointer to the frame.
 @Param[in]     data   -  The data buffer to set.
 @Param[in]     length -  The length of the data buffer (in bytes).
 @Param[in]     offset -  The data offset (header space) for the frame.
 
 @Return        The previous data buffer. If this is a valid buffer (not NULL),
                it must be freed using osFrameBufferFree().
 
 @Cautions      This frame is available for single-buffer frames only.
                Use this function only if the buffers_per_frame in osFramePoolCreate
                equals 1.
 
*//***************************************************************************/
INLINE uint8_t * osFrameBufferSetWithOffset(os_frame_t *frame, uint8_t *data, uint32_t length, uint32_t offset)
{
 
#ifdef FRAME_SINGLE_BUFFER_ONLY
    uint8_t *prev_data = frame->ctrl.buffers[0].data;
    frame->ctrl.buffers[0].data = data;
    frame->ctrl.buffers[0].data_length = length;
    frame->ctrl.buffers[0].offset = (int32_t)offset;
    return prev_data;
 
#else // FRAME_SINGLE_BUFFER_ONLY
    osFrameBufferAppend(frame, data, length);
    osFrameHeaderPop(frame, offset);
#endif
    return NULL;
}


/**************************************************************************//**
 @Function      osFrameTailRemove

 @Description   Removes data from the end of frame.
 
 @Param[in]     frame  -  Pointer to the frame.
 @Param[in]     length -  Number bytes to chop
 
 @Return        The last data buffer in the frame; NULL if no buffers exist.
                This buffer must be freed using osFrameBufferFree().
 
*//***************************************************************************/
#ifdef FRAME_SINGLE_BUFFER_ONLY
INLINE os_status osFrameTailRemove(os_frame_t* frame, uint32_t length)
{
    if(frame->ctrl.buffers[0].data_length > length)
        frame->ctrl.buffers[0].data_length -= length;
    else
    {
        uint8_t* buffer = osFrameTailBufferRemove(frame);
        osSharedResourceLock(frame->data_part);
        osMemBlockUnsafeFree(frame->data_part, buffer);
        osSharedResourceUnlock(frame->data_part);
    }
    return OS_SUCCESS;
}
#else
os_status osFrameTailRemove(os_frame_t* frame, uint32_t length);
#endif


/**************************************************************************//**
 @Function      osFrameTailAppend

 @Description   Appends data to the end of frame.
 
 @Param[in]     frame  -  Pointer to the frame.
 @Param[in]     length -  Number of bytes to add
 
 @Return        The last data buffer in the frame; NULL if no buffers exist.
                This buffer must be freed using osFrameBufferFree().
 
*//***************************************************************************/
#ifdef FRAME_SINGLE_BUFFER_ONLY
INLINE os_status osFrameTailAppend(os_frame_t* frame, uint32_t length)
{
    OS_ASSERT_COND(frame->ctrl.buffers[0].data != NULL);
 
    if(length <= GET_MAX_BUFFER_SIZE(frame) - frame->ctrl.buffers[0].data_length)
       frame->ctrl.buffers[0].data_length += length;
    else
        return OS_FAIL;
 
    return OS_SUCCESS;
}
#else
os_status osFrameTailAppend(os_frame_t* frame, uint32_t length);
#endif


/**************************************************************************//**
 @Function      osFrameDuplicate

 @Description   Duplicate the given frame including all buffers

 @Param[in]     src_frame -  Pointer to the source frame.
 @Param[in]     dst_frame -  Pointer to the destination frame.
 
*//***************************************************************************/
void osFrameDuplicate(os_frame_t *src_frame, os_frame_t *dst_frame);


/**************************************************************************//**
 @Function      osFrameCopyToMem

 @Description   Copies frame data to continuous memory
 
 @Param[in]     frame -  Pointer to the frame.
 @Param[in]     mem   -  The data buffer to copy to.
 
 @Cautions      mem should point at a memory location with enough space for
                osFrameLengthGet() bytes
 
*//***************************************************************************/
INLINE void osFrameCopyToMem(os_frame_t *frame, uint8_t *mem)
{

    int i;
    uint8_t* buffer;
    uint32_t buf_length;
    int8_t index = 0;
    do
    {
        buffer = osFrameBufferGet(frame, &buf_length,&index);
        for(i = 0; i < buf_length; i++)
            *mem++ = *buffer++;
    }while(index != FRAME_NONE);

}


/**************************************************************************//**
 @Function      osFramePoolNumBuffersGet

 @Description   Gets the max number of buffers per frame in the frame pool.
 
 @Param[in]     frame_pool -  Pointer to the frame pool.
 
 @Return        Maximal number of buffers per frame.
 
*//***************************************************************************/
#define osFramePoolNumBuffersGet(frame_pool)  \
    ((frame_pool)->buffers_per_frame)


/**************************************************************************//**
 @Function      osFrameLldHeaderVerify

 @Description   Verifies the LLD gets 0 offset and enough header space.
 
 @Param[in]     frame -  Pointer to the frame.
 @Param[in]     header_length -  The length (in bytes) to verify.
 
 @Return        Status.
 
 @Cautions      This function is not supported for Multi-Buffer frames
 
*//***************************************************************************/
INLINE os_status osFrameLldHeaderVerify(os_frame_t *frame, uint32_t header_length)
{
    OS_ASSERT_COND(frame->total == 1);
    if(((frame->ctrl.buffers[0].offset + frame->frames_pool->init_frame_offset) < header_length) ||
       (frame->ctrl.buffers[0].offset != 0))
    {
        OS_ASSERT;
        return OS_FAIL;
    }
    return OS_SUCCESS;
}

/** @} */ // end of frame_id

_OS_END_EXTERN_C

#endif // __OS_FRAME_H

