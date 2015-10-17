/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/05/26 18:07:52 $
 $Id: os_hw_watchdogs.h,v 1.19 2011/05/26 18:07:52 b17975 Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_hw_watchdogs.h,v $
 $Revision: 1.19 $
******************************************************************************/

/**************************************************************************//**
 @File          os_hw_watchdogs.h

 @Description   OS Hardware watchdog timer header file.

 @Cautions      None.
*//***************************************************************************/

#ifndef __OS_HW_WATCHDOGS_H
#define __OS_HW_WATCHDOGS_H

#include "os_cpp.h"
_OS_BEGIN_EXTERN_C

/**************************************************************************//**
 @Group         wdt_id   Hardware Watchdogs API

 @Description   Hardware Watchdogs setup and control.
 
 @{
*//***************************************************************************/

/*************************************************************//**
 @Collection    OS HW Watchdog identifiers
 @{
*//**************************************************************/
#define OS_HW_WATCHDOG0        0
#define OS_HW_WATCHDOG1        1
#define OS_HW_WATCHDOG2        2
#define OS_HW_WATCHDOG3        3
#define OS_HW_WATCHDOG4        4
/* @} */

/**************************************************************************//**
 @Description   Watchdog action modes
*//***************************************************************************/
typedef enum {GENERATE_RESET = 0, GENERATE_NMI = 1} watchdog_action_mode;

/*************************************************************//**
 @Collection    Hardware Watchdogs Handling Modes
 @{
*//**************************************************************/
#define OS_WDT_AUTO_HANDLING    0
#define OS_WDT_USER_HANDLING    1
#define OS_WDT_DISABLED         2
/* @} */

#define DISABLED                OS_WDT_DISABLED
#define AUTO_OS_HANDLING        OS_WDT_AUTO_HANDLING
#define MANUAL_USER_HANDLING    OS_WDT_USER_HANDLING

/**************************************************************************//**
 @Function      osHwWatchdogSupport

 @Description

 @Param         watchdog_id - The identifier for the hardware watchdog to be created.
 @Param         timer_count - The timeout period of the watchdog, in system clocks.
 @Param         priority - The SW Interrupt priority of the watchdog service handler.
 @Param         mode  - Watchdog operation mode. Permitted values:
                            - GENERATE_RESET - The watchdog generates reset on timeout.
                            - GENERATE_NMI - The watchdog generates NMI on timeout.
 @Param         nmi_handler - optional interrupt function (ISR), relevant only if mode is GENERATE_NMI.
                if mode is GENERATE_RESET or if there is no specific handler, this parameter must be NULL.
 @Param         argument - optional argument to be passed to the interrupt handler function.
 @Param         tick_parameter -  number ticks in a second
 
 @Retval        OS_SUCCESS - the watchdog SWI and timer were initialized successfully.
 @Retval        OS_FAIL - the OS failed to initialize the watchdog SWI and timer.
*//***************************************************************************/

os_status osHwWatchdogSupport(  os_watchdog_handle watchdog_id,
                                uint32_t timer_count,
                                watchdog_action_mode mode,
                                os_swi_priority priority,
                                os_hwi_function nmi_handler,
                                os_hwi_arg argument,
                                uint32_t tick_parameter);

/**************************************************************************//**
 @Function      osHwWatchdogCreate

 @Description   Creates a hardware watchdog and sets its parameters.
                The watchdog is not enabled until osHwWatchdogEnable() is called.

 @Param         watchdog_id  - The identifier for the hardware watchdog to be created.
 @Param         timer_count  - The timeout period of the watchdog, in system clocks.
 @Param         mode - Watchdog operation mode. Permitted values:
                            - GENERATE_RESET - The watchdog generates reset on timeout;
                            - GENERATE_NMI - The watchdog generates NMI on timeout;
 @Param         nmi_handler - optional interrupt function (ISR), relevant only if mode is GENERATE_NMI;
                              if mode is GENERATE_RESET or if there is no specific handler, this parameter must be NULL.
 @Param         argument - optional argument to be passed to the interrupt handler function.
 @Retval        OS_SUCCESS - The watchdog was created successfully.
 @Retval        OS_ERR_WDT_ALREADY_CREATED - The specified watchdog has already been created.
 @Retval        OS_ERR_WDT_INVALID_TIMEOUT - The specified timeout value is not valid.
*//***************************************************************************/
os_status osHwWatchdogCreate(   os_watchdog_handle watchdog_id,
                                uint32_t timer_count,
                                watchdog_action_mode mode,
                                os_hwi_function nmi_handler,
                                os_hwi_arg argument);
/**************************************************************************//**
 @Function      osHwWatchdogEnable

 @Description   Enables the watchdog.
                This function should be used on a watchdog that was created by a call to osHwWatchdogCreate().

 @Param[in]     watchdog_id - The identifier for the hardware watchdog to be enabled.

 @Retval        OS_SUCCESS - The watchdog was enabled successfully.
 @Retval        OS_ERR_WDT_NOT_CREATED - The specified watchdog has not been created.

*//***************************************************************************/
os_status osHwWatchdogEnable(os_watchdog_handle watchdog_id);

/**************************************************************************//**
 @Function      osHwWatchdogDisable

 @Description   Disables the watchdog.
 
 @Param[in]     watchdog_id - The identifier for the hardware watchdog to be disabled.

*//***************************************************************************/
void osHwWatchdogDisable(os_watchdog_handle watchdog_id);

/**************************************************************************//**
 @Function      osHwWatchdogService
 
 @Description   Services the watchdog.
 
 @Param[in]     watchdog_id  - The identifier for the hardware watchdog to be serviced.
 
*//***************************************************************************/
void osHwWatchdogService(os_watchdog_handle watchdog_id);

/**************************************************************************//**
 @Function      osHwWatchdogCounterGet
 
 @Description   Retrieves the remaining time for the specified watchdog.

 @Param[in]     watchdog_id - The identifier for the hardware watchdog
 
 @Return        The remaining time for the specified watchdog.

*//***************************************************************************/
uint32_t osHwWatchdogCounterGet(os_watchdog_handle watchdog_id);

/** @} */ // end of Watchdogs group

_OS_END_EXTERN_C

#endif // __OS_HW_WATCHDOGS_H
