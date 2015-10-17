/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_queue.c,v 1.26 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_queue.c,v $
 $Revision: 1.26 $
******************************************************************************/

/******************************************************************************
 
 @File          os_queue.c

 @Description   OS Queue implementation

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_queue_.h"
#include "os_rm_.h"
#ifdef SC39XX
#include "sc39xx_cache.h"
#endif

 
/*****************************************************************************/
os_status osQueueDelete(os_queue_handle que_handle)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COND(queue != NULL);
 
    osSpinLockIrqGet(&(queue->guard));
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    osFree(queue->buff);
    queue->status = OS_RESOURCE_DELETED;
 
    osSpinLockIrqRelease(&(queue->guard));
 
    return OS_SUCCESS;
}
/*****************************************************************************/
os_status osQueueDeleteUnsafe(os_queue_handle que_handle)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COND(queue != NULL);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    osFree(queue->buff);
    queue->status = OS_RESOURCE_DELETED;

    return OS_SUCCESS;
}
/*****************************************************************************/
os_status osQueueResetUnsafe(os_queue_handle que_handle)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;

    OS_ASSERT_COND(queue != NULL);
 
 
    queue->low_indx = 0;
    queue->high_indx = 0;
    queue->used = 0;

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osQueueReset(os_queue_handle que_handle)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;

    OS_ASSERT_COND(queue != NULL);
 
    osSpinLockIrqGet(&(queue->guard));
 
    queue->low_indx = 0;
    queue->high_indx = 0;
    queue->used = 0;
 
    osSpinLockIrqRelease(&(queue->guard));
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osQueueEnqueue(os_queue_handle que_handle, uint32_t value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;

    OS_ASSERT_COND(queue != NULL);
 
    osSpinLockIrqGet(&(queue->guard));

#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == queue->size)
    {
        /* queue is full */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_FULL);
    }
 
    queue->buff[queue->high_indx++] = value;
    /* optimize: queue->high_indx %= queue->size; */
    if (queue->high_indx == queue->size)
    {
        queue->high_indx = 0;
    }
 
    queue->used++;

    osSpinLockIrqRelease(&(queue->guard));
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osQueueUnsafeEnqueue(os_queue_handle que_handle, uint32_t value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;

    OS_ASSERT_COND(queue != NULL);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == queue->size)
    {
        /* queue is full */
        RETURN_ERROR(OS_ERR_Q_FULL);
    }
 
    queue->buff[queue->high_indx++] = value;
    /* optimize: queue->high_indx %= queue->size; */
    if (queue->high_indx == queue->size)
    {
        queue->high_indx = 0;
    }
 
    queue->used++;
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osQueueHeadEnqueue(os_queue_handle que_handle, uint32_t value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;

    OS_ASSERT_COND(queue != NULL);
 
    osSpinLockIrqGet(&(queue->guard));

#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == queue->size)
    {
        /* queue is full */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_FULL);
    }
 
    /* get back one entry */
    if(queue->low_indx)
        queue->low_indx--;
    else
        queue->low_indx = (uint16_t)(queue->size-1);
 
    queue->buff[queue->low_indx] = value;
    queue->used++;

    osSpinLockIrqRelease(&(queue->guard));
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osQueueUnsafeHeadEnqueue(os_queue_handle que_handle, uint32_t value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;

    OS_ASSERT_COND(queue != NULL);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == queue->size)
    {
        /* queue is full */
        RETURN_ERROR(OS_ERR_Q_FULL);
    }
 
    /* get back one entry */
    if(queue->low_indx)
        queue->low_indx--;
    else
        queue->low_indx = (uint16_t)(queue->size-1);
 
    queue->buff[queue->low_indx] = value;
    queue->used++;
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osQueueDequeue(os_queue_handle que_handle, uint32_t *value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(value != NULL);
 
    osSpinLockIrqGet(&(queue->guard));
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == 0)
    {
        /* queue is empty */
        *value = 0;
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_EMPTY);
    }
 
    *value = queue->buff[queue->low_indx++];
    /* optimize: queue->low_indx %= queue->size; */
    if (queue->low_indx == queue->size)
    {
        queue->low_indx = 0;
    }
 
    queue->used--;

    osSpinLockIrqRelease(&(queue->guard));
    return OS_SUCCESS;
}
 

/*****************************************************************************/
os_status osQueueUnsafeDequeue(os_queue_handle que_handle, uint32_t *value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(value != NULL);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == 0)
    {
        /* queue is empty */
        *value = 0;
        RETURN_ERROR(OS_ERR_Q_EMPTY);
    }
 
    *value = queue->buff[queue->low_indx++];
    /* optimize: queue->low_indx %= queue->size; */
    if (queue->low_indx == queue->size)
    {
        queue->low_indx = 0;
    }
 
    queue->used--;

    return OS_SUCCESS;
}
 

