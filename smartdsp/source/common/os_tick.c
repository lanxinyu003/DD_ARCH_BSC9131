/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2013/07/08 07:01:20 $
 $Id: os_tick.c,v 1.15 2013/07/08 07:01:20 b42006 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_tick.c,v $
 $Revision: 1.15 $
******************************************************************************/

/******************************************************************************
 
 @File          os_tick.c

 @Description   OS Tick module runtime functions.
 
                This file also contains functions for core-load calculations.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"

#include "os_.h"
#include "os_tick_.h"
#include "os_timer_.h"
#include "os_list_.h"
#include "os_task_.h"

extern volatile unsigned long long g_os_ticks;
extern list_object_t       g_sorted_expiration_list;


/*****************************************************************************/
void osTickDispatcher()
{
    os_tcb_t *expired_task;
    bool need_scheduling = FALSE, found;

    osHwiSwiftDisable();

    do
    {
        found = FALSE;

        LIST_FOR_EACH(expired_task, &g_sorted_expiration_list, os_tcb_t, timer_list)
        {
            if(expired_task->expiration_tick <= g_os_ticks)
            {
                list_delete(&expired_task->timer_list);
 
                expired_task->task_status &= ~(OS_TASK_TIMEOUT);
                expired_task->task_status |= OS_TASK_EXPIRED;
 
                if(expired_task->task_status & OS_TASK_PEND)
                {
                    expired_task->task_status &= ~(OS_TASK_PEND);
                    list_delete(&expired_task->pend_list);
                    expired_task->task_event = NULL;
                }
                else
                {
                    expired_task->task_status &= ~(OS_TASK_DELAY);
                }
 
                if(!(expired_task->task_status & OS_TASK_SUSPEND))
                {
                    taskReadyAdd(expired_task);
                    need_scheduling = TRUE;
                }
 
                found = TRUE;
                break;
            }
            else
            {
                break;
            }
        }
    }
    while(found);

    osHwiSwiftEnable();
 
    if(need_scheduling == TRUE)
    {
        taskSchedule();
    }

}


/*****************************************************************************/
void osTickSwiHandler(os_swi_count count)
{
    VAR_UNUSED(count);
 
    osTickDispatcher();
}


/*****************************************************************************/
void osTickTimeSwiHandler(os_swi_count count)
{
    VAR_UNUSED(count);
 
    if (osTimerDecrement() == TRUE)
    {
        osTimerDispatcher();
    }

    osTickDispatcher();
}


/*****************************************************************************/
void osTickHwiHandler(os_hwi_arg arg)
{
    VAR_UNUSED(arg);
 
    g_os_ticks++;
 
    osTickIntClear();
    osSwiActivate(OS_TIMER_SWI);
}


/*****************************************************************************/
void osTickCount(uint32_t *up_time)
{
    *up_time = (uint32_t)g_os_ticks;
}
