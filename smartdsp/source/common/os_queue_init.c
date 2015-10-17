/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/03/24 16:39:22 $
 $Id: os_queue_init.c,v 1.33 2014/03/24 16:39:22 b41640 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_queue_init.c,v $
 $Revision: 1.33 $
******************************************************************************/

/******************************************************************************
 
 @File          os_queue_init.c

 @Description   OS Queue initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_rm_.h"
#include "os_queue_.h"
#if (OS_MULTICORE == 1)
#include "os_queue_shared_.h"
#endif /* OS_MULTICORE */

#include "os_l1_defense.h"

uint16_t    g_total_queues;
os_queue_t  *g_queue_list = NULL;

 
/*****************************************************************************/
#if (OS_MULTICORE == 1)
os_status osQueueInitialize(uint16_t total_queues, uint16_t total_squeues)
#else
os_status osQueueInitialize(uint16_t total_queues)
#endif /* OS_MULTICORE */
{
	g_queue_list = NULL;
    g_total_queues = total_queues;
 
    if (g_total_queues > 0)
    {
        g_queue_list = (os_queue_t *)
            osMalloc((sizeof(os_queue_t) * g_total_queues), OS_MEM_LOCAL);
 
        if (g_queue_list == NULL)
        {
            RETURN_ERROR(OS_ERR_NO_MEMORY);
        }
    }

#if (OS_MULTICORE == 1)
    g_total_squeues = total_squeues;
 
    if((osL1dGetResetMode() == OS_L1D_NO_WARM_RESET) || (osL1dGetResetMode() == OS_L1D_MODE_3))
    {
        if (osGetCoreID() == osGetMasterCore())
        {
            if (g_total_squeues > 0)
            {
                g_squeue_list = (os_queue_t *)
                    osMalloc((sizeof(os_queue_t) * g_total_squeues), OS_MEM_SHARED);
 
                if (g_squeue_list == NULL)
                {
                    RETURN_ERROR(OS_ERR_NO_MEMORY);
                }
 
                osSpinLockInitialize(&_shared_queue_guard);
            }
        } //master-core
    } //osL1dGetResetMode()
#endif /* OS_MULTICORE */
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osQueueCreate(os_queue_handle que_handle, uint16_t queue_size)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    os_mem_type mem_type = OS_MEM_LOCAL;
 
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(queue_size > 0);
 
#if (OS_MULTICORE == 1)
    if (queue->shared)
    {
        mem_type = OS_MEM_SHARED;
    }
#endif /* OS_MULTICORE */

    osSpinLockIrqGet(&(queue->guard));

#ifdef QUEUE_ERROR_CHECKING
    if (queue->status >= OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_ALREADY_CREATED);
    }
    else
#endif /* QUEUE_ERROR_CHECKING */
    {
        uint32_t reset_mode = osL1dGetResetMode();
 
    	if((!queue->shared) || (reset_mode == OS_L1D_NO_WARM_RESET) || (reset_mode == OS_L1D_MODE_3))
    	{
			//@@@@ if (queue->buff == NULL)
			queue->buff = (uint32_t *)
				osMalloc((sizeof(uint32_t) * queue_size), mem_type);
	
			if (queue->buff == NULL)
			{
#ifdef QUEUE_ERROR_ASSERT
				OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
				osSpinLockIrqRelease(&(queue->guard));
				RETURN_ERROR(OS_ERR_NO_MEMORY);
			}
	
			queue->used = 0;
			queue->low_indx = 0;
			queue->high_indx = 0;
			queue->size = queue_size;
			queue->status = OS_RESOURCE_USED;
    	}
    }
 
    osSpinLockIrqRelease(&(queue->guard));
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osQueueCreateMultiple(os_queue_handle que_handle, uint16_t queue_size, uint16_t num)
{
    os_status status;
    uint16_t size = (uint16_t)(queue_size * num);
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COMPILER_COND(0xffff >= size);
 
    status = osQueueCreate(que_handle, size);
    OS_ASSERT_COND(status == OS_SUCCESS);
    queue->size |= num << 16;
 
    return status;
}


/*****************************************************************************/
os_status osQueueFind(os_queue_handle *que_handle, bool shared)
{
    os_queue_t  *queue_list;
    uint16_t    i, total_queues;
 
    OS_ASSERT_COND(que_handle != NULL);
 
#if (OS_MULTICORE == 1)
    if (shared)
    {
        total_queues = g_total_squeues;
        queue_list = g_squeue_list;
        osSpinLockGet(&_shared_queue_guard);
    } else
#else
    VAR_UNUSED(shared);
#endif /* OS_MULTICORE */
    {
        total_queues = g_total_queues;
        queue_list = g_queue_list;
    }
    for (i=0; i < total_queues; i++)
    {
        osHwiSwiftDisable();
 
        if (queue_list[i].status < OS_RESOURCE_ACQUIRED)
        {
            queue_list[i].status = OS_RESOURCE_ACQUIRED;
            *que_handle = (os_queue_handle)&(queue_list[i]);
	
#if (OS_MULTICORE == 1)
            if (shared)
            {
                osSpinLockInitialize(&queue_list[i].guard);
                queue_list[i].shared = 1;
                osSpinLockRelease(&_shared_queue_guard);
            }
            else
#endif /* OS_MULTICORE */
            {
                queue_list[i].shared = 0;
                osSpinLockDisable(&queue_list[i].guard);
            }

            osHwiSwiftEnable();
            return OS_SUCCESS;
        }
 
        osHwiSwiftEnable();
    }
#if (OS_MULTICORE == 1)
	if (shared)
		osSpinLockRelease(&_shared_queue_guard);
#endif
    *que_handle = NULL;

    RETURN_ERROR(OS_ERR_Q_UNAVAILABLE);
}


