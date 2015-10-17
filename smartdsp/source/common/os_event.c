/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_event.c,v 1.21 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_event.c,v $
 $Revision: 1.21 $
******************************************************************************/

/******************************************************************************
 
 @File          os_event.c

 @Description   OS Event module runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os.h"
#include "os_runtime.h"

#include "os_list_.h"
#include "os_task_.h"
#include "os_event_.h"
#include "os_core_utils_.h"


/* Unused ECBs that can be allocated. */
extern list_object_t         g_unused_events_list;

extern int                 g_max_event_queue;
extern int                 g_max_event_semaphore;
extern volatile uint16_t   g_os_task_lock;

extern volatile os_tcb_t   *g_running_task;
extern os_tcb_t            *g_all_tasks_array;

/*****************************************************************************/
os_status osEventFind(os_event_handle *event_handle)
{
    os_status status = OS_SUCCESS;
    os_ecb_t *found_event;
    list_object_t *unused_event;

    osHwiSwiftDisable();

    if (!list_empty(&g_unused_events_list))
    {
        unused_event = LIST_FIRST(&(g_unused_events_list));
        list_delete(unused_event);
        found_event = (GET_ECB_LIST(unused_event));
        *event_handle = (os_event_handle) found_event->event_id;
    }
    else
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_EVENT_UNAVAILABLE);
    }

    osHwiSwiftEnable();

    return status;

}


/*****************************************************************************/
bool osEventReset(os_ecb_t *reset_event, uint32_t message)
{
    os_tcb_t *temp_task;
    bool scheduling_required = FALSE;
 
    osHwiSwiftDisable();

    /* Delete all the tasks on this event */
    while((!list_empty(&reset_event->event_list)))
    {
        temp_task = g_all_tasks_array + osEventReadyUnsafe(reset_event, message);
        scheduling_required |= !(temp_task->task_status & OS_TASK_BLOCK);
    }

    osHwiSwiftEnable();
 
    return scheduling_required;

}

/*****************************************************************************/

bool osEventDelete(os_ecb_t *deleted_event)
{
    os_tcb_t *temp_task;
    bool scheduling_required = FALSE;
 
    osHwiSwiftDisable();
 
    /* Delete all the tasks on this event */
    while((!list_empty(&deleted_event->event_list)))
    {
        temp_task = g_all_tasks_array + osEventReadyUnsafe(deleted_event, 0);
        scheduling_required |= !(temp_task->task_status & OS_TASK_BLOCK);
    }
 
    deleted_event->event_type = EVENT_TYPE_UNUSED;

    list_add(&deleted_event->event_list, &g_unused_events_list);
 
    osHwiSwiftEnable();
 
    return scheduling_required;
}


/*****************************************************************************/

os_status osEventPendUnsafe(os_ecb_t *event_handle, uint32_t timeout)
{
    TASK_NOT_IDLE_CHECK(g_running_task->task_handle);

    if(g_os_task_lock != 0)
    {
        RETURN_ERROR(OS_ERR_EVENT_PEND_IN_LOCK);
    }


    g_running_task->task_event = (struct os_ecb_t *)event_handle;
    g_running_task->task_status |= OS_TASK_PEND;
 
    /* Delete the task from the ready list */
    /* and add it to event list            */
    taskReadyDel((os_tcb_t *)g_running_task);
    list_add((list_object_t *)&g_running_task->pend_list, &event_handle->event_list);


    if (timeout > 0)
    {
        /* if timeout defined then add it to timer list also */
        g_running_task->task_status |= OS_TASK_TIMEOUT;
        taskTimerAdd((os_tcb_t *)g_running_task, timeout);
    }

    return OS_SUCCESS;
}




/*****************************************************************************/
os_task_handle osEventReadyUnsafe(os_ecb_t *event, uint32_t message)
{
    os_tcb_t *resumed_task;
    resumed_task = GET_TCB_PEND(LIST_FIRST(&(event->event_list)));
 
    list_delete(LIST_FIRST(&(event->event_list)));

    OS_ASSERT_COND(resumed_task != NULL);
    OS_ASSERT_COND(resumed_task->task_status & OS_TASK_PEND);

    /* remove pend bit */
    resumed_task->task_status &= (~OS_TASK_PEND);

    /* if task is with timeout, remove it from timers list */
    if(resumed_task->task_status & OS_TASK_TIMEOUT)
    {
        list_delete(&resumed_task->timer_list);
        resumed_task->task_status &= (~OS_TASK_TIMEOUT);
        resumed_task->expiration_tick = 0;
    }

    resumed_task->task_event = NULL;
    resumed_task->task_message = message;

    /* If not suspended then reschedule */
    if(!(resumed_task->task_status & OS_TASK_SUSPEND))
    {
        taskReadyAdd(resumed_task);
    }

    return resumed_task->task_handle;
}

