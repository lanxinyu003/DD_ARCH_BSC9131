/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/
/******************************************************************************
 $Date: 2014/07/20 15:40:35 $
 $Id: os_error.h,v 1.93 2014/07/20 15:40:35 b41640 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_error.h,v $
 $Revision: 1.93 $
******************************************************************************/

/**************************************************************************//**

 @File          os_error.h

 @Description   OS errors and status definitions.

 @Cautions      Some OS functions may return error codes that are built ad-hoc
                within the function itself. If an error code is unidentified
                numerically in this file; it can be understood by looking at
                OS_ERROR() or OS_ARCH_ERROR() and breaking it down to its basic
                components.

*//***************************************************************************/

#ifndef __OS_ERROR_H
#define __OS_ERROR_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         error_id OS Error Coding

 @Description   All error codes in SmartDSP OS
 
                The error ([31:0]) is encoded according to the following scheme:
 
                error[31:31] - set to 1 for architecture specific error.
 
                error[30:30] - reserved, set to 0.
 
                error[29:29] - set to 1 for system events indication.
 
                error[28:28] - set to 1 for system information indication.
 
                error[27:24] - architecture ID, valid only if error[31] is 1.
 
                error[23:22] - reserved, set to 0.
 
                error[21:16] - module ID.
 
                error[15:0]  - error ID.
 
 
 @{
*//***************************************************************************/


#define OS_ERR_ARCH_SPECIFIC            0x80000000
/**< Indicated architecture specific error */
#define OS_ERROR_BITS                   0x00000000
/**< Indicates system error */
#define OS_EVENT_BITS                   0x20000000
/**< Indicates system event */
#define OS_INFORMATION_BITS             0x10000000
/**< Indicates system information */
#define OS_ERROR(err, mod)              ((err) | (mod) | OS_ERROR_BITS)
/**< Constructing macro for building a module specific error code */
#define OS_EVENT(err, mod)              ((err) | (mod) | OS_EVENT_BITS)
/**< Constructing macro for building a module specific event code */
#define OS_INFO(err, mod)              ((err) | (mod) | OS_INFORMATION_BITS)
/**< Constructing macro for building a module specific information code */
#define OS_ARCH_ERROR(err, mod, arch)   ((err) | (mod) | (arch) | OS_ERR_ARCH_SPECIFIC)
/**< Constructing macro for building a architecture and module specific error code */


/**************************************************************************//**
 @Function      osReportEvent

 @Description   Reports a system event
 
 @Param[in]     event   -  The event to report
 @Param[in]     file    -  The file that is reporting this event
 @Param[in]     line    -  The line in the file that is reporting this event
 @Param[in]     remarks -  Additional remarks to report, may be set to #NULL

*//***************************************************************************/
void osReportEvent(uint32_t event, const char* file, int line, const char* remarks);

#ifdef OS_DOCUMENTATION_ONLY
/**************************************************************************//**
 @Function      REPORT_EVENT

 @Description   Reports a system event by calling osReportEvent() -
                may be disabled, based on compilation flags
 
 @Param[in]     _err   -  The event to report

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void REPORT_EVENT(uint32_t _err);


/**************************************************************************//**
 @Function      REPORT_EVENT_MESSAGE

 @Description   Reports a system event by calling osReportEvent() -
                may be disabled, based on compilation flags
 
 @Param[in]     _err    -  The event to report
 @Param[in]     message -  The additional remarks to report

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void REPORT_EVENT_MESSAGE(uint32_t _err, const char* message);


/**************************************************************************//**
 @Function      REPORT_ERROR

 @Description   Reports a system error by calling osReportEvent() -
                may be disabled, based on compilation flags
 
 @Param[in]     _err   -  The event to report

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void REPORT_ERROR(uint32_t _err);


/**************************************************************************//**
 @Function      REPORT_ERROR_MESSAGE

 @Description   Reports a system error by calling osReportEvent() -
                may be disabled, based on compilation flags
 
 @Param[in]     _err    -  The event to report
 @Param[in]     message -  The additional remarks to report

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void REPORT_ERROR_MESSAGE(uint32_t _err, const char* message);


/**************************************************************************//**
 @Function      RETURN_ERROR

 @Description   Reports a system error by calling osReportEvent() and returns
                from the calling function with the error code -
                may be disabled, based on compilation flags
 
 @Param[in]     _err   -  The event to report and return

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void RETURN_ERROR(uint32_t _err);


/**************************************************************************//**
 @Function      RETURN_ERROR_MESSAGE

 @Description   Reports a system error by calling osReportEvent() and returns
                from the calling function with the error code -
                may be disabled, based on compilation flags
 
 @Param[in]     _err    -  The event to report
 @Param[in]     message -  The additional remarks to report

 @Cautions      The function signature is for documentation purposes only

*//***************************************************************************/
void RETURN_ERROR_MESSAGE(uint32_t _err, const char* message);


#else // OS_DOCUMENTATION_ONLY

#ifdef DISABLE_EVENT_LOGGING

#define REPORT_EVENT(_err) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
    } while (0)

#define REPORT_EVENT_MESSAGE(_err,message) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
    } while (0)

#else

#define REPORT_EVENT(_err) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
        osReportEvent(_err, __FILE__, __LINE__,NULL); \
    } while (0)

#define REPORT_EVENT_MESSAGE(_err,_message) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
        osReportEvent(_err, __FILE__, __LINE__,_message); \
    } while (0)

#endif //DISABLE_EVENT_LOGGING



#ifdef DISABLE_ERROR_LOGGING
#define REPORT_ERROR(_err) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
    } while (0)

#define REPORT_ERROR_MESSAGE(_err,message) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
    } while (0)


#define RETURN_ERROR(_err) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
        return _err;\
    } while (0)

#define RETURN_ERROR_MESSAGE(_err,_message) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
        return _err;\
    } while (0)

#else

#define REPORT_ERROR(_err) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
        osReportEvent(_err, __FILE__, __LINE__,NULL); \
    } while (0)

#define REPORT_ERROR_MESSAGE(_err,_message) \
    do { \
        extern os_status g_os_last_error; g_os_last_error=_err; \
        osReportEvent(_err, __FILE__, __LINE__,_message); \
    } while (0)

#define RETURN_ERROR(_err) \
    do { \
        REPORT_ERROR(_err); \
        return _err;\
    } while (0)

#define RETURN_ERROR_MESSAGE(_err,_message) \
    do { \
        REPORT_ERROR_MESSAGE(_err,_message); \
        return _err;\
    } while (0)

#endif //DISABLE_ERROR_LOGGING
#endif // OS_DOCUMENTATION_ONLY

/* OS error architecture codes */

/************************************************************//**
 @Anchor        error_arch
 
 @Collection    Error architecture coding
 
 @{
*//*************************************************************/
#define OS_ERRARCH_MSC815X              0x07000000  /**< Architecture is MSC815X */
#define OS_ERRARCH_PSC9X3X              0x08000000  /**< Architecture is PSC9X3X */
#ifndef OS_DOCUMENTATION_ONLY
#endif // OS_DOCUMENTATION_ONLY

/* @} */ /* end of error_arch */


