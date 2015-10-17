/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_log.h,v 1.20 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_log.h,v $
 $Revision: 1.20 $
******************************************************************************/

/**************************************************************************//**
 
 @File          os_log.h

 @Description   OS Logging header file.

 @Cautions      None.
 
*//***************************************************************************/


#ifndef __OS_LOG_H
#define __OS_LOG_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         dbg_log_id Logging API

 @Description   Logging API
 
 @{
*//***************************************************************************/

/**************************************************************************//**
 @Group         dbg_log_rutime_id Logging runtime API
 
 @{
*//***************************************************************************/


/**************************************************************************//**
 @Function      osLog

 @Description   Log events to the internal logging module.
 
 @Param[in]     op -  Event opcode.
     Opcodes supported:
        - define LOG_HWI_ENTER               (0x1 | OS_LOG_HWI | LOG_OPCODE_TO_STACK)
        - define LOG_HWI_EXIT                (0x2 | OS_LOG_HWI | LOG_OPCODE_FROM_STACK)
        - define LOG_HWI_CREATE              (0x3 | OS_LOG_HWI)
        - define LOG_HWI_SET_PRIORITY        (0x4 | OS_LOG_HWI)
        - define LOG_SWI_CREATE              (0x1 | OS_LOG_SWI)
        - define LOG_SWI_ENTER               (0x2 | OS_LOG_SWI | LOG_OPCODE_TO_STACK)
        - define LOG_SWI_EXIT                (0x3 | OS_LOG_SWI | LOG_OPCODE_FROM_STACK)
        - define LOG_SWI_ACTIVATE            (0x4 | OS_LOG_SWI)
        - define LOG_SWI_SET_PRIORITY        (0x5 | OS_LOG_SWI)
        - define LOG_SPINLOCK_GET            (0x1 | OS_LOG_SPINLOCK)
        - define LOG_SPINLOCK_RELEASE        (0x2 | OS_LOG_SPINLOCK)
        - define LOG_SPINLOCK_IRQ_GET        (0x3 | OS_LOG_SPINLOCK)
        - define LOG_SPINLOCK_IRQ_RELEASE    (0x4 | OS_LOG_SPINLOCK)
        - define LOG_CLASS_TRGT_NUM(LOG_CLASS,TRGT_NUM)
        - define LOG_CLASS_INIT_NUM(LOG_CLASS,INIT_NUM)

 @Param[in]    val -  Event-related value; This value is specific for each type of event.
*//***************************************************************************/
void osLog(uint32_t op, uint32_t val);


/**************************************************************************//**
 @Function      osLogRegisterHandler

 @Description   Define user callback function for os events.
 
                This function sets callback for specific event.
                User may use it in order to handle all the events
                of this type. If the user wish also to log these
                events he should call osLogImmediate() function
                for each event with proper arguments.
 
 @Param[in]    type_of_command -  Event opcode.
               Supported values:
               - OS_LOG_HWI
               - OS_LOG_SWI
               - OS_LOG_SPINLOCK

 @Param[in]    handler -  pointer to user callback, that
                          will be called on each event.
*//***************************************************************************/
void osLogRegisterHandler(uint32_t type_of_command, void  (*handler)(uint32_t op, uint32_t val, uint32_t time[2]));


/**************************************************************************//**
 @Function      osLogImmediate

 @Description   The same as osLog(), but does not call
                user-defined callback and does not check
                if this event was enabled.
 
                Usally called by user in event handler function
                to enable logging of this event.
 
 @Param[in]    op   - Event opcode.
 @Param[in]    val  - Event-related value.
                      This value is specific for each type of event.
 @Param[in]    time - Time value for log.
*//***************************************************************************/
void osLogImmediate(uint32_t op, uint32_t val,  uint32_t  time[2]);



void osLogClassProfilingDispatch();


/******************************************************************************

    User-defined events
 
******************************************************************************/

/* System defined part */




#define LOG_ENTER_EVENT     0x01000000
/**< start flag for opening LOG event */
#define LOG_EXIT_EVENT      0x02000000
/**< ending flag for LOG event */

/* User defined part - Example:

In this example, the user defines three events:

MY_EVENT        - user defined event that will be shown at log graph

MY_EVENT_ENTER  - user defined event that will be shown at log graph and
                  indicates start of user-defined event. All other events
                  will be marked until this event ends.

MY_EVENT_EXIT   - user defined event that will be shown at log graph and
                  indicates end of user-defined event.


#define MY_EVENT            (0x1 | USER_DEFINED_EVENT)
#define MY_EVENT_ENTER      (0x2 | USER_DEFINED_EVENT | LOG_ENTER_EVENT)
#define MY_EVENT_EXIT       (0x3 | USER_DEFINED_EVENT | LOG_EXIT_EVENT)


void foo()
{
    osLog(MY_EVENT_ENTER, 0xDEADBEE1);

    ……………………

    osLog(MY_EVENT, 0xDEADBEEF);

    ……………………

    osLog(MY_EVENT_EXIT, 0xDEADBEE3);
}


Consider a situation, where the user performs some calculations in
the idle task and wishes that this computation time will be considered in
core utilization calculation. In order to do it, the user should call
event of type LOG_ENTER_EVENT and LOG_EXIT_EVENT:

void Calculate()
{
    osLog(MY_EVENT_ENTER, 0xDEADBEE1);

    // Calculations...

    osLog(MY_EVENT_EXIT, 0xDEADBEE3);
}

*/

/*************************************************************//**
 @Collection    Types of opcodes

 @{
*//**************************************************************/
#define OS_LOG_HWI                  0x1000 /**< HWI events */
#define OS_LOG_SWI                  0x2000 /**< SWI events */
#define OS_LOG_SPINLOCK             0x3000 /**< Spinlock events */
#define OS_LOG_HWI_LATENCY          0x4000
#define OS_LOG_TASK                 0x5000 /**< Task events */
#define OS_LOG_CLASS_UTILIZATION    0x6000 /**< CLASS profiling */
#define OS_LOG_USER_DEFINED_EVENT   0x8000 /**< User-defined events */

#define LOG_OPCODE_TO_STACK         0x01000000
#define LOG_OPCODE_FROM_STACK       0x02000000

#define LOG_COMMAND_TYPE_MASK       0xFF00
/* @} */

/*************************************************************//**
 @Collection    Opcodes supported for osLog

 @{
*//**************************************************************/
#define LOG_HWI_ENTER               (0x1 | OS_LOG_HWI | LOG_OPCODE_TO_STACK) /**< HWI enter event */
#define LOG_HWI_EXIT                (0x2 | OS_LOG_HWI | LOG_OPCODE_FROM_STACK) /**< HWI exit event */
#define LOG_HWI_CREATE              (0x3 | OS_LOG_HWI) /**< HWI create event */
#define LOG_HWI_SET_PRIORITY        (0x4 | OS_LOG_HWI) /**< HWI priority set event */

#define LOG_SWI_CREATE              (0x1 | OS_LOG_SWI) /**< SWI create event */
#define LOG_SWI_ENTER               (0x2 | OS_LOG_SWI | LOG_OPCODE_TO_STACK) /**< SWI enter event */
#define LOG_SWI_EXIT                (0x3 | OS_LOG_SWI | LOG_OPCODE_FROM_STACK) /**< SWI exit event */
#define LOG_SWI_ACTIVATE            (0x4 | OS_LOG_SWI) /**< SWI activate event */
#define LOG_SWI_SET_PRIORITY        (0x5 | OS_LOG_SWI) /**< SWI priority set event */

#define LOG_SPINLOCK_GET            (0x1 | OS_LOG_SPINLOCK) /**< Spinlock get event */
#define LOG_SPINLOCK_RELEASE        (0x2 | OS_LOG_SPINLOCK) /**< Spinlock release event */
#define LOG_SPINLOCK_IRQ_GET        (0x3 | OS_LOG_SPINLOCK) /**< Spinlock irq disable get event */
#define LOG_SPINLOCK_IRQ_RELEASE    (0x4 | OS_LOG_SPINLOCK) /**< Spinlock irq disable release event */


#define LOG_CLASS_M2_UTILIZATION                (0x1 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_M3_UTILIZATION                (0x2 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_DDR_UTILIZATION               (0x3 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_SKYBLUE_UTILIZATION           (0x4 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_SYSTEM_TO_M2_UTILIZATION      (0x5 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_CORES_TO_M3_DDR_UTILIZATION   (0x6 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_QE_UTILIZATION                (0x7 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_SRIO_UTILIZATION              (0x8 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_TDM_UTILIZATION               (0x9 | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_DMA_PORT0_UTILIZATION         (0xA | OS_LOG_CLASS_UTILIZATION)
#define LOG_CLASS_DMA_PORT1_UTILIZATION         (0xB | OS_LOG_CLASS_UTILIZATION)
/* @} */

/*************************************************************//**
 @Collection    Opcodes not supported for osLog

 @{
*//**************************************************************/
#define LOG_HWI_ENABLE              (0x1 | OS_LOG_HWI_LATENCY)
#define LOG_HWI_DISABLE             (0x2 | OS_LOG_HWI_LATENCY)
#define LOG_SWIFT_HWI_DISABLE       (0x3 | OS_LOG_HWI_LATENCY)
#define LOG_SWIFT_HWI_ENABLE        (0x4 | OS_LOG_HWI_LATENCY)

#define LOG_TASK_CREATE              (0x1 | OS_LOG_TASK) /**< TASK create event */
#define LOG_TASK_SWITCH              (0x2 | OS_LOG_TASK) /**< TASK switch event */
#define LOG_TASK_ACTIVATE            (0x3 | OS_LOG_TASK) /**< TASK activate event */
#define LOG_TASK_SET_PRIORITY        (0x4 | OS_LOG_TASK) /**< TASK priority set event */
#define LOG_TASK_PEND_QUEUE          (0x5 | OS_LOG_TASK) /**< TASK queue pend */
#define LOG_TASK_PEND_SEM            (0x6 | OS_LOG_TASK) /**< TASK sem pend */
#define LOG_TASK_DELETE              (0x7 | OS_LOG_TASK) /**< TASK switch event */
#define LOG_TASK_POST_QUEUE          (0x8 | OS_LOG_TASK) /**< TASK queue post */
#define LOG_TASK_POST_SEM            (0x9 | OS_LOG_TASK) /**< TASK sem pend   */
#define LOG_TASK_SUSPEND             (0xA | OS_LOG_TASK) /**< TASK suspend */
#define LOG_TASK_DELAY               (0xB | OS_LOG_TASK) /**< TASK delay */
#define LOG_TASK_YIELD               (0xC | OS_LOG_TASK) /**< TASK yield*/
/* @} */


#define GET_FUNCTION_CODE(num) (num>>12)

/** @} */ /* end of dbg_log_rutime_id */
/** @} */ /* end of dbg_log_id */

_OS_END_EXTERN_C

#endif // __OS_LOG_H
