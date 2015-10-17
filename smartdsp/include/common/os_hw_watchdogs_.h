/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:25 $
 $Id: os_hw_watchdogs_.h,v 1.10 2012/11/22 16:28:25 sw Exp $
 $Source: /cvsdata/SmartDSP/include/common/os_hw_watchdogs_.h,v $
 $Revision: 1.10 $
******************************************************************************/

/******************************************************************************
 
 @File          os_hw_watchdogs_.h

 @Description   OS Hardware watchdog timer internal header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_HW_WATCHDOGS_H_
#define __OS_HW_WATCHDOGS_H_

#include "os_hw_watchdogs.h"
/******************************************************************************
 OS WATCHDOG STRUCTURE
******************************************************************************/
typedef struct os_watchdog_t
{
    uint16_t                status;
    uint32_t                timer_count;
    watchdog_action_mode    mode;
    os_hwi_function         nmi_handler;
    os_hwi_arg              argument;
} os_watchdog_t;

#ifdef _shared_data
#define os_shared
#else
#define os_shared extern
#endif

os_shared volatile os_watchdog_t    *watchdogs;
os_shared os_spinlock_handle        shared_watchdogs_guard;


void osHwWatchdogTimerHandler(void);

os_status osHwWatchdogServiceInitialize(os_swi_priority priority);

void osHwWatchdogServiceHandler(os_swi_count count);

os_status osHwWatchdogTimeoutValueCheck(uint32_t timer_count, uint32_t tick_parameter);

uint32_t socHwWdtNmiSource();

#endif // __OS_HW_WATCHDOGS_H_