/*****************************************************************************/
os_status osQueuePeek(os_queue_handle que_handle, uint32_t *value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(value != NULL);
 
    osSpinLockIrqGet(&(queue->guard));
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == 0)
    {
        /* queue is empty */
        *value = 0;
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_EMPTY);
    }
 
    *value = queue->buff[queue->low_indx];
 
    osSpinLockIrqRelease(&(queue->guard));
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osQueueTailDequeue(os_queue_handle que_handle, uint32_t *value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(value != NULL);
 
    osSpinLockIrqGet(&(queue->guard));
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == 0)
    {
        /* queue is empty */
        *value = 0;
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_EMPTY);
    }
 
    *value = queue->buff[queue->high_indx];
    if (queue->high_indx == 0)
        queue->high_indx = (uint16_t)(queue->size - 1);
    else
        queue->high_indx--;
 
    queue->used--;

    osSpinLockIrqRelease(&(queue->guard));
    return OS_SUCCESS;
}
 

/*****************************************************************************/
os_status osQueueUnsafeTailDequeue(os_queue_handle que_handle, uint32_t *value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
 
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(value != NULL);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == 0)
    {
        /* queue is empty */
        *value = 0;
        RETURN_ERROR(OS_ERR_Q_EMPTY);
    }
 
    *value = queue->buff[queue->high_indx];
    if (queue->high_indx == 0)
        queue->high_indx = (uint16_t)(queue->size - 1);
    else
        queue->high_indx--;
 
    queue->used--;

    return OS_SUCCESS;
}
 

/*****************************************************************************/
os_queue_status osQueueStatus(os_queue_handle que_handle)
{
    os_queue_t      *queue = (os_queue_t *)que_handle;
    os_queue_status queue_status;
 
    OS_ASSERT_COND(queue != NULL);
 
    osSpinLockIrqGet(&(queue->guard));
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        return OS_QUEUE_EMPTY;
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == 0)
        queue_status = OS_QUEUE_EMPTY;
    else if (queue->used == queue->size)
        queue_status = OS_QUEUE_FULL;
    else
        queue_status = OS_QUEUE_MIDDLE;
 
    osSpinLockIrqRelease(&(queue->guard));

    return queue_status;
}



/*****************************************************************************/
os_queue_status osQueueUnsafeStatus(os_queue_handle que_handle)
{
    os_queue_t      *queue = (os_queue_t *)que_handle;
    os_queue_status queue_status;
 
    OS_ASSERT_COND(queue != NULL);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        return OS_QUEUE_EMPTY;
    }
#endif /* QUEUE_ERROR_CHECKING */

    if (queue->used == 0)
        queue_status = OS_QUEUE_EMPTY;
    else if (queue->used == queue->size)
        queue_status = OS_QUEUE_FULL;
    else
        queue_status = OS_QUEUE_MIDDLE;
 
    return queue_status;
}


