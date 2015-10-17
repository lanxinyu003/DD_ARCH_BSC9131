/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/11/06 12:34:08 $
 $Id: os_task.c,v 1.35 2013/11/06 12:34:08 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_task.c,v $
 $Revision: 1.35 $
******************************************************************************/

/******************************************************************************
 
 @File          os_task.c

 @Description   OS Task module runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "os_list_.h"
#include "os_task_.h"
#include "os_task.h"
#include "os_event_.h"
#include "os_core_utils_.h"
#include "os_swi_.h"
#include "os_.h"
#include "smartdsp_os_device.h"
#ifdef MMU_SUPPORT
#include "os_mmu.h"
#endif

/* Running and highest task in the system. */
volatile os_tcb_t   *g_running_task = NULL;
volatile os_tcb_t   *g_highest_task = NULL;

/* Unused TCBs and ECBs that can be allocated. */
LIST_OBJECT(g_unused_tasks_list);
LIST_OBJECT(g_unused_events_list);

/* Total TCBs and ECBs lists. */
extern uint32_t     g_total_events_number;
os_tcb_t            *g_all_tasks_array;
struct os_ecb_t     *g_all_events_array;

/* Tasks waiting for tick or scheduling. */
LIST_OBJECT(g_sorted_expiration_list);
list_object_t         g_ready_tasks[OS_NUM_OF_PRIORITIES];

uint32_t            g_ready_priorities_bit_map = 0;



/* Environment variables. */
extern volatile uint16_t   g_os_flags;
extern volatile uint16_t   g_os_task_lock;
extern volatile unsigned long long   g_os_ticks;
char file_buf[] = __FILE__;
int LINE = __LINE__;


uint32_t stack_default[] = STACK_DEFAULT;

#define LOWEST_BIT_INDEX 31

#ifndef B4860_FAMILY
/*****************************************************************************/
void taskReadyAdd(os_tcb_t *ready_task)
{
    uint32_t priority;
    list_object_t *ready_list;

    priority = ready_task->priority;
    /* find list of ready tasks */
    ready_list = &(g_ready_tasks[priority]);

    ready_task->task_status |= OS_TASK_READY;

    /* if first task that is added then update */
    /* toggle the corresponing bit in g_ready_priorities_bit_map*/
    if(list_empty(ready_list))
    {
        g_ready_priorities_bit_map |= (0x80000000>>priority);
    }
    list_tail_add(&ready_task->pend_list, ready_list);
}


/*****************************************************************************/
void taskReadyDel(os_tcb_t *ready_task)
{
    uint32_t priority;
    list_object_t *ready_list;

    priority = ready_task->priority;
    /* find list of ready tasks */
    ready_list = &(g_ready_tasks[priority]);

    list_delete(&ready_task->pend_list);
    ready_task->task_status &= (~OS_TASK_READY);
 
    /* if last task that is deleted from the list then*/
    /* toggle the corresponding bit in g_ready_priorities_bit_map*/

    if(list_empty(ready_list))
    {
        g_ready_priorities_bit_map &= (~(0x80000000 >> priority));
    }
 
}

#endif //B4860_FAMILY

/*****************************************************************************/
void taskTimerAdd(os_tcb_t *timer_task, uint32_t timeout)
{
    os_tcb_t *temp_timer_task;
    unsigned long long expiration_tick;
 
    osHwiSwiftDisable();

    expiration_tick = g_os_ticks + timeout;
    timer_task->expiration_tick = expiration_tick;
 
    if(list_empty(&g_sorted_expiration_list))
    {
        list_tail_add(&timer_task->timer_list, &g_sorted_expiration_list);
    }
    else
    {
        /* Put the task to right location */
        LIST_FOR_EACH(temp_timer_task, &g_sorted_expiration_list, os_tcb_t, timer_list)
        {
            if(temp_timer_task->expiration_tick > expiration_tick)
            {
                break;
            }
        }
 
        list_tail_add(&timer_task->timer_list, &temp_timer_task->timer_list);
    }

    osHwiSwiftEnable();
 
}


