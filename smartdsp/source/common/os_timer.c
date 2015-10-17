/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2014/06/19 10:20:09 $
 $Id: os_timer.c,v 1.18 2014/06/19 10:20:09 b41932 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_timer.c,v $
 $Revision: 1.18 $
******************************************************************************/

/******************************************************************************
 
 @File          os_timer.c

 @Description   OS Timer module runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_timer_.h"
#include "os_rm_.h"


os_timer_t          g_os_timers;
volatile uint16_t   g_max_os_timers;
volatile uint16_t   g_cur_timer;


/*****************************************************************************/
os_status osTimerCreate(os_timer_handle     timer_num,
                        os_timer_mode       mode,
                        os_timer_interval   interval,
                        os_timer_function   handler)
{
#ifdef TIMER_ERROR_CHECKING
    if (timer_num >= g_max_os_timers)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_INVALID);
    }
    if ((mode < OS_TIMER_ONE_SHOT) || (mode > OS_TIMER_FREE_RUN))
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_MODE_INVALID);
    }
    if (handler == NULL)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_FUNCTION_INVALID);
    }
#endif /* TIMER_ERROR_CHECKING */
 
    osHwiSwiftDisable();
 
#ifdef TIMER_ERROR_CHECKING
    if (g_os_timers.list[timer_num].status >= OS_RESOURCE_USED)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TM_ALREADY_CREATED);
    }
#endif /* TIMER_ERROR_CHECKING */
 
    g_os_timers.list[timer_num].status          = OS_RESOURCE_USED;
    g_os_timers.list[timer_num].timer           = 0;
    g_os_timers.list[timer_num].timer_config    = mode;
    g_os_timers.list[timer_num].reload_value    = interval;
    g_os_timers.list[timer_num].timer_function  = handler;
 
    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osTimerDelete(os_timer_handle timer_num)
{
#ifdef TIMER_ERROR_CHECKING
    if (timer_num >= g_max_os_timers)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_INVALID);
    }
#endif /* TIMER_ERROR_CHECKING */
 
    osHwiSwiftDisable();
 
#ifdef TIMER_ERROR_CHECKING
    if (g_os_timers.list[timer_num].status < OS_RESOURCE_USED)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        osHwiSwiftEnable();
        RETURN_ERROR(OS_ERR_TM_NOT_CREATED);
    }
#endif /* TIMER_ERROR_CHECKING */
 
    g_os_timers.list[timer_num].status = OS_RESOURCE_DELETED;
    g_os_timers.list[timer_num].timer = 0;
    g_os_timers.list[timer_num].timer_config = 0;
 
    osHwiSwiftEnable();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osTimerStart(os_timer_handle timer_num)
{
#ifdef TIMER_ERROR_CHECKING
    if (timer_num >= g_max_os_timers)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_INVALID);
    }
    if (g_os_timers.list[timer_num].status < OS_RESOURCE_USED)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_NOT_CREATED);
    }
#endif /* TIMER_ERROR_CHECKING */

    g_os_timers.list[timer_num].timer = g_os_timers.list[timer_num].reload_value;

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osTimerStartDelayed(os_timer_handle timer_num,os_timer_interval delay)
{
#ifdef TIMER_ERROR_CHECKING
    if (timer_num >= g_max_os_timers)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_INVALID);
    }
    if (g_os_timers.list[timer_num].status < OS_RESOURCE_USED)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_NOT_CREATED);
    }
#endif /* TIMER_ERROR_CHECKING */

    g_os_timers.list[timer_num].timer = delay + g_os_timers.list[timer_num].reload_value;

    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osTimerStop(os_timer_handle timer_num)
{
#ifdef TIMER_ERROR_CHECKING
    if (timer_num >= g_max_os_timers)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_INVALID);
    }
    if (g_os_timers.list[timer_num].status < OS_RESOURCE_USED)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_NOT_CREATED);
    }