/************************************************************//**
 @Collection    Error module coding
 
 @{
*//*************************************************************/
#define OS_ERRMODULE_GENERAL            0x00010000  /**< General software module */
#define OS_ERRMODULE_HWI                0x00020000  /**< Hardware interrupt software module */
#define OS_ERRMODULE_SWI                0x00030000  /**< Software interrupt software module */
#define OS_ERRMODULE_TIMERS             0x00040000  /**< Software timers software module */
#define OS_ERRMODULE_HW_TIMERS          0x00050000  /**< Hardware timers software module */
#define OS_ERRMODULE_QUEUES             0x00060000  /**< Queues software module */
#define OS_ERRMODULE_MESSAGES           0x00070000  /**< Messaging software module */
#define OS_ERRMODULE_DMA                0x00080000  /**< DMA software module */
#define OS_ERRMODULE_NET                0x00090000  /**< Network stacks software module */
#define OS_ERRMODULE_BIO                0x000A0000  /**< Buffered I/O (BIO) software module */
#define OS_ERRMODULE_CIO                0x000B0000  /**< Character I/O (CIO) software module */
#define OS_ERRMODULE_SIO                0x000C0000  /**< Synchronized I/O (SIO) software module */
#define OS_ERRMODULE_WDT                0x000D0000  /**< Watchdog timers software module */
#define OS_ERRMODULE_TSK                0x000E0000  /**< Tasks software module */
#define OS_ERRMODULE_EVENT              0x000F0000  /**< Events software module */
#define OS_ERRMODULE_EVENT_QUEUE        0x00100000  /**< Events queue software module */
#define OS_ERRMODULE_EVENT_SEMAPHORE    0x00110000  /**< Events semaphore software module */
#define OS_ERRMODULE_LLD                0x00120000  /**< Low Level Driver (LLD) software module */
#define OS_ERRMODULE_TICK               0x00130000  /**< System tick software module */
#define OS_ERRMODULE_COP                0x00140000  /**< Co-Processor (COP) software module */

/* @} */ /* end of error_mod */

/************************************************************//**
 @Collection    Error Low Level Driver (LLD) coding
 
 @{
*//*************************************************************/
#define OS_ERRMODULE_TDM                0x00200000  /**< Time Division Multiplexing (TDM) LLD */
#define OS_ERRMODULE_ETHERNET           0x00210000  /**< Ethernet LLD */
#define OS_ERRMODULE_UART               0x00220000  /**< UART LLD */
#define OS_ERRMODULE_HDI                0x00230000  /**< Host Interface (HDI) LLD */
#define OS_ERRMODULE_DSI                0x00240000  /**< Direct Slave Interface (DSI) LLD */
#define OS_ERRMODULE_MMU                0x00250000  /**< Memory Management Unit (MMU) LLD */
#define OS_ERRMODULE_SRIO               0x00260000  /**< Serial RapidIO LLD */
#define OS_ERRMODULE_I2C                0x00270000  /**< I2C LLD */
#define OS_ERRMODULE_SPI                0x00280000  /**< Serial Peripheral Interface (SPI) LLD */
#define OS_ERRMODULE_CLOCK              0x00290000  /**< Clocks LLD */
#define OS_ERRMODULE_CLASS              0x002A0000  /**< Chip-Level Arbitration and Switching System LLD */
#define OS_ERRMODULE_PERF_MON           0x002B0000  /**< Performance Monitor LLD */
#define OS_ERRMODULE_L2_CACHE           0x002C0000  /**< L2 cache LLD */
#define OS_ERRMODULE_MAPLE              0x002D0000  /**< Multi Accelerator Platform Engine (MAPLE) LLD */
#define OS_ERRMODULE_VIRQ               0x002E0000  /**< Virtual Interrupts LLD */
#define OS_ERRMODULE_DEBUG_PRINT        0x002F0000  /**< Debug Print LLD */
#define OS_ERRMODULE_EMSG               0x00300000  /**< Enhanced Message Unit (eMSG) LLD */
#define OS_ERRMODULE_QML                0x00310000  /**< QMan-Lite (QML) LLD */
#define OS_ERRMODULE_BML                0x00320000  /**< BMan-Lite (BML) LLD */
#define OS_ERRMODULE_HSSI               0x00330000  /**< High Speed Serial nterface (HSSI) LLD */
#define OS_ERRMODULE_HDLC               0x00340000  /**< High-Level Data Link Control LLD    */
#define OS_ERRMODULE_AIC                0x00350000  /**< Antenna Interface Controller    */
#define OS_ERRMODULE_CPRI               0x00360000  /**< Antenna Interface Controller    */
#define OS_ERRMODULE_BMAN               0x00370000  /**< Buffer's manager */
#define OS_ERRMODULE_QMAN               0x00380000  /**< Queue's manager */
#define OS_ERRMODULE_DTU                0x00390000  /**< Debug and Trace Unit */

#define OS_ERRMODULE_BOOT               0x003F0000  /**< Reserved coding space for boot ROM */

/* @} */ /* end of error_lld */


/* OS error values */

#define OS_FAIL                         0x00000000  /**< General failure */
#define OS_SUCCESS                      0x00000001  /**< General success */

#define OS_ERR_UNKNOWN                  0x00000002  /**< Unknown error */

#define OS_ERR_NO_MEMORY                0x0000FFFF  /**< No memory available */

#define OS_ERR_HANDLE_INVALID           0x0000FFEF  /**< Passed handle is invalid */
#define OS_ERR_PARAM_INVALID            0x0000FFEE  /**< Passed parameter is invalid */
#define OS_ERR_MODE_INVALID             0x0000FFED  /**< Passed mode is invalid */
#define OS_ERR_FUNCTION_INVALID         0x0000FFEC  /**< Passed function is invalid */
#define OS_ERR_COMMAND_INVALID          0x0000FFEB  /**< Passed command is invalid */
#define OS_ERR_COMMAND_UNSUPPORTED      0x0000FFEA  /**< Passed command is unsupported */
#define OS_ERR_SPINLOCK_TAKEN           0x0000FFE9  /**< The command could not obtain a spinlock, and failed. */

#define OS_ERR_UNAVAILABLE              0x0000FFDF  /**< Resource is unavailable */
#define OS_ERR_ALREADY_CREATED          0x0000FFDE  /**< Resource was already created */
#define OS_ERR_NOT_CREATED              0x0000FFDD  /**< Resource was not created */
#define OS_ERR_NOT_ACTIVE               0x0000FFDC  /**< Resource was created, but not activated */
#define OS_ERR_ALREADY_FREE             0x0000FFDB  /**< resource was already frees */

#define OS_ERR_PRIORITY_INVALID         0x0000FFDA  /**< Passed priority is invalid */

#define OS_ERR_SOURCE_INVALID           0x0000FFD9  /**< Passed source is invalid */
#define OS_ERR_DEST_INVALID             0x0000FFD8  /**< Passed destination is invalid */