/*****************************************************************************/
os_status osQueueUnsafeEnqueueMultiple(os_queue_handle que_handle, uint32_t * restrict value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    uint32_t * restrict buff;
    uint16_t    num, size;
    uint16_t    high_indx,used;
    int i;
    OS_ASSERT_COND(queue != NULL);

    num =  (uint16_t)(queue->size >> 16);
    size = (uint16_t)(queue->size & 0xFFFF);
    buff = queue->buff;

    OS_ASSERT_COMPILER_COND(num > 0);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    /* get queue info into registers */
    used      = queue->used;
    high_indx = queue->high_indx;
    buff     += high_indx;
 
    /* queue is full */
    if (used == size) return OS_ERR_Q_FULL;

    /* push data into queue */
    for (i = 0; i < num; i++)
        buff[i] = value[i];
 
    /* optimize: queue->high_indx %= queue->size; */
    /* the size is devisable by num that is why it works  */
    used +=num;
    high_indx += num;
    if (high_indx == size) high_indx = 0;
 
    /* update index and size */
    queue->high_indx = high_indx;
    queue->used = used;
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osQueueEnqueueMultiple(os_queue_handle que_handle, uint32_t * restrict value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    uint32_t * restrict buff;
    uint16_t    num, size;
    uint16_t    high_indx,used;
    int i;
    OS_ASSERT_COND(queue != NULL);

    num =  (uint16_t)(queue->size >> 16);
    size = (uint16_t)(queue->size & 0xFFFF);
    buff = queue->buff;
 
    OS_ASSERT_COMPILER_COND(num > 0);

    osSpinLockIrqGet(&(queue->guard));

#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    /* get queue info into registers */
    used      = queue->used;
    high_indx = queue->high_indx;
    buff     += high_indx;
 
    /* queue is full */
    if (used == size)
    {
        /* queue is full */
        osSpinLockIrqRelease(&(queue->guard));
        return OS_ERR_Q_FULL;
    }

    /* push data into queue */
    for (i = 0; i < num; i++)
        buff[i] = value[i];
 
    /* optimize: queue->high_indx %= queue->size; */
    /* the size is devisable by num that is why it works  */
    used +=num;
    high_indx += num;
    if (high_indx == size) high_indx = 0;
 
    /* update index and size */
    queue->high_indx = high_indx;
    queue->used = used;
 
    osSpinLockIrqRelease(&(queue->guard));
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osQueueDequeueMultiple(os_queue_handle que_handle, uint32_t * restrict value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    uint32_t * restrict buff;
    uint16_t    num, size;
    uint16_t    low_indx,used;
    int i;
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(value != NULL);
#ifdef SC39XX
    osCacheLoadBarrier(NOT_CACHED);
#endif

    num =  (uint16_t)(queue->size >> 16);
    size = (uint16_t)(queue->size & 0xFFFF);
    buff = queue->buff;

    OS_ASSERT_COMPILER_COND(num > 0);
 
    osSpinLockIrqGet(&(queue->guard));
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        osSpinLockIrqRelease(&(queue->guard));
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    /* get queue info into registers */
    used      = queue->used;
    low_indx  = queue->low_indx;
    buff     += low_indx;
 
    if (used == 0)
    {
        /* queue is empty */
        *value = 0;
        osSpinLockIrqRelease(&(queue->guard));
        return OS_ERR_Q_EMPTY;
    }
 
    for(i = 0; i < num; i++)
        value[i] = buff[i];
 
    /* optimize: queue->low_indx %= queue->size; */
    used -= num;
    low_indx += num;
    if (low_indx == size) low_indx = 0;
 
    /* update index and size */
    queue->low_indx = low_indx;
    queue->used = used;

    osSpinLockIrqRelease(&(queue->guard));
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osQueueUnsafeDequeueMultiple(os_queue_handle que_handle, uint32_t * restrict value)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    uint32_t * restrict buff;
    uint16_t    num, size;
    uint16_t    low_indx,used;
    int i;
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(value != NULL);

    num =  (uint16_t)(queue->size >> 16);
    size = (uint16_t)(queue->size & 0xFFFF);
    buff = queue->buff;

    OS_ASSERT_COMPILER_COND(num > 0);
 
#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_Q_NOT_CREATED);
    }
#endif /* QUEUE_ERROR_CHECKING */

    /* get queue info into registers */
    used      = queue->used;
    low_indx  = queue->low_indx;
    buff     += low_indx;
 
    if (used == 0)
    {
        /* queue is empty */
        *value = 0;
        return OS_ERR_Q_EMPTY;
    }
 
    for(i = 0; i < num; i++)
        value[i] = buff[i];
 
    /* optimize: queue->low_indx %= queue->size; */
    used -= num;
    low_indx += num;
    if (low_indx == size) low_indx = 0;
 
    /* update index and size */
    queue->low_indx = low_indx;
    queue->used = used;

    return OS_SUCCESS;
}

/*****************************************************************************/
os_queue_shared   osQueueShared(os_queue_handle que_handle)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    OS_ASSERT_COND(queue != NULL);
 
    return (queue->shared == 0) ? OS_QUEUE_PRIVATE : OS_QUEUE_SHARED;
}


/*****************************************************************************/
os_status   osQueueSize(os_queue_handle que_handle, uint32_t* size)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(size != NULL);

#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        return OS_QUEUE_EMPTY;
    }
#endif /* QUEUE_ERROR_CHECKING */

    *size = queue->size;
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status   osQueueMultipleSize(os_queue_handle que_handle, uint32_t* size)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(size != NULL);

#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        return OS_QUEUE_EMPTY;
    }
#endif /* QUEUE_ERROR_CHECKING */
 
    *size = (queue->size & 0xFFFF);
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status   osQueueMultipleNum(os_queue_handle que_handle, uint32_t* num)
{
    os_queue_t  *queue = (os_queue_t *)que_handle;
    OS_ASSERT_COND(queue != NULL);
    OS_ASSERT_COND(num != NULL);

#ifdef QUEUE_ERROR_CHECKING
    if (queue->status < OS_RESOURCE_USED)
    {
#ifdef QUEUE_ERROR_ASSERT
        OS_ASSERT;
#endif /* QUEUE_ERROR_ASSERT */
        return OS_QUEUE_EMPTY;
    }
#endif /* QUEUE_ERROR_CHECKING */
 
    *num = (queue->size >> 16);
    return OS_SUCCESS;
}
