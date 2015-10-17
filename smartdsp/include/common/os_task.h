/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:26 $
 $Id: os_task.h,v 1.28 2012/11/22 16:28:26 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_task.h,v $
 $Revision: 1.28 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_task.h

 @Description   OS task's definitions and prototypes.

 @Cautions      None.
*//***************************************************************************/



#ifndef __OS_TASK_H
#define __OS_TASK_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         task_id Tasks API

 @Description   Multitasking setup and control.
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         task_runtime_id Tasks Runtime API.

 @{
*//***************************************************************************/


#define TASK_NAME_LEN   12             /**< Maximal task name */

#define TASK_IDLE_ID    0              /**< ID of background task */

#define TASK_NULL_ID    0xFFFFFFFF     /**< This ID never belongs to a valid task */


/*************************************************************//**
 @Collection    Available task priorities.
 
 @{
*//**************************************************************/
#define OS_TASK_PRIORITY_HIGHEST     0
#define OS_TASK_PRIORITY_01          1
#define OS_TASK_PRIORITY_02          2
#define OS_TASK_PRIORITY_03          3
#define OS_TASK_PRIORITY_04          4
#define OS_TASK_PRIORITY_05          5
#define OS_TASK_PRIORITY_06          6
#define OS_TASK_PRIORITY_07          7
#define OS_TASK_PRIORITY_08          8
#define OS_TASK_PRIORITY_09          9
#define OS_TASK_PRIORITY_10          10
#define OS_TASK_PRIORITY_11          11
#define OS_TASK_PRIORITY_12          12
#define OS_TASK_PRIORITY_13          13
#define OS_TASK_PRIORITY_14          14
#define OS_TASK_PRIORITY_15          15
#define OS_TASK_PRIORITY_16          16
#define OS_TASK_PRIORITY_17          17
#define OS_TASK_PRIORITY_18          18
#define OS_TASK_PRIORITY_19          19
#define OS_TASK_PRIORITY_20          20
#define OS_TASK_PRIORITY_21          21
#define OS_TASK_PRIORITY_22          22
#define OS_TASK_PRIORITY_23          23
#define OS_TASK_PRIORITY_24          24
#define OS_TASK_PRIORITY_25          25
#define OS_TASK_PRIORITY_26          26
#define OS_TASK_PRIORITY_27          27
#define OS_TASK_PRIORITY_28          28
#define OS_TASK_PRIORITY_29          29
#define OS_TASK_PRIORITY_30          30
#define OS_TASK_PRIORITY_LOWEST      31
/* @} */

#define OS_NUM_OF_PRIORITIES         32
/**<Number of Available task priorities*/


/*************************************************************//**
 @Collection    Task status codes.
 
 @{
*//**************************************************************/
#define OS_TASK_UNUSED              0x0000001   /**< Task is not allocated. */
#define OS_TASK_ACQUIRED            0x0000002   /**< Task was allocated but not created. */
#define OS_TASK_SUSPEND             0x0000004   /**< Task was created but not activated yet. It may also been activated and then suspended.*/
#define OS_TASK_PEND                0x0000008   /**< Task is waiting for an event. */
#define OS_TASK_TIMEOUT             0x0000010   /**< Task is waiting for an event with a timeout. At the beginning OS_TASK_PEND is also ON, but after timeout expires, it may stay alone. */
#define OS_TASK_DELAY               0x0000020   /**< Task is waiting for a timeout to expire. */
#define OS_TASK_READY               0x0000040   /**< A task is ready to run. This state may be exist with or without OS_TASK_RUNNING. */
#define OS_TASK_RUNNING             0x0000080   /**< This task is the highest priority ready task and it is the first in its priority, thus it is the currently running task. */
#define OS_TASK_EXPIRED             0x0000100   /**< The timeout on which this task was waiting has expired. */
/* @} */

/**************************************************************************//**
 @Description   Config parameters for a task.
 
*//***************************************************************************/
typedef struct os_task_init_param_t
{
    os_task_function task_function;
    /**< Task function - This function will run when the task will be activated.
    This function should never exit */
    os_task_arg      task_arg;
    /**< Task argument - Will be the parameter of task_function. */
    uint32_t         top_of_stack;
    /**< Top of task's stack. */
    uint32_t         stack_size;
    /**< The size of the above space. */
    uint32_t         task_flags;
    /**< Currently reserved. */
    os_task_priority task_priority;
    /**< Task priority - From OS_TASK_PRIORITY_HIGHEST to OS_TASK_PRIORITY_LOWEST*/
    char             *task_name;
    /**< Task name - Should be not longer then TASK_NAME_LEN */
    uint32_t         private_data;
    /**< This field can be read and written from any point during the life span of the task. */
    bool             init_stack;
    /**< Whether to initialize the stack. This initialization is used by kernel awareness to see usage of the stack. */
} os_task_init_param_t;


/**************************************************************************//**
 @Function      osTaskFind

 @Description   Get the index of an unused task.
 
 @Param[out]    task_handle - Gets the task handle.

 @Retval        OS_SUCCESS             - Found
 @Retval        OS_ERR_TSK_UNAVAILABLE - Not found
*//***************************************************************************/
os_status osTaskFind(os_task_handle *task_handle);


/**************************************************************************//**
 @Function      osTaskCreate

 @Description   Create a task.
 
                This function initializes the stack of a task, including
                the function to run.
                The task stack may also be filled with special value to monitor
                its usage. Note, that task is created in suspended state. User should
                call osTaskActivate().
 
 @Param[in]     task_init_param  - Task configuration parameters.
 @Param[in]     task_handle      - Task to be created.

 @Retval        OS_SUCCESS                  - Created
 @Retval        OS_ERR_TSK_ALREADY_CREATED  - Not created
*//***************************************************************************/
os_status osTaskCreate(os_task_handle task_handle, os_task_init_param_t *task_init_param);


/**************************************************************************//**
 @Function      osTaskActivate

 @Description   This function removes task from suspend state.
 
                If the task is not suspended, the function has no effect. If
                the task is not delayed or pended, it is moved to the ready
                state, and scheduler is called. If this is the highest priority
                ready task and it is the first in its priority queue, then it
                will resume or start.
 
 
 @Param[in]     task_handle - Handle of the task to be activated.

 @Retval        OS_SUCCESS                          - Activated
 @Retval        OS_ERR_TSK_OS_ERR_ALREADY_ACTIVE    - Not activated
*//***************************************************************************/
os_status osTaskActivate(os_task_handle task_handle);


/**************************************************************************//**
 @Function      osTaskSuspend

 @Description   Move task to suspend state.
 
                When a task is created it is placed in suspend state. Then it can
                be removed from that state by osTaskActivate() and returned to the
                suspend state by this function.
 
 @Param[in]     task_handle - Handle of task to suspend.

 @Retval        OS_SUCCESS                      - Suspended
 @Retval        OS_ERR_TSK_ALREADY_SUSPENDED    - Not suspended

 @Cautions      Do not call this function for background
*//***************************************************************************/
os_status osTaskSuspend(os_task_handle task_handle);


/**************************************************************************//**
 @Function      osTaskDelete

 @Description   Deletes a task.
 
                If a task is created, then it is removed from any event its pending
                on and its TCB is freed. If task deletes itself, then scheduler is called.
 
 @Param[in]     task_handle - Handle of task to delete.

 @Retval        OS_SUCCESS                  - Deleted
 @Retval        OS_ERR_TSK_NOT_CREATED      - Not deleted
 @Retval        OS_ERR_TSK_DELETE_LOCKED    - Can not delete a running task when sceduler is locked. Will cause crash.

@Cautions      Do not call this function for background
*//***************************************************************************/
os_status osTaskDelete(os_task_handle task_handle);


/**************************************************************************//**
 @Function      osTaskDelay

 @Description   Delay a running task.
 
                Blocks a task for the specified number of ticks. Background task
                can never be delayed and must be always ready to run.
                This functionality relies on Tick operation.

 @Param[in]     timeout -  number ticks to delay.

 @Retval        OS_SUCCESS                  - Delayed
 @Retval        OS_ERR_TSK_DELAY_IN_INTERR  - Can not delay a task from interrupt context.
 @Retval        OS_ERR_TSK_DELAY_IN_LOCK    - Can not delay a running task when sceduler is locked.
                                              This will prevent multiple delays on the same task.
 
@Cautions      Do not call this function for background
*//***************************************************************************/
os_status osTaskDelay(uint32_t timeout);


/**************************************************************************//**
 @Function      osTaskPrioritySet

 @Description   Set priority of a task.
 
                Task priority is dynamic. Users can change its priority
                at any point and this change is immediate.

 @Param[in]     task_handle     - Handle of task to set priority of.
 @Param[in]     task_priority   - Task's new priority.

 @Retval        OS_SUCCESS              - Priority was set
 @Retval        OS_ERR_TSK_NOT_CREATED  - Task was uncreated
*//***************************************************************************/
os_status osTaskPrioritySet(os_task_handle task_handle, os_task_priority task_priority);


/**************************************************************************//**
 @Function      osTaskSchedulerLock

 @Description   Lock scheduler so running task will not be replaced.
 
                This is useful for critical sections where the task wants to
                protect itself only from other tasks and not from HWI or SWI,
                So it does not prolong interrupt latency.
 
@Cautions       Call this function only from task
*//***************************************************************************/
void osTaskSchedulerLock();


/**************************************************************************//**
 @Function      osTaskSchedulerUnlock

 @Description   Unlock scheduler so running task can be replaced.
                This is useful for critical sections where the task wants to
                protect itself only from other tasks and not from HWI or SWI,
                So it does not prolong interrupt latency.
 
@Cautions       Call this function only from task
*//***************************************************************************/
void osTaskSchedulerUnlock();


/**************************************************************************//**
 @Function      osTaskSelf

 @Description   Gets the handle of the running task.

 @Param[out]    task_handle -  reference to a task handle.

 @Retval        OS_SUCCESS              - Handle is valid
 @Retval        OS_ERR_TSK_NOT_ACTIVE   - In SWI or HWI.

 @Cautions      Call this function only from task
*//***************************************************************************/
os_status osTaskSelf(os_task_handle *task_handle);


/**************************************************************************//**
 @Function      osTaskStatusGet

 @Description   Gets the status of a task.
                Returns the status of a task. A status can be any valid combination
                of status bits as explained in chapter 4 of the  documentation.
 
 @Param[in]     task_handle - Gets the task handle.

 @Return        Task's Status, i.e. a legal combination of:
        - OS_TASK_UNUSED
        - OS_TASK_ACQUIRED
        - OS_TASK_SUSPEND
        - OS_TASK_PEND
        - OS_TASK_TIMEOUT
        - OS_TASK_DELAY
        - OS_TASK_READY
        - OS_TASK_RUNNING
*//***************************************************************************/
uint32_t osTaskStatusGet(os_task_handle task_handle);


/**************************************************************************//**
 @Function      osTaskPrivateDataGet

 @Description   Gets private data of a task.
                Private data is a 4 bytes field associated with task that are maintained
                through the life span of a task. This field can be retrieved by any
                entity in the system.
 
 @Param[in]     task_handle - Handle of task to get private data of.

 @Return        Private data of a task.
*//***************************************************************************/
uint32_t osTaskPrivateDataGet(os_task_handle task_handle);


/**************************************************************************//**
 @Function      osTaskPrivateDataSet

 @Description   Sets private data of a task.
                Private data is a 4 bytes field associated with task that are maintained
                through the life span of a task. This field Can be set by any
                entity in the system to pass data into a task.

 
 @Param[in]    task_handle  -  Handle of task to set private data of.
 @Param[in]    private_data -  Private data.
*//***************************************************************************/
void osTaskPrivateDataSet(os_task_handle task_handle, uint32_t private_data);


/**************************************************************************//**
 @Function      osTaskPriorityGet

 @Description   Gets task's priority.
 
 @Param[in]     task_handle - Handle of task to get the priority of.

 @Return        Task's priority.
*//***************************************************************************/
os_task_priority  osTaskPriorityGet(os_task_handle task_handle);


/**************************************************************************//**
 @Function      osTaskPriorityReadyCount

 @Description   Counts ready tasks in specified priority.
 
                This function is useful to see if user may call osTaskYield for this priority.

 @Param[in]     priority -  Priority to count.

 @Return        Number of tasks in the priority.
*//***************************************************************************/
uint32_t  osTaskPriorityReadyCount(uint32_t priority);


/**************************************************************************//**
 @Function      osTaskYield

 @Description   Schedule a different task in same priority.
 
                If there are multiple tasks in the same priority then this function
                can be used. Either:to perform cooperative scheduling, when a task may
                step a side and let another task in it's own priority take the
                CPU, or, other thread of execution may determine what is
                the next task that will run at this priority.
                For instance by calling this function in tick software interrupt handler.
                The function can let the next task in the same priority run, or let the
                task specified by next_task run, if its in task_priority.
                The task that was resumed is returned in yielded_to.

 @Param[in]     task_priority   - priority to yield in.
 @Param[in]     next_task       - Move to if in same priority.
 @Param[out]    yielded_to      - The task that was promoted to be first in its priority.

 @Retval        OS_SUCCESS
 @Retval        OS_ERR_TSK_YIELD_TO_ILLEGAL_TASK    - If next_task is not in task_priority priority.
 @Retval        OS_ERR_TSK_YIELD_TO_SINGLE_TASK     - there is only one task on this priority.
 
@Cautions       Do not call this before osStart
*//***************************************************************************/
os_status      osTaskYield(os_task_priority task_priority,
                           os_task_handle next_task,
                           os_task_handle *yielded_to);


/**************************************************************************//**
 @Function      osTaskNameGet

 @Description   Gets name of a task.
                The name is what was registered in the task_name field of #os_task_init_param_t
                when osTaskCreate() was called.
 
 @Param[in]     task_handle - task

 @Return        The name of the task.
 
 @Cautions      Do not call this function in HWI/SWI
*//***************************************************************************/
char * osTaskNameGet(os_task_handle task_handle);

#ifdef MMU_SUPPORT
#if defined(SC3X50)

/**************************************************************************//**
 @Function      osTaskMmuDataContextSet

 @Description   Set a data context for a task
 
                This function sets context for a task.
                The context is transfered "by value";
                it means that if the context is changed after
                call to this function - task context will remain the same.
                (This behavior is different for SWI)
 
 @Param[in]     task_handle - task
 @Param[in]     context - context to enable

 @Retval        OS_SUCCESS
 @Retval        OS_ERR_TSK_MMU_SET_IN_INTERR - If called from HWI or SWI priority.
 
@Cautions       Do not call this function in HWI/SWI
*//***************************************************************************/
os_status osTaskMmuDataContextSet(os_task_handle task_handle, os_mmu_context_t* context);


/**************************************************************************//**
 @Function      osTaskMmuProgContextSet

 @Description   Set a program context for a task
 
                This function sets context for a task.
                The context is transfered "by value";
                it means that if the context is changed after
                call to this function - task context will remain the same.
                (This behavior is different for SWI)
 
 @Param         task_handle - task

 @Param         context - context to enable

 @Retval        OS_SUCCESS
 @Retval        OS_ERR_TSK_MMU_SET_IN_INTERR - If called from HWI or SWI priority.
 
@Cautions       Do not call this function in HWI/SWI
*//***************************************************************************/
os_status osTaskMmuProgContextSet(os_task_handle task_handle, os_mmu_context_t* context);

#endif
#endif MMU_SUPPORT

/** @} */ // task_runtime_id
/** @} */ // task_id

_OS_END_EXTERN_C

#endif // __OS_TASK_H