/*****************************************************************************/
INLINE void taskHighestSet()
{
    list_object_t *ready_tasks_list;
    uint32_t    priority_index;

    /* find the highest priority */
    priority_index = (uint32_t)(LOWEST_BIT_INDEX - get_num_of_first_one_bit(g_ready_priorities_bit_map));
    ready_tasks_list = &(g_ready_tasks[priority_index]);
    g_highest_task = GET_TCB_PEND(LIST_FIRST(ready_tasks_list));

}

/*****************************************************************************/
#ifndef B4860_FAMILY
void taskSchedule()
{
 
    osHwiSwiftDisable();
 
    /* Find the highest task */
    taskHighestSet();

    /* In case that running is not highest then reschedule */
    if ((g_running_task != g_highest_task) && (g_os_task_lock == 0))
    {
        g_os_flags |= OS_FLG_TSK_REQ;
 
        osHwiSwiftEnable();
        /* only if there is not HWI or SWI the trap */
        if (OS_HWI_INACTIVE)
        {
            osTaskTrap();
        }

    }
    else
    {
        osHwiSwiftEnable();
    }

}
#endif //B4860_FAMILY

/*****************************************************************************/
os_status osTaskFind(os_task_handle *task_handle)
{
    os_tcb_t *task_found;

    osHwiSwiftDisable();

    if (!list_empty(&g_unused_tasks_list))
    {
        task_found = GET_TCB_PEND(LIST_FIRST(&g_unused_tasks_list));
        list_delete(LIST_FIRST(&g_unused_tasks_list));
        *task_handle = task_found->task_handle;
        task_found->task_status = OS_TASK_ACQUIRED;
    }
    else
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TSK_UNAVAILABLE);
    }

    osHwiSwiftEnable();

    return OS_SUCCESS;;
}

/*****************************************************************************/
os_status osTaskCreate( os_task_handle task_handle, os_task_init_param_t *task_init_param)
{
    uint32_t  counter;
    uint32_t  *top_of_stack;
    os_tcb_t  *created_task;
    os_status status;

    OS_ASSERT_COND(task_init_param != NULL);
    OS_ASSERT_COND(g_all_tasks_array != NULL);
    OS_ASSERT_COND(task_init_param->top_of_stack != NULL);
 
    TASK_HANDLE_CHECK(task_handle);
    top_of_stack = (uint32_t *)task_init_param->top_of_stack;
    created_task = (g_all_tasks_array + task_handle);

    /* Verify unused. */
    if(created_task->task_status != OS_TASK_ACQUIRED)
    {
        RETURN_ERROR(OS_ERR_TSK_ALREADY_CREATED);
    }

    /* Verify stack 8 align. */
    OS_ASSERT_COMPILER_COND(!((uint32_t)top_of_stack & STACK_MINIMAL_ALIGNMENT));
 
    created_task->top_of_stack = task_init_param->top_of_stack;

    /* check priority */
    OS_ASSERT_COMPILER_COND(task_init_param->task_priority <= OS_TASK_PRIORITY_LOWEST);
 
    /* Initialize all context in the stack */
 
    osHwiSwiftDisable();
 


    /* Initialize tasks' stack with default values */
    OS_ASSERT_COMPILER_COND(task_init_param->stack_size >= CONTEXT_SIZE);
    for(counter=0; counter < CONTEXT_SIZE; counter++)
    {
        top_of_stack[counter] = stack_default[counter];
    }

    /* Initialize tasks' stack with core specific values */
    status = coreTaskContextInitialize(created_task, task_init_param);
    OS_ASSERT_COND(status == OS_SUCCESS);

    /* Initialize stack with some known values */
    OS_ASSERT_COMPILER_COND((task_init_param->stack_size/(sizeof(uint32_t))) > 0);
    if(task_init_param->init_stack == TRUE)
    {
        for(; counter < (task_init_param->stack_size/(sizeof(uint32_t))); counter++)
        {
            top_of_stack[counter] = EMPTY_STACK;
        }
    }



    /* Initialize task structure. */
    created_task->stack_pointer = coreTaskStackVirtInitialize(top_of_stack, (CONTEXT_SIZE));
    created_task->stack_size = task_init_param->stack_size;
    created_task->task_event = NULL;
    created_task->task_message = 0;
    created_task->task_status = OS_TASK_SUSPEND;
    created_task->expiration_tick = 0;
    created_task->priority = task_init_param->task_priority;
    created_task->task_function = (uint32_t)task_init_param->task_function;
    created_task->private_data = task_init_param->private_data;

#ifdef TASK_LOGGING
    osLog(LOG_TASK_CREATE, (task_init_param->task_priority << 16) | task_handle);
#endif /* SWI_LOGGING */

    for(counter = 0; counter < TASK_NAME_LEN; counter++)
    {
        created_task->name[counter] = task_init_param->task_name[counter];
    }

#ifdef ENABLE_DEBUG_HOOKS
    if (debug_hook_functions[OS_DEBUG_TASK_CREATE])
        debug_hook_functions[OS_DEBUG_TASK_CREATE]((debug_hook_arg)task_handle, (debug_hook_arg)task_init_param);
#endif

    osHwiSwiftEnable();

    return OS_SUCCESS;
}


