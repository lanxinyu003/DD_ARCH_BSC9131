/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_configdebug_.h,v 1.14 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_configdebug_.h,v $
 $Revision: 1.14 $
******************************************************************************/

/******************************************************************************
 
 @File          os_configdebug_.h

 @Description   OS Debug configuration header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_CONFIGDEBUG_H_
#define __OS_CONFIGDEBUG_H_


/* ***************************************************************************** */
/* BELOW ARE ALL THE COMPILE OPTIONS FOR THE OPERATING SYSTEM */
/* ***************************************************************************** */


/*****************************************************************************/
/* THESE ARE GENERAL DEFINES THAT ENABLE OR DISABLE ALL ERROR CHECKING AND   */
/* ASSERTS ON ERRORS                                                         */
/*****************************************************************************/
//#define DISABLE_ALL_ERROR_CHECKING
//#define DISABLE_ALL_ASSERTS
//#define DISABLE_ALL_DYNAMIC_CONTROL
//#define DISABLE_ALL_DEBUG_HOOKS


/*****************************************************************************/
/* BELOW EACH OBJECT CAN BE CONFIGURED FOR ITS LEVEL OF ERROR CHECKING       */
/*****************************************************************************/

#define HWI_ERROR_CHECKING
#define HWI_ERROR_ASSERT

#define SWI_ERROR_CHECKING
#define SWI_ERROR_ASSERT
#define SWI_DYNAMIC_CONTROL

#define TIMER_ERROR_CHECKING
#define TIMER_ERROR_ASSERT
#define TIMER_DYNAMIC_CONTROL

#define HW_TIMER_ERROR_CHECKING
#define HW_TIMER_ERROR_ASSERT
#define HW_TIMER_DYNAMIC_CONTROL

#define MEM_ERROR_CHECKING
#define MEM_ERROR_ASSERT

#define QUEUE_ERROR_CHECKING
#define QUEUE_ERROR_ASSERT

#define MESSAGE_ERROR_CHECKING
#define MESSAGE_ERROR_ASSERT

#define MESSAGE_QUEUE_ERROR_CHECKING
#define MESSAGE_QUEUE_ERROR_ASSERT

#define FRAME_ERROR_CHECKING
#define FRAME_ERROR_ASSERT

#define MMU_ERROR_CHECKING
#define L2_CACHE_ERROR_CHECKING

#define ENABLE_DEBUG_HOOKS


/*****************************************************************************/
/* BELOW EACH OBJECT CAN BE CONFIGURED FOR ITS LEVEL OF LOGGING       */
/*****************************************************************************/

// Backward compatibility
#if defined(ENABLE_KERNEL_AWARENESS_DEBUGGING) || defined(HWI_LOGGING) || defined(TASK_LOGGING) || defined(SWI_LOGGING) || defined(HWI_PRIORITY_LOGGING) || defined(SYSTEM_PROFILING_LOGGING)
#define ENABLE_KERNEL_AWARENESS_DEBUGGING
#endif

#undef HWI_LOGGING
#undef TASK_LOGGING
#undef SWI_LOGGING
#undef HWI_PRIORITY_LOGGING
#undef SYSTEM_PROFILING_LOGGING

#define HWI_LOGGING
#define TASK_LOGGING
#define SWI_LOGGING
#define HWI_PRIORITY_LOGGING
#define SYSTEM_PROFILING_LOGGING


/* ***************************************************************************** */
/*  THE USER SHOULD NOT HAVE TO MODIFY ANYTHING BELOW THIS POINT */
/* ***************************************************************************** */

#ifdef DISABLE_ALL_ERROR_CHECKING
    #undef HWI_ERROR_CHECKING
    #undef SWI_ERROR_CHECKING
    #undef QUEUE_ERROR_CHECKING
    #undef MESSAGE_ERROR_CHECKING
    #undef MESSAGE_QUEUE_ERROR_CHECKING
    #undef TIMER_ERROR_CHECKING
    #undef HW_TIMER_ERROR_CHECKING
    #undef MEM_ERROR_CHECKING
    #undef FRAME_ERROR_CHECKING

    #undef HWI_ERROR_ASSERT
    #undef SWI_ERROR_ASSERT
    #undef QUEUE_ERROR_ASSERT
    #undef TIMER_ERROR_ASSERT
    #undef HW_TIMER_ERROR_ASSERT
    #undef MEM_ERROR_ASSERT
    #undef FRAME_ERROR_ASSERT
    #undef MMU_ERROR_CHECKING
    #undef L2_CACHE_ERROR_CHECKING
#endif

#ifdef DISABLE_ALL_ASSERTS
    #undef HWI_ERROR_ASSERT
    #undef SWI_ERROR_ASSERT
    #undef QUEUE_ERROR_ASSERT
    #undef TIMER_ERROR_ASSERT
    #undef HW_TIMER_ERROR_ASSERT
    #undef MEM_ERROR_ASSERT
    #undef FRAME_ERROR_ASSERT
#endif

#ifdef DISABLE_ALL_DYNAMIC_CONTROL
    #undef SWI_DYNAMIC_CONTROL
    #undef TIMER_DYNAMIC_CONTROL
    #undef HW_TIMER_DYNAMIC_CONTROL
#endif

#ifndef ENABLE_KERNEL_AWARENESS_DEBUGGING
    #undef HWI_LOGGING
    #undef TASK_LOGGING
    #undef SWI_LOGGING
    #undef HWI_PRIORITY_LOGGING
    #undef SYSTEM_PROFILING_LOGGING
#endif


#ifdef DISABLE_ALL_DEBUG_HOOKS
    #undef ENABLE_DEBUG_HOOKS
#endif

#endif // __OS_CONFIGDEBUG_H_