#define OS_ERR_INTERVAL_UNSUPPORTED     0x0000FFD7  /**< Passed interval is unsupported */
#define OS_ERR_EMPTY                    0x0000FFD6  /**< Resource is empty */
#define OS_ERR_FULL                     0x0000FFD4  /**< Resource is full */
#define OS_ERR_TOO_MUCH                 0x0000FFD3  /**< Attempt to process too much data */
#define OS_ERR_BUSY                     0x0000FFD2  /**< Resource is currently busy */
#define OS_ERR_NO_ROOM                  0x0000FFD1  /**< No room to store all of the data */
#define OS_ERR_TYPE_INVALID             0x0000FFD0  /**< Indicated type of resource is invalid */
#define OS_ERR_LENGTH_INVALID           0x0000FFCF  /**< Passed length is invalid */
#define OS_ERR_TOO_BIG                  0x0000FFCE  /**< Data is too large */
#define OS_ERR_TOO_SMALL                0x0000FFCD  /**< Data is too small */
#define OS_ERR_VERSION_ERROR            0x0000FFCC  /**< Version is not compatible */
#define OS_ERR_TIMEOUT                  0x0000FFCB  /**< Timeout */
#define OS_ERR_PEND_IN_INTERRUPT        0x0000FFCA  /**< Attempt to pend while in interrupt context */
#define OS_ERR_OVERFLOW                 0x0000FFC9  /**< Overflow error */
#define OS_ERR_DELAY_IN_INTERRUPT       0x0000FFC8  /**< Attempt to delay while in interrupt context */
#define OS_ERR_YIELD_IN_INTERRUPT       0x0000FFC7  /**< Attempt to yield while in interrupt context */
#define OS_ERR_YIELD_TO_ILLEGAL_TASK    0x0000FFC6  /**< Attempt to yield to an illegal task */
#define OS_ERR_YIELD_TO_SINGLE_TASK     0x0000FFC5  /**< Attempt to yield when there are no other tasks */
#define OS_ERR_PEND_IN_LOCK             0x0000FFC4  /**< Attempt to pend when scheduler locked */
#define OS_ERR_ALREADY_ACTIVE           0x0000FFC3  /**< Attempt to activate an already active resource */
#define OS_ERR_DELETE_LOCKED            0x0000FFC2  /**< Attempt to delete a locked resource */
#define OS_ERR_DELAY_IN_LOCK            0x0000FFC1  /**< Attempt to delay from within a locked resource */
#define OS_ERR_UNDERRUN                 0x0000FFC0  /**< Underrun error */

/**************************************************************************//**
 @Description   Check and return on failure.
*//***************************************************************************/
#define OS_RETURN_NO_SUCCESS(status) if(status != OS_SUCCESS) RETURN_ERROR(status);

/* For module specific errors, use values below 0xF000 (i.e. 0xEFFF and down) */

/************************************************************//**
 @Collection    Task related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_TSK_BLOCKING                     OS_ERROR(OS_ERR_NOT_ACTIVE, OS_ERRMODULE_TSK)
/**< Task is blocking */
#define OS_ERR_TSK_ALREADY_SUSPENDED            OS_ERROR(0x0000EFFE, OS_ERRMODULE_TSK)
/**< Attempt to suspend an already suspended task */
#define OS_ERR_TSK_NOT_CREATED                  OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_TSK)
/**< Task not yet created */
#define OS_ERR_TSK_ALREADY_CREATED              OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_TSK)
/**< Attempt to create an already created task */
#define OS_ERR_TSK_UNAVAILABLE                  OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_TSK)
/**< Task is unavailable */
#define OS_ERR_TSK_DELAY_IN_INTERR              OS_ERROR(OS_ERR_DELAY_IN_INTERRUPT, OS_ERRMODULE_TSK)
/**< Attempt to delay a task while in interrupt context */
#define OS_ERR_TSK_YIELD_TO_ILLEGAL_TASK        OS_ERROR(OS_ERR_YIELD_TO_ILLEGAL_TASK, OS_ERRMODULE_TSK)
/**< Attempt to yield to an illegal task */
#define OS_ERR_TSK_YIELD_TO_SINGLE_TASK         OS_ERROR(OS_ERR_YIELD_TO_SINGLE_TASK, OS_ERRMODULE_TSK)
/**< Attempt to yield when there is a single task in the system */
#define OS_ERR_TSK_OS_ERR_YIELD_IN_INTERR       OS_ERROR(OS_ERR_YIELD_IN_INTERRUPT, OS_ERRMODULE_TSK)
/**< Attempt to yield a task while in interrupt context */
#define OS_ERR_TSK_OS_ERR_ALREADY_ACTIVE        OS_ERROR(OS_ERR_ALREADY_ACTIVE, OS_ERRMODULE_TSK)
/**< Attempt activate an already active task */
#define OS_ERR_TSK_NOT_ACTIVE                   OS_ERROR(OS_ERR_NOT_ACTIVE, OS_ERRMODULE_TSK)
/**< Attempt to query an inactive task */
#define OS_ERR_TSK_DELAY_IN_LOCK                OS_ERROR(OS_ERR_DELAY_IN_LOCK, OS_ERRMODULE_TSK)
/**< Attempt to delay a task while the scheduler is locked */
#define OS_ERR_TSK_DELETE_LOCKED                OS_ERROR(OS_ERR_DELETE_LOCKED, OS_ERRMODULE_TSK)
/**< Attempt to delete a locked task */
#define OS_ERR_TSK_MMU_SET_IN_INTERR            OS_ERROR(0x0000EFFD, OS_ERRMODULE_TSK)
/**< Attempt to set a task MMU context while in interrupt context */
#define OS_ERR_TSK_STACK_TOO_BIG                OS_ERROR(OS_ERR_TOO_BIG, OS_ERRMODULE_TSK)
/**< Attempt to create a task with a stack too large to be handled by the OS */
#define OS_ERR_TSK_STACK_UNALIGNED              OS_ERROR(OS_ERR_LENGTH_INVALID, OS_ERRMODULE_TSK)
/**< Attempt to create a task with a stack that is unaligned to software/hardware requirements */


/* @} */

/************************************************************//**
 @Collection    Event related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_EVENT_UNAVAILABLE                OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_EVENT)
/**< No event is available */
#define OS_ERR_EVENT_PEND_IN_LOCK               OS_ERROR(OS_ERR_PEND_IN_LOCK, OS_ERRMODULE_EVENT)
/**< Attempt to pend on an event while the scheduler is locked */

#define OS_ERR_EVENT_QUEUE_TIMEOUT              OS_ERROR(OS_ERR_TIMEOUT, OS_ERRMODULE_EVENT_QUEUE)
/**< Timeout while pending on an event queue */
#define OS_ERR_EVENT_QUEUE_UNAVAILABLE          OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_EVENT_QUEUE)
/**< No event queue is available */
#define OS_ERR_EVENT_QUEUE_PEND_INTERR          OS_ERROR(OS_ERR_PEND_IN_INTERRUPT, OS_ERRMODULE_EVENT_QUEUE)
/**< Attempt to pend on an event queue while in interrupt context */
#define OS_ERR_EVENT_QUEUE_OVERFLOW             OS_ERROR(OS_ERR_OVERFLOW, OS_ERRMODULE_EVENT_QUEUE)
/**< Overflow on an event queue */
#define OS_ERR_EVENT_QUEUE_ALREADY_CREATED      OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_EVENT_QUEUE)
/**< Attempt to create an already created event queue */
#define OS_ERR_EVENT_QUEUE_INVALID              OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_EVENT_QUEUE)
/**< Attempt to perform an operation on an invalid event queue */

