/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/01/08 15:31:20 $
 $Id: hw_watchdogs_init.c,v 1.3 2012/01/08 15:31:20 b08551 Exp $
 $Source: /cvsdata/SmartDSP/drivers/wdt/rev1/hw_watchdogs_init.c,v $
 $Revision: 1.3 $
******************************************************************************/

/**************************************************************************//**
 
 @File          hw_watchdogs_init.c

 @Description   Software watchdog timer initialization function.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"
#include "os_rm_.h"
#include "os_hw_watchdogs.h"
#include "os_hw_watchdogs_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"

/*****************************************************************************/

os_status osHwWatchdogsInit(void)
{
    os_watchdog_handle   wdt = 0;
 
    if (osGetCoreID() == osGetMasterCore())
    {
 
        watchdogs = (os_watchdog_t *)
#if (OS_MULTICORE == ON)
            osMalloc((sizeof(os_watchdog_t) * NUM_OF_HW_WATCHDOGS), OS_MEM_SHARED);
#else
            osMalloc((sizeof(os_watchdog_t) * NUM_OF_HW_WATCHDOGS), OS_MEM_LOCAL);
#endif

        if (watchdogs == NULL)
        {
            RETURN_ERROR(OS_ERR_NO_MEMORY);
        }
 
        for (wdt = 0; wdt < NUM_OF_HW_WATCHDOGS; wdt++)
        {
            watchdogs[wdt].status = OS_RESOURCE_UNUSED;
            watchdogs[wdt].timer_count = 0;
            watchdogs[wdt].mode = GENERATE_RESET;
            watchdogs[wdt].nmi_handler = NULL;
            watchdogs[wdt].argument = 0;
        }
    }
    /* All the other cores must wait until the master core finishes the initialization of the data structure */
    osWaitForAllCores();
 
    return OS_SUCCESS;
}

/*****************************************************************************/


