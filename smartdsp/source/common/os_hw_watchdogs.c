/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_hw_watchdogs.c,v 1.14 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_hw_watchdogs.c,v $
 $Revision: 1.14 $
******************************************************************************/

/******************************************************************************
 
 @File          os_hw_watchdogs.c

 @Description   OS Hardware watchdog timer functions.

 @Cautions      None.
 
*//***************************************************************************/
#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "os_rm_.h"
#include "os_hw_watchdogs.h"
#include "os_hw_watchdogs_.h"

/* Globals: */
os_swi_handle   watchdog_swi;
os_timer_handle watchdog_timer;

/*****************************************************************************/

void osHwWatchdogServiceHandler(os_swi_count count)
{
    VAR_UNUSED(count);
    /* service the watchdog */
    osHwWatchdogService((os_watchdog_handle)osGetCoreID());
}

/*****************************************************************************/

/* Watchdog timer handler - activates the watchdog SWI */
void osHwWatchdogTimerHandler(void)
{
    osSwiActivate(watchdog_swi);
}

/*****************************************************************************/

os_status osHwWatchdogServiceInitialize(os_swi_priority priority)
{
    os_status   status = OS_FAIL;
 
    if (priority > OS_SWI_PRIORITY_ALL)
        RETURN_ERROR(OS_ERR_SWI_PRIORITY_INVALID);
 
    status = osSwiFind(&watchdog_swi);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    /* Create a WDT servicing SWI */
    status = osSwiCreate(osHwWatchdogServiceHandler, watchdog_swi, priority, 0);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    /* Define a software timer. The timer will activate */
    /* the watchdog SWI in its assigned handler.        */
    /* Timer is periodic, running every 1 OS ticks. */
    osTimerFind(&watchdog_timer);
    status = osTimerCreate(watchdog_timer, OS_TIMER_PERIODIC, 1, osHwWatchdogTimerHandler);
    status = osTimerStart(watchdog_timer);
    OS_ASSERT_COND(status == OS_SUCCESS);
 
    return status;
}

/*****************************************************************************/

os_status osHwWatchdogSupport(  os_watchdog_handle watchdog_id,
                                uint32_t timer_count,
                                watchdog_action_mode mode,
                                os_swi_priority priority,
                                os_hwi_function nmi_handler,
                                os_hwi_arg argument,
                                uint32_t tick_parameter)
{
    os_status   status = OS_FAIL;
 
    if (osHwWatchdogTimeoutValueCheck(timer_count, tick_parameter) != OS_SUCCESS)
        RETURN_ERROR(OS_ERR_WDT_TIMEOUT_TOO_SMALL);
 
    status = osHwWatchdogCreate(watchdog_id, timer_count, mode, nmi_handler, argument);
    if (status != OS_SUCCESS)
        return status;
    status = osHwWatchdogServiceInitialize(priority);
    if (status != OS_SUCCESS)
        return status;
    status = osHwWatchdogEnable(watchdog_id);
 
    return status;
}

/*****************************************************************************/