#define OS_ERR_EVENT_SEMA_TIMEOUT               OS_ERROR(OS_ERR_TIMEOUT,     OS_ERRMODULE_EVENT_SEMAPHORE)
/**< Timeout while pending on an event semaphore */
#define OS_ERR_EVENT_SEMA_UNAVAILABLE           OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_EVENT_SEMAPHORE)
/**< No event semaphore is available */
#define OS_ERR_EVENT_SEMA_PEND_INTERR           OS_ERROR(OS_ERR_PEND_IN_INTERRUPT, OS_ERRMODULE_EVENT_SEMAPHORE)
/**< Attempt to pend on an event semaphore while in interrupt context */
#define OS_ERR_EVENT_SEMA_OVERFLOW              OS_ERROR(OS_ERR_OVERFLOW, OS_ERRMODULE_EVENT_SEMAPHORE)
/**< Overflow on an event semaphore */
#define OS_ERR_EVENT_SEMA_ALREADY_CREATED       OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_EVENT_QUEUE)
/**< Attempt to create an already created event semaphore */
#define OS_ERR_EVENT_SEMA_INVALID               OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_EVENT_QUEUE)
/**< Attempt to perform an operation on an invalid event semaphore */

/* @} */

/************************************************************//**
 @Collection    Hardware Interrupt related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_HWI_INVALID                      OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_HWI)
/**< Hardware interrupt handle is invalid */
#define OS_ERR_HWI_MODE_INVALID                 OS_ERROR(OS_ERR_MODE_INVALID, OS_ERRMODULE_HWI)
/**< Hardware interrupt mode is invalid */
#define OS_ERR_HWI_FUNCTION_INVALID             OS_ERROR(OS_ERR_FUNCTION_INVALID, OS_ERRMODULE_HWI)
/**< Hardware interrupt function (ISR) is invalid */
#define OS_ERR_HWI_PRIORITY_INVALID             OS_ERROR(OS_ERR_PRIORITY_INVALID, OS_ERRMODULE_HWI)
/**< Hardware interrupt priority is invalid */
#define OS_ERR_HWI_COMMAND_INVALID              OS_ERROR(OS_ERR_COMMAND_INVALID, OS_ERRMODULE_HWI)
/**< Hardware interrupt command is invalid */
#define OS_ERR_HWI_DISPATCHER_INVALID           OS_ERROR(OS_ERR_PARAM_INVALID, OS_ERRMODULE_HWI)
/**< Hardware interrupt dispatcher is invalid */
#define OS_ERR_HWI_NO_ROOM                      OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_HWI)
/**< No room to install a new dispatcher */

/* @} */

/************************************************************//**
 @Collection    Software interrupt related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_SWI_INVALID                      OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_SWI)
/**< Software interrupt handle is invalid */
#define OS_ERR_SWI_UNAVAILABLE                  OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_SWI)
/**< No software interrupt is available */
#define OS_ERR_SWI_ALREADY_CREATED              OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_SWI)
/**< Attempt to create an already created software interrupt */
#define OS_ERR_SWI_NOT_CREATED                  OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_SWI)
/**< Software interrupt handle is not created */
#define OS_ERR_SWI_NOT_ACTIVE                   OS_ERROR(OS_ERR_NOT_ACTIVE, OS_ERRMODULE_SWI)
/**< Attempt to query an inactive software interrupt */
#define OS_ERR_SWI_FUNCTION_INVALID             OS_ERROR(OS_ERR_FUNCTION_INVALID, OS_ERRMODULE_SWI)
/**< Software interrupt function (ISR) is invalid */
#define OS_ERR_SWI_PRIORITY_INVALID             OS_ERROR(OS_ERR_PRIORITY_INVALID, OS_ERRMODULE_SWI)
/**< Software interrupt priority is invalid */

/* @} */


/************************************************************//**
 @Collection    Software timer error codes
 
 @{
*//*************************************************************/
#define OS_ERR_TM_INVALID                       OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_TIMERS)
/**< Software timer handle is invalid */
#define OS_ERR_TM_MODE_INVALID                  OS_ERROR(OS_ERR_MODE_INVALID, OS_ERRMODULE_TIMERS)
/**< Software timer mode is invalid */
#define OS_ERR_TM_FUNCTION_INVALID              OS_ERROR(OS_ERR_FUNCTION_INVALID, OS_ERRMODULE_TIMERS)
/**< Software timer function is invalid */
#define OS_ERR_TM_UNAVAILABLE                   OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_TIMERS)
/**< No software timer is available */
#define OS_ERR_TM_ALREADY_CREATED               OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_TIMERS)
/**< Attempt to create an already created software timer */
#define OS_ERR_TM_NOT_CREATED                   OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_TIMERS)
/**< Software timer handle is not created */
#define OS_ERR_TM_NOT_ACTIVE                    OS_ERROR(OS_ERR_NOT_ACTIVE, OS_ERRMODULE_TIMERS)
/**< Attempt to query an inactive software timer */

/* @} */

/************************************************************//**
 @Collection    Hardware timer related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_HW_TM_INVALID                    OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_HW_TIMERS)
/**< Hardware timer handle is invalid */
#define OS_ERR_HW_TM_UNAVAILABLE                OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_HW_TIMERS)
/**< No hardware timer is available */
#define OS_ERR_HW_TM_ALREADY_CREATED            OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_HW_TIMERS)
/**< Attempt to create an already created hardware timer */
#define OS_ERR_HW_TM_NOT_CREATED                OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_HW_TIMERS)
/**< Hardware timer handle is not created */
#define OS_ERR_HW_TM_MODE_INVALID               OS_ERROR(OS_ERR_MODE_INVALID, OS_ERRMODULE_HW_TIMERS)
/**< Hardware timer mode is invalid */
#define OS_ERR_HW_TM_PRIORITY_INVALID           OS_ERROR(OS_ERR_PRIORITY_INVALID, OS_ERRMODULE_HW_TIMERS)
/**< Hardware interrupt priority is invalid */
#define OS_ERR_HW_TM_SOURCE_INVALID             OS_ERROR(OS_ERR_SOURCE_INVALID, OS_ERRMODULE_HW_TIMERS)
/**< Clock source for hardware timer is invalid */
#define OS_ERR_HW_TM_INTERVAL_UNSUPPORTED       OS_ERROR(OS_ERR_INTERVAL_UNSUPPORTED, OS_ERRMODULE_HW_TIMERS)
/**< Hardware timer interval is unsupported (out of range) */
#define OS_ERR_HW_TM_ALREADY_ACTIVE             OS_ERROR(OS_ERR_ALREADY_ACTIVE, OS_ERRMODULE_HW_TIMERS)
/**< hardware timer is already active           */
#define OS_ERR_HW_TM_NOT_ACTIVE                 OS_ERROR(OS_ERR_NOT_ACTIVE, OS_ERRMODULE_HW_TIMERS)
/**< hardware timer was created but not activated */
#define OS_ERR_HW_TM_OS_ERR_BUSY                OS_ERROR(OS_ERR_BUSY, OS_ERRMODULE_HW_TIMERS)
/**< hardware timer is running and can't be modified*/
/* @} */

/************************************************************//**
 @Collection    Queue related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_Q_UNAVAILABLE                    OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_QUEUES)
/**< Queue handle is invalid */
#define OS_ERR_Q_ALREADY_CREATED                OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_QUEUES)
/**< Attempt to create an already created queue */
#define OS_ERR_Q_NOT_CREATED                    OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_QUEUES)
/**< Queue handle is not created */
#define OS_ERR_Q_EMPTY                          OS_ERROR(OS_ERR_EMPTY, OS_ERRMODULE_QUEUES)
/**< Queue is empty */
#define OS_ERR_Q_FULL                           OS_ERROR(OS_ERR_FULL , OS_ERRMODULE_QUEUES)
/**< Queue is full */

