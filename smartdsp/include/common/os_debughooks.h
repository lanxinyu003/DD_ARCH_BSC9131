/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/06/10 09:02:35 $
 $Id: os_debughooks.h,v 1.17 2013/06/10 09:02:35 b41932 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_debughooks.h,v $
 $Revision: 1.17 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_debughooks.h

 @Description   OS Debug hooks configuration header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_DEBUGHOOKS_H
#define __OS_DEBUGHOOKS_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         dbg_log_id   Logging API

 @Description   Debug hook API.
 
                Debug Hooks are callback functions that are called by the
                OS runtime and initialization code at pre-determined locations
                in the code.
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Anchor        dbg_hook_opts
 
 @Collection    Debug Hook Options
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Description   Debug Hooks configuration options
*//***************************************************************************/
typedef enum
{
    OS_DEBUG_TASK_CREATE,
    /**< Called when task is created.
         Arguments are: os_task_handle and pointer to os_task_init_param_t */
    OS_DEBUG_TASK_SWITCH,
    /**< Called when there is a task switch.
         Arguments are: g_running_task->task_handle and g_highest_task->task_handle */
    OS_DEBUG_PROG_MMU,
    /**< Called on program MMU exception.
         Arguments are: pointer to os_sys_info_t and pointer to struct os_mmu_error  */
    OS_DEBUG_DATA_MMU,
    /**< Called on data MMU exception.
         Arguments are: pointer to os_sys_info_t and pointer to struct os_mmu_error  */
    OS_DEBUG_IPC_BASIC_SEND,
    /**< Called on IPC message send
         Arguments are: pointer to os_het_ipc_channel_t, pointer to local copy of current BD (message address
         in this BD is physical);
         Caution: this local copy is not valid after exiting the debug hook */
    OS_DEBUG_IPC_BASIC_RECEIVE,
    /**< Called on IPC message receive
         Arguments are: pointer to os_het_ipc_channel_t, pointer to local copy of current BD (message address
         in this BD is physical);
         Caution: this local copy is not valid after exiting the debug hook */
    OS_DEBUG_LAST
    /**< Number of supported debug hooks - MUST reain last */
} os_debug_hooks_enum;

/* @} */ /* end of dbg_hook_opts */

extern volatile debug_hook_function debug_hook_functions[];

/**************************************************************************//**
 @Function      osDebugHookCreate

 @Description   Register a debug hook callback function
 
 @Param[in]     hook_num    -  The event to register to - Must be one of
                               @ref dbg_hook_opts "Debug Hook Options"
 @Param[in]     func        -  The callback function to register.

 @Retval        OS_SUCCESS              - the hook was registered successfully
 @Retval        OS_ERR_TOO_MUCH         - hook_num is larger than OS_DEBUG_LAST
 @Retval        OS_ERR_ALREADY_CREATED  - the hook was previously registered
 
*//***************************************************************************/
os_status osDebugHookCreate(os_debug_hooks_enum hook_num, debug_hook_function func);

/**************************************************************************//**
 @Function      osDebugHookDelete

 @Description   Unregister a debug hook callback function
 
 @Param[in]     hook_num    -  The event to unregister from - Must be one of
                               @ref dbg_hook_opts "Debug Hook Options"

 @Retval        OS_SUCCESS              - the hook was unregistered successfully
 @Retval        OS_ERR_TOO_MUCH         - hook_num is larger than OS_DEBUG_LAST
 
*//***************************************************************************/
#define osDebugHookDelete(hook_num)     osDebugHookCreate(hook_num, NULL)

/** @} */ // end of dbg_log_id group

_OS_END_EXTERN_C

#endif // __OS_DEBUGHOOKS_H