/*****************************************************************************/
#ifndef B4860_FAMILY
os_status osTaskActivate(os_task_handle task_handle)
{
    os_tcb_t *activated_task = (g_all_tasks_array + task_handle);
 
    TASK_HANDLE_CHECK(task_handle);

    osHwiSwiftDisable();
 
    /* If task is not suspended then return */
    if (!(activated_task->task_status & OS_TASK_SUSPEND))
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TSK_OS_ERR_ALREADY_ACTIVE);
    }
 
#ifdef TASK_LOGGING
    osLog(LOG_TASK_ACTIVATE, task_handle);
#endif /* TASK_LOGGING */
 
    activated_task->task_status &= (~OS_TASK_SUSPEND);
 
    /* If task is not blocked then move it to ready list */
    if (!(activated_task->task_status & OS_TASK_BLOCK))
    {
        taskReadyAdd(activated_task);
 
        osHwiSwiftEnable();


        if(g_os_flags & OS_FLG_BGD_ACTIVE)
        {
            taskSchedule();
        }
        else
        {
            taskHighestSet();
        }
    }
    else
    {
        osHwiSwiftEnable();
    }
 
    return OS_SUCCESS;
}



/*****************************************************************************/
os_status osTaskSuspend(os_task_handle task_handle)
{
    os_tcb_t *suspend_task = (g_all_tasks_array + task_handle);

    TASK_HANDLE_CHECK(task_handle);

    TASK_NOT_IDLE_CHECK(task_handle);
 
    osHwiSwiftDisable();

    /* If task is suspended then return */
    if (suspend_task->task_status & OS_TASK_SUSPEND)
    {
        osHwiSwiftEnable();
 
        RETURN_ERROR(OS_ERR_TSK_ALREADY_SUSPENDED);
    }

    /* If task is ready then remove from ready list */
    if (suspend_task->task_status & OS_TASK_READY)
    {
        taskReadyDel(suspend_task);
    }
#ifdef TASK_LOGGING
    osLog(LOG_TASK_SUSPEND, task_handle);
#endif /* TASK_LOGGING */

    suspend_task->task_status |= OS_TASK_SUSPEND;

    osHwiSwiftEnable();

    /* If task is running then reschedule */
    if(suspend_task->task_status & OS_TASK_RUNNING)
    {
        taskSchedule();
    }
 
    return OS_SUCCESS;
}

#endif // B4860_FAMILY


/*****************************************************************************/
os_status osTaskDelete(os_task_handle task_handle)
{
    os_tcb_t *deleted_task = (g_all_tasks_array + task_handle);

    TASK_HANDLE_CHECK(task_handle);

    TASK_NOT_IDLE_CHECK(task_handle);

    osHwiSwiftDisable();
 
    if (deleted_task->task_status & OS_TASK_UNUSED)
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TSK_NOT_CREATED);
    }

    /* If the task is running and scheduler is locked */
    /* then can not delete it                         */
    if ((deleted_task->task_status & OS_TASK_RUNNING) &&
        (g_os_task_lock != 0))
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TSK_DELETE_LOCKED);
    }
#ifdef TASK_LOGGING
    osLog(LOG_TASK_DELETE, task_handle);
