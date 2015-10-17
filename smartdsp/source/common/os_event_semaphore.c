/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_event_semaphore.c,v 1.24 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_event_semaphore.c,v $
 $Revision: 1.24 $
******************************************************************************/

/******************************************************************************

 @File          os_event_semaphore.c

 @Description   OS Semaphore Evaent module runtime functions.

 @Cautions      None.

*//***************************************************************************/

#include "smartdsp_os.h"
#include "os_runtime.h"
#include "os_.h"
#include "os_list_.h"
#include "os_task_.h"
#include "os_event_.h"


extern int g_event_semaphore_available;
extern volatile os_tcb_t    *g_running_task;

/*****************************************************************************/
os_status osEventSemaphoreFind(os_event_handle *event_handle)
{
    os_status status;
    bool available_semaphore = FALSE;
    os_ecb_t *event_found;

    osHwiSwiftDisable();
    if (g_event_semaphore_available > 0)
    {
        g_event_semaphore_available--;
        available_semaphore = TRUE;
    }
    osHwiSwiftEnable();

    if (available_semaphore)
    {
        status = osEventFind(event_handle);
        if (status != OS_SUCCESS)
        {
            RETURN_ERROR(status);
        }

        event_found = ECB_GET(* event_handle);
        event_found->event_type |= EVENT_TYPE_SEMAPHORE;
        event_found->event_specific = NULL;
    }
    else
    {
        RETURN_ERROR(OS_ERR_EVENT_SEMA_UNAVAILABLE);
    }

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osEventSemaphoreCreate(os_event_handle event_handle, uint32_t count)
{
    os_ecb_t *event_created = ECB_GET(event_handle);

    CHECK_EVENT_ID(event_handle);

    osHwiSwiftDisable();

    if(!(event_created->event_type & EVENT_TYPE_UNUSED) ||
       !(event_created->event_type & EVENT_TYPE_SEMAPHORE))
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_EVENT_SEMA_ALREADY_CREATED);
    }

    event_created->event_type &= (~EVENT_TYPE_UNUSED);
    osHwiSwiftEnable();

    event_created->event_count = count;
    INIT_LIST_OBJECT(&event_created->event_list);

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osEventSemaphoreDelete(os_event_handle event_handle)
{
    os_ecb_t *event_deleted = ECB_GET(event_handle);
    bool scheduling_required = FALSE;

    CHECK_EVENT_ID(event_handle);

    if(!(event_deleted->event_type & EVENT_TYPE_SEMAPHORE))
    {
        RETURN_ERROR(OS_ERR_EVENT_SEMA_INVALID);
    }

    scheduling_required = osEventDelete(event_deleted);

    osHwiSwiftDisable();
    g_event_semaphore_available++;
    osHwiSwiftEnable();


    if (scheduling_required)
    {
        taskSchedule();
    }

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osEventSemaphoreReset(os_event_handle event_handle, uint32_t count)
{
    os_ecb_t *event_reset = ECB_GET(event_handle);
    bool scheduling_required = FALSE;

    CHECK_EVENT_ID(event_handle);

    if(!(event_reset->event_type & EVENT_TYPE_SEMAPHORE))
    {
        RETURN_ERROR(OS_ERR_EVENT_SEMA_INVALID);
    }

    osHwiSwiftDisable();
    scheduling_required = osEventReset(event_reset, 0);
    event_reset->event_count = count;
    osHwiSwiftEnable();


    if (scheduling_required) taskSchedule();

    return OS_SUCCESS;


}

/*****************************************************************************/
os_status osEventSemaphoreCountGet(os_event_handle event_handle, uint32_t* count)
{
    os_ecb_t *event = ECB_GET(event_handle);

    CHECK_EVENT_ID(event_handle);

    if(!(event->event_type & EVENT_TYPE_SEMAPHORE))
    {
        RETURN_ERROR(OS_ERR_EVENT_SEMA_INVALID);
    }

    osHwiSwiftDisable();
    *count = event->event_count;
    osHwiSwiftEnable();


    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osEventSemaphorePend(os_event_handle event_handle, uint32_t timeout)
{
    os_status status;
    os_ecb_t *event_pended = ECB_GET(event_handle);

    CHECK_EVENT_ID(event_handle);

    if (OS_HWI_ACTIVE)
    {
        RETURN_ERROR(OS_ERR_EVENT_SEMA_PEND_INTERR);
    }

    osHwiSwiftDisable();
#ifdef TASK_LOGGING
        osLog(LOG_TASK_PEND_SEM, (event_handle << 16) | g_running_task->task_handle);
#endif /* TASK_LOGGING */
    if (event_pended->event_count > 0)
    {
        event_pended->event_count--;
        osHwiSwiftEnable();

    }
    else
    {
        /* Pend on event */
        status = osEventPendUnsafe(event_pended, timeout);
        osHwiSwiftEnable();

        if (status != OS_SUCCESS)
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
            RETURN_ERROR(OS_ERR_EVENT_SEMA_TIMEOUT);
        }

    }

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osEventSemaphoreAccept(os_event_handle event_handle)
{
    os_ecb_t *event_accepted = ECB_GET(event_handle);

    CHECK_EVENT_ID(event_handle);

    osHwiSwiftDisable();
    if (event_accepted->event_count > 0)
    {
#ifdef TASK_LOGGING
        osLog(LOG_TASK_PEND_SEM, (event_handle << 16) | g_running_task->task_handle);
#endif /* TASK_LOGGING */
        event_accepted->event_count--;
        osHwiSwiftEnable();
        return OS_SUCCESS;
    }
    osHwiSwiftEnable();

    RETURN_ERROR(OS_ERR_EVENT_SEMA_UNAVAILABLE);
}


/*****************************************************************************/
os_status osEventSemaphorePost(os_event_handle event_handle, os_task_handle *resumed_task)
{
    os_ecb_t *event_posted = ECB_GET(event_handle);
    os_task_handle resumed;

    CHECK_EVENT_ID(event_handle);

    osHwiSwiftDisable();

    if (event_posted->event_count == 0xFFFFFFFF)
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_EVENT_SEMA_OVERFLOW);
    }
#ifdef TASK_LOGGING
    osLog(LOG_TASK_POST_SEM, (event_handle << 16) | g_running_task->task_handle);
#endif /* TASK_LOGGING */

    /* If waiting on this event then make it ready */
    if (!list_empty(&event_posted->event_list))
    {
        resumed = osEventReadyUnsafe(event_posted, 0);
        osHwiSwiftEnable();

        taskSchedule();
    }
    else
    {
        event_posted->event_count++;
        osHwiSwiftEnable();

        resumed = TASK_NULL_ID;
    }

    if(resumed_task != NULL)
    {
        *resumed_task = resumed;
    }

    return OS_SUCCESS;
}

