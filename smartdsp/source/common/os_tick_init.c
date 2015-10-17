/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:44 $
 $Id: os_tick_init.c,v 1.11 2012/11/22 16:28:44 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/os_tick_init.c,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************
 
 @File          os_tick_init.c

 @Description   OS Tick module initialization functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_init.h"

#include "os_tick_.h"
#include "os_swi_.h"


extern volatile uint16_t  g_max_os_timers;

volatile unsigned long long      g_os_ticks;
uint32_t                  g_tick_parameter = 0;


/*****************************************************************************/
os_status osTickInitialize(uint32_t tick_parameter)
{
    g_os_ticks = 0;
    g_tick_parameter = tick_parameter;
 
    osTickIntProgram();
 
    return OS_SUCCESS;
}


/*****************************************************************************/
os_status osTickStart(os_swi_priority priority)
{
    os_status status;
 
    if (g_max_os_timers > 0)
    {
        status = osTickSetup(osTickTimeSwiHandler, priority);
    }
    else
    {
        status = osTickSetup(osTickSwiHandler, priority);
    }
 
    return status;
}


/*****************************************************************************/
os_status osTickSetup(os_swi_function handler, os_swi_priority priority)
{
    os_status status;

    /* The Tick handler is always in the 0th SWI slot */
    status = osSwiCreate(handler, OS_TIMER_SWI, priority, 255);

    osTickIntStart();

    return status;
}
