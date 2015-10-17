/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_sysinfo.c,v 1.8 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_sysinfo.c,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************
 
 @File          os_sysinfo.c

 @Cautions      None.
 
*//***************************************************************************/
#include "smartdsp_os_.h"
#include "os_init.h"
#include "os_.h"
#include "os_swi_.h"
#include "os_list_.h"
#include "os_task_.h"


#define MIN(A,B) (((A)<(B))?(A):(B))

/*****************************************************************************/

/* reference to current scheduler status */
extern volatile uint16_t        g_os_flags;

/* reference to active SWI               */
extern volatile os_swi_struct_  *g_active_swi_ptr;
extern os_swi_struct            swi_data;

/* reference to active task              */
extern volatile os_tcb_t        *g_running_task;

/*****************************************************************************/

os_status osSysInfoGet(os_sys_info_t* sys_info)
{
    uint32_t diff;
    int i;
 
    OS_ASSERT_COND(sys_info != 0);
 
    osHwiSwiftDisable();
 
    sys_info->os_context_info = OS_CONTEXT_NOT_CREATED;
 
    if (g_os_flags & OS_FLG_HWI_ACTIVE)
    {
        sys_info->os_context_info = OS_CONTEXT_HWI;
        sys_info->active_swi_info.swi_num = SWI_INFO_UNAVAILABLE;
    }
    else if (g_os_flags & OS_FLG_SWI_ACTIVE)
    {
        diff = (unsigned int)g_active_swi_ptr - (unsigned int)swi_data.list;
        sys_info->active_swi_info.swi_num = diff / sizeof(*g_active_swi_ptr);
        sys_info->os_context_info = OS_CONTEXT_SWI;
    }
    else if (g_os_flags & OS_FLG_BGD_ACTIVE)
    {
        sys_info->active_swi_info.swi_num = SWI_INFO_UNAVAILABLE;
        sys_info->active_task_info.stack_pointer = g_running_task->stack_pointer;
        sys_info->active_task_info.top_of_stack  = (uint32_t*)g_running_task->top_of_stack;
        sys_info->active_task_info.task_function = (uint32_t*)g_running_task->task_function;
        for(i = 0; i < MIN(TASK_NAME_LEN,TASK_INFO_NAME_LEN); i++)
            sys_info->active_task_info.name[i] = g_running_task->name[i];
        sys_info->active_task_info.priority = g_running_task->priority;
        sys_info->active_task_info.task_handle  = g_running_task->task_handle;
        sys_info->os_context_info = OS_CONTEXT_TASK;
    }
 
 
    osHwiSwiftEnable();

    return OS_SUCCESS;
}
