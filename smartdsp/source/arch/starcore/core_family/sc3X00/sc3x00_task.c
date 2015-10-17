/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_task.c,v 1.4 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_task.c,v $
 $Revision: 1.4 $
******************************************************************************/

/******************************************************************************
 
 @File          sc3x00_task.c

 @Description   OS Task module runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "os_list_.h"
#include "os_task_.h"
#include "os_task.h"
#include "os_.h"
#include "os_mmu_.h"

extern os_tcb_t            *g_all_tasks_array;
extern uint32_t              g_total_tasks_number;


/*****************************************************************************/
os_status coreTasksInitialize()
{
#ifdef MMU_SUPPORT
    os_mmu_context_t* context;
    os_status status;
    int data_context_num;
    int program_context_num;
    int max_data_id;
    int max_program_id;

    status = osMmuDataContextFindExisting(&context, OS_MMU_SYSTEM_CONTEXT);
    OS_ASSERT_COND(status == OS_SUCCESS);
    osMmuDataTasksContextSet(stack_default,context);
 
    status = osMmuProgContextFindExisting(&context, OS_MMU_SYSTEM_CONTEXT);
    OS_ASSERT_COND(status == OS_SUCCESS);
    osMmuProgTasksContextSet(stack_default,context);
 
    data_context_num = osMmuNumOfDataContextsGet();
    program_context_num = osMmuNumOfProgContextsGet();
 
    /* program and data contexts consume task ids */
    /* make sure total task ids don't exceed the MMU limit */
    max_program_id = osMmuProgMaxIdGet();
    max_data_id    = osMmuDataMaxIdGet();
 
    OS_ASSERT_COND(data_context_num+g_total_tasks_number <= max_data_id);
    OS_ASSERT_COND(program_context_num+g_total_tasks_number <= max_program_id);
 
#endif
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status coreTaskContextInitialize(os_tcb_t *created_task, os_task_init_param_t *task_init_param)
{
    int counter;
    uint32_t *top_of_stack;

    OS_ASSERT_COND(task_init_param != NULL);
    OS_ASSERT_COND(created_task != NULL);
    OS_ASSERT_COND(task_init_param->top_of_stack != NULL);

    top_of_stack = (uint32_t *)task_init_param->top_of_stack;

    /* Initialize architecture specific data */
    for(counter = 0; counter < ARCH_CONTEXT_SIZE; counter++)
    {
        created_task->arch_spec[counter] = 0;
    }
 
    created_task->arch_spec[TASK_STATUS_REG_PLACE] = TASK_STATUS_REG;
    created_task->arch_spec[TASK_PC_REG_PLACE] = (uint32_t)task_init_param->task_function;
    created_task->arch_spec[TASK_ARG_REG_PLACE] = (uint32_t)task_init_param->task_arg;

#ifdef MMU_SUPPORT
    {
        os_mmu_context_handle sys_context;
        os_status status;
 
        status = osMmuDataContextFindExisting(&sys_context, OS_MMU_SYSTEM_CONTEXT);
        OS_ASSERT_COND(status == OS_SUCCESS);
 
        top_of_stack[REG_OFF_MMU_DATA_ID] = created_task->task_id;
        top_of_stack[REG_OFF_MMU_DATA_SEG] = sys_context->segments;


        status = osMmuProgContextFindExisting(&sys_context, OS_MMU_SYSTEM_CONTEXT);
        OS_ASSERT_COND(status == OS_SUCCESS);
 
        top_of_stack[REG_OFF_MMU_PROG_ID] = created_task->task_id;
        top_of_stack[REG_OFF_MMU_PROG_SEG] = sys_context->segments;
    }
#endif

    return OS_SUCCESS;
}


/*****************************************************************************/
uint32_t * coreTaskStackVirtInitialize(uint32_t *top_of_stack, int context_size)
{
    return top_of_stack + context_size;
}

#ifdef MMU_SUPPORT

/*****************************************************************************/
os_status osTaskMmuProgContextSet(os_task_handle task_handle, os_mmu_context_t* context)
{
    os_tcb_t *task = (g_all_tasks_array + task_handle);
    /* task is currently running    */
    /* so, change directly in MMU   */
    /* This change will be active immediately and it will */
    /* be persistent because MMU is a part of a context   */
    if (OS_HWI_ACTIVE)
        RETURN_ERROR(OS_ERR_TSK_MMU_SET_IN_INTERR);
    if((task->task_status & OS_TASK_RUNNING) && !OS_HWI_ACTIVE)
    {
        osMmuProgContextActivate(context, NULL);
    }
    /* task is not running currently        */
    /* so, change in the stack              */
    else
    {
        osMmuProgTasksContextSet(task->stack_pointer - CONTEXT_SIZE,context);
    }

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osTaskMmuDataContextSet(os_task_handle task_handle, os_mmu_context_t* context)
{
    os_tcb_t *task = (g_all_tasks_array + task_handle);
    /* task is currently running    */
    /* so, change directly in MMU   */
    /* This change will be active immidiatley and it will */
    /* be persistent because MMU is a part of a context   */
    if (OS_HWI_ACTIVE)
        RETURN_ERROR(OS_ERR_TSK_MMU_SET_IN_INTERR);
    if((task->task_status & OS_TASK_RUNNING) && !OS_HWI_ACTIVE)
    {
        osMmuDataContextActivate(context, NULL);
    }
    /* task is not running currently        */
    /* so, change in the stack              */
    else
    {
        osMmuDataTasksContextSet(task->stack_pointer - CONTEXT_SIZE,context);
    }

    return OS_SUCCESS;
}

 
#endif

