/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_debughooks.c,v 1.8 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_debughooks.c,v $
 $Revision: 1.8 $
******************************************************************************/

/******************************************************************************
 
 @File          os_debughooks.c

 @Description   OS Debug hooks source file.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "os_list_.h"
#include "os_task_.h"

volatile debug_hook_function debug_hook_functions[OS_DEBUG_LAST] = { NULL };
extern volatile os_tcb_t    *g_running_task;
extern volatile os_tcb_t    *g_highest_task;

void taskSwitchDebugHandler();


/*****************************************************************************/
os_status osDebugHookCreate(os_debug_hooks_enum hook_num, debug_hook_function func)
{
    osHwiSwiftDisable();
    if (hook_num >= OS_DEBUG_LAST)
    {
        OS_ASSERT;
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TOO_MUCH);
    }

    if ((debug_hook_functions[hook_num] != NULL) && (func != NULL))
    {
        OS_ASSERT;
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_ALREADY_CREATED);
    }

    debug_hook_functions[hook_num] = func;
 
    osHwiSwiftEnable();
    return OS_SUCCESS;
}


/*****************************************************************************/
void taskSwitchDebugHandler()
{
    if (debug_hook_functions[OS_DEBUG_TASK_SWITCH])
        debug_hook_functions[OS_DEBUG_TASK_SWITCH]((debug_hook_arg)g_running_task->task_handle, (debug_hook_arg)g_highest_task->task_handle);
}