#endif /* TASK_LOGGING */
 
 
    if (deleted_task->task_status & OS_TASK_PEND)
    {
        list_delete(&deleted_task->pend_list);
    }
 
    if (deleted_task->task_status & (OS_TASK_DELAY | OS_TASK_TIMEOUT))
    {
        list_delete(&deleted_task->timer_list);
    }
 
    if (deleted_task->task_status & OS_TASK_READY)
    {
        taskReadyDel(deleted_task);
    }

    deleted_task->task_status &= (~(OS_TASK_SUSPEND));
 
    deleted_task->task_status |= OS_TASK_UNUSED;

    list_add(&deleted_task->pend_list, &g_unused_tasks_list);

    if(deleted_task->task_status & OS_TASK_RUNNING)
    {
        taskHighestSet();
        g_os_flags |= OS_FLG_TSK_REQ;

        if (OS_HWI_INACTIVE)
        {
            osHwiSwiftNestReduce();
            osTaskTrap();
        }
    }
 
    /* restore original task id */
    deleted_task->task_id = deleted_task->task_handle+1;
 
    osHwiSwiftEnable();

    return OS_SUCCESS; /* if deleting current task this is unreachable. */
}


/*****************************************************************************/
os_status osTaskDelay(uint32_t timeout)
{
    if (OS_HWI_ACTIVE)
    {
        RETURN_ERROR(OS_ERR_TSK_DELAY_IN_INTERR);
    }

    if (g_os_task_lock != 0)
    {
        RETURN_ERROR(OS_ERR_TSK_DELAY_IN_LOCK);
    }

    TASK_NOT_IDLE_CHECK(g_running_task->task_handle);


    if (timeout > 0)
    {
        osHwiSwiftDisable();
#ifdef TASK_LOGGING
        osLog(LOG_TASK_DELAY, (timeout << 16) | g_running_task->task_handle);
#endif /* TASK_LOGGING */
        /* remove from ready list */
        taskReadyDel((os_tcb_t *)g_running_task);
        taskTimerAdd((os_tcb_t *)g_running_task, timeout);
        g_running_task->task_status |= OS_TASK_DELAY;
 
        osHwiSwiftEnable();
 
        taskSchedule();
 
        g_running_task->task_status &= ~(OS_TASK_EXPIRED);
    }

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osTaskPrioritySet(os_task_handle task_handle, os_task_priority task_priority)
{
    os_tcb_t    *set_task = (g_all_tasks_array + task_handle);
    bool        is_ready;
 
    TASK_HANDLE_CHECK(task_handle);

    is_ready = (bool)(set_task->task_status & OS_TASK_READY);
 
    /* check priority */
    OS_ASSERT_COMPILER_COND(task_priority <= OS_TASK_PRIORITY_LOWEST);

    osHwiSwiftDisable();

    if (set_task->task_status & OS_TASK_UNUSED)
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TSK_NOT_CREATED);
    }
#ifdef TASK_LOGGING
    osLog(LOG_TASK_SET_PRIORITY, (task_handle << 16) | task_priority );
#endif /* TASK_LOGGING */

    /* delete the task & insert with right priority into ready queue */
    if(is_ready)
    {
        taskReadyDel(set_task);
        set_task->priority = task_priority;
        taskReadyAdd(set_task);
    }
    else
    {
        set_task->priority = task_priority;
    }
 
    osHwiSwiftEnable();


    /* reschedule if ready changed */
    if(is_ready) taskSchedule();
 
    return OS_SUCCESS;
}
 

/*****************************************************************************/
void osTaskSchedulerLock()
{
    osHwiSwiftDisable();
    if(g_os_task_lock == 0)
    {
        g_os_flags &= (~OS_FLG_TSK_REQ);
    }
    g_os_task_lock++;
 
    osHwiSwiftEnable();
}


/*****************************************************************************/
void osTaskSchedulerUnlock()
{
    if(g_os_task_lock > 0)
    {
        osHwiSwiftDisable();
        g_os_task_lock--;

        if(g_os_task_lock == 0)
        {
            osHwiSwiftEnable();
            taskSchedule();
        }
        else
        {
            osHwiSwiftEnable();
        }
    }
 
}