/* @} */

/************************************************************//**
 @Collection    Messaging related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_MSG_INVALID                      OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_MESSAGES)
/**< Message handle is invalid */
#define OS_ERR_MSG_UNAVAILABLE                  OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_MESSAGES)
/**< No message handle is available */
#define OS_ERR_MSG_ALREADY_CREATED              OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_MESSAGES)
/**< Attempt to create an already created message handle */
#define OS_ERR_MSG_NOT_CREATED                  OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_MESSAGES)
/**< Message handle is not created */
#define OS_ERR_MSG_FUNCTION_INVALID             OS_ERROR(OS_ERR_FUNCTION_INVALID, OS_ERRMODULE_MESSAGES)
/**< Message function is invalid */
#define OS_ERR_MSG_NUM_LIMIT                    OS_ERROR(OS_ERR_TOO_MUCH, OS_ERRMODULE_MESSAGES)
/**< Too many message handles */
#define OS_ERR_MSG_DEST_INVALID                 OS_ERROR(OS_ERR_DEST_INVALID, OS_ERRMODULE_MESSAGES)
/**< Message destination is invalid */
#define OS_ERR_MSG_SRC_INVALID                  OS_ERROR(OS_ERR_SOURCE_INVALID, OS_ERRMODULE_MESSAGES)
/**< Message source is invalid */
#define OS_ERR_MSG_BUSY                         OS_ERROR(OS_ERR_BUSY, OS_ERRMODULE_MESSAGES)
/**< Message mailbox/queue is busy */
#define OS_ERR_MSG_DEST_LOCKED                  OS_ERR_MSG_BUSY
/**< Message is locked by the destination */
#define OS_ERR_MSG_NO_MSG_TO_GET                OS_ERROR(OS_ERR_ALREADY_FREE, OS_ERRMODULE_MESSAGES)
/**< No message available */

/* @} */

/************************************************************//**
 @Collection    DMA related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_DMA_RESOURCE_UNAVAILABLE         OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_DMA)
/**< DMA resource is unavailable */
#define OS_ERR_DMA_RESOURCE_ALREADY_FREE        OS_ERROR(OS_ERR_ALREADY_FREE, OS_ERRMODULE_DMA)
/**< Attempt to free an already freed DMA resource */
#define OS_ERR_DMA_NO_ROOM_FOR_CHAIN            OS_ERROR(OS_ERR_NO_ROOM, OS_ERRMODULE_DMA)
/**< No room available to create DMA chain (BD ring) */
#define OS_ERR_DMA_CHAIN_FULL                   OS_ERROR(OS_ERR_FULL, OS_ERRMODULE_DMA)
/**< No room in DMA chain (BD ring) for additional transfers/buffers */
#define OS_ERR_DMA_CHAIN_TYPE_INVALID           OS_ERROR(OS_ERR_TYPE_INVALID, OS_ERRMODULE_DMA)
/**< DMA chain (BD ring) type is invalid */
#define OS_ERR_DMA_CHAIN_LENGTH_INVALID         OS_ERROR(OS_ERR_LENGTH_INVALID, OS_ERRMODULE_DMA)
/**< DMA chain (BD ring) length is invalid */
#define OS_ERR_DMA_CHAIN_BOUND                  OS_ERROR(0x0000EFFB, OS_ERRMODULE_DMA)
/**< DMA chain (BD ring) is currently bound to a channel */
#define OS_ERR_DMA_CHANNEL_BOUND                OS_ERROR(0x0000EFFA, OS_ERRMODULE_DMA)
/**< DMA channel is currently bound to a chain (BD ring) */
#define OS_ERR_DMA_CHANNEL_NOT_BOUND            OS_ERROR(0x0000EFF9, OS_ERRMODULE_DMA)
/**< DMA channel is currently not bound to a chain (BD ring) */
#define OS_ERR_DMA_CHANNEL_INDEX_INVALID        OS_ERROR(0x0000EFF8, OS_ERRMODULE_DMA)
/**< DMA channel number is invalid */
#define OS_ERR_DMA_PRIORITY_TAKEN               OS_ERROR(0x0000EFF7, OS_ERRMODULE_DMA)
/**< DMA channel priority is already taken */
#define OS_ERR_DMA_DEST_NOT_ALIGNED             OS_ERROR(0x0000EFF6, OS_ERRMODULE_DMA)
/**< DMA destination address is not aligned */
#define OS_ERR_DMA_SOURCE_NOT_ALIGNED           OS_ERROR(0x0000EFF5, OS_ERRMODULE_DMA)
/**< DMA source address is not aligned */
#define OS_ERR_DMA_SIZE_NOT_ALIGNED             OS_ERROR(0x0000EFF4, OS_ERRMODULE_DMA)
/**< DMA transfer size is not aligned */

/* @} */

/************************************************************//**
 @Collection    Network stack related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_NET_CONSUMED                     OS_ERROR(0x0000EFFF, OS_ERRMODULE_NET)
/**< Network packet was already consumed */
#define OS_ERR_NET_TOO_LARGE                    OS_ERROR(OS_ERR_TOO_BIG, OS_ERRMODULE_NET)
/**< Network packet is too large */
#define OS_ERR_NET_TOO_SMALL                    OS_ERROR(OS_ERR_TOO_SMALL, OS_ERRMODULE_NET)
/**< Network packet is too small */
#define OS_ERR_SOCKET_UNAVAILABLE               OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_NET)
/**< Socket is unavailable */
#define OS_ERR_RTP_LENGTH_ERROR                 OS_ERROR(OS_ERR_LENGTH_INVALID, OS_ERRMODULE_NET)
/**< RTP length is erroneous */
#define OS_ERR_RTP_VERSION_ERROR                OS_ERROR(OS_ERR_VERSION_ERROR, OS_ERRMODULE_NET)
/**< RTP version is erroneous */
#define OS_ERR_RTCP_VERSION_ERROR               OS_ERROR(OS_ERR_VERSION_ERROR, OS_ERRMODULE_NET)
/**< RTCP length is erroneous */
#define OS_ERR_RTCP_TYPE_INVALID                OS_ERROR(OS_ERR_TYPE_INVALID, OS_ERRMODULE_NET)
/**< RTCP type is invalid */
#define OS_ERR_RTCP_LENGTH_ERROR                OS_ERROR(OS_ERR_LENGTH_INVALID, OS_ERRMODULE_NET)
/**< RTCP length is erroneous */
#define OS_ERR_NET_UNREACHABLE                  OS_ERROR(0x0000EFFC, OS_ERRMODULE_NET)
/**< Network destination is unreachable */
#define OS_ERR_NET_SP_MISMATCH                  OS_ERROR(0x0000EFFB, OS_ERRMODULE_NET)
/**< IPSec security policy mismatch */
#define OS_ERR_NO_BUFFERS                       OS_ERROR(OS_ERR_NO_MEMORY, OS_ERRMODULE_GENERAL)
/**< No available buffers in buffer pool */
#define OS_ERR_NO_FRAMES                        OS_ERROR(OS_ERR_NO_MEMORY, OS_ERRMODULE_GENERAL)
/**< No available frames in frame pool */
#define OS_ERR_ARP_TABLE_FULL                   OS_ERROR(OS_ERR_FULL, OS_ERRMODULE_NET)
/**< ARP table is full - can't add another ARP entry */
#define OS_ERR_NET_UNKNOWN_PROTOCOL             OS_ERROR(0x0000EFFA, OS_ERRMODULE_NET)
/**< Network protocol is unknown */
#define OS_ERR_NET_ND_TABLE_FULL                OS_ERROR(OS_ERR_FULL, OS_ERRMODULE_NET)
/**< Neighbor discovery table is full - can't add another ND entry */

