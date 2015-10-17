/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/01/19 14:34:29 $
 $Id: os_frame_init.c,v 1.24 2011/01/19 14:34:29 eitanb Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_frame_init.c,v $
 $Revision: 1.24 $
******************************************************************************/

/******************************************************************************
 
 @File          os_frame_init.c

 @Description   OS Frames initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"



typedef struct
{
    uint32_t    data_length;
    int32_t     offset;         /**< Reserved header size (same as in the @a mbf structure). */
    uint8_t     *data;
} frame_buffer_t_;


/*****************************************************************************/
os_status osFrameInitialize()
{
    return OS_SUCCESS;
}


/*****************************************************************************/
os_frames_pool_t * osFramePoolCreate(uint16_t       total_frames,
                                     uint8_t        buffers_per_frame,
                                     uint16_t       frame_expansion,
                                     uint32_t       init_frame_offset,
                                     os_mem_type    mem_type)
{
    os_frames_pool_t        *frames_pool;
    volatile os_mem_part_t  *mem_part;
    uint8_t                 *base_addr;
    uint32_t                frame_size;
 
    if (total_frames && buffers_per_frame)
    {
        frames_pool = (os_frames_pool_t *)osMalloc(sizeof(os_frames_pool_t), mem_type);
 
        if (frames_pool == NULL)
        {
            return NULL;
        }

        frames_pool->frames_part = (os_mem_part_t *)
            osMalloc(MEM_PART_SIZE(total_frames), mem_type);
 
        if (frames_pool->frames_part == NULL)
        {
            return NULL;
        }
 
        frame_size = (uint32_t)sizeof(os_frame_t);
 
        if (buffers_per_frame > 1)
        {
            frame_size += (buffers_per_frame * sizeof(frame_buffer_t_));
        }
 
        frame_size += frame_expansion;
 
        base_addr = (uint8_t *)
            osMalloc(MEM_PART_DATA_SIZE(total_frames, frame_size, ALIGNED_4_BYTES), mem_type);
 
        if (base_addr == NULL)
        {
            return NULL;
        }
 
        /* do not change this indirect assignment in the code below.
           direct assignment caused problems in optimized code. */
        mem_part = osMemPartCreate(frame_size,
                                   total_frames,
                                   base_addr,
                                   ALIGNED_4_BYTES,
                                   OFFSET_0_BYTES,
                                   frames_pool->frames_part
#if (OS_MULTICORE == 1)
                                   ,((mem_type & OS_MEM_SHARED_TYPE) != 0)
#endif
                                   );
        frames_pool->frames_part = (os_mem_part_t *)mem_part;
 
        frames_pool->buffers_per_frame = buffers_per_frame;
        frames_pool->frame_expansion   = (uint8_t)frame_expansion;
        frames_pool->init_frame_offset = init_frame_offset;

 
#if (OS_MULTICORE == 1)
        if ((mem_type & OS_MEM_SHARED_TYPE) != 0)
        {
            frames_pool->f_MemBlockGet = osMemBlockSyncGet;
            frames_pool->f_MemBlockFree = osMemBlockSyncFree;
        }
        else
#endif // OS_MULTICORE
        {
            frames_pool->f_MemBlockGet = osMemBlockGet;
            frames_pool->f_MemBlockFree = osMemBlockFree;
        }
 
        return frames_pool;
    }
 
    return NULL;
}


