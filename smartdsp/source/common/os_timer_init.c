/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2010/03/21 12:20:17 $
 $Id: os_timer_init.c,v 1.14 2010/03/21 12:20:17 b06368 Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_timer_init.c,v $
 $Revision: 1.14 $
******************************************************************************/

/******************************************************************************
 
 @File          os_timer_init.c

 @Description   OS Timer module initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_timer_.h"
#include "os_rm_.h"


extern os_timer_t           g_os_timers;
extern volatile uint16_t    g_max_os_timers;
extern volatile uint16_t    g_cur_timer;


/*****************************************************************************/
os_status osTimerInitialize(uint16_t total_timers)
{
    uint16_t i;
 
    g_max_os_timers = total_timers;
 
    g_os_timers.list = (os_timer_t_ *)
        osMalloc((sizeof(os_timer_t_) * g_max_os_timers), OS_MEM_LOCAL);
 
    if (g_os_timers.list == NULL)
    {
        RETURN_ERROR(OS_ERR_NO_MEMORY);
    }

    OS_ASSERT_COMPILER_COND(g_max_os_timers > 0);

    for ( i=0; i < g_max_os_timers; i++ )
    {
        g_os_timers.list[i].status          = OS_RESOURCE_UNUSED;
        g_os_timers.list[i].timer_config    = 0;
        g_os_timers.list[i].timer           = 0;
        g_os_timers.list[i].timer_function  = &osTimerNullHandler;
    }

    g_cur_timer = 0xff;

    return OS_SUCCESS;
}