#endif /* TIMER_ERROR_CHECKING */
 
    osHwiSwiftDisable();
 
    g_os_timers.list[timer_num].timer = 0;
    /* clear the RUN_TIMER_FUNCTION flag */
    g_os_timers.list[timer_num].timer_config &= ~RUN_TIMER_FUNCTION;
 
    osHwiSwiftEnable();

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osTimerSetInterval(os_timer_handle timer_num, os_timer_interval interval)
{
#ifdef TIMER_ERROR_CHECKING
    if (timer_num >= g_max_os_timers)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_INVALID);
    }
    if (g_os_timers.list[timer_num].status < OS_RESOURCE_USED)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_NOT_CREATED);
    }
#endif /* TIMER_ERROR_CHECKING */
 
    /* set the new reload value */
    g_os_timers.list[timer_num].reload_value = interval;
 
    return OS_SUCCESS;
}

/*****************************************************************************/
os_status osTimerGetDelay(os_timer_handle timer_num,os_timer_interval* delay)
{
#ifdef TIMER_ERROR_CHECKING
    if (timer_num >= g_max_os_timers)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_INVALID);
    }
    if (g_os_timers.list[timer_num].status < OS_RESOURCE_USED)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_NOT_CREATED);
    }
#endif /* TIMER_ERROR_CHECKING */

    *delay = g_os_timers.list[timer_num].timer;

    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osTimerFind(os_timer_handle *timer_num)
{
#ifdef TIMER_DYNAMIC_CONTROL
    uint16_t    i;
 
    OS_ASSERT_COMPILER_COND(g_max_os_timers > 0);

    for (i=0; i < g_max_os_timers; i++)
    {
        osHwiSwiftDisable();
 
        if (g_os_timers.list[i].status < OS_RESOURCE_ACQUIRED)
        {
            *timer_num = i;
            g_os_timers.list[i].status = OS_RESOURCE_ACQUIRED;
            osHwiSwiftEnable();
            return OS_SUCCESS;
        }
 
        osHwiSwiftEnable();
    }
#endif /* TIMER_DYNAMIC_CONTROL */

    *timer_num = 0xFFFF;

    RETURN_ERROR(OS_ERR_TM_UNAVAILABLE);
}


/*****************************************************************************/
bool osTimerDecrement()
{
    bool        timer_expired = FALSE;
    uint16_t    i;
 
    OS_ASSERT_COMPILER_COND(g_max_os_timers > 0);

    for (i=0; i < g_max_os_timers; i++)
    {
        if (g_os_timers.list[i].timer != 0)
        {
            g_os_timers.list[i].timer--;
 
            if (g_os_timers.list[i].timer == 0)
            {
                /* set RUN_TIMER_FUNCTION flag */
                g_os_timers.list[i].timer_config |= RUN_TIMER_FUNCTION;
                timer_expired = TRUE;
 
                if ((g_os_timers.list[i].timer_config & 0x00FF) == OS_TIMER_PERIODIC)
                {
                    /* if the timer is periodic, reload its value */
                    g_os_timers.list[i].timer = g_os_timers.list[i].reload_value;
                }
            }
        }
    }
 
    return timer_expired;
}


/*****************************************************************************/
void osTimerDispatcher()
{
    uint16_t i;
 
    /* loop through all timers, checking the RUN_TIMER_FUNCTION flag */
    OS_ASSERT_COMPILER_COND(g_max_os_timers > 0);

    for (i=0; i < g_max_os_timers; i++)
    {
        if (g_os_timers.list[i].timer_config & RUN_TIMER_FUNCTION)
        {
            /* clear the RUN_TIMER_FUNCTION flag */
            g_os_timers.list[i].timer_config &= ~RUN_TIMER_FUNCTION;

            g_cur_timer = i;
            /* call the timer function */
            (*g_os_timers.list[i].timer_function)();
            g_cur_timer = 0xff;
        }
    }
}


/*****************************************************************************/
os_status osTimerSelf(os_timer_handle *number)
{
    *number = g_cur_timer;

#ifdef TIMER_ERROR_CHECKING
    if (*number == 0x00ff)
    {
#ifdef TIMER_ERROR_ASSERT
        OS_ASSERT;
#endif /* TIMER_ERROR_ASSERT */
        RETURN_ERROR(OS_ERR_TM_NOT_ACTIVE);
    }
#endif /* TIMER_ERROR_CHECKING */

    return OS_SUCCESS;
}


/*****************************************************************************/
void osTimerNullHandler(void)
{
    return;
}