/*****************************************************************************/
os_status osTaskSelf(os_task_handle *task_handle)
{
    if (OS_HWI_ACTIVE)
    {
        RETURN_ERROR(OS_ERR_TSK_NOT_ACTIVE);
    }

    osHwiSwiftDisable();
    *task_handle = g_running_task->task_handle;
    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
uint32_t osTaskStatusGet(os_task_handle task_handle)
{
    os_tcb_t    *id_task = (g_all_tasks_array + task_handle);

    TASK_HANDLE_CHECK(task_handle);

    return (id_task->task_status);
}


/*****************************************************************************/
uint32_t osTaskPrivateDataGet(os_task_handle task_handle)
{
    os_tcb_t    *priority_task = (g_all_tasks_array + task_handle);

    TASK_HANDLE_CHECK(task_handle);

    return (priority_task->private_data);
}


/*****************************************************************************/
void osTaskPrivateDataSet(os_task_handle task_handle, uint32_t private_data)
{
    os_tcb_t    *private_data_task = (g_all_tasks_array + task_handle);

    TASK_HANDLE_CHECK(task_handle);

    private_data_task->private_data = private_data;
}


/*****************************************************************************/
os_task_priority  osTaskPriorityGet(os_task_handle task_handle)
{
    os_tcb_t    *priority_task = (g_all_tasks_array + task_handle);

    TASK_HANDLE_CHECK(task_handle);

    return (priority_task->priority);
}


/*****************************************************************************/
uint32_t  osTaskPriorityReadyCount(uint32_t priority)
{
    os_tcb_t *temp_task;
    uint32_t count_tasks = 0;

    osHwiSwiftDisable();

    LIST_FOR_EACH(temp_task, &g_ready_tasks[priority], os_tcb_t, pend_list)
    {
        count_tasks++;
        OS_ASSERT_COND(count_tasks < 30);
    }

    osHwiSwiftEnable();

    return count_tasks;
}


/*****************************************************************************/

os_status      osTaskYield(os_task_priority task_priority,
                           os_task_handle next_task,
                           os_task_handle *yielded_to)
{
    os_tcb_t *current_task, *next_task_handle;
 
    TASK_PRIORITY_CHECK(task_priority);
 
    osHwiSwiftDisable();
 
    if((osTaskPriorityReadyCount(task_priority)) > 1)
    {
        /* In case there are more then one ready tasks at */
        /* this priority, remove first task and add it */
        /* to the end of the queue */
        current_task = GET_TCB_PEND(LIST_FIRST(&g_ready_tasks[task_priority]));
 
        /* there is no task that user particularly wishes */
        /* therefore second task will become ready        */
        if (next_task == TASK_NULL_ID)
        {
            if(yielded_to != NULL)
            {
                *yielded_to = (GET_TCB_PEND(LIST_FIRST(&current_task->pend_list)))->task_handle;
            }
#ifdef TASK_LOGGING
        osLog(LOG_TASK_YIELD, (GET_TCB_PEND(LIST_FIRST(&current_task->pend_list)))->task_handle);
#endif /* TASK_LOGGING */

            list_delete(&current_task->pend_list);
            list_tail_add(&current_task->pend_list,
                          &g_ready_tasks[task_priority]);
        }
        else
        {
            TASK_HANDLE_CHECK(next_task);
 
            if(yielded_to != NULL)
            {
                *yielded_to = next_task;
            }
 
            next_task_handle = (g_all_tasks_array + next_task);
 
            if((next_task_handle->priority == task_priority) &&
               (next_task_handle->task_status & OS_TASK_READY))
            {
#ifdef TASK_LOGGING
                osLog(LOG_TASK_YIELD, next_task);
#endif /* TASK_LOGGING */

                list_delete(&next_task_handle->pend_list);
                list_add(&next_task_handle->pend_list,
                         &g_ready_tasks[task_priority]);
            }
            else /* task is illegal */
            {
                osHwiSwiftEnable();
                RETURN_ERROR(OS_ERR_TSK_YIELD_TO_ILLEGAL_TASK);
            }
        }
    }
    else /* There is only one task */
    {
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TSK_YIELD_TO_SINGLE_TASK);
    }
 
    osHwiSwiftEnable();

    taskSchedule();

    return OS_SUCCESS;
}

/*****************************************************************************/
char * osTaskNameGet(os_task_handle task_handle)
{
    os_tcb_t *task = (g_all_tasks_array + task_handle);
 
    TASK_HANDLE_CHECK(task_handle);
 
    return &(task->name[0]);
}
