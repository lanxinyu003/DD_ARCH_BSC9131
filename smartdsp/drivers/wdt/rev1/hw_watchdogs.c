/******************************************************************************
 Copyright �1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:24 $
 $Id: hw_watchdogs.c,v 1.8 2012/11/22 16:28:24 sw Exp $
 $Source: /cvsdata/SmartDSP/drivers/wdt/rev1/hw_watchdogs.c,v $
 $Revision: 1.8 $
******************************************************************************/


/******************************************************************************
 
 @File          hw_watchdog.c

 @Description   Software watchdog timer functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtimecritical.h"
#include "os_rm_.h"
#include "os_hw_watchdogs.h"
#include "os_hw_watchdogs_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"

#ifndef WATCHDOGS_NO_RESET
#define WATCHDOGS_NO_RESET FALSE
#endif

/*****************************************************************************/

#define WATCHDOG_SERVICE_PART_1     0x556C
#define WATCHDOG_SERVICE_PART_2     0xAA39

#define WATCHDOG_PRE_SCALE                  8192

/*****************************************************************************/

void osHwWatchdogService(os_watchdog_handle watchdog_id)
{
    /* service the watchdog */
    WRITE_UINT16((g_dsp_ccsr_map->wdt[watchdog_id]).swsrr, WATCHDOG_SERVICE_PART_1);
    WRITE_SYNCIO_UINT16((g_dsp_ccsr_map->wdt[watchdog_id]).swsrr, WATCHDOG_SERVICE_PART_2);
}

/*****************************************************************************/

uint32_t osHwWatchdogCounterGet(os_watchdog_handle watchdog_id)
{
    return (GET_UINT32((g_dsp_ccsr_map->wdt[watchdog_id]).swcnr) & 0x0000FFFF);
}

/*****************************************************************************/

#if 0
static void _osHwWatchdogIsr(os_watchdog_handle watchdog_id)
{
    uint32_t wdt_source;
 
    OS_ASSERT_COND(watchdogs[watchdog_id].status == OS_RESOURCE_USED);
    OS_ASSERT_COND(watchdogs[watchdog_id].mode   == GENERATE_NMI);
    OS_ASSERT_COND(watchdogs[watchdog_id].nmi_handler != NULL);

    /* calculate watchdog NMI source */
    wdt_source = (uint32_t)socHwWdtNmiSource();
 
    /* call user function */
    watchdogs[watchdog_id].nmi_handler(wdt_source);
}
#endif
/***********************************************************************//**
                To API documentaion of osHwWatchdogCreate()
                timer_count (in) - The timeout period of the watchdog, in system clocks.
                Permitted values: a value between 0 and 0xFFFF.
*//*************************************************************************/
os_status osHwWatchdogCreate(   os_watchdog_handle watchdog_id,
                                uint32_t timer_count,
                                watchdog_action_mode mode,
                                os_hwi_function nmi_handler,
                                os_hwi_arg argument)
{
    os_status   status = OS_SUCCESS;
 
    VAR_UNUSED(argument);
 
    if (watchdogs == NULL)
        RETURN_ERROR(OS_ERR_WDT_DISABLED);

#if (WATCHDOGS_NO_RESET == TRUE)
    if (mode == GENERATE_RESET)
        RETURN_ERROR(OS_ERR_WDT_INVALID_MODE);
#endif

    osSpinLockSwiftIrqGet(&shared_watchdogs_guard);
 
    if (watchdogs[watchdog_id].status == OS_RESOURCE_USED)
        RETURN_ERROR(OS_ERR_WDT_ALREADY_CREATED);
    if (timer_count > 0xFFFF)
        RETURN_ERROR(OS_ERR_WDT_INVALID_TIMEOUT);
    watchdogs[watchdog_id].status = OS_RESOURCE_USED;
    watchdogs[watchdog_id].timer_count = timer_count;
    watchdogs[watchdog_id].mode = mode;
    watchdogs[watchdog_id].nmi_handler = nmi_handler;
 
    osSpinLockSwiftIrqRelease(&shared_watchdogs_guard);
 
    if (nmi_handler != NULL)
    {
        /* no point in setting mode to reset and specifying an nmi handler */
        if (mode == GENERATE_RESET)
            RETURN_ERROR(OS_ERR_WDT_INVALID_MODE);
 
        /*status = osHwiCreate(OS_INT_ORED_WDT,
                    OS_HWI_PRIORITY_NMI,
                    LEVEL_MODE,
                    _osHwWatchdogIsr,
                    watchdog_id);*/
        status = OS_SUCCESS;
 
    }
    return status;
}

/*****************************************************************************/

os_status osHwWatchdogEnable(os_watchdog_handle watchdog_id)
{
    uint32_t    value = 0;
 
    if (watchdogs == NULL)
        RETURN_ERROR(OS_ERR_WDT_DISABLED);
 
    osSpinLockSwiftIrqGet(&shared_watchdogs_guard);
 
    if (watchdogs[watchdog_id].status != OS_RESOURCE_USED)
        RETURN_ERROR(OS_ERR_WDT_NOT_CREATED);
 
    value = ((watchdogs[watchdog_id].timer_count << 16) & 0xFFFF0000);
    if (watchdogs[watchdog_id].mode == GENERATE_RESET)
        value |= 0x00000002;
 
    osSpinLockSwiftIrqRelease(&shared_watchdogs_guard);

    /* Enable watchdog */
    value |= 0x05;
    WRITE_UINT32((g_dsp_ccsr_map->wdt[watchdog_id]).swcrr, value);

    /* init counter to predefined value */
    osHwWatchdogService(watchdog_id);
 
    return OS_SUCCESS;
}

/*****************************************************************************/

void osHwWatchdogDisable(os_watchdog_handle watchdog_id)
{
    uint32_t    disable_value = 0xFFFF0003; /* set bit SWEN (bit no.29) to 0 */
    /* Disable the watchdog. Note that this register can be written only once after reset */
    WRITE_UINT32((g_dsp_ccsr_map->wdt[watchdog_id]).swcrr, disable_value);
}

/*****************************************************************************/

os_status osHwWatchdogTimeoutValueCheck(uint32_t timer_count, uint32_t tick_parameter)
{
    uint32_t num_of_ticks_per_second = tick_parameter;
 
    /* we want to check that the number of system ticks per second is larger than             */
    /* the number of watchdog timeouts per second. The number of watchdog timeouts per second */
    /* is (SysClock*10^6)/((timer_count+1)*prescaler). we avoid using division.               */
    if ((num_of_ticks_per_second * timer_count * WATCHDOG_PRE_SCALE) >
        ((osSlowPeriphClockGet()+10) * 1000000) )
        return OS_SUCCESS;
    RETURN_ERROR(OS_FAIL);
}

/*****************************************************************************/