/* @} */

/************************************************************//**
 @Collection    MMU related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_MMU_VIRT_OVERLAPPED              OS_ERROR(0x0000FFFF, OS_ERRMODULE_MMU)
/**< Overlap of virtual addresses between MMU descriptors */
#define OS_ERR_MMU_PHYS_OVERLAPPED              OS_ERROR(0x0000FFFE, OS_ERRMODULE_MMU)
/**< Overlap of physical addresses between MMU descriptors */
#define OS_ERR_MMU_WRONG_ALIGNMENT              OS_ERROR(0x0000FFFD, OS_ERRMODULE_MMU)
/**< MMU addresses are not aligned */
#define OS_ERR_MMU_WRONG_SIZE                   OS_ERROR(0x0000FFFC, OS_ERRMODULE_MMU)
/**< MMU descriptor size is not legal */
#define OS_ERR_MMU_WRONG_DESC                   OS_ERROR(0x0000FFFB, OS_ERRMODULE_MMU)
/**< MMU descriptor is not legal */
#define OS_ERR_MMU_WRONG_BOUNDARY               OS_ERROR(0x0000FFFA, OS_ERRMODULE_MMU)
/**< MMU descriptor is not within boundary restrictions */


/* Hardware-detected MMU errors */
#define OS_ERR_MMU_MULTIPLE_HIT                 OS_ERROR(0x0000FF0F, OS_ERRMODULE_MMU)
/**< MMU hit on multiple descriptors */
#define OS_ERR_MMU_NON_MAPPED                   OS_ERROR(0x0000FF1F, OS_ERRMODULE_MMU)
/**< Access to address that is not mapped by the hardware */
#define OS_ERR_MMU_PRIVILEGE                    OS_ERROR(0x0000FF2F, OS_ERRMODULE_MMU)
/**< Access with wrong privilege level */
#define OS_ERR_MMU_NOT_ALIGNED                  OS_ERROR(0x0000FF3F, OS_ERRMODULE_MMU)
/**< Access size not aligned to address */
#define OS_ERR_MMU_THESAME_ADDR                 OS_ERROR(0x0000FF4F, OS_ERRMODULE_MMU)
/**< Accessing the same address on more than one bus */
#define OS_ERR_MMU_SEG_MISS                     OS_ERROR(0x0000FF5F, OS_ERRMODULE_MMU)
/**< Access to address that is not mapped in any MMU descriptor */
#define OS_ERR_MMU_EDC                          OS_ERROR(0x0000FF6F, OS_ERRMODULE_MMU)
/**< Access caused an EDC error */
#define OS_ERR_MMU_DSPE_ERR                     OS_ERROR(0x0000FF7F, OS_ERRMODULE_MMU)
/**< Data MMU programming error */
#define OS_ERR_MMU_PSPE_ERR                     OS_ERROR(0x0000FF8F, OS_ERRMODULE_MMU)
/**< Program MMU programming error */

/* sc39xx addition Hardware-detected MMU errors */
#define OS_ERR_MMU_DPV_ERR                      OS_ERROR(0x0000FF9F, OS_ERRMODULE_MMU)
/**< Data Permission Violation */
#define OS_ERR_MMU_DCCV_ERR                     OS_ERROR(0x0000FFAF, OS_ERRMODULE_MMU)
/**< Data Cache Command Violation */
#define OS_ERR_MMU_DAVW_ERR                     OS_ERROR(0x0000FFBF, OS_ERRMODULE_MMU)
/**< Data Access Violation Width */
#define OS_ERR_MMU_DAVD_ERR                     OS_ERROR(0x0000FFCF, OS_ERRMODULE_MMU)
/**< Data Access Violation Direction */
#define OS_ERR_MMU_DECC_ERR                     OS_ERROR(0x0000FFDF, OS_ERRMODULE_MMU)
/**< Data ECC Error */
#define OS_ERR_MMU_DRE_ERR                      OS_ERROR(0x0000FFEF, OS_ERRMODULE_MMU)
/**< Data Read Error */
#define OS_ERR_MMU_DNC_ERR                      OS_ERROR(0x0000F0FF, OS_ERRMODULE_MMU)
/**< NC hit */
#define OS_ERR_MMU_DNCC_ERR                     OS_ERROR(0x0000F1FF, OS_ERRMODULE_MMU)
/**< Noncacheable Cache Command */
#define OS_ERR_MMU_DSOV_ERR                     OS_ERROR(0x0000F2FF, OS_ERRMODULE_MMU)
/**< Stack Overrun Violation */
#define OS_ERR_MMU_DSAS_ERR                     OS_ERROR(0x0000F3FF, OS_ERRMODULE_MMU)
/**< Semaphore Access Size Error */
#define OS_ERR_MMU_DPAS_ERR                     OS_ERROR(0x0000F4FF, OS_ERRMODULE_MMU)
/**< Peripheral Access Size Error */
#define OS_ERR_MMU_PECC_ERR                     OS_ERROR(0x0000F5FF, OS_ERRMODULE_MMU)
/**< Program ECC */
#define OS_ERR_MMU_PFE_ERR                      OS_ERROR(0x0000F6FF, OS_ERRMODULE_MMU)
/**< Program Fetch Error */
#define OS_ERR_MMU_PNC_ERR                      OS_ERROR(0x0000F7FF, OS_ERRMODULE_MMU)
/**< NC hit */

/* @} */

/************************************************************//**
 @Collection    BIO related error codes
 
 @{
*//*************************************************************/
#define OS_EVENT_BIO_DEQUEUE                    OS_EVENT(OS_ERR_MODE_INVALID, OS_ERRMODULE_BIO)
/**< Error in dequeueing from BIO queue (frame or buffer) */

/* @} */

/************************************************************//**
 @Collection    COP related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_COP_QUEUE_SIZE_MISMATCH          OS_ERROR(OS_ERR_LENGTH_INVALID, OS_ERRMODULE_COP)
/**< Queue size not large enough for COP */

/* @} */

