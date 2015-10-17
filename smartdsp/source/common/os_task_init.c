/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_task_init.c,v 1.20 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_task_init.c,v $
 $Revision: 1.20 $
******************************************************************************/

/******************************************************************************
 
 @File          os_task_init.c

 @Description   OS Task module initializations.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os.h"
#include "smartdsp_init.h"
#include "os_init.h"
#include "os_list_.h"
#include "os_task_.h"
#include "os_task.h"
#include "os_event_.h"
#include "os_.h"
#include "os_mmu_.h"
#ifdef MMU_SUPPORT
extern os_status mmuTasksInitialize(uint32_t* default_stack);
extern uint32_t stack_default[];
#endif

/* Running and highest task in the system. */
extern volatile os_tcb_t    *g_running_task;
extern volatile os_tcb_t    *g_highest_task;

/* Unused TCBs and ECBs that can be allocated. */
extern list_object_t         g_unused_tasks_list;
extern list_object_t         g_unused_events_list;

/* Total TCBs and ECBs lists. */
extern uint32_t            g_total_events_number;
extern os_tcb_t            *g_all_tasks_array;
extern struct os_ecb_t     *g_all_events_array;

/* Tasks waiting for tick or scheduling. */
extern list_object_t         g_sorted_expiration_list;
extern list_object_t         g_ready_tasks[OS_NUM_OF_PRIORITIES];

extern uint32_t            g_ready_priorities_bit_map;

/* Num of tasks in the system. */
uint32_t                   g_total_tasks_number;


os_status  osTasksInitialize(int max_task)
{
    int counter;
    os_ecb_t *new_event;
    os_status status;
 
    /* Always reserve task idle for background. */
    g_total_tasks_number = (uint32_t)max_task;
 
    OS_ASSERT_COND(TASK_IDLE_ID == 0);
 
    INIT_LIST_OBJECT(&g_unused_tasks_list);
    INIT_LIST_OBJECT(&g_unused_events_list);
    INIT_LIST_OBJECT(&g_sorted_expiration_list);

    /* Perform core specific tasks initializations */
    status = coreTasksInitialize();
    OS_ASSERT_COND(status == OS_SUCCESS);

    /* Initialize a task with dummy lowest + 1priority to prevent if != NULL in taskHighestSet. */
    g_running_task = osMalloc(sizeof(os_tcb_t), OS_MEM_LOCAL);
 
    if(g_running_task == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }
 
    g_running_task->priority = 32;

    /* Allocate ECB structures. */
    if(g_total_events_number > 0)
    {
        g_all_events_array = osMalloc(g_total_events_number * sizeof(os_ecb_t), OS_MEM_LOCAL);

        if (g_all_events_array == NULL)
        {
            RETURN_ERROR(OS_ERR_NO_MEMORY);
        }
    }

    /* Connect all the ECBs in a doubly linked list. */
    for (counter = 0; counter < g_total_events_number; counter++)
    {
        new_event = ((os_ecb_t *)g_all_events_array) + counter;
        new_event->event_id = (uint32_t)counter;
        new_event->event_type = EVENT_TYPE_UNUSED;
        list_tail_add(&new_event->event_list, &g_unused_events_list);
    }

    /* Allocate TCB structures. */
    g_all_tasks_array =  osMalloc(g_total_tasks_number * sizeof(os_tcb_t), OS_MEM_LOCAL);

    if (g_all_tasks_array == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }

    /* Connect all the TCBs in a doubly linked list. */
    /* first one is reserved for background. */

    g_all_tasks_array [TASK_IDLE_ID].task_event      = NULL;
    g_all_tasks_array [TASK_IDLE_ID].task_message    = 0;
    g_all_tasks_array [TASK_IDLE_ID].task_status     = OS_TASK_ACQUIRED;
    g_all_tasks_array [TASK_IDLE_ID].expiration_tick = 0;
    g_all_tasks_array [TASK_IDLE_ID].task_id         = TASK_IDLE_ID;
    g_all_tasks_array [TASK_IDLE_ID].task_handle     = 0;
    g_all_tasks_array [TASK_IDLE_ID].private_data    = 0;
 
    /* TASK_IDLE_ID must be 0 in order to refrain from holes in the linked list */
    OS_ASSERT_COMPILER_COND(TASK_IDLE_ID == 0);

    OS_ASSERT_COMPILER_COND(g_total_tasks_number > 0);
    for (counter = (TASK_IDLE_ID + 1); counter < g_total_tasks_number ; counter++)
    {
        g_all_tasks_array [counter].task_event      = NULL;
        g_all_tasks_array [counter].task_message    = 0;
        g_all_tasks_array [counter].task_status     = OS_TASK_UNUSED;
        g_all_tasks_array [counter].expiration_tick = 0;
        g_all_tasks_array [counter].task_id         = (uint32_t)(counter+1) ;
        g_all_tasks_array [counter].task_handle     = (os_task_handle)counter;
        g_all_tasks_array [counter].private_data    = 0;
        list_tail_add(&(g_all_tasks_array + counter)->pend_list, &g_unused_tasks_list);
    }

    /* Init empty ready list for each priority. */
    for (counter = 0; counter < OS_NUM_OF_PRIORITIES; counter++)
    {
        INIT_LIST_OBJECT(&g_ready_tasks[counter]);
    }

    return OS_SUCCESS;
}

