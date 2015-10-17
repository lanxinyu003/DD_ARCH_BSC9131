/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:26 $
 $Id: os_sysinfo.h,v 1.10 2012/11/22 16:28:26 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_sysinfo.h,v $
 $Revision: 1.10 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_sysinfo.h

 @Description   OS definitions for system information

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_SYSINFO_H
#define __OS_SYSINFO_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/***********************************************************//**
 @Group         gen_utility_id   General System Utilities

 @{
*//************************************************************/

/***********************************************************//**
 @Group         sys_info_id   System Information
 
 @Description   System information structures and function.

 @{
*//************************************************************/

#define TASK_INFO_NAME_LEN 12
#define SWI_INFO_UNAVAILABLE 0xFFFFFFFF

/*************************************************************//**
 @Description   OS contexts
*//**************************************************************/
typedef enum
{
    OS_CONTEXT_HWI = 1,        /**< Current context is hardware interrupt */
    OS_CONTEXT_SWI,            /**< Current context is software interrupt */
    OS_CONTEXT_TASK,           /**< Current context is task */
    OS_CONTEXT_NOT_CREATED     /**< Only valid during initialization */
} os_context_info_t;

/*************************************************************//**
 @Description   task information
*//**************************************************************/
typedef struct task_info
{
    uint32_t             *stack_pointer;           /**< Current SP in the task. */
    uint32_t             *top_of_stack;            /**< Top of task's stack. */
    uint32_t             *task_function;           /**< Function ran in this task. */
    char                 name[TASK_INFO_NAME_LEN];
    os_task_priority     priority;                 /**< Priority of task. */
    os_task_handle       task_handle;              /**< Task ID. */
} os_task_info_t;

/*************************************************************//**
 @Description   HW interrupt information
*//**************************************************************/
typedef struct hwi_info
{
    uint32_t             vector;          /**< Interrupt Vector */
} os_hwi_info_t;

/*************************************************************//**
 @Description   SW interrupt information
*//**************************************************************/
typedef struct
{
    uint32_t             swi_num;         /**< SWI Number       */
} os_swi_info_t;

/*************************************************************//**
 @Description   OS current system information
*//**************************************************************/
typedef struct
{
    os_task_info_t    active_task_info;  /**< Currentely active task info */
    os_swi_info_t     active_swi_info;   /**< Currentely active SWI info  */
    os_hwi_info_t     active_hwi_info;   /**< Not supported               */
    os_context_info_t os_context_info;   /**< Current context */
} os_sys_info_t;

/*************************************************************//**
 @Function      osSysInfoGet

 @Description   Returns current OS system information.

 @Param[out]    sys_info - structure to fill OS info in.

 @Retval        OS_SUCCESS
 
 @Cautions      None.
*//**************************************************************/
os_status osSysInfoGet(os_sys_info_t* sys_info);

/** @} */ /* end of sys_info_id */
/** @} */ /* end of gen_utility_id */

_OS_END_EXTERN_C

#endif // __OS_SYSINFO_H
