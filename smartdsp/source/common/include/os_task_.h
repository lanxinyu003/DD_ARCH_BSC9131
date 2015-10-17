/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/04/09 15:02:07 $
 $Id: os_task_.h,v 1.12 2014/04/09 15:02:07 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_task_.h,v $
 $Revision: 1.12 $
******************************************************************************/

/******************************************************************************
 
 @File          os_task_.h

 @Description   OS internal tasks definitions and prototypes.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_TASK_H_
#define __OS_TASK_H_


/**************************************************************************//*
 @Description   Tasks Status.
 
*//***************************************************************************/
#define OS_TASK_BLOCK               (OS_TASK_DELAY | OS_TASK_PEND | OS_TASK_SUSPEND)

#define TASK_NAME

#define TASK_HANDLE_CHECK(TASK) \
{extern uint32_t g_total_tasks_number; \
    OS_ASSERT_COND((TASK) < g_total_tasks_number);}

#define TASK_PRIORITY_CHECK(P) OS_ASSERT_COND(!((P) > OS_TASK_PRIORITY_LOWEST))


#define TASK_NOT_IDLE_CHECK(TASK) OS_ASSERT_COND(TASK != TASK_IDLE_ID)

/**************************************************************************//**
 @Description   Task Control Structure.
 
                All information needed during a task life span.
 
*//***************************************************************************/
typedef struct
{
    uint32_t             *stack_pointer;  /**< Current SP in the task. */
    uint16_t             task_status;     /**< Task flags. */
    uint16_t             task_flags;      /**< Currently reserved. */
#if defined(ARCH_CONTEXT_SIZE) && (ARCH_CONTEXT_SIZE > 0)
    uint32_t             arch_spec[ARCH_CONTEXT_SIZE];  /**< Save task context to that is saved to exception stack. */
#endif
    uint32_t             stack_size;      /**< Total size of the stack. */
    uint32_t             top_of_stack;    /**< Top of task's stack. */
    uint32_t             task_function;   /**< Function ran in this task. */
    struct os_ecb_t      *task_event;     /**< Current event task is pending on. */
    uint32_t             task_message;    /**< Message received from an event. */
    char                 name[TASK_NAME_LEN];
                                          /**< Task name. */
    list_object_t        pend_list;       /**< List of ready or event queue. */
    list_object_t        timer_list;      /**< List of waiting for timer tasks. */
    os_task_priority     priority;        /**< Priority of task. */
    os_task_handle       task_handle;     /**< Task Handle is an index to global tasks array */
    uint32_t             task_id;         /**< Task ID. */
    uint32_t             private_data;    /**< Parameters in and out of the task. */
    unsigned long long   expiration_tick; /**< Value of tick to resume task. */
} os_tcb_t;


extern uint32_t stack_default[];

#define GET_TCB_PEND(ptr)  LIST_COMPONENT(ptr, os_tcb_t, pend_list)
#define GET_TCB_TIMER(ptr) LIST_COMPONENT(ptr, os_tcb_t, timer_list)

/**************************************************************************//**
 @Function      taskHighestSet

 @Description   Finds the highest priority ready task and puts it in
                g_highest_task.
 
*//***************************************************************************/
void taskHighestSet();


/**************************************************************************//**
 @Function      taskSchedule

 @Description   Uses taskHighestSet to update g_highest_task. Then if it is
                different then g_running_task, asserts OS_FLG_TSK_REQ, and
                if not in interrupt call trap to enter it.
 
*//***************************************************************************/
void taskSchedule();

/**************************************************************************//**
 @Function      taskReadyAdd

 @Description   Add ready_task to ready list and assert the priority
                corresponding bit if neccessary..
 
 @Param         task_handle - Handle to a task structure.

*//***************************************************************************/
void taskReadyAdd(os_tcb_t *ready_task);

/**************************************************************************//**
 @Function      taskReadyDel

 @Description   Delete ready_task from ready list and deassert the priority
                corresponding bit if neccessary..
 
 @Param         task_handle - Handle to a task structure.

*//***************************************************************************/
void taskReadyDel(os_tcb_t *ready_task);

/**************************************************************************//**
 @Function      taskTimerAdd

 @Description   Add task to sorted timer list according to timeout..
 
 @Param         task_handle - Handle to a task structure.
 @Param         timeout - How many ticks to saty in timer list.

*//***************************************************************************/
void taskTimerAdd(os_tcb_t *timer_task, uint32_t timeout);

/**************************************************************************//**
 @Function      osFirstTimeSwitch

 @Description   Start multitasking.

*//***************************************************************************/
void osFirstTimeSwitch();

/**************************************************************************//**
 @Function      coreTasksInitialize

 @Description   Initialize all tasks - core specific.

*//***************************************************************************/
os_status coreTasksInitialize();

/**************************************************************************//**
 @Function      taskContextInitialize

 @Description   Initialize the task's context - core specific.

*//***************************************************************************/
os_status coreTaskContextInitialize(os_tcb_t *task, os_task_init_param_t *task_init_param);


/**************************************************************************//**
 @Function      coreTaskStackVirtGet

 @Description   Returns the task's virtual stack pointer - core specific.

*//***************************************************************************/
uint32_t * coreTaskStackVirtInitialize(uint32_t *top_of_stack, int context_size);


/**************************************************************************//**
 @Function      osTaskTrap

 @Description   Perform a task switch; reverts to SWI triggering on older architectures.

*//***************************************************************************/
#ifndef SC39XX
#define osTaskTrap()    osSwiTrap()
#else
void    osTaskTrap(void);
#endif

#endif // __OS_TASK_H_
