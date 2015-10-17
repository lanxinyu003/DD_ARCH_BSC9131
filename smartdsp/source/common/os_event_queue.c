/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_event_queue.c,v 1.25 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_event_queue.c,v $
 $Revision: 1.25 $
******************************************************************************/

/******************************************************************************

 @File          os_event_queue.c

 @Description   OS event queue module runtime functions.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os.h"
#include "os_runtime.h"
#include "os_.h"
#include "os_list_.h"
#include "os_task_.h"
#include "os_event_.h"
#include "os_event_queue.h"
#include "os_queue.h"

extern int g_event_queue_available;
extern volatile os_tcb_t    *g_running_task;


/*****************************************************************************/
os_status osEventQueueFind(os_event_handle *event_handle)
{
    os_status status;
    bool available_queue = FALSE;
    os_ecb_t *found_queue;

    osHwiSwiftDisable();
    if (g_event_queue_available > 0)
    {
        g_event_queue_available--;
        available_queue = TRUE;
    }
    osHwiSwiftEnable();

    if (available_queue)
    {
        status = osEventFind(event_handle);
        if(status == OS_SUCCESS)
        {
            found_queue = ECB_GET(* event_handle);
            found_queue->event_type = EVENT_TYPE_QUEUE;
            found_queue->event_count = 0;
            status = osQueueFind(&found_queue->event_specific, FALSE);
        }
        else
        {
            RETURN_ERROR(status);
        }
    }
    else
    {
        status = OS_ERR_EVENT_QUEUE_UNAVAILABLE;
    }

    return status;
}



/*****************************************************************************/
os_status osEventQueueCreate(os_event_handle event_handle, uint16_t size)
{
    os_status status;
    os_ecb_t *event_created = ECB_GET(event_handle);

    CHECK_EVENT_ID(event_handle);

    osHwiSwiftDisable();
    if(!(event_created->event_type & EVENT_TYPE_UNUSED) ||
       !(event_created->event_type & EVENT_TYPE_QUEUE))
    {
        status = OS_ERR_EVENT_QUEUE_ALREADY_CREATED;
    }
    else
    {
        event_created->event_type &= (~EVENT_TYPE_UNUSED);
    }
    osHwiSwiftEnable();

    status = osQueueCreate(event_created->event_specific, size);

    if(status == OS_SUCCESS)
    {
        INIT_LIST_OBJECT(&event_created->event_list);
    }
    else
    {
        RETURN_ERROR(status);
    }

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osEventQueueReset(os_event_handle event_handle,  uint32_t message)
{

    os_ecb_t *event_reset = ECB_GET(event_handle);
    bool scheduling_required = FALSE;
    os_status status;

    CHECK_EVENT_ID(event_handle);

    if(!(event_reset->event_type & EVENT_TYPE_QUEUE))
    {
        RETURN_ERROR(OS_ERR_EVENT_QUEUE_INVALID);
    }

    osHwiSwiftDisable();
    if (!list_empty(&event_reset->event_list))
    {
        scheduling_required = osEventReset(event_reset, message);
    }
    else
    {
        status = osQueueResetUnsafe(event_reset->event_specific);
        OS_ASSERT_COND(status == OS_SUCCESS);
    }

    osHwiSwiftEnable();


    if (scheduling_required) taskSchedule();

    return OS_SUCCESS;

}

/*****************************************************************************/
os_status osEventQueuePend(os_event_handle event_handle, uint32_t *message, uint32_t timeout)
{
    os_status status;
    os_ecb_t *event_pended = ECB_GET(event_handle);

    CHECK_EVENT_ID(event_handle);

    if (OS_HWI_ACTIVE)
    {
        RETURN_ERROR(OS_ERR_EVENT_QUEUE_PEND_INTERR);
    }

#ifdef TASK_LOGGING
    osLog(LOG_TASK_PEND_QUEUE, (event_handle << 16) | g_running_task->task_handle);
#endif /* TASK_LOGGING */

    osHwiSwiftDisable();
    status = osQueueUnsafeDequeue(event_pended->event_specific, message);

    /* Pend on event in case that it is empty */
    if(status == OS_ERR_Q_EMPTY)
    {
        status = osEventPendUnsafe(event_pended, timeout);
        osHwiSwiftEnable();

        if(status != OS_SUCCESS)
        {
            RETURN_ERROR(status);
        }

        taskSchedule();

        /* Clear timeout indication */
        g_running_task->task_status &= (~OS_TASK_TIMEOUT);
 
        /* Return from pending with timeout */
        if (g_running_task->task_status &  OS_TASK_EXPIRED)
        {
            osHwiSwiftDisable();
            g_running_task->task_status &= (~OS_TASK_EXPIRED);
            osHwiSwiftEnable();
            RETURN_ERROR(OS_ERR_EVENT_QUEUE_TIMEOUT);
        }

        *message = g_running_task->task_message;
        g_running_task->task_message = 0;

    }
    else
    {
        osHwiSwiftEnable();
    }

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osEventQueueAccept(os_event_handle event_handle, uint32_t *message)
{
    os_status status;
    os_ecb_t *event_accepted = ECB_GET(event_handle);

    CHECK_EVENT_ID(event_handle);

#ifdef TASK_LOGGING
    osLog(LOG_TASK_PEND_QUEUE, (event_handle << 16) | g_running_task->task_handle);
#endif /* TASK_LOGGING */
    status = osQueueDequeue(event_accepted->event_specific, message);


    if(status != OS_SUCCESS)
    {
        RETURN_ERROR(status);
    }

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osEventQueuePost(os_event_handle event_handle, uint32_t message, os_task_handle *resumed_task)
{
    os_status status;
    os_ecb_t *event_posted = ECB_GET(event_handle);
    os_task_handle resumed;


    CHECK_EVENT_ID(event_handle);

    osHwiSwiftDisable();

    if (osQueueUnsafeStatus(event_posted->event_specific) == OS_QUEUE_FULL)
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_EVENT_QUEUE_OVERFLOW);
    }
#ifdef TASK_LOGGING
    osLog(LOG_TASK_POST_QUEUE, (event_handle << 16) | g_running_task->task_handle);
#endif /* TASK_LOGGING */
    if (!list_empty(&event_posted->event_list))
    {
        /* If waiting on this event then make it ready */
        resumed = osEventReadyUnsafe(event_posted, message);
        osHwiSwiftEnable();

        taskSchedule();
    }
    else
    {
        status = osQueueUnsafeEnqueue(event_posted->event_specific, message);

        osHwiSwiftEnable();

        if(status != OS_SUCCESS)
        {
            RETURN_ERROR(status);
        }

        resumed = TASK_NULL_ID;

    }

    if(resumed_task != NULL)
    {
        *resumed_task = resumed;
    }

    return OS_SUCCESS;
}