/************************************************************//**
 @Collection    L2 cache related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_L2_CACHE_INVALID_FUNCTION        OS_ERROR(OS_ERR_FUNCTION_INVALID, OS_ERRMODULE_L2_CACHE)
/**< L2 cache function is invalid */
#define OS_ERR_L2_CACHE_INVALID                 OS_ERROR(OS_ERR_HANDLE_INVALID, OS_ERRMODULE_L2_CACHE)
/**< L2 cache resource (partition/prefetch-channel) is invalid */
#define OS_ERR_L2_CACHE_UNAVAILABLE             OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_L2_CACHE)
/**< L2 cache resource (partition/prefetch-channel) is unavailable */
#define OS_ERR_L2_CACHE_ALREADY_CREATED         OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_L2_CACHE)
/**< L2 cache resource (partition/prefetch-channel) is already created */
#define OS_ERR_L2_CACHE_NOT_CREATED             OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_L2_CACHE)
/**< L2 cache resource (partition/prefetch-channel) not created yet */
#define OS_ERR_L2_CACHE_WRONG_ALIGNMENT         OS_ERROR(0x0000FFFF, OS_ERRMODULE_L2_CACHE)
/**< L2 cache alignment error */
#define OS_ERR_L2_CACHE_WRONG_SIZE              OS_ERROR(0x0000FFFE, OS_ERRMODULE_L2_CACHE)
/**< L2 cache size error */
#define OS_ERR_L2_CACHE_NOT_CACHEABLE           OS_ERROR(0x0000FFFD, OS_ERRMODULE_L2_CACHE)
/**< Attempt to perform cache operation on a non-cacheable memory region */
#define OS_ERR_L2_CACHE_PARTITION_FULL           OS_ERROR(0x0000FFFC, OS_ERRMODULE_L2_CACHE)
/**< Attempt to perform cache operation on a non-cacheable memory region */
#define OS_ERR_L2_CACHE_CME_LACK_OF_SPACE        OS_ERROR(0x0000FFFB, OS_ERRMODULE_L2_CACHE)
/**< Maintenance instruction failed insertion into CME channel due to lack of space */
#define OS_ERR_L2_CACHE_CME_DISABLE_IN_CONTROL_REGS        OS_ERROR(0x0000FFFA, OS_ERRMODULE_L2_CACHE)
/**< Maintenance instruction failed insertion into CME channel because CME is disabled by control registers */
#define OS_ERR_L2_CACHE_CME_INVALID_COMMAND        OS_ERROR(0x0000FFFA, OS_ERRMODULE_L2_CACHE)
/**< Maintenance instruction failed insertion into CME channel because it is invalid */
#define OS_ERR_L2_CACHE_CME_IS_BUSY        OS_ERROR(0x0000FFF9, OS_ERRMODULE_L2_CACHE)
/**< Maintenance instruction failed because CME is busy allocating channel to previous job */
#define OS_ERR_L2_CACHE_CME_INVALID_PARAMS        OS_ERROR(0x0000FFF8, OS_ERRMODULE_L2_CACHE)
/**< Maintenance instruction failed because CME is busy allocating channel to previous job */

/* @} */

/************************************************************//**
 @Collection     Debug and Trace Unit related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_DTU_TRIAD_COUNTERS_ARE_ALREADY_ALLOCATED  OS_ERROR(0x0000EFFF, OS_ERRMODULE_DTU)
/**< DTU triad counters are already allocated */

/* @} */

/************************************************************//**
 @Collection    I2C related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_I2C_INVALID_PARAM                OS_ERROR(0x0000EFFF, OS_ERRMODULE_I2C)
/**< I2C parameter is invalid */
#define OS_ERR_I2C_LOST_ARBITRATION             OS_ERROR(0x0000EFFE, OS_ERRMODULE_I2C)
/**< Lost arbitration on I2C bus */
#define OS_ERR_I2C_9TH_CLOCK_TIMEOUT            OS_ERROR(0x0000EFFD, OS_ERRMODULE_I2C)
/**< Timed out waiting for acknowledge on 9th clock cycle */
#define OS_ERR_I2C_BUS_STUCK                    OS_ERROR(0x0000EFFC, OS_ERRMODULE_I2C)
/**< I2C bus is stuck */
#define OS_ERR_I2C_UNAVAILABLE                  OS_ERROR(0x0000EFFB, OS_ERRMODULE_I2C)
/**< I2C module is unavailable */
#define OS_ERR_I2C_BUS_BUSY                     OS_ERROR(0x0000EFFA, OS_ERRMODULE_I2C)
/**< I2C bus is busy */

/* @} */

/************************************************************//**
 @Collection    Qman related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_QMAN_INVALID_PARAM               OS_ERROR(0x0000EFFF, OS_ERRMODULE_QMAN)
/**< I2C parameter is invalid */
#define OS_ERR_QMAN_INVALID_STATE               OS_ERROR(0x0000EFFE, OS_ERRMODULE_QMAN)
/**< The Frame queue is in invalid state */
#define OS_ERR_QMAN_QUEUE_IS_EMPTY              OS_ERROR(0x0000EFFD, OS_ERRMODULE_QMAN)
/**< The requested queue is empty cannot dequeue */
#define OS_ERR_QMAN_PORTAL_IS_BUSY              OS_ERROR(0x0000EFFC, OS_ERRMODULE_QMAN)
/**< The Qman portal is currently busy */
#define OS_ERR_QMAN_FQ_IS_BUSY             		OS_ERROR(0x0000EFFB, OS_ERRMODULE_QMAN)
/**< The Qman portal is currently busy */
/* @} */

/************************************************************//**
 @Collection    Watchdog related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_WDT_ALREADY_CREATED              OS_ERROR(OS_ERR_ALREADY_CREATED, OS_ERRMODULE_WDT)
/**< Watchdog timer is already created */
#define OS_ERR_WDT_NOT_CREATED                  OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_WDT)
/**< Watchdog timer is not created */
#define OS_ERR_WDT_INVALID_TIMEOUT              OS_ERROR(OS_ERR_PARAM_INVALID, OS_ERRMODULE_WDT)
/**< Watchdog timer timeout is invalid */
#define OS_ERR_WDT_INVALID_MODE                 OS_ERROR(OS_ERR_MODE_INVALID, OS_ERRMODULE_WDT)
/**< Watchdog timer mode is invalid */
#define OS_ERR_WDT_DISABLED                     OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_WDT)
/**< Watchdog timer is disabled */
#define OS_ERR_WDT_TIMEOUT_TOO_SMALL            OS_ERROR(0x0000EFFF, OS_ERRMODULE_WDT)
/**< Watchdog timer timeout is too short */

/* @} */

/************************************************************//**
 @Collection    SPI related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_SPI_INVALID_MODE                 OS_ERROR(OS_ERR_MODE_INVALID, OS_ERRMODULE_SPI)
/**< SPI mode is invalid */
#define OS_ERR_SPI_NO_MEMORY                    OS_ERROR(OS_ERR_NO_MEMORY, OS_ERRMODULE_SPI)
/**< No memory for SPI */
#define OS_ERR_SPI_UNAVAILABLE                  OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_SPI)
/**< SPI module is unavailable */
#define OS_ERR_SPI_INVALID_PARAM                OS_ERROR(OS_ERR_PARAM_INVALID, OS_ERRMODULE_SPI)
/**< SPI parameter is invalid */
#define OS_ERR_SPI_TX_BD_BUSY                   OS_ERROR(0x0000EFFF, OS_ERRMODULE_SPI)
/**< SPI transmit BD is busy */
#define OS_ERR_SPI_TX_ERROR                     OS_ERROR(0x0000EFFE, OS_ERRMODULE_SPI)
/**< SPI transmit error */
#define OS_ERR_SPI_RX_BUSY                      OS_ERROR(OS_ERR_BUSY, OS_ERRMODULE_SPI)
/**< SPI receive BD is busy */
#define OS_ERR_SPI_MULTI_MASTER_ERROR           OS_ERROR(0x0000EFFD, OS_ERRMODULE_SPI)
/**< SPI multiple master error */

/* @} */

