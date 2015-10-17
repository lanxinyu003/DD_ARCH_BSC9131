/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:51 $
 $Id: sc3x00_tick.c,v 1.4 2012/11/22 16:28:51 sw Exp $
 $Source: /cvsdata/SmartDSP/source/arch/starcore/core_family/sc3X00/sc3x00_tick.c,v $
 $Revision: 1.4 $
******************************************************************************/

/**************************************************************************//**
 
 @File          sc3x00_tick.c

 @Description   sc3x00-specific Tick runtime functions.

 @Cautions      None.
 
*//***************************************************************************/

#include "smartdsp_os_.h"
#include "os_runtime.h"
#include "os_tick_.h"
#include "smartdsp_os_device.h"
#include "memmap.h"


extern os_timer_handle      g_system_timer;
extern volatile uint64_t    g_os_ticks;



/*****************************************************************************/
void osTickIntClear()
{
 
}
/*****************************************************************************/
uint64_t    osTickTime()
{
    uint32_t base_timer, curr_timer;
 
    curr_timer = GET_UINT32(g_dsp_plat_map->timer.platform_timer[0].tm_tv);
    base_timer = GET_UINT32(g_dsp_plat_map->timer.platform_timer[0].tm_tp);
 
    /* g_os_ticks*base_timer + base_timer-curr_timer */
    return (((1 + g_os_ticks) * base_timer) - curr_timer);
}