/************************************************************//**
 @Collection    Ethernet related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_ETHERNET_CHANNEL_UNAVAILABLE     OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_ETHERNET)
/**< Ethernet channel is unavailable */

/* @} */

/************************************************************//**
 @Collection    HDLC related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_HDLC_CHANNEL_UNAVAILABLE         OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_HDLC)
/**< HDLC channel is unavailable */

/* @} */

/************************************************************//**
 @Collection    CLASS related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_CLASS_UNAVAILABLE                OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_CLASS)
/**< CLASS module is unavailable */
#define OS_ERR_CLASS_INVALID_CLASS              OS_ERROR(OS_ERR_DEST_INVALID, OS_ERRMODULE_CLASS)
/**< CLASS module is invalid */
#define OS_ERR_CLASS_BAD_CONFIG                 OS_ERROR(0x0000EFFF, OS_ERRMODULE_CLASS)
/**< Attempt to configure erroneous CLASS configuration */

/* @} */

/************************************************************//**
 @Collection    Performance Monitor related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_PERF_MON_BAD_CONFIG              OS_ERROR(0x0000EFFF, OS_ERRMODULE_PERF_MON)
/**< Attempt to configure erroneous performance monitor configuration */

/* @} */

/************************************************************//**
 @Collection    MAPLE related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_MAPLE_CHANNEL_UNAVAILABLE        OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_MAPLE)
/**< MAPLE channel is unavailable */
#define OS_ERR_MAPLE_NO_ROOM                    OS_ERROR(OS_ERR_FULL, OS_ERRMODULE_MAPLE)
/**< No room on MAPLE BD ring for all requested BD */
#define OS_ERR_MAPLE_NO_MEM                     OS_ERROR(OS_ERR_NO_MEMORY, OS_ERRMODULE_MAPLE)
/**< No memory available in MAPLE Parameter RAM */
#define OS_ERR_MAPLE_TOO_MANY_BD                OS_ERROR(OS_ERR_TOO_MUCH, OS_ERRMODULE_MAPLE)
/**< Attempt to send more BD that available on BD ring */

/* @} */

/************************************************************//**
 @Collection    LLD related error codes
 
 @{
*//*************************************************************/
#define OS_SUCESS_LLD_TX                        OS_ERROR(0x0000EFFF, OS_ERRMODULE_LLD)
/**< LLD successfully transmitted the data - used internally */
#define OS_EVENT_LLD_EVENT                      OS_EVENT(0x00000FFF, OS_ERRMODULE_LLD)
/**< LLD general event */

/* @} */


/************************************************************//**
 @Collection    Virtual interrupts related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_VIRQ_UNAVAILABLE                 OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_VIRQ)
/**< No virtual interrupt available */
#define OS_ERR_VIRQ_ALREADY_FREE                OS_ERROR(OS_ERR_ALREADY_FREE, OS_ERRMODULE_VIRQ)
/**< Attempt to free an already freed virtual interrupt resource */

/* @} */

/************************************************************//**
 @Collection    Debug Print related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_DEBUG_PRINT_UNAVAILABLE          OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_DEBUG_PRINT);
/**< Debug print module unavailable */
#define OS_ERR_DEBUG_PRINT_NOT_CREATED          OS_ERROR(OS_ERR_NOT_CREATED, OS_ERRMODULE_DEBUG_PRINT);
/**< Debug print resource not created */
#define OS_ERR_DEBUG_PRINT_NOT_ACTIVE           OS_ERROR(OS_ERR_NOT_ACTIVE, OS_ERRMODULE_DEBUG_PRINT);
/**< Debug print resource not active */
#define OS_ERR_DEBUG_PRINT_LENGTH_INVALID       OS_ERROR(OS_ERR_LENGTH_INVALID, OS_ERRMODULE_DEBUG_PRINT)
/**< Debug print length is erroneous */
#define OS_ERR_DEBUG_PRINT_MDP_ACTIVE           OS_ERROR(OS_ERR_BUSY, OS_ERRMODULE_DEBUG_PRINT)
/**< Attempt to perform debug print activity while mass debug print (mdp) is in progress */
#define OS_ERR_DEBUG_PRINT_OVERFLOW             OS_ERROR(OS_ERR_OVERFLOW, OS_ERRMODULE_DEBUG_PRINT)
/**< Debug print overflow */

/* @} */

/************************************************************//**
 @Collection    RapidIO related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_RIO_NO_SYNC                      OS_ERROR(0x0000EFFF, OS_ERRMODULE_SRIO)
/**< No synchronization on RapidIO bus */
#define OS_ERR_RIO_PORT_STOPPED                 OS_ERROR(0x0000EFFE, OS_ERRMODULE_SRIO)
/**< RapidIO port is stopped */
#define OS_ERR_RIO_LINK_RESPONSE_TIME_OUT       OS_ERROR(0x0000EFFD, OS_ERRMODULE_SRIO)
/**< RapidIO link timed out */
#define OS_ERR_RIO_LINK_UNRECOVERABLE_ERROR     OS_ERROR(0x0000EFFC, OS_ERRMODULE_SRIO)
/**< Unrecoverable error detected by RapidIO controller */

/* @} */


/************************************************************//**
 @Collection    eMSG related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_EMSG_CHANNEL_UNAVAILABLE         OS_ERROR(OS_ERR_UNAVAILABLE,  OS_ERRMODULE_EMSG)
/**< eMSG channel is unavailable */

/* @} */

/************************************************************//**
 @Collection    QML related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_QML_QUEUE_FULL                   OS_ERROR(OS_ERR_FULL,  OS_ERRMODULE_QML)
/**< QMan-Lite queue is full */
#define OS_ERR_QML_QUEUE_EMPTY                  OS_ERROR(OS_ERR_EMPTY, OS_ERRMODULE_QML)
/**< QMan-Lite queue is empty */
#define OS_ERR_QML_CHANNEL_UNAVAILABLE          OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_QML)
/**< QMan-Lite channel is unavailable */

/* @} */

/************************************************************//**
 @Collection    BML related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_BML_CHANNEL_UNAVAILABLE          OS_ERROR(OS_ERR_UNAVAILABLE, OS_ERRMODULE_BML)
/**< BMan-Lite channel is unavailable */
#define OS_ERR_BML_RING_FULL                    OS_ERROR(OS_ERR_FULL, OS_ERRMODULE_BML)
/**< BMan-Lite ring is full */
#define OS_ERR_BML_RING_EMPTY                   OS_ERROR(OS_ERR_EMPTY, OS_ERRMODULE_BML)
/**< BMan-Lite ring is empty */

/* @} */

/************************************************************//**
 @Collection    HSSI related error codes
 
 @{
*//*************************************************************/
#define OS_ERR_HSSI_NO_SYNC                      OS_ERROR(0x0000EFFF, OS_ERRMODULE_HSSI)
/**< No synchronization on HSSI */

/* @} */

#define OS_EVENT_BUSY                           OS_EVENT(OS_ERR_BUSY, OS_ERRMODULE_GENERAL)
/**< General busy event */


#ifndef OS_DOCUMENTATION_ONLY
#ifndef OS_TEST_PASSED
  #define OS_TEST_PASSED                         osTestDebug
#endif  // OS_TEST_PASSED
#endif  // OS_DOCUMENTATION_ONLY

/** @} */ // end of error_id


_OS_END_EXTERN_C

#endif // __OS_ERROR_H

